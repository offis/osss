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

#ifndef OSSS_OBJECT_CONTAINER_H_INCLUDED
#define OSSS_OBJECT_CONTAINER_H_INCLUDED

#include "o3s/utils/osss_types.h"
#include <string>

namespace osss {

// forward declaration of user-class base class
class osss_object;

namespace osssi {

// forward declarations
class object_registry;
class time_consumer_base;

/**
 * \brief base class for object containers
 *
 */
class object_container
{

  friend class osss::osss_object;
  friend class object_registry;

public:
  /**
   * \brief get global container/object registry
   * \note Internal use only.
   */
  static object_registry& registry();

  /**
   * \brief generic type to reference user objects
   *
   * This is currently \c void, but should be the
   * common base class, in future versions.
   *
   * \see osss_object
   */
  typedef void const * const object_handle;

  virtual
  std::string const & if_typename() const = 0;

protected:
  object_container()
    : registered_( false )
  {}

  virtual object_handle       content() const = 0;
  virtual time_consumer_base* time_consumer() const;

  virtual void wait( unsigned int n = 1 );
  virtual void wait( osss_time_type const & );

  void registry_subscribe();
  void registry_unsubscribe();

  /// no polymorphic destruction - protected destructor
  virtual // only virtual, to shut up GCC
  ~object_container();

private:
  bool registered_;

}; // class object_container

} // namespace osssi
} // namespace osss

#endif //  OSSS_OBJECT_CONTAINER_H_INCLUDED

// $Id: object_container.h 3021 2009-03-04 22:55:21Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_full,h)
