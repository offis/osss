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

#include "o3s/utils/osss_report.h"

namespace osss {

inline
void 
osss_software_task::clock_port(sc_core::sc_signal_in_if<bool>& _if)
{
  if (!m_clock_bound)
  {
    clk_port(_if);
    m_clock_bound = true;
  }
  else
  {
    std::cerr << "ERROR in " << __func__ << "() clock port of "
              << "software task " << this->name() 
              << " has already been bound." 
              << std::endl;
    OSSS_ABORT();
  }
}

inline
void 
osss_software_task::clock_port(sc_core::sc_in<bool>& _port)
{
  if (!m_clock_bound)
  {
    clk_port(_port);
    m_clock_bound = true;
  }
  else
  {
    std::cerr << "ERROR in " << __func__ << "() clock port of "
	      << "software task " << this->name() 
	      << " has already been bound." 
	      << std::endl;
    OSSS_ABORT();
  }
}

inline
void 
osss_software_task::reset_port(sc_core::sc_signal_in_if<bool>& _if)
{
  if (!m_reset_bound)
  {
    rst_port(_if);
    m_reset_bound = true;
  }
  else
  {
    std::cerr << "ERROR in " << __func__ << "() reset port of "
	      << "software task " << this->name() 
	      << " has already been bound." 
	      << std::endl;
    OSSS_ABORT();
  }
}

inline
void 
osss_software_task::reset_port(sc_core::sc_in<bool>& _port)
{
  if (!m_reset_bound)
  {
    rst_port(_port);
    m_reset_bound = true;
  }
  else
  {
    std::cerr << "ERROR in " << __func__ << "() reset port of "
	      << "software task " << this->name() 
	      << " has already been bound." 
	      << std::endl;
    OSSS_ABORT();
  }
}

inline
osss_software_task::osss_software_task()
  : sc_core::sc_module()
  , m_clock_bound(false)
  , m_reset_bound(false)
  , m_running_on_processor(NULL)
  , m_defunct(false)
{
  initSWTask();
}

inline
osss_software_task::
osss_software_task(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , m_clock_bound(false)
  , m_reset_bound(false)
  , m_running_on_processor(NULL)
  , m_defunct(false)
{
  initSWTask();
}

inline
void
osss_software_task::initSWTask()
{
  SC_CTHREAD(startMain, clk_port.pos());
  reset_signal_is(rst_port, true);
}

inline
void
osss_software_task::startMain()
{
  bool m_RunSW = true;
  bool m_SWFinishedFirst = true;
  // sort the IRQ Objects by their prio, 
  // the first has the highest and so forth, store them in a new vector
  sortVector();

  sc_core::sc_time first = sc_core::sc_time_stamp(); 
    
  // mulitple times, since the reset takes some clock cycles
  //std::cout << "before the wait reset" 
  //          << sc_core::sc_time_stamp() << std::endl;
  base_type::wait();
  //std::cout << "after the wait reset" 
  //          << sc_core::sc_time_stamp() << std::endl; 

  sc_core::sc_time second = sc_core::sc_time_stamp(); 
  setPeriod(first, second);
  //std::cout << "THE PERIOD is " << m_clockPeriod 
  //          << "    value: " << m_clockPeriod.value() << std::endl;

  //propagate period to all osss_sw_shared_objects bound to 
  //this osss_software_task
  std::vector<osss_sw_port_if*>::const_iterator iter;
  for (iter = m_sw_port_list.begin(); 
       iter != m_sw_port_list.end(); 
       ++iter)
  {
    (*iter)->setPeriod(this->getPeriod());
  }

  while(true)
  {
    if (m_RunSW)
    {
      this->main();
    }
    if (m_SWFinishedFirst)
    {
      m_SWFinishedFirst = false;
      std::cout << std::endl
                << "---------------------------------------------------------------"
                << std::endl
                << "osss_software_task " << name() << " done" << std::endl
                << "reset needed to restart osss_software_task " << name()
                << std::endl
                << "---------------------------------------------------------------"
                << std::endl << std::endl;
    }
    m_RunSW = false;
    base_type::wait();
  }
}

inline
void 
osss_software_task::osss_sleep( bool interruptable )
{
  base_type::wait();
  if(interruptable)
    this->handleInterrupts();
}

inline
void 
osss_software_task::osss_sleep( int n, bool interruptable )
{
  if (interruptable)
  {
    for(int i=0; i<n; i++)
    {
      base_type::wait();
      this->handleInterrupts();
    }
  }
  else
    base_type::wait(n);
}

inline 
void 
osss_software_task::osss_sleep( double v, sc_core::sc_time_unit tu, bool interruptable )
{
  this->osss_sleep(sc_core::sc_time(v, tu), interruptable);
}

inline
void 
osss_software_task::osss_sleep( const sc_core::sc_time& t, bool interruptable )
{
  if (interruptable)
  {
    // this cast might be dangerous !!!
    int num_clock_cycles = (int)ceil(t/this->getPeriod());
    this->osss_sleep(num_clock_cycles, true);
  }
  else
    base_type::wait(t);
}

inline
bool
osss_software_task::running()
{
  return m_RunSW;
}

inline
bool
osss_software_task::defunct()
{
  return m_defunct;
}

inline
osssi::osss_processor_base*
osss_software_task::mapped_on_processor()
{
  return m_running_on_processor;
}

inline 
void 
osss_software_task::sortVector()
{
  osss_software_task::less_sw_port_priority temp_functor;
  sort(m_sw_port_list.begin(), m_sw_port_list.end(), temp_functor);
}

inline 
void
osss_software_task::setPeriod(sc_core::sc_time first, sc_core::sc_time second)
{ 
  m_clockPeriod = second - first;   
}

inline 
sc_core::sc_time
osss_software_task::getPeriod()
{ 
  return m_clockPeriod;   
}

inline
void 
osss_software_task::end_of_elaboration()
{
  const std::vector< sc_core::sc_object * > tmp_object_vector = 
    this->get_child_objects();
  std::vector< sc_core::sc_object* >::const_iterator iter;
  for (iter = tmp_object_vector.begin(); 
       iter != tmp_object_vector.end(); 
       iter++)
  {
    if (std::string((*iter)->kind()) == std::string("osss_sw_port"))
    {
      //std::cout << (*iter)->kind() << std::endl;
      osss_sw_port_if* tmp_sw_port_if = dynamic_cast<osss_sw_port_if*>(*iter);
      m_sw_port_list.push_back(tmp_sw_port_if);
    }
  }
}

inline
void 
osss_software_task::handleInterrupts()
{
  bool irq_is_pending = true;
  while (irq_is_pending) // loop to handle all pending interrupts
  {
    irq_is_pending = false;
   
    //iterate through the vector of interrupt objects
    std::vector<osss_sw_port_if*>::const_iterator iter;
    for (iter = m_sw_port_list.begin(); 
         iter != m_sw_port_list.end(); 
         ++iter)
    {
      // since the irq objects in the vector are sorted by their priorities,
      // the first pending IRQ which is found in the vector has the highest 
      // priority, this will be executed
      if ( (*iter)->pendingRequest() ) 
      {
        irq_is_pending = true;

        //std::cout << sc_core::sc_time_stamp()
        //          << " ISR is pending" << std::endl;

        (*iter)->grantAccess(true); // the request is granted

        //std::cout << sc_core::sc_time_stamp() 
        //          << " ISRs execution is started" << std::endl;

        // wait after destroying the waste of time object to guarantee, 
        // that the IRQ method is finished
        while( (*iter)->pendingRequest() )
        {
          //std::cout << "\t\t\t" << sc_core::sc_time_stamp() 
          //          << " ISR is pending. "
          //          << " waiting for finished of the object"
          //          << std::endl;

          base_type::wait(); 
        }

        //std::cout << "Got the ISR finish." 
        //          << " go back to Sw or another interrupt "
        //             "(from lower priority)"
        //          << std::endl;

        (*iter)->grantAccess(false);
        base_type::wait();
        break;
      }
    }
  }
}

} // namespace osss

// $Id: osss_software_task.tpp 2835 2008-12-04 09:29:32Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
