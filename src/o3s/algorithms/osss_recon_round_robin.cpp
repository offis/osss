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

#include "osss_recon_round_robin.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

osss_recon_round_robin::osss_recon_round_robin()
  : osss_recon_scheduler()
{
  m_last_grant = getNumberOfUserProcesses() - 1;
}

//-----------------------------------------------------------------------//

osss_recon_job_idx
osss_recon_round_robin::schedule()
{
  osss_recon_job_idx nextGrant = m_last_grant;

  for ( unsigned int i = 0; i < getNumberOfUserProcesses(); ++i )
  {
    if ( m_last_grant == ( getNumberOfUserProcesses() - 1 ) ) {
      nextGrant = 0;
    } else {
      nextGrant = nextGrant + 1;
    }
    if ( jobs[nextGrant].m_valid )
    {
      break;
    }
  }
  m_last_grant = nextGrant;
  return  nextGrant;
}

OSSS_SYNTHESISABLE_END_
} // end namespace osss

//-----------------------------------------------------------------------//
// $Id: osss_recon_round_robin.cpp 2916 2008-12-15 18:25:30Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s) (fossy,H)
