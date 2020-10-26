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

#ifndef OSSS_CLASS_PROXY_REGISTRY_H
#define OSSS_CLASS_PROXY_REGISTRY_H

#include "osss_class_proxy_interface.h"
#include "o3s/utils/type_id.h"

#include <map>

namespace osss{
namespace osssi{

/** This class maintains a registry of class proxies. One may look up
    a proxy by providing the C++ RTTI type_info. This proxy gives
    the caller additional abilities. */
class osss_class_proxy_registry
{
private:
  /// current type
  typedef osss_class_proxy_registry  this_type;
  /// base class of all class proxies
  typedef osss_class_proxy_interface proxy_type;
  /// Map which associates a proxy pointer to a type_info pointer
  typedef std::map< type_id, proxy_type * > registry_type;

  /// Registry instance
  registry_type m_registry;

public:

  /**
   * \brief singleton pattern - get instance
   *
   * Call methods on the returned object of this method,
   * if you want to use the registry.
   */
  inline
  static this_type& instance();

  /**
   * \brief create and add proxy
   *
   * Creates and adds a proxy for the class T.
   *
   * The object "t" is not relevant and may well
   * be NULL. It is just used to make the C++
   * compiler automatically select T when doing
   * things like "addProxy(this)". The proxy
   * is destructed automatically when this object
   * dies.
   */
  template< class T >
  osss_class_proxy_interface *
  addProxy(T * t = NULL);

  /** Lookup a proxy for a given C++ RTTI type_info */
  osss_class_proxy_interface *
  findProxy( type_id for_type ) const;

  /** Use a proxy to get a clone of a given object */
  template< class T >
  static
  T* getClone( const T& );

  /// cleans up registered proxies
  ~osss_class_proxy_registry();

private:
  /**
   * \brief private default constructor
   *
   * This class is a singleton, therefore has a private constructor.
   * \see instance()
   */
  osss_class_proxy_registry();

  /// disabled copy constructor (not implemented)
  osss_class_proxy_registry( const this_type& );

  /**
   * This is the static and only instance of the registry
   */
  static this_type* instance_;
};

inline
osss_class_proxy_registry&
osss_class_proxy_registry::instance()
{
  if (! this_type::instance_ )
    this_type::instance_ = new this_type;
  return *this_type::instance_;
}

} // end namespace osssi
} // end namespace osss

#include "osss_class_proxy_registry.tpp"

#endif // OSSS_CLASS_PROXY_REGISTRY_H
// $Id: osss_class_proxy_registry.h 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
