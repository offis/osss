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

#include "osss_device_modified_round_robin.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

//-----------------------------------------------------------------------//

osss_device_job_idx
osss_device_modified_round_robin::schedule()
{
  if (! m_last_grant_initialised)
  {
    m_last_grant = getNumberOfReconObjects() - 1;
    m_last_grant_initialised = true;
  }

  osss_device_job_idx next_grant = m_last_grant;
  for ( osss_device_job_idx i = 0; i < getNumberOfReconObjects(); ++i )
  {
    if ( next_grant == ( getNumberOfReconObjects() - 1 ) )
    {
      next_grant = 0;
    }
    else
    {
      next_grant = next_grant + 1;
    }
    if ( jobs[next_grant].m_valid )
    {
      break;
    }
  }
  if( m_last_grant != next_grant)
  {
    if ( m_last_grant == ( getNumberOfReconObjects() - 1 ) )
    {
      m_last_grant = 0;
    } else {
      m_last_grant = m_last_grant + 1;
    }
  }
  return next_grant ;
}

OSSS_SYNTHESISABLE_END_
} // namespace osss

//-----------------------------------------------------------------------//
// $Id: osss_device_modified_round_robin.cpp 2918 2008-12-15 18:25:36Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s) (fossy,H)
