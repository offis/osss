/* -----------------------------------------------------------------------
 * Copyright (c) 2005-2008  OFFIS Institute for Information Technology,
 *                          Carl von Ossietzky University,
 *                          Oldenburg, Germany
 * All rights reserved.
 *
 * This file is directly or indirectly part of the OSSS simulation
 * library, a library for synthesisable system level models in
 * SystemC.
 *
 * Created for the projects:
 *   - ICODES  (1)
 *   - PolyDyn (2)
 *   - ANDRES  (3)
 *
 * 1) http://icodes.offis.de/
 * 2) http://ehs.informatik.uni-oldenburg.de/en/research/projects/polydyn/
 * 3) http://andres.offis.de/
 *
 * A list of authors and contributors can be found in the accompanying
 * AUTHORS file.  For detailed copyright information, please refer
 * to the COPYING file.
 *
 * -----------------------------------------------------------------------
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 *
 * Contact information:
 *  OFFIS
 *  Institute for Information Technology
 *    Escherweg 2
 *    D-26121 Oldenburg
 *    Germany
 *  www  : http://offis.de/
 *  phone: +49 (441) 9722-0
 *  fax  : +49 (441) 9722-102
 * -----------------------------------------------------------------------
 */
#ifndef OSSS_PROCESSOR_BASE_H_INCLUDED_
#define OSSS_PROCESSOR_BASE_H_INCLUDED_

#include "o3s/communication/osss_port.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_channel_client_if.h"
#include "o3s/communication/arch/osss_channel/osss_port_channel.h"
#include "o3s/structural/arch/osss_clock_reset_if.h"

