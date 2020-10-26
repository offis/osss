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

#ifndef OSSS_CLEAR_HELPER_SET_INTERFACE_H
#define OSSS_CLEAR_HELPER_SET_INTERFACE_H

#include <iostream>

#include "o3s/utils/osss_class_proxy_interface.h"
#include "o3s/datatypes/osss_object.h"

namespace osss{
namespace osssi{

class osss_clear_helper_set_interface
{
public:
  virtual
  ~osss_clear_helper_set_interface(){}

  virtual
  void
  printOn(std::ostream & os) const = 0;

  /** Walk the class tree from current class to all base
      classes and apply all clear helpers while doing so. */
  virtual
  void
  clearRecursive(      osss_object                * object_ptr,
                 const osss_class_proxy_interface * proxy) const = 0;

};

inline
std::ostream & operator<<(std::ostream & os
                         , const osss_clear_helper_set_interface & object)
{
  object.printOn(os);
  return os;
}

} // end namespace osssi
} // end namespace osss


#endif // OSSS_CLEAR_HELPER_SET_INTERFACE_H

// $Id: osss_clear_helper_set_interface.h 2397 2008-07-01 20:45:43Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
