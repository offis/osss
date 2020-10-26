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

#include "object_registry.h"
#include "osss_systemc.h"
#include "osss_common.h"
#include "osss_report.h"

namespace osss {
namespace osssi {

object_registry&
object_registry::instance()
{
  // there can be only one (not MT-safe)
  static this_type mac_leod;
  return mac_leod;
}

/* ------------------------------------------------------------------ */

void
object_registry::subscribe( container_type& cnt, bool update )
{
  object_handle current  = cnt.content();
  if( osss_unlikely( !current ) ) {
     //TODO: need specific error message here
     OSSS_REPORT_FATAL( osss::report::internal_error )
       % "Missing user object!";
  }

  if ( update ) {
    // TODO: check registry, even if !update ?
    if ( object_handle previous = find( cnt ) ) {
      map_.erase( previous );
    }
  }
  // store element in registry
  map_[ current ] = &cnt;
}

void
object_registry::unsubscribe( container_type& cnt )
{
  object_handle current  = cnt.content();

  // try to remove from map
  if( ! map_.erase( current ) ) {
    // TODO: add warning - current object does not match container

    // try harder: unsubscribe by container
    if( object_handle previous = find( cnt ) ) {
      // container is still in registry - delete it
      map_.erase( previous );
    }
  }
}

/* ------------------------------------------------------------------ */

object_registry::container_handle
object_registry::find( object_handle obj ) const
{
  const_iterator pos = map_.find( obj );
  if( pos != map_.end() ) {
    return pos->second;
  }
  // no container found - return NULL
  return 0;
}

object_registry::object_handle
object_registry::find( container_type& cnt ) const
{
  for( const_iterator it = map_.begin(); it != map_.end(); ++it ) {
    if( &cnt == it->second )
      return it->first;
  }
  // no object for this container found - return NULL
  return 0;
}

} // namespace osssi
} // namespace osss

// $Id: object_registry.cpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_full,s)
