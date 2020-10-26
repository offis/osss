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

namespace osss {
namespace osssi {

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_port_list_element< AttrType >::osss_port_list_element(
  AttrType* _attr_ptr, std::string _alias)
{
  m_attr_ptr = _attr_ptr;
  m_alias = _alias;
  m_name = _alias;  // to do: change to _name
  m_has_alias = true;
  m_trace = false;
  m_isBound = false;
  m_boundSignal = NULL;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_port_list_element< AttrType >::osss_port_list_element(
  AttrType* _attr_ptr )
{
  m_attr_ptr = _attr_ptr;
  m_alias = std::string("");
  m_name = std::string("");
  m_has_alias = false;
  m_trace = false;
  m_isBound = false;
  m_boundSignal = NULL;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_port_list_element< AttrType >::osss_port_list_element(
  const osss_port_list_element< AttrType >& port_list) : osss_port_list_element_base()
{
  m_attr_ptr = new AttrType;
  m_alias = port_list.m_alias;
  m_name = port_list.m_name;
  m_has_alias = port_list.m_has_alias;
  m_trace = port_list.m_trace;
  m_isBound = port_list.m_isBound;
  m_boundSignal = port_list.m_boundSignal;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::generateSignal(
  std::vector< osss_signal_list_element_base * > & _signal_list)
{
  sc_core::sc_signal< data_type > * tmpSignal = new sc_core::sc_signal< data_type >;
  osss_signal_list_element_base * slb =
    new osss_signal_list_element< data_type >(tmpSignal, m_alias);
  _signal_list.push_back(slb);
}

template< class AttrType >
inline
osss_signal_list_element_base *
osss_port_list_element< AttrType >::newSignal()
{
  sc_core::sc_signal< data_type > * tmpSignal = new sc_core::sc_signal< data_type >;
  return new osss_signal_list_element< data_type >(tmpSignal, m_alias);
}

template< class AttrType >
inline
osss_signal_list_element_base *
osss_port_list_element< AttrType >::getBoundSignal()
{
  if (!m_isBound)
  {
    m_boundSignal = newSignal();
    bind(m_boundSignal);
  }
  return m_boundSignal;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::bind(
  osss_signal_list_element_base * _a_signal_ptr)
{
  // sanity check
  if(isequal(_a_signal_ptr))
  {
    osss_signal_list_element< data_type > * signal_helper_ptr =
      dynamic_cast< osss_signal_list_element< data_type > * >(_a_signal_ptr);
    sc_core::sc_signal < data_type > * signal_ptr = signal_helper_ptr->getSignal();

    m_attr_ptr->bind(*signal_ptr);
    m_boundSignal = _a_signal_ptr;
    m_isBound = true;
  }
  else
    std::cerr << "Error: Type mismatch during binding!" << std::endl;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::trace(
  osss_signal_list_element_base * _a_signal_ptr)
{
  if (m_trace)
    _a_signal_ptr->addTraceName(m_trace_name);
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
bool
osss_port_list_element< AttrType >::isequal(osss_port_list_element_base *base_ptr)
{
  if (typeid(*base_ptr) != typeid(*this))
    return false;

  osss_port_list_element< AttrType > * derived =
    dynamic_cast< osss_port_list_element< AttrType > * >(base_ptr);
  if (derived == NULL)
    return false;

  if (*(derived->m_attr_ptr) != *m_attr_ptr)
    return false;

  if (derived->m_alias != m_alias)
    return false;

  return true;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
bool
osss_port_list_element< AttrType >::isequal(
  osss_signal_list_element_base *base_ptr)
{
  sc_core::sc_signal< data_type > * tmp_signal = new sc_core::sc_signal< data_type >;
  osss_signal_list_element_base * tmp_slb =
    new osss_signal_list_element< data_type >(tmp_signal, m_alias);
  bool ret_value = tmp_slb->isequal(base_ptr);

  delete tmp_signal;
  delete tmp_slb;
  return ret_value;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
bool
osss_port_list_element< AttrType >::hasAlias()
{
  return m_has_alias;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::setAlias(std::string alias)
{
  m_has_alias = true;
  m_alias = alias;
}

template< class AttrType >
inline
std::string
osss_port_list_element< AttrType >::getAlias()
{
  return m_alias;
}

template< class AttrType >
inline
std::string
osss_port_list_element< AttrType >::name()
{
  return m_name;
}

template< class AttrType >
inline
std::string
osss_port_list_element< AttrType >::kind()
{
  return m_attr_ptr->kind();
}

template< class AttrType >
inline
int
osss_port_list_element< AttrType >::size()
{
  return m_attr_ptr->size();
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::setTraceName(std::string name)
{
  m_trace_name = name;
  m_trace = true;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::setTraceScope(std::string scope_name)
{
  if (m_trace)
    m_trace_name = scope_name + "." + m_trace_name;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::trace(sc_core::sc_trace_file* tf)
{
  if (m_trace)
    sc_core::sc_trace(tf, *m_attr_ptr, m_trace_name);
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
AttrType *
osss_port_list_element< AttrType >::getPort()
{
  return m_attr_ptr;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::print()
{
  std::cout << osssi::type_id(*m_attr_ptr) << std::endl;

  if (m_has_alias)
    std::cout << "alias: " <<  m_alias << std::endl;

  std::cout << "direction: " << m_attr_ptr->kind() << std::endl;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::write(
  osss_signal_list_element_base * // base_ptr
                                          ) {}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::read(
  osss_signal_list_element_base * // base_ptr
                                         ) {}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_port_list_element< AttrType >::~osss_port_list_element() {}


} // namespace osssi


//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_port_list_element< AttrType >::osss_port_list_element(
  AttrType* _attr_ptr, std::string _alias)
  : osssi::osss_port_list_element<AttrType>(_attr_ptr, _alias)
{}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_port_list_element< AttrType >::osss_port_list_element(
  AttrType* _attr_ptr )
  : osssi::osss_port_list_element<AttrType>(_attr_ptr)
{}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_port_list_element_base*
osss_port_list_element< AttrType >::clone() const
{
  return this;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::write(
  osss_signal_list_element_base *base_ptr) {}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_port_list_element< AttrType >::read(
  osss_signal_list_element_base *base_ptr) {}




//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element< sc_core::sc_out< T > >::osss_port_list_element(
  sc_core::sc_out< T >* _attr_ptr, std::string _alias)
  : osssi::osss_port_list_element<sc_core::sc_out<T> >(_attr_ptr, _alias)

{}

//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element< sc_core::sc_out< T > >::osss_port_list_element(
  sc_core::sc_out< T >* _attr_ptr )
  : osssi::osss_port_list_element<sc_core::sc_out<T> >(_attr_ptr)
{}

//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element< sc_core::sc_out< T > >::osss_port_list_element(
  const osss_port_list_element< sc_core::sc_out< T > >& port_list) : osssi::osss_port_list_element< sc_core::sc_out< T > >(port_list)
{
   osssi::osss_port_list_element< sc_core::sc_out< T > >::m_attr_ptr = new sc_core::sc_out< T >;
   osssi::osss_port_list_element< sc_core::sc_out< T > >::m_alias = port_list.m_alias;
   osssi::osss_port_list_element< sc_core::sc_out< T > >::m_name = port_list.m_name;
   osssi::osss_port_list_element< sc_core::sc_out< T > >::m_has_alias = port_list.m_has_alias;
   osssi::osss_port_list_element< sc_core::sc_out< T > >::m_trace = port_list.m_trace;
   osssi::osss_port_list_element< sc_core::sc_out< T > >::m_isBound = port_list.m_isBound;
   osssi::osss_port_list_element< sc_core::sc_out< T > >::m_boundSignal = port_list.m_boundSignal;
}

//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element_base*
osss_port_list_element< sc_core::sc_out< T > >::clone() const
{
  return new osss_port_list_element< sc_core::sc_out< T > >(*this);
}

//-------------------------------------------------------------------------//

template< class T >
inline
void
osss_port_list_element< sc_core::sc_out< T > >::write(
  osss_signal_list_element_base *base_ptr)
{
  //if (this->isequal(base_ptr) == true)
  //{
    osss_signal_list_element< T > * signalList_ptr =
      dynamic_cast< osss_signal_list_element< T > * >(base_ptr);
    if (signalList_ptr == NULL)
      std::cerr << "osss_port_list_element: "
                   "dynamic cast failed during reading!"
                << std::endl;
    else
      osssi::osss_port_list_element< sc_core::sc_out< T > >::m_attr_ptr->
        write(signalList_ptr->getSignal()->read());
  //}
  //else
  //std::cerr << "osss_port_list_element: "
  //             "type mismatch during writing!" << std::endl;
}

//-------------------------------------------------------------------------//

template< class T >
inline
void
osss_port_list_element< sc_core::sc_out< T > >::read(
  osss_signal_list_element_base * // base_ptr
                                           ) {}




//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element< sc_core::sc_in< T > >::osss_port_list_element(
  sc_core::sc_in< T >* _attr_ptr, std::string _alias)
  : osssi::osss_port_list_element<sc_core::sc_in<T> >(_attr_ptr, _alias)
{}

//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element< sc_core::sc_in< T > >::osss_port_list_element(
  sc_core::sc_in< T >* _attr_ptr )
  : osssi::osss_port_list_element<sc_core::sc_in<T> >(_attr_ptr)
{}

//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element< sc_core::sc_in< T > >::osss_port_list_element(
  const osss_port_list_element< sc_core::sc_in< T > >& port_list) : osssi::osss_port_list_element< sc_core::sc_in< T > >( port_list )
{
  osssi::osss_port_list_element< sc_core::sc_in< T > >::m_attr_ptr = new sc_core::sc_in< T >;
  osssi::osss_port_list_element< sc_core::sc_in< T > >::m_alias = port_list.m_alias;
  osssi::osss_port_list_element< sc_core::sc_in< T > >::m_name = port_list.m_name;
  osssi::osss_port_list_element< sc_core::sc_in< T > >::m_has_alias = port_list.m_has_alias;
  osssi::osss_port_list_element< sc_core::sc_in< T > >::m_trace = port_list.m_trace;
  osssi::osss_port_list_element< sc_core::sc_in< T > >::m_isBound = port_list.m_isBound;
  osssi::osss_port_list_element< sc_core::sc_in< T > >::m_boundSignal = port_list.m_boundSignal;
}

//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element_base*
osss_port_list_element< sc_core::sc_in< T > >::clone() const
{
  return new osss_port_list_element< sc_core::sc_in< T > >(*this);
}

//-------------------------------------------------------------------------//

template< class T >
inline
void
osss_port_list_element< sc_core::sc_in< T > >::read(
  osss_signal_list_element_base *base_ptr)
{
  //if (this->isequal(base_ptr) == true)
  //{
    osss_signal_list_element< T > * signalList_ptr =
      dynamic_cast< osss_signal_list_element< T > * >(base_ptr);
    if (signalList_ptr == NULL)
      std::cerr << "osss_port_list_element: "
                   "dynamic cast failed during reading!"
                << std::endl;
    else
      signalList_ptr->getSignal()->write(
        osssi::osss_port_list_element< sc_core::sc_in< T > >:: m_attr_ptr->read());
  //}
  //else
  //std::cerr << "osss_port_list_element: "
  //             "type mismatch during reading!" << std::endl;
}

//-------------------------------------------------------------------------//

template< class T >
inline
void
osss_port_list_element< sc_core::sc_in< T > >::write(
  osss_signal_list_element_base * // base_ptr
                                           ) {}




//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element< sc_core::sc_inout< T > >::osss_port_list_element(
  sc_core::sc_inout< T >* _attr_ptr, std::string _alias)
  : osssi::osss_port_list_element<sc_core::sc_inout<T> >(_attr_ptr, _alias)
{}

//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element< sc_core::sc_inout< T > >::osss_port_list_element(
  sc_core::sc_inout< T >* _attr_ptr )
  : osssi::osss_port_list_element<sc_core::sc_inout<T> >(_attr_ptr)
{}

//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element< sc_core::sc_inout< T > >::osss_port_list_element(
  const osss_port_list_element< sc_core::sc_inout< T > >& port_list)
{
  osssi::osss_port_list_element< sc_core::sc_inout< T > >::m_attr_ptr = new sc_core::sc_inout< T >;
  osssi::osss_port_list_element< sc_core::sc_inout< T > >::m_alias = port_list.m_alias;
  osssi::osss_port_list_element< sc_core::sc_inout< T > >::m_name = port_list.m_name;
  osssi::osss_port_list_element< sc_core::sc_inout< T > >::m_has_alias = port_list.m_has_alias;
  osssi::osss_port_list_element< sc_core::sc_inout< T > >::m_trace = port_list.m_trace;
  osssi::osss_port_list_element< sc_core::sc_inout< T > >::m_isBound = port_list.m_isBound;
  osssi::osss_port_list_element< sc_core::sc_inout< T > >::m_boundSignal = port_list.m_boundSignal;
}

//-------------------------------------------------------------------------//

template< class T >
inline
osss_port_list_element_base*
osss_port_list_element< sc_core::sc_inout< T > >::clone() const
{
  return new osss_port_list_element< sc_core::sc_inout< T > >(*this);
}

//-------------------------------------------------------------------------//

template< class T >
inline
void
osss_port_list_element< sc_core::sc_inout< T > >::write(
  osss_signal_list_element_base *base_ptr)
{
  //if (this->isequal(base_ptr) == true)
  //{
    osss_signal_list_element< T > * signalList_ptr =
      dynamic_cast< osss_signal_list_element< T > * >(base_ptr);
    if (signalList_ptr == NULL)
      std::cerr << "osss_port_list_element: "
                   "dynamic cast failed during reading!"
                << std::endl;
    else
      osssi::osss_port_list_element< sc_core::sc_inout< T > >::m_attr_ptr->
        write(signalList_ptr->getSignal()->read());
  //}
  //else
  //std::cerr << "osss_port_list_element: "
  //             "type mismatch during writing!" << std::endl;
}

//-------------------------------------------------------------------------//

template< class T >
inline
void
osss_port_list_element< sc_core::sc_inout< T > >::read(
  osss_signal_list_element_base *base_ptr)
{
  //if (this->isequal(base_ptr) == true)
  //{
    osss_signal_list_element< T > * signalList_ptr =
      dynamic_cast< osss_signal_list_element< T > * >(base_ptr);
    if (signalList_ptr == NULL)
      std::cerr << "osss_port_list_element: "
                   "dynamic cast failed during reading!" << std::endl;
    else
      signalList_ptr->getSignal()->write(
        osssi::osss_port_list_element< sc_core::sc_inout< T > >::m_attr_ptr->read());
  //}
  //else
  //std::cerr << "osss_port_list_element: "
  //             "type mismatch during reading!" << std::endl;
}

} // namespace osss

//-------------------------------------------------------------------------//
// $Id: osss_port_list_element.tpp 3023 2009-03-04 22:55:26Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
