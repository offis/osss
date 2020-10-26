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

#ifndef OSSS_SIGNAL_LIST_ELEMENT_HH
#define OSSS_SIGNAL_LIST_ELEMENT_HH

#include <string>
#include <list>

#include "o3s/communication/arch/osss_channel/osss_signal_list_element_base.h"

namespace osss {

template< class AttrType >
class osss_signal_list_element : public osss_signal_list_element_base
{
public:
  typedef AttrType data_type;

  osss_signal_list_element(sc_core::sc_signal< AttrType > * _signal_ptr,
			   std::string _alias);

  virtual void print();

  sc_core::sc_signal< AttrType > * getSignal();

  virtual bool doTypecheck(osss_signal_list_element_base * sig);
  virtual bool isequal(osss_signal_list_element_base* base_ptr );

  virtual std::string getAlias();

  virtual void trace(sc_core::sc_trace_file* tf);
  virtual void trace(sc_core::sc_trace_file* tf, std::string trace_name);

  virtual void addTraceName(std::string name);

  virtual void addToSensitivity(sc_core::sc_sensitive * sensitive_ptr);

  virtual ~osss_signal_list_element();

  virtual osss_signal_list_element_base * clone() const;

  virtual bool assign(osss_signal_list_element_base * sig);
  virtual bool assign_all_zero();

  virtual bool bitwise_or(osss_signal_list_element_base * sig);
  virtual bool bitwise_and(osss_signal_list_element_base * sig);
  virtual bool bitwise_xor(osss_signal_list_element_base * sig);

  virtual void bitwise_or(std::vector< osss_signal_list_element_base * > * sig_vec);
  virtual void bitwise_and(std::vector< osss_signal_list_element_base * > * sig_vec);
  virtual void bitwise_xor(std::vector< osss_signal_list_element_base * > * sig_vec);

  virtual bool bitwise_not();

  virtual bool isBoolean();
  virtual bool getBoolean();

  virtual osss_signal_list_element_base& operator=(osss_signal_list_element_base & sig);

private:
  osss_signal_list_element(const osss_signal_list_element & sig);

  sc_core::sc_signal< AttrType > * m_signal_ptr;
  std::string m_alias;
  std::list<std::string> m_trace_name_list;
};

} // namespace osss

#include "o3s/communication/arch/osss_channel/osss_signal_list_element.tpp"

#endif
// $Id: osss_signal_list_element.h 3023 2009-03-04 22:55:26Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
