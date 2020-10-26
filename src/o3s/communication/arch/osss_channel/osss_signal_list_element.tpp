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

#include "o3s/utils/type_id.h"
#include "o3s/utils/osss_report.h"

namespace osss {

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_signal_list_element< AttrType >::osss_signal_list_element(
  sc_core::sc_signal< AttrType > * _signal_ptr, std::string _alias = std::string(""))
{
  m_signal_ptr = _signal_ptr;
  m_alias =_alias;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_signal_list_element< AttrType >::print()
{
  std::cout << osssi::type_id(*m_signal_ptr) << std::endl;
  std::cout << "alias: " << m_alias << std::endl;
  std::cout << "value: " << m_signal_ptr->read() << std::endl;
  std::cout << std::endl;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
sc_core::sc_signal< AttrType > *
osss_signal_list_element< AttrType >::getSignal()
{
  return m_signal_ptr;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >::isequal(
  osss_signal_list_element_base* base_ptr )
{
  if (typeid(*base_ptr) != typeid(*this))
    return false;

  osss_signal_list_element< AttrType > * derived =
    dynamic_cast< osss_signal_list_element< AttrType > * >(base_ptr);
  if (derived == NULL)
    return false;

  if (*(derived->m_signal_ptr) != *m_signal_ptr)
    return false;

  if (derived->m_alias != m_alias)
    return false;

  return true;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
std::string
osss_signal_list_element< AttrType >::getAlias()
{
  return m_alias;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_signal_list_element< AttrType >::trace(sc_core::sc_trace_file* tf)
{
  if (!m_trace_name_list.empty()) {
    std::list< std::string >::iterator iter;
    for(iter = m_trace_name_list.begin();
	iter != m_trace_name_list.end(); ++iter)
      sc_trace( tf, *m_signal_ptr, *iter );
  }
}

template< class AttrType >
inline
void
osss_signal_list_element< AttrType >::trace(sc_core::sc_trace_file* tf,
					    std::string trace_name)
{
  sc_trace( tf, *m_signal_ptr, trace_name);
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_signal_list_element< AttrType >::addTraceName(std::string name)
{
  m_trace_name_list.push_back(name);
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
void
osss_signal_list_element< AttrType >::addToSensitivity(
  sc_core::sc_sensitive * sensitive_ptr)
{
  *sensitive_ptr << *m_signal_ptr;
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
osss_signal_list_element< AttrType >::~osss_signal_list_element()
{/*
  if (m_signal_ptr != NULL)
    delete m_signal_ptr;
  m_signal_ptr = NULL; */
}

//-------------------------------------------------------------------------//

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >::doTypecheck(
  osss_signal_list_element_base * sig)
{
  if (typeid(*sig) != typeid(*this))
    return false;

  osss_signal_list_element< AttrType > * derived =
    dynamic_cast< osss_signal_list_element< AttrType > * >(sig);
  if (derived == NULL)
    return false;

  return true;
}

template< class AttrType >
inline
osss_signal_list_element_base *
osss_signal_list_element< AttrType >::clone() const
{
  return new osss_signal_list_element< AttrType >(*this);
}

template< class AttrType >
inline
osss_signal_list_element< AttrType >::osss_signal_list_element(
  const osss_signal_list_element & sig) : osss_signal_list_element_base()
{
  m_signal_ptr = sig.m_signal_ptr;
  m_alias = sig.m_alias;
  m_trace_name_list = sig.m_trace_name_list;
}

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >::assign(osss_signal_list_element_base * sig)
{
  if (typeid(*sig) != typeid(*this))
    return false;

  osss_signal_list_element< AttrType > * derived =
    dynamic_cast< osss_signal_list_element< AttrType > * >(sig);
  if (derived == NULL)
    return false;

  m_signal_ptr->write(derived->getSignal()->read());

  return true;
}

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >::assign_all_zero()
{
  m_signal_ptr->write(0);

  return true;
}

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >:: bitwise_or(osss_signal_list_element_base * sig)
{
  if (typeid(*sig) != typeid(*this))
    return false;

  osss_signal_list_element< AttrType > * derived =
    dynamic_cast< osss_signal_list_element< AttrType > * >(sig);
  if (derived == NULL)
    return false;

  AttrType rhs = derived->getSignal()->read();
  AttrType lhs = m_signal_ptr->read();

  m_signal_ptr->write(lhs | rhs);

  return true;
}

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >:: bitwise_and(osss_signal_list_element_base * sig)
{
  if (typeid(*sig) != typeid(*this))
    return false;

  osss_signal_list_element< AttrType > * derived =
    dynamic_cast< osss_signal_list_element< AttrType > * >(sig);
  if (derived == NULL)
    return false;

  AttrType rhs = derived->getSignal()->read();
  AttrType lhs = m_signal_ptr->read();

  m_signal_ptr->write(lhs & rhs);

  return true;
}

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >::bitwise_xor(osss_signal_list_element_base * sig)
{
  if (typeid(*sig) != typeid(*this))
    return false;

  osss_signal_list_element< AttrType > * derived =
    dynamic_cast< osss_signal_list_element< AttrType > * >(sig);
  if (derived == NULL)
    return false;

  AttrType rhs = derived->getSignal()->read();
  AttrType lhs = m_signal_ptr->read();

  m_signal_ptr->write(lhs ^ rhs);

  return true;
}

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >:: bitwise_not()
{
  AttrType old_sig = m_signal_ptr->read();

  m_signal_ptr->write(~old_sig);

  return true;
}

template< class AttrType >
inline
void
osss_signal_list_element< AttrType >::bitwise_or(
  std::vector< osss_signal_list_element_base * > * sig_vec)
{
  AttrType sig;

  std::vector< osss_signal_list_element_base * >::iterator sig_iter;
  for(sig_iter = sig_vec->begin(); sig_iter != sig_vec->end(); ++sig_iter)
  {
    osss_signal_list_element< AttrType > * derived =
      dynamic_cast< osss_signal_list_element< AttrType > * >(*sig_iter);

    if (sig_iter == sig_vec->begin())
      sig =  derived->getSignal()->read();
    else
      sig = sig | derived->getSignal()->read();
  }
  m_signal_ptr->write(sig);
}

template< class AttrType >
inline
void
osss_signal_list_element< AttrType >::bitwise_and(
  std::vector< osss_signal_list_element_base * > * sig_vec)
{
  AttrType sig;

  std::vector< osss_signal_list_element_base * >::iterator sig_iter;
  for(sig_iter = sig_vec->begin(); sig_iter != sig_vec->end(); ++sig_iter)
  {
    osss_signal_list_element< AttrType > * derived =
      dynamic_cast< osss_signal_list_element< AttrType > * >(*sig_iter);

    if (sig_iter == sig_vec->begin())
      sig =  derived->getSignal()->read();
    else
      sig = sig & derived->getSignal()->read();
  }
  m_signal_ptr->write(sig);
}

template< class AttrType >
inline
void
osss_signal_list_element< AttrType >::bitwise_xor(
  std::vector< osss_signal_list_element_base * > * sig_vec)
{
  AttrType sig;

  std::vector< osss_signal_list_element_base * >::iterator sig_iter;
  for(sig_iter = sig_vec->begin(); sig_iter != sig_vec->end(); ++sig_iter)
  {
    osss_signal_list_element< AttrType > * derived =
      dynamic_cast< osss_signal_list_element< AttrType > * >(*sig_iter);

    if (sig_iter == sig_vec->begin())
      sig =  derived->getSignal()->read();
    else
      sig = sig ^ derived->getSignal()->read();
  }
  m_signal_ptr->write(sig);
}

template<>
inline
bool
osss_signal_list_element< bool >::isBoolean()
{
  return true;
}

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >::isBoolean()
{
  return false;
}

template<>
inline
bool
osss_signal_list_element< bool >::getBoolean()
{
  return m_signal_ptr->read();
}

template< class AttrType >
inline
bool
osss_signal_list_element< AttrType >::getBoolean()
{
  OSSS_REPORT_ERROR( osss::report::channel_boolean_signal_invalid )
    % m_signal_ptr->name() % osssi::type_id::from<AttrType>();
  return false;
}

template< class AttrType >
inline
osss_signal_list_element_base&
osss_signal_list_element< AttrType >::operator= (
  osss_signal_list_element_base & sig)
{
  osss_signal_list_element< AttrType > * derived =
    dynamic_cast< osss_signal_list_element< AttrType > * >(&sig);

  m_signal_ptr = derived->m_signal_ptr;
  m_alias = derived->m_alias;
  m_trace_name_list = derived->m_trace_name_list;
  return *this;
}

} // namespace osss

// $Id: osss_signal_list_element.tpp 3023 2009-03-04 22:55:26Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
