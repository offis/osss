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
#include "o3s/algorithms/osss_no_scheduler.h"
#include "o3s/utils/osss_report.h"
#include "o3s/utils/osss_assert.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

osss_index_type
osss_no_scheduler::schedule( const RequestVector & clientRequests )
{
  osss_index_type grant = 0;

  for ( osss_index_type id = 0; id < numberOfClients; id++ )
  {
    // ensure, that only one request 
    OSSS_ASSERT( !grant || clientRequests[id] == 0 );

    // update next_grant with a plain logical OR
    // (which assumes implicitly, that only one request is present)
    grant |= ( ( clientRequests[id] ) ? id : 0 ) ;

    // deliberately no break, to improve synthesis results
  }
  return grant;

}

OSSS_SYNTHESISABLE_END_
} //namespace osss

// $Id: osss_no_scheduler.cpp 2914 2008-12-15 18:25:24Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_full,s) (fossy,H)
