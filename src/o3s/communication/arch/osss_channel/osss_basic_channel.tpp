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

//-------------------------------------------------------------------------//

template <class InterfaceType>
inline
InterfaceType* 
osss_basic_channel::create()
{
  typedef typename getTransactorType<InterfaceType>::return_t transactor_t;
  return new transactor_t();
}

//-------------------------------------------------------------------------//

inline
osssi::osss_object_registry*
osss_basic_channel::getObjectRegistry()
{
  return m_object_registry;
}

//-------------------------------------------------------------------------//

inline
std::vector< osss_signal_list_element_base * > *
osss_basic_channel::getSharedSignalList()
{
  return &m_sharedSignalList;
}

//-------------------------------------------------------------------------//

inline
std::vector< std::vector< osss_signal_list_element_base * > * > *
osss_basic_channel::getMultiSignalList()
{
  return NULL;
}

inline
std::vector< osss_via_list_element * > *
osss_basic_channel::getMuxViaList()
{
  return NULL;
}

inline
std::vector< osss_via_list_element * > *
osss_basic_channel::getOrViaList()
{
  return NULL;
}

inline
std::vector< osss_via_list_element * > *
osss_basic_channel::getAndOrViaList()
{
  return NULL;
}

inline
std::vector< osss_via_list_element * > *
osss_basic_channel::getAndViaList()
{
  return NULL;
}

inline
sc_core::sc_signal<unsigned int> * 
osss_basic_channel::getArbiterSelect()
{
  return NULL;
}

inline
sc_core::sc_signal<unsigned int> * 
osss_basic_channel::getAddressDecoderSelect()
{
  return NULL;
}

inline
unsigned int 
osss_basic_channel::getNumTransactors()
{
  return m_numTransactors;
}

inline
unsigned int 
osss_basic_channel::getNumMasters()
{
  return 0;
}

inline
void 
osss_basic_channel::incNumMasters()
{
}

inline
unsigned int 
osss_basic_channel::getNumSlaves()
{
  return 0;
}

inline
void 
osss_basic_channel::incNumSlaves()
{
}

inline
void
osss_basic_channel::incNumTransactors()
{
  m_numTransactors++;
}

inline
void
osss_basic_channel::complete()
{
  // this method does nothing by default

/*
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::cout << "for basic channel: " << this->name() << std::endl;
*/
}

inline
osssi::osss_clock_reset_if* 
osss_basic_channel::getPorts()
{
  return this;
}

#if 0
inline
void 
osss_basic_channel::clock_port(const sc_core::sc_signal_in_if<bool>& in_if)
{
  if (m_reset_module)
  {
    osssi::osss_clock_reset_if::clock_port(in_if);
    m_reset_module->clock_port_in(osssi::osss_clock_reset_if::clock_port_port);
    m_reset_module_clock_bound = true;
  }
}

inline
void 
osss_basic_channel::clock_port(sc_core::sc_port<sc_core::sc_signal_in_if<bool>, 1>& in_port)
{
  if (m_reset_module)
  {
    osssi::osss_clock_reset_if::clock_port(in_port);
    m_reset_module->clock_port_in(osssi::osss_clock_reset_if::clock_port_port);
    m_reset_module_clock_bound = true;
  }
}

inline
void 
osss_basic_channel::clock_port(sc_core::sc_port<sc_core::sc_signal_inout_if<bool>, 1>& inout_port)
{
  if (m_reset_module)
  {
    osssi::osss_clock_reset_if::clock_port(inout_port);
    m_reset_module->clock_port_in(osssi::osss_clock_reset_if::clock_port_port);
    m_reset_module_clock_bound = true; 
  }
}

inline
void 
osss_basic_channel::reset_port(const sc_core::sc_signal_in_if<bool>& in_if)
{
  if (m_reset_module)
  {
    osssi::osss_clock_reset_if::reset_port(in_if);
    m_reset_module->reset_port_in(osssi::osss_clock_reset_if::reset_port_port);
    m_reset_module_reset_bound = true;
  }
}

inline
void 
osss_basic_channel::reset_port(sc_core::sc_port<sc_core::sc_signal_in_if<bool>, 1>& in_port)
{
  if (m_reset_module)
  {
    osssi::osss_clock_reset_if::reset_port(in_port);
    m_reset_module->reset_port_in(osssi::osss_clock_reset_if::reset_port_port);
    m_reset_module_reset_bound = true;
  }
}

inline
void 
osss_basic_channel::reset_port(sc_core::sc_port<sc_core::sc_signal_inout_if<bool>, 1>& inout_port)
{
  if (m_reset_module)
  {
    osssi::osss_clock_reset_if::reset_port(inout_port);
    m_reset_module->reset_port_in(osssi::osss_clock_reset_if::reset_port_port);
    m_reset_module_reset_bound = true;
  }
}
#endif


} // namespace osss

// $Id: osss_basic_channel.tpp 2842 2008-12-04 09:30:13Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
