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
#ifndef OSSS_MODULE_H_INCLUDED
#define OSSS_MODULE_H_INCLUDED

#include "o3s/structural/arch/osss_architecture_object.h"
#include "o3s/structural/arch/osss_clock_reset_if.h"

namespace osss {

class osss_module
  : public osss_architecture_object
  , public osssi::osss_clock_reset_if
{
  typedef osss_architecture_object   base_type;
  typedef osssi::osss_clock_reset_if ports_type;
protected:

  osss_module()
    : base_type()
    , ports_type()
  {}

  explicit
  osss_module( sc_core::sc_module_name name )
    : base_type( name )
    , ports_type()
  {}

public:
  virtual const char* kind() const
    { return "osss_module"; }
  using base_type::name;

protected:

  virtual void before_end_of_elaboration()
  {
    ports_type::check_clock_reset_port();
    base_type::before_end_of_elaboration();
  }

}; // class osss_module

// -----------------------------------------------------------------------------
// SOME MACROS TO SIMPLIFY SYNTAX:
// -----------------------------------------------------------------------------
 
#define OSSS_MODULE( UserModule )          \
  struct UserModule : ::osss::osss_module
 
#define OSSS_CTOR( UserModule )            \
  OSSS_HAS_PROCESS( UserModule );          \
  explicit                                 \
  UserModule( ::sc_core::sc_module_name )

#define OSSS_DEFAULT_CTOR( UserModule )                            \
  OSSS_HAS_PROCESS( UserModule );                                  \
  explicit                                                         \
  UserModule( ::sc_core::sc_module_name                            \
                   = ::sc_core::sc_gen_unique_name( #UserModule ) )

// the OSSS_HAS_PROCESS macro call must be followed by a ;
#define OSSS_HAS_PROCESS( UserModule ) \
  SC_HAS_PROCESS( UserModule )

#define OSSS_CTHREAD( ProcessName, ResetSensitivity )             \
  SC_CTHREAD( ProcessName, this->clock_port().pos() );            \
  this->reset_signal_is( this->reset_port(), ResetSensitivity )

} // namespace osss

#endif // OSSS_MODULE_H_INCLUDED
// $Id: osss_module.h 2833 2008-12-04 09:29:20Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
