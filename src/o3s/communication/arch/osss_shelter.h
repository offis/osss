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
#ifndef OSSS_SHELTER_HH
#define OSSS_SHELTER_HH

#include <vector>

namespace osss{
namespace osssi{

/** This class is intended to be used by osss_socket class.
    It provides survival ability for a member attribute
    when one object is destructed and a fresh one is constructed
    directly afterwards. The member attribute has to be a pointer.

    The principle is simple: Encapsule the member in a osss_shelter
    container. When the enclosing object dies, the shelter's
    destructor is called. This will put the content on a global
    stack ("shelter_stack", see below). When a new object is
    constructed, the value is fetched from that stack.
    If the stack is empty on construction, the value NULL is used.
    */
template< class T >
class osss_shelter
{
  private:
  /** Value for ordinary use. This will be saved upon destruction
      and restored (if possible) during next construction. */
  T value;

  public:
  osss_shelter();
  ~osss_shelter();

  /// Assignment (write) operator
  void operator=(const T & rhs);

  /// Implicit cast operator (for read)
  operator T& ();
};

/// Stack for shelter values
extern std::vector<void*> shelter_stack;
} // namespace osssi
} // namespace osss

#include "o3s/communication/arch/osss_shelter.tpp"

#endif // OSSS_SHELTER_HH
// $Id: osss_shelter.h 2394 2008-07-01 20:45:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
