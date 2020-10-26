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

#include "osss_object.h"
#include "o3s/utils/object_container.h"
#include "o3s/utils/object_registry.h"
#include "o3s/utils/osss_process_handle.h"

namespace osss {

void
osss_object::wait( unsigned int n ) const
{
  // wait for clock of current object container
  // -> reports an error, in case we have no current container!
  //
  // TODO: improve container lookup, it's currently quite inefficient
  osssi::object_container * holder
    = osssi::object_container::registry().find( this );

  if( holder ) {
    // let container handle the wait
    // -> reports an error, in case of an invalid consumer
    holder->wait( n );
  } else {
    // fallback to plain implementation
    if( ! osssi::osss_process_handle::wait( n ) ) {
      SC_REPORT_FATAL( "OSSS Object",
                       "Invalid wait() used within user-object!" );
    }
  }
}

/// TODO: implement EET's consistently
void
osss_object::wait( sc_core::sc_time const & delay ) const //,
                  // osssi::source_pos const & = osssi::source_pos::missing ) const
{
  // wait according to time consumer of current object container
  // -> reports an error, in case we have no current container!
  //
  // TODO: improve container lookup, it's not optimal
  osssi::object_container * holder
    = osssi::object_container::registry().find( this );

  if( holder ) {
    // let container handle the wait
    // -> reports an error, in case of an invalid consumer
    holder->wait( delay );
  } else {
    // fallback to plain implementation
    if( ! osssi::osss_process_handle::wait( delay ) ) {
      SC_REPORT_FATAL( "OSSS Object",
                       "Invalid wait() used within user-object!" );
    }
  }
}

} // end namespace osss

// $Id: osss_object.cpp 2394 2008-07-01 20:45:28Z philipph $
// :flavour:(osss_recon,s) (osss,s) (osss_icodes,s) (osss_full,s)
