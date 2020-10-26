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

#include "type_id.h"
#include "osss_common.h"

#include <string>

// demangling currently supported for GCC >= 3.1.x
#if defined( OSSS_GCC_ )
#if OSSS_GCC_ >= OSSS_MAKE_VERSION(3,1,0)

#  define OSSS_HAVE_CXA_DEMANGLE_
#  include <cstdlib>
#  include <cxxabi.h>

#endif
#endif // GCC >= 3.1

namespace osss {
namespace osssi {

const type_id type_id::none;

std::string
type_id::name() const
{
#ifdef OSSS_HAVE_CXA_DEMANGLE_

  int status;
  char * raw = abi::__cxa_demangle( ptr_->name(), 0, 0, &status);

  if( status<0 ) {
    free( raw );
    return ptr_->name(); // fall-back
  }

  std::string result = raw;
  free(raw);
  return result;

#else
  return ptr_->name();   // fall-back
#endif
}

std::ostream&
operator << ( std::ostream& os, type_id t )
{
  os << t.name();
  return os;
}

} // namespace osssi
} // namespace osss

#undef OSSS_HAVE_CXA_DEMANGLE_
/* Taf!
 * :flavour: (all,s)
 */
