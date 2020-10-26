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

#ifndef OSSS_PORT_LIST_ELEMENT_BASE_HH
#define OSSS_PORT_LIST_ELEMENT_BASE_HH

#include <string>
#include <vector>

namespace osss {

class osss_signal_list_element_base;

class osss_port_list_element_base
{
public:
  friend std::ostream& operator<< (std::ostream& o, 
				   const osss_port_list_element_base& pleb);

  virtual osss_port_list_element_base * clone() const = 0; 
  virtual void print() = 0;
  virtual bool hasAlias() = 0;
  virtual void setAlias(std::string alias) = 0;
  virtual std::string getAlias() = 0;
  // "new" binding concept
  virtual std::string name() = 0;
  virtual std::string kind() = 0;
  virtual int size() = 0;

  virtual void generateSignal(std::vector< osss_signal_list_element_base * > & 
			      _signal_list) = 0;

  virtual osss_signal_list_element_base * newSignal() = 0;
  virtual osss_signal_list_element_base * getBoundSignal() = 0;

  virtual void bind(osss_signal_list_element_base * _a_signal_ptr) = 0;
  virtual void trace(osss_signal_list_element_base * _a_signal_ptr) = 0;
  virtual bool isequal(osss_port_list_element_base *base_ptr) = 0;
  virtual bool isequal(osss_signal_list_element_base *base_ptr) = 0;
  virtual void setTraceName(std::string name) = 0;
  virtual void setTraceScope(std::string scope_name) = 0;
  virtual void trace(sc_core::sc_trace_file* tf) = 0;

  virtual void write(osss_signal_list_element_base *base_ptr) = 0;
  virtual void read(osss_signal_list_element_base *base_ptr) = 0;

  virtual ~osss_port_list_element_base() {}
};

inline
std::ostream& operator<< (std::ostream& o, 
			  osss_port_list_element_base& pleb)
{
  o << "name: " << pleb.name() << " | ";
  o << "kind: " << pleb.kind() << " | ";
  o << "size: " << pleb.size();

  if (pleb.hasAlias())
    o << " | " << "alias: " <<  pleb.getAlias();

  return o;
}

} // namespace osss

#endif
// $Id: osss_port_list_element_base.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
