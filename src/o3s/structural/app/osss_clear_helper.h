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

#ifndef OSSS_CLEAR_HELPER_H
#define OSSS_CLEAR_HELPER_H

#include "osss_clear_helper_interface.h"

namespace osss{
namespace osssi{

/** This class provides the ability to clear a
    certain member of type A within an object
    of type T. */
template< class T, class A >
class osss_clear_helper : public osss_clear_helper_interface< T >
{
  private:
  /// Value to be written during clear()
  A clear_value;

  public:
  /** Constructor - Simply pass the value to be written
      when performing clear() */
  explicit osss_clear_helper(A val);

  virtual
  void
  printOn(std::ostream & os) const;

  /** Clear the attribute at the given offset within the
      given object. */
  virtual
  void
  clear(T * object_ptr, offset_type offset) const;
};

} // end namespace osssi
} // end namespace osss

#include "osss_clear_helper.tpp"

#endif // OSSS_CLEAR_HELPER_H

// $Id: osss_clear_helper.h 2394 2008-07-01 20:45:28Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
