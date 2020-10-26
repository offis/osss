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

#ifndef OSSS_SOFTWARE_TASK_HH
#define OSSS_SOFTWARE_TASK_HH

#include <string>
#include <iostream>
#include <algorithm>
#include <math.h>

#include "o3s/utils/osss_systemc.h"

#include "o3s/structural/app/osss_sw_object_if.h"
#include "o3s/communication/app/osss_sw_port_if.h"

//------------------------------------------------------------------------------

namespace osss {

  namespace osssi {
    class osss_processor_base;
  }


// Base class for user-defined software tasks
class osss_software_task
  : public sc_core::sc_module
  , public osss_sw_object_if
{

  friend class osss_required_time;
  friend class osss_waste_of_time;
  friend class osssi::osss_processor_base;

public:

  void clock_port(sc_core::sc_signal_in_if<bool>& _if);
  void clock_port(sc_core::sc_in<bool>& _port);

  void reset_port(sc_core::sc_signal_in_if<bool>& _if);
  void reset_port(sc_core::sc_in<bool>& _port);

  typedef sc_core::sc_module base_type;
  SC_HAS_PROCESS(osss_software_task);

protected:
  osss_software_task();

  explicit
  osss_software_task( sc_core::sc_module_name );

public:
  bool running();
  bool defunct();
  osssi::osss_processor_base* mapped_on_processor();
  
  sc_core::sc_time getPeriod();
  
  virtual const char* kind () const
  {
    return "osss_software_task";
  }

  const char* swObjectKind() const
  {
    return kind();
  }

  virtual void main() = 0;

 protected:

  void osss_sleep( bool interruptable = true);
  void osss_sleep( int n, bool interruptable = true );
  void osss_sleep( double v, sc_core::sc_time_unit tu, bool interruptable = true );
  void osss_sleep( const sc_core::sc_time& t, bool interruptable = true );

// disable all wait statements which can be used inside SC_CTHREADs
  void wait() { wrongWaitError(); }
  void wait( const sc_core::sc_event& ) { wrongWaitError(); }
  void wait( sc_core::sc_event_or_list& ) { wrongWaitError(); }
  void wait( sc_core::sc_event_and_list& ) { wrongWaitError(); }
  void wait( const sc_core::sc_time& ) { wrongWaitError(); }
  void wait( double , sc_core::sc_time_unit ) { wrongWaitError(); }
  void wait( const sc_core::sc_time&, const sc_core::sc_event& ) { wrongWaitError(); }
  void wait( double , sc_core::sc_time_unit , const sc_core::sc_event& ) { wrongWaitError(); }
  void wait( const sc_core::sc_time&, sc_core::sc_event_or_list& ) { wrongWaitError(); }
  void wait( double , sc_core::sc_time_unit , sc_core::sc_event_or_list& ) { wrongWaitError(); }
  void wait( const sc_core::sc_time&, sc_core::sc_event_and_list& ) { wrongWaitError(); }
  void wait( double , sc_core::sc_time_unit , sc_core::sc_event_and_list& ) { wrongWaitError(); }

  void wait( int ) { wrongWaitError(); }
#if defined( OSSS_SYSTEMC_2_1 )
  void wait_until( const sc_core::sc_lambda_ptr& ) { wrongWaitError(); }
  void wait_until( const sc_core::sc_signal_bool_deval& ) { wrongWaitError(); }
#endif
  void at_posedge( const sc_core::sc_signal_in_if<bool>& ) { wrongWaitError(); }
  void at_posedge( const sc_core::sc_signal_in_if<sc_dt::sc_logic>& ) { wrongWaitError(); }
  void at_negedge( const sc_core::sc_signal_in_if<bool>& ) { wrongWaitError(); }
  void at_negedge( const sc_core::sc_signal_in_if<sc_dt::sc_logic>& ) { wrongWaitError(); }
  
 private:

  osss_software_task(const osss_software_task& swt) {}

  sc_core::sc_in<bool>  clk_port;
  sc_core::sc_in<bool>  rst_port;

  void initSWTask();

  void setPeriod(sc_core::sc_time first, sc_core::sc_time second);

  void startMain();
      
  void handleInterrupts();
  
  void sortVector();
    
  // to handle more software shared objects with different priorities
  std::vector< osss_sw_port_if* > m_sw_port_list; 
  
  // to determine the current clock period, this is necessary 
  // to compute the deadline in ~RET
  sc_core::sc_time m_clockPeriod;
  bool m_RunSW;
  bool m_SWFinishedFirst;
  bool m_clock_bound;
  bool m_reset_bound;
  osssi::osss_processor_base* m_running_on_processor;
  bool m_defunct;

  virtual void end_of_elaboration();

  void wrongWaitError()
  {
    std::cerr << "Error in "  
              << name() << ":"
              << " wrong type of wait used."
              << " Only osss_sleep statements are allowed to be used"
              << " inside any osss_software_task." << std::endl;
    sc_core::sc_stop();
  }

  // functor to sort the list of osss_sw_port interfaces
  struct less_sw_port_priority
  {
    bool operator()(osss_sw_port_if* x, osss_sw_port_if* y)
    {
      // is prio of object x greater than the prio of object y, return true
      return ((x->getPriority()) < (y->getPriority()));
    }
  };
  
};

} // namespace osss

#include "o3s/structural/app/osss_software_task.tpp"

#endif
// $Id: osss_software_task.h 2833 2008-12-04 09:29:20Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
