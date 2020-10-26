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
#ifndef OSSS_MODIFIED_ROUND_ROBIN_H_INCLUDED_
#define OSSS_MODIFIED_ROUND_ROBIN_H_INCLUDED_

#include "o3s/algorithms/osss_round_robin.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a modified round robin scheduler.
///
/// \note
///
////////////////////////////////////////////////////////////////////////////

class osss_modified_round_robin
  : public osss_round_robin
{
public:
  //! \brief Constructor. 
  //!
  //! The declaration of this constructor is requested for every
  //! scheduler class. The parameter noc must be passed to the constructor of
  //! the ancestor class without any modification.
  explicit
  osss_modified_round_robin( osss_index_type noc )
    : osss_round_robin( noc )
  {
    m_lastGrant = numberOfClients -1;
  }

  //! Overloading this method is required for every scheduler class.
  //! Scheduling works as follows:
  //! All clients are sorted in a fixed order and numbered from 0 to n, with
  //! n = number of clients - 1. If the scheduling method is activated, the
  //! first requesting client following in that order onto a certain start
  //! index m, with 0 <= m <= n, is granted next. Each time a client was
  //! granted the start index m is increased by 1, and, if m + 1 exceeds n,
  //! reset to 0 (i.e., clients are treated as being organised in a logical
  //! ring).
  virtual
  osss_index_type
  schedule( const RequestVector & clientRequests );

};

OSSS_SYNTHESISABLE_END_
OSSS_SYNTHESISABLE_EXPORT_( osss_modified_round_robin );

} // namespace osss

#ifdef OSSS_SYNTHESIS_
#  include "osss_modified_round_robin.cpp"
#endif // OSSS_SYNTHESIS_

#endif // OSSS_MODIFIED_ROUND_ROBIN_H_INCLUDED_
// $Id: osss_modified_round_robin.h 2914 2008-12-15 18:25:24Z philipph $
// :flavour:(osss,H) (osss_recon,h) (osss_icodes,h) (osss_full,h)
