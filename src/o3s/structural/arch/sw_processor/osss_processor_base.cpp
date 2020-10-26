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

#include "osss_processor_base.h"
#include "o3s/structural/app/osss_software_task.h"

namespace osss {
namespace osssi {

void
osss_processor_base::add_sw_task( osss_software_task* sw_task )
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
    sw_task->clock_port( clock_port() );
    sw_task->m_clock_bound = true;
    sw_task->reset_port(reset_port() );
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

osss_software_task*
osss_processor_base::get_sw_task(unsigned int task_nr)
{
  if( osss_unlikely(m_sw_task_vec.size() == 0) )
  {
    std::cerr << "ERROR in " << __func__ 
              << "(): No software task registered" 
              << std::endl;
    OSSS_ABORT();
  }

  if( osss_unlikely(task_nr >= m_sw_task_vec.size()) )
  {
    std::cerr << "ERROR in " << __func__ 
              << "(): Requested software task number not available" 
              << std::endl;
    OSSS_ABORT();
  }
  return m_sw_task_vec[task_nr];
}

void
osss_processor_base::check_sw_task_vector()
{
  if (m_sw_task_vec.size() > 1)
  {
    std::cout << "Warning: "
              << m_sw_task_vec.size()
              << " software tasks ( " ;
    for(unsigned int i=0; i<m_sw_task_vec.size() - 1; i++)
      std::cout << m_sw_task_vec[i]->name() << ", ";
    std::cout << m_sw_task_vec[m_sw_task_vec.size() - 1]->name() 
              << " )"
              << " have been added to processor " << name() << "."
              << " Up to now only a single software task per processor"
              << " is allowed." 
              << " The following software task(s) will never be executed: ";
    for(unsigned int i=1; i<m_sw_task_vec.size() - 1; i++)
      std::cout << m_sw_task_vec[i]->name() << ", ";
    std::cout << m_sw_task_vec[m_sw_task_vec.size() - 1]->name() 
              << std::endl 
              << std::endl;
  }
}

} // namespace osssi
} // namespace osss

// $Id: osss_processor_base.cpp 2923 2008-12-16 10:32:25Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
