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

#include "object_container.h"
#include "object_registry.h"
#include "o3s/utils/time_consumer.h"
#include "o3s/utils/osss_process_handle.h"

namespace osss {
namespace osssi {

object_registry&
object_container::registry()
{
  return object_registry::instance();
}

void
object_container::registry_subscribe()
{
  object_container::registry().subscribe( *this, registered_ );
  registered_ = true;
}

void
object_container::registry_unsubscribe()
{
  if( registered_ )
    object_container::registry().unsubscribe( *this );
  registered_ = false;
}

time_consumer_base*
object_container::time_consumer() const
{
  static no_time_consumer no_consumer;
  return &no_consumer;
}

void
object_container::wait( unsigned int cycles )
{
  if( ! osss_process_handle::wait( cycles ) )
    time_consumer()->wait( cycles );
}

void
object_container::wait( osss_time_type const & delay )
{
  if( ! osss_process_handle::wait( delay ) )
    time_consumer()->wait( delay );
}

object_container::~object_container()
{
  OSSS_ASSERT( ! registered_ );
}

} // namespace osssi
} // namespace osss

// $Id: object_container.cpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_full,s)
