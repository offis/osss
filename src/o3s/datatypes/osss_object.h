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
#ifndef OSSS_OBJECT_H
#define OSSS_OBJECT_H 1

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_class_proxy_registry.h"

namespace osss {

/** 
 * \class osss_object
 *
 *  This class is intended as a base class for all
 *  contents of osss_context<> and osss_recon<> containers.
 *  In future, this may also be used as a common base
 *  for use within other containers.
 */
class osss_object
{
public:

  /// (empty) destructor
  virtual ~osss_object(){}

  /** This is the very base for internal lock evaluation. See macro definitions
      how to overload it. Don't do it directly. */
  virtual
  bool _internal_lock_release_guard_method_() const
  {
    return true;
  }

protected:
  /**
   * \brief SystemC clocked wait
   * \see sc_core::wait()
   */
  void wait( unsigned int cycles = 1 ) const;
  void wait( sc_core::sc_time const & ) const;

};

} // end namespace osss

#endif // OSSS_OBJECT_H
// $Id: osss_object.h 2394 2008-07-01 20:45:28Z philipph $
// :flavour:(osss_recon,h) (osss,h) (osss_icodes,h) (osss_full,h)
