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

#ifndef OSSS_SIGNAL_IF_HH
#define OSSS_SIGNAL_IF_HH

#include <iostream>
#include <fstream>

#include <string>
#include <vector>

#include "o3s/utils/osss_systemc.h"
#include "o3s/communication/arch/osss_channel/osss_channel.h"
#include "o3s/communication/arch/osss_channel/osss_channel_fwd.h"

namespace osss {

class osss_signal_if
{
public:
  
  osss_signal_if();

  virtual bool isMaster();
  virtual bool isSlave();

  virtual void connect(osss_basic_channel &c);

  std::vector< osss_port_list_element_base * > * getPortRegistry();
  std::vector< osss_connection_list_element * > * getConnectionList();

  virtual void trace_simple(std::string name);
  virtual void trace_in_file(sc_core::sc_trace_file* tf, std::string name);

  virtual ~osss_signal_if();

protected: 
  // "new" connection concept:
  //--------------------------

  // connects an sc_out<T> or sc_inout<T> port directly to an osss_shared_signal
  void osss_connect(osss_port_list_element_base * outPort,
                    osssi::osss_shared_signal * sharedSignal);


  // connects an osss_shared_signal directly to an sc_in<T> or sc_inout<T> port
  void osss_connect(osssi::osss_shared_signal * sharedSignal,
                    osss_port_list_element_base * inPort);

  osssi::osss_shared_signal * osss_shared_signal(std::string name);

  osss_port_list_element_base * addToPortRegistry(
    osss_port_list_element_base * pleb);
 
  std::vector< osss_connection_list_element * > m_connectionList;

  bool m_connected;
  
  osss_basic_channel * m_channel;

private:
  std::string m_name;

  std::vector< osss_port_list_element_base * > m_portRegistry;

};

//-----------------------------------------------------------------------------

void osss_print_channel_graph(osss_basic_channel &ch,
                              const std::string& file_name);

// TODO: include arbiter(s) and address decoder(s)
std::ofstream& operator<< (std::ofstream& o,
                           osss_basic_channel &ch);

} // namespace osss

#endif
// $Id: osss_signal_if.h 2839 2008-12-04 09:29:59Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
