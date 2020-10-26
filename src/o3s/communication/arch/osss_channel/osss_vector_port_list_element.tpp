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

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_vector_port_list_element< AttrType >::osss_vector_port_list_element( 
  AttrType* _attr_ptr, std::string _alias)
{
  m_attr_ptr = _attr_ptr;
  m_alias = _alias;
  m_name = _alias;  // to do: change to _name
  m_has_alias = true;
  m_trace = false;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_vector_port_list_element< AttrType >::osss_vector_port_list_element( 
  AttrType* _attr_ptr )
{
  m_attr_ptr = _attr_ptr;
  m_alias = std::string("");
  m_name = std::string("");
  m_has_alias = false;
  m_trace = false;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_vector_port_list_element< AttrType >::bind(
  osss_signal_list_element_base * _a_signal_ptr)
{
  // sanity check
  if(isequal(_a_signal_ptr))
  {
    osss_signal_list_element< data_type > * signal_helper_ptr = 
      dynamic_cast< osss_signal_list_element< data_type > * >(_a_signal_ptr);
    sc_core::sc_signal < data_type > * signal_ptr
        = signal_helper_ptr->getSignal(); 

    m_attr_ptr->bind(*signal_ptr);
  }
  else
    std::cerr << "Error: Type mismatch during binding!" << std::endl;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
bool
osss_vector_port_list_element< AttrType >::isequal(
  osss_signal_list_element_base *base_ptr)
{
  sc_core::sc_signal< data_type >     tmp_signal;
  osss_signal_list_element<data_type> tmp_slb( &tmp_signal, m_alias );

  return tmp_slb.isequal(base_ptr);

}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
bool
osss_vector_port_list_element< AttrType >::hasAlias()
{
  return m_has_alias;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_vector_port_list_element< AttrType >::setAlias(std::string alias)
{
  m_has_alias = true;
  m_alias = alias;
}

template< class AttrType >
inline
std::string
osss_vector_port_list_element< AttrType >::getAlias()
{
  return m_alias;
}

template< class AttrType >
inline
std::string
osss_vector_port_list_element< AttrType >::name()
{
  return m_name;
}

template< class AttrType >
inline
std::string
osss_vector_port_list_element< AttrType >::kind()
{
  return m_attr_ptr->kind();
}

template< class AttrType >
inline
unsigned int
osss_vector_port_list_element< AttrType >::size()
{
  return m_attr_ptr->size();
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_vector_port_list_element< AttrType >::setTraceName(std::string name)
{
  m_trace_name = name;
  m_trace = true;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_vector_port_list_element< AttrType >::setTraceScope(std::string scope_name)
{
  if (m_trace)
    m_trace_name = scope_name + "." + m_trace_name;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void 
osss_vector_port_list_element< AttrType >::trace(sc_core::sc_trace_file* tf)
{
  if (m_trace)
    sc_trace(tf, *m_attr_ptr, m_trace_name);
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
AttrType *
osss_vector_port_list_element< AttrType >::getVectorPort()
{
  return m_attr_ptr;
}

//-------------------------------------------------------------------------//

/*
template< class AttrType >
void
_osss_port_list_element< AttrType >::write(
  osss_signal_list_element_base *base_ptr) {}

template< class AttrType >
void 
_osss_port_list_element< AttrType >::read(
  osss_signal_list_element_base *base_ptr) {}
*/

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_vector_port_list_element< AttrType >::~osss_vector_port_list_element() {}
// $Id: osss_vector_port_list_element.tpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
