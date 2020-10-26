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

#ifndef OSSS_SIGNAL_LIST_ELEMENT_BASE_HH
#define OSSS_SIGNAL_LIST_ELEMENT_BASE_HH

#include <string>

namespace osss {

class osss_signal_list_element_base
{
public:
  virtual void print() = 0;
  virtual std::string getAlias() = 0;
  virtual void trace(sc_core::sc_trace_file* tf) = 0;
  virtual void trace(sc_core::sc_trace_file* tf, std::string trace_name) = 0;
  virtual void addTraceName(std::string name) = 0;
  virtual bool doTypecheck(osss_signal_list_element_base * sig) = 0;
  virtual bool isequal(osss_signal_list_element_base* base_ptr) = 0;
  virtual void addToSensitivity(sc_core::sc_sensitive * sensitive_ptr) = 0;

  virtual osss_signal_list_element_base * clone() const = 0;

  virtual bool assign(osss_signal_list_element_base * sig) = 0;
  virtual bool assign_all_zero() = 0;

  virtual bool bitwise_or(osss_signal_list_element_base * sig) = 0;
  virtual bool bitwise_and(osss_signal_list_element_base * sig) = 0;
  virtual bool bitwise_xor(osss_signal_list_element_base * sig) = 0;

  virtual void bitwise_or(std::vector< osss_signal_list_element_base * > * sig_vec) = 0;
  virtual void bitwise_and(std::vector< osss_signal_list_element_base * > * sig_vec) = 0;
  virtual void bitwise_xor(std::vector< osss_signal_list_element_base * > * sig_vec) = 0;

  virtual bool bitwise_not() = 0;

  virtual bool isBoolean() = 0;
  virtual bool getBoolean() = 0;

  virtual osss_signal_list_element_base& operator=(osss_signal_list_element_base & sig) = 0;

  virtual ~osss_signal_list_element_base() {}
  
};

} // namespace osss

#endif
// $Id: osss_signal_list_element_base.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
