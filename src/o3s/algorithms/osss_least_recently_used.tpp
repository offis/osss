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

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

template< osss_index_type MaxClients >
inline
osss_least_recently_used<MaxClients>::
 osss_least_recently_used( osss_index_type noc )
  : osss_scheduler( noc )
{
  for(osss_index_type i = 0; i < MaxClients; ++i)
  {
    history[i] = i;
  }
}

//-----------------------------------------------------------------------//

template< osss_index_type MaxClients >
osss_index_type
osss_least_recently_used<MaxClients>::
 schedule( const RequestVector &clientRequests )
{
  osss_index_type grant = 0;
  for(osss_index_type i = 0; i < numberOfClients; i++)
  {
    if(clientRequests[ history[i] ])
    {
      grant = history[i];
      for(osss_index_type j = i; j < numberOfClients - 1; j++)
      {
        history[j] = history[j + 1];
      }
      history[numberOfClients - 1] = grant;
      break;
    }
  }
  return grant;
}

OSSS_SYNTHESISABLE_END_
} // namespace osss

// $Id: osss_least_recently_used.tpp 2950 2009-01-05 15:59:31Z claus $
// :flavour:(osss,H) (osss_recon,h) (osss_icodes,h) (osss_full,h)
