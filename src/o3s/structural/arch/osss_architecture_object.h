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
#ifndef OSSS_ARCHITECTURE_OBJECT_H_INCLUDED
#define OSSS_ARCHITECTURE_OBJECT_H_INCLUDED

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/noncopyable.h"

namespace osss {

/**
 * \brief base class for architectural elements
 *
 * This class provides a base class for a set of
 * OSSS Virtual Target Architecture classes.  It does not
 * provide any specific functionality (yet).
 *
 * \note Direct creation of instances of this class is not
 *       allowed, therefore the available constructors are
 *       protected.
 *
 * \todo maybe rename this class to avoid the
 *       otherwise used 'object' in its name
 *
 * \warning This class relies on the correct naming as any usual
 *          SystemC module.  That means, that either an explicit
 *          name is given or the current naming stack contains
 *          the correct name on top.
 *
 * \see sc_core::sc_module , SC_CTOR()
 *
 */
class osss_architecture_object
  : public  sc_core::sc_module
  , private osssi::noncopyable
{
protected:

  /**
   * \brief default constructor (relies on sc_core::sc_module)
   *
   * \warning The SystemC internal naming stack has to contain
   *          the correct sc_core::sc_module_name on top, otherwise
   *          members with get the wrong hierarchical name.
   */
  osss_architecture_object()
    : sc_core::sc_module()
  {}

  /**
   * \brief preferred contructor
   *
   * \param name The name of this VTA element.
   *
   * \see sc_core:.sc_module, SC_CTOR()
   */
  explicit
  osss_architecture_object(sc_core::sc_module_name name)
    : sc_core::sc_module( name )
  {}

public:

  virtual const char* kind () const
    { return "osss_architecture_object"; }

}; // osss_architecture_object

} // namespace osss

#endif /* OSSS_ARCHITECTURE_OBJECT_H_INCLUDED */
// $Id: osss_architecture_object.h 2833 2008-12-04 09:29:20Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
