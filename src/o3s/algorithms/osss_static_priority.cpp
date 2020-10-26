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

#include "osss_static_priority.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

//----------------------------------------------------------------------//

template< /* bool ZeroIsHighestPriority */ >
osss_index_type
osss_static_priority<true>::schedule( const RequestVector &clientRequests )
{
  for(osss_index_type i = 0; i < numberOfClients; i++)
  {
    if( clientRequests[i] )
      return i;
  }
  return 0; // fallback, to shut up the compiler
}

//----------------------------------------------------------------------//

template< /* bool ZeroIsHighestPriority */ >
osss_index_type
osss_static_priority<false>::schedule( const RequestVector &clientRequests )
{
  for(osss_index_type i = numberOfClients-1; i > 0; --i)
  {
    if(clientRequests[i])
      return i;
  }
  // clientRequests[0] doesn't matter, we return 0 anyway
  return 0;
}

//----------------------------------------------------------------------//
OSSS_SYNTHESISABLE_END_
} // end namespace osss

// $Id: osss_static_priority.cpp 2914 2008-12-15 18:25:24Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s) (fossy,H)
