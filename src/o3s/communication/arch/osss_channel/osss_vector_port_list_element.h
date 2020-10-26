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

#ifndef OSSS_VECTOR_PORT_LIST_ELEMENT_HH
#define OSSS_VECTOR_PORT_LIST_ELEMENT_HH

#include <string>
#include <vector>

#include "o3s/communication/arch/osss_channel/osss_vector_port_list_element_base.h"
#include "o3s/communication/arch/osss_channel/osss_signal_list_element.h"
#include "o3s/communication/arch/osss_channel/osss_vector_port.h"

namespace osss {

template< class AttrType >
class osss_vector_port_list_element : public osss_vector_port_list_element_base
{
public:
  osss_vector_port_list_element( AttrType* _attr_ptr, std::string _alias);
  explicit osss_vector_port_list_element( AttrType* _attr_ptr );
  osss_vector_port_list_element() {}
   
  virtual void bind(osss_signal_list_element_base * _a_signal_ptr);

  virtual bool isequal(osss_signal_list_element_base *base_ptr);

  virtual bool hasAlias();

  virtual void setAlias(std::string alias);
  virtual std::string getAlias();

  // "new" binding concept
  virtual std::string name();
  virtual std::string kind();
  virtual unsigned int size();

  virtual void setTraceName(std::string name);

  virtual void setTraceScope(std::string scope_name);

  virtual void trace(sc_core::sc_trace_file* tf);

  AttrType * getVectorPort();
 
  /*
  //not implemented
  virtual void write(osss_signal_list_element_base *base_ptr);

  //not implemented
  virtual void read(osss_signal_list_element_base *base_ptr);
  */

  virtual ~osss_vector_port_list_element();

  typedef typename AttrType::data_type data_type;

protected:
  AttrType* m_attr_ptr;
  std::string m_alias;
  std::string m_name;
  std::string m_trace_name;
  bool m_trace;
  bool m_has_alias;
};

//----------------------------------------------------------------------------//

template<typename T>
inline
osss_vector_port_list_element<T>* 
make_vector_port_list_element(T* port, const std::string& name)
{
  return new osss_vector_port_list_element<T>(port, name);
}

#include "o3s/communication/arch/osss_channel/osss_vector_port_list_element.tpp"  

} // namespace osss

#endif
// $Id: osss_vector_port_list_element.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
