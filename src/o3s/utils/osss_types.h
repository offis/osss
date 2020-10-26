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

#ifndef OSSS_TYPES_H_INCLUDED_
#define OSSS_TYPES_H_INCLUDED_

#include "osss_common.h"

#include <cstddef> // std::size_t

// forward declaration
namespace sc_core {
class sc_time;
}

namespace osss {

  enum osss_sensitivity
  {
    OSSS_POS = 0, //!< sensitive on the positive edge of the reset signal 
                  //!< (active high reset)
    OSSS_NEG,     //!< sensitive on the negative edge of the reset signal
                  //!< (active low reset)
    OSSS_LEVEL    //!< sensitive on the level (high or low) of the 
                  //!< reset signal.
  };

  typedef sc_core::sc_time osss_time_type;

  typedef unsigned int     osss_priority;
  typedef std::size_t      osss_index_type;

  typedef void *           osss_id_type;
  typedef bool *           osss_request_vector;

  /* ---- deprecated old names - to be removed soon (tm) ---- */
  typedef osss_id_type        IDType;
  typedef osss_request_vector RequestVector;


} // namespace osss

#endif // OSSS_TYPES_H_INCLUDED_

// $Id: osss_types.h 2919 2008-12-15 18:25:38Z philipph $
// :flavour: (all,H)
