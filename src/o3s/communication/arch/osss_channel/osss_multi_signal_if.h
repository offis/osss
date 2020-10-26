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

#ifndef OSSS_MULTI_SIGNAL_IF_HH
#define OSSS_MULTI_SIGNAL_IF_HH

#include "o3s/communication/arch/osss_channel/osss_signal_if.h"

namespace osss {

class osss_multi_signal_if
  : public osss_signal_if
{
public:
  
  // "new" binding concept
  void connect(osss_basic_channel &c);  

protected: 
  // "new" connection concept:
  //--------------------------

  // overwritten from osss_signal_if
  void osss_connect(osss_port_list_element_base * outPort, 
		    osssi::osss_shared_signal * sharedSignal);

  // connects an osss_shared_signal directly to an sc_in<T> port
  void osss_connect(osssi::osss_shared_signal * sharedSignal, 
		    osss_port_list_element_base * inPort);

   // connects an osss_multi_signal directly to an sc_in<T> port
  void osss_connect(osssi::osss_multi_signal * multiSignal, 
		    osss_port_list_element_base * inPort);

  // connects an sc_out<T> port directly to an osss_multi_signal
  void osss_connect(osss_port_list_element_base * outPort, 
		    osssi::osss_multi_signal * multiSignal);

  // connects an sc_out<T> port which is gated by a via to an 
  // osss_shared_signal
  void osss_connect(osss_port_list_element_base * outPort, 
		    osss_via_base * via, 
		    osssi::osss_shared_signal * sharedSignal);

  /*
  // TO DO ????
  // this kind of shared of multi signal combination should be
  // done inside each transactor

  // connects an  osss_signal_base (either osss_multi_signal or 
  // osss_shared_signal) which is gated by a via to an
  // sc_in<T> port
  void osss_connect(osss_signal_base * signal, 
		    osss_via_base * via, 
		    osss_port_list_element_base * inPort);
  */

  osssi::osss_multi_signal  * osss_multi_signal (std::string name);

  osssi::osss_mux * osss_mux(osss_mux_controller muxController);

  osssi::osss_or * osss_or();

  osssi::osss_and * osss_and(osss_port_list_element_base * outPort);
  osssi::osss_and * osss_and(osssi::osss_shared_signal * signal);
  osssi::osss_and * osss_and(osssi::osss_multi_signal * signal);

  osssi::osss_and_or * osss_and_or(osss_port_list_element_base * outPort);
  osssi::osss_and_or * osss_and_or(osssi::osss_shared_signal * signal);
  osssi::osss_and_or * osss_and_or(osssi::osss_multi_signal * signal);

private:
  void connect_mux(osss_basic_channel &c, 
		   std::vector< osss_connection_list_element * >::iterator 
		   &connection_iter);

  void connect_or(osss_basic_channel &c, 
		  std::vector< osss_connection_list_element * >::iterator 
		  &connection_iter);

  void connect_and_or(osss_basic_channel &c, 
		      std::vector< osss_connection_list_element * >::iterator 
		      &connection_iter);
};

} // namespace osss

#endif
// $Id: osss_multi_signal_if.h 2839 2008-12-04 09:29:59Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