namespace osss {

class osss_software_task;

namespace osssi {

class osss_processor_base
  : public osss_clock_reset_if
{
public:

  typedef osss_port<osss_channel_if<osss_rmi_channel_client_if> >
    rmi_client_port_type;

  osss_processor_base()
    : osss_clock_reset_if()
    , m_sw_task_vec()
    , clock_stub("clock_stub")
    , reset_stub("reset_stub")
    , m_rmi_client_port(NULL)
    , m_bound_channel(NULL)
  {}

  virtual ~osss_processor_base() = 0;

  template<class ChannelType>
  void rmi_client_port(ChannelType& channel)
  {
    if (m_rmi_client_port == NULL)
    {
      // create port
      m_rmi_client_port = 
        new rmi_client_port_type(sc_core::sc_gen_unique_name("rmi_client_port"));
      // perform port to channel if binding
      m_rmi_client_port->bind(channel);

      m_bound_channel = &channel;
    }
    else
    {
      std::cerr << "ERROR in " << __PRETTY_FUNCTION__ << ": "
                << m_rmi_client_port->name() 
                << " has already been bound to a channel."
                << std::endl;
      OSSS_ABORT();
    }
  }

  sc_core::sc_object* bound_channel() const
  {
    return m_bound_channel;
  }

#if 0
  void add_sw_task(osss_software_task& sw_task)
    { this->add_sw_task(&sw_task); }

  void add_sw_task(osss_software_task* sw_task)
  {
    // check if simulation has begun.
    // adding software tasks during simulation is not allowed.
    if (sc_core::sc_start_of_simulation_invoked())
    {
      std::cerr << "ERROR " << __func__ 
                << "() called after elaboration phase."
                << std::endl;
      OSSS_ABORT();
    }

    // check if this software task has already been added to another processor
    if (sw_task->m_running_on_processor != NULL)
    {
      std::cerr << "ERROR in " << __func__ << "(): "
                << "The software task " << sw_task->name() 
                << " has already been added to another processor: " 
                << sw_task->m_running_on_processor->name()
                << std::endl;
      OSSS_ABORT();
    }

    // check if the ports of the software task have already been bound
    if (sw_task->m_clock_bound)
    {
      std::cerr << "ERROR in " << __func__ << "(): Clock port of "
                << "software task " << sw_task->name() 
                << " has already been bound." 
                << std::endl;
      OSSS_ABORT();
    }
    if (sw_task->m_reset_bound)
    {
      std::cerr << "ERROR in " << __func__ << "(): Reset port of "
                << "software task " << sw_task->name() 
                << " has already been bound." 
                << std::endl;
      OSSS_ABORT();
    }

    // in the ICODES project we support only a single software task
    // per CPU. Therefore we do not perform the port binding of 
    // software tasks that have an index greater than 0 
    if (m_sw_task_vec.size() > 0)
    {
      //do the reset and clock port binding to local signal stubs
      sw_task->clock_port(clock_stub);
      sw_task->m_clock_bound = true;
      sw_task->reset_port(reset_stub);
      sw_task->m_reset_bound = true;

      sw_task->m_defunct = true;

      //do the RMI port binding (without RMI registry)
      const std::vector< sc_core::sc_object* > obj_vector = 
	sw_task->get_child_objects();
      for(std::vector< sc_core::sc_object* >::size_type i = 0;
	  i < obj_vector.size(); i++)
      {
	if (std::string(obj_vector[i]->kind()) == std::string("osss_port_rmi"))
	{
	  rmi_client_port_type* cp = 
	    dynamic_cast<rmi_client_port_type*>(obj_vector[i]);
	  if(cp != NULL)
          {
            if (m_rmi_client_port)
              m_rmi_client_port->bind_to_rmi(*cp, false);
            else
            {
              std::cerr << "ERROR in " << __func__ << "(): "
                        << "The processor " << name() 
                        << " has not been bound to an RMI-Channel. "
                        << "Call method rmi_client_port(channelIF) before "
                        << "you call add_sw_task(...)." << std::endl;
              OSSS_ABORT();
            }
          }
	}
      }
    }
    else
    {
      //do the port binding to the "real" ports
      sw_task->clock_port(clock_port_port);
      sw_task->m_clock_bound = true;
      sw_task->reset_port(reset_port_port);
      sw_task->m_reset_bound = true;

      sw_task->m_defunct = false;

      //do the RMI port binding (with RMI registry)
      const std::vector< sc_core::sc_object* > obj_vector = 
	sw_task->get_child_objects();
      for(std::vector< sc_core::sc_object* >::size_type i = 0;
	  i < obj_vector.size(); i++)
      {
	if (std::string(obj_vector[i]->kind()) == std::string("osss_port_rmi"))
	{
	  rmi_client_port_type* cp = 
	    dynamic_cast<rmi_client_port_type*>(obj_vector[i]);
	  if(cp != NULL)
          {
            if (m_rmi_client_port)
              m_rmi_client_port->bind_to_rmi(*cp, true);
            else
            {
              std::cerr << "ERROR in " << __func__ << "(): "
                        << "The processor " << name() 
                        << " has not been bound to an RMI-Channel. "
                        << "Call method rmi_client_port(channelIF) before "
                        << "you call add_sw_task(...)." << std::endl;
              OSSS_ABORT();
            }
          }
        }
      }
    }

    //register this processor at the software task
    sw_task->m_running_on_processor = this;
    //add software task to task vector
    m_sw_task_vec.push_back(sw_task);
  }

  osss_software_task* get_sw_task()
  {
    if (m_sw_task_vec.size() == 0)
    {
      std::cerr << "ERROR in " << __func__ 
                << "(): No software task registered" 
                << std::endl;
      OSSS_ABORT();
    }
    else
      return m_sw_task_vec[0];
  }

  osss_software_task* get_sw_task(unsigned int task_nr)
  {
    if (m_sw_task_vec.size() - 1 < task_nr)
    {
      std::cerr << "ERROR in " << __func__ 
                << "(): Requested software task number not available" 
                << std::endl;
      OSSS_ABORT();
    }
    else
      return m_sw_task_vec[task_nr];
  }
#endif
  void                add_sw_task( osss_software_task* sw_task );
  osss_software_task* get_sw_task( unsigned int task_nr = 0 );

protected:
  void check_sw_task_vector();

  std::vector<osss_software_task*> m_sw_task_vec;

  //signal stubs for binding unused software tasks
  sc_core::sc_signal<bool> clock_stub;
  sc_core::sc_signal<bool> reset_stub;

  rmi_client_port_type* m_rmi_client_port;

  //contains a pointer to the RMI Channel this processor is bound to
  sc_core::sc_object* m_bound_channel;

};

inline
osss_processor_base::~osss_processor_base()
{
  delete m_rmi_client_port;
}

} // namespace osssi
} // namespace osss

#endif
// $Id: osss_processor_base.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
