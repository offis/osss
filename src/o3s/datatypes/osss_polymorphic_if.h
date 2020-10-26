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
#ifndef OSSS_POLYMORPHIC_IF_H
#define OSSS_POLYMORPHIC_IF_H

namespace osss {


/**
 * \interface osss_polymorphic_if
 *
 * \brief This class is the interface of a polymorphic object.
 *        It becomes necessary when a polymorphic object is used inside
 *        a shared object.
 *
 * \see osss_polymorphic
 */
class osss_polymorphic_if
    : public virtual sc_core::sc_interface
{
public:

  //! Returns the interface of the object inside the polymorphic object.
  //! If the object has no interface of type sc_interface it returns NULL.
  virtual
  sc_core::sc_interface*
  get_interface() const = 0;

  osss_polymorphic_if(){}

  osss_polymorphic_if( const osss_polymorphic_if & )
    : sc_core::sc_interface()
  {}

};

} // namespace osss

#endif // OSSS_POLYMORPHIC_IF_H
// $Id: osss_polymorphic_if.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
