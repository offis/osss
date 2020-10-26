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

#ifndef OSSS_CHANNEL_ARBITER_HH
#define OSSS_CHANNEL_ARBITER_HH


#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_types.h"
#include "o3s/communication/arch/osss_channel/osss_signal_list_element_base.h"

#include <vector>

namespace osss {

class osss_port_list_element_base;

template< class SchedulerType >
class osss_channel_arbiter
  : public sc_core::sc_module
{
public:

  explicit
  osss_channel_arbiter( sc_core::sc_module_name moduleName );

  //void subscribe(osss_master_if * _osss_ll_master_if);
  void subscribe( sc_core::sc_out<bool> * request_port_ptr
                , sc_core::sc_in<bool> *  grant_port_ptr
                , std::vector< osss_port_list_element_base * > * portList_ptr
                , std::vector< osss_signal_list_element_base * > * signalList_ptr );

  virtual const char* kind() const;

private:

  std::vector< std::vector< osss_signal_list_element_base * > * >
    m_scheduled_signals;
  std::vector< sc_core::sc_signal<bool> * > m_request_signals;
  std::vector< sc_core::sc_signal<bool> * > m_grant_signals;
  osss_index_type m_number_masters;

  osssi::osss_method_handle mux_proc_handle;
  osssi::osss_method_handle request_proc_handle;

  std::vector< osss_port_list_element_base * > m_arbiter_portList;
  osss_index_type m_granted_master;

  bool m_first_use;
  std::vector<bool> m_last_requestVector;

  SchedulerType * m_scheduler;

  SC_HAS_PROCESS(osss_channel_arbiter);

  // sc_method process to multiplex the signals of the
  // granted master to the shared bus
  void mux_proc();

  // sc_method process to register requests of the masters
  void request_proc();

  void updateGrantVector();

  void updateRequestVector();

};

} // namespace osss

#include "o3s/communication/arch/osss_channel/osss_channel_arbiter.tpp"

#endif
// $Id: osss_channel_arbiter.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
