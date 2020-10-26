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
#ifndef OSSS_SERIALISABLE_MACROS_HH
#define OSSS_SERIALISABLE_MACROS_HH

/**
 * \def OSSS_SERIALISABLE_CLASS
 *
 * This macro is used for convenience when defining a user-defined
 * class that should be serialisable.
 */
#define OSSS_SERIALISABLE_CLASS( _class_name_ ) \
  class _class_name_ : public osss::osss_serialisable_object

/**
 * \def OSSS_IS_SERIALISABLE
 *
 * Generates a constructor for rebuilding an osss_serialisable_object 
 * from an osss_serialisable_archive.
 * This needs to be added to each user-class that should be serialisable.
 */
#define OSSS_IS_SERIALISABLE( _class_name_ ) \
  CLASS( _class_name_, osss::osss_serialisable_object); \
  CONSTRUCTOR( public, \
               _class_name_, \
               (osss::osss_serialisable_archive* arch) ) \
  { \
    arch->rebuild(*this); \
  }

/**
 * \def OSSS_SERIALISABLE_CTOR
 *
 * This macro needs do be used for all constructors of user-defined
 * classes that should be serialisable.
 */
#define OSSS_SERIALISABLE_CTOR( _class_name_, _parameter_list_ ) \
  CONSTRUCTOR( public , _class_name_, _parameter_list_ )

#endif
// $Id: osss_serialisable_macros.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
