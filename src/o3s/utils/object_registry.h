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
#ifndef OSSS_OBJECT_REGISTRY_H_INCLUDED
#define OSSS_OBJECT_REGISTRY_H_INCLUDED

#include "o3s/utils/object_container.h"
#include "o3s/utils/noncopyable.h"
#include <map>

namespace osss {
namespace osssi {

/**
 * \brief Global object/container registry singleton
 *
 * This class implements a global registry for user-objects
 * and their respective containers.
 *
 * Every object container should 'subscribe()' itself on
 * construction, and 'unsubscribe()' in its destructor
 * and update() the mapping on each substitution of
 * the content object (at least, if its address changes).
 *
 * \note The subscribe() call requires, that an object
 *       is already present in the calling container.
 *       You need to delay the subscription, until
 *       a content object is available.
 *
 * \see object_container
 */
class object_registry
  : private noncopyable
{
public:
  typedef object_registry  this_type;
  typedef object_container container_type;

  typedef container_type*               container_handle;
  typedef container_type::object_handle object_handle;

  void subscribe( container_type&, bool update = false );
  void unsubscribe( container_type& );

  container_handle find( object_handle )   const;
  object_handle    find( container_type& ) const;

  /// singleton class - returns THE instance of this class
  static this_type& instance();

private:
  /// private default constructor - singleton class
  object_registry(){}

  /// mapping between objects and their containers
  typedef std::map< object_handle, container_handle > map_type;
  typedef map_type::iterator       iterator;
  typedef map_type::const_iterator const_iterator;

  map_type map_;

}; // object registry

} // namespace osssi
} // namespace osss

#endif // OSSS_OBJECT_REGISTRY_H_INCLUDED
// $Id: object_registry.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_full,s)
