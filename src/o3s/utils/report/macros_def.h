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
#ifndef O3S_UTILS_REPORT_MACROS_DEF_H_INCLUDED
#define O3S_UTILS_REPORT_MACROS_DEF_H_INCLUDED

/// mark beginning of report definition block
#define OSSS_REPORT_BEGIN_DEFINITION_ \
  namespace report {

/// mark end of report definition block
#define OSSS_REPORT_END_DEFINITION_ \
  } /* namespace report */

/**
 * \def OSSS_DEFINE_REPORT_MSG
 * \brief report type definition helper
 *
 * Internal macro that is recommended to be used for
 * the definition of new report types. This macro results
 * in the definition of a new class, derived from
 * report::report_base.
 *
 * \param Type        class name of newly defined report type
 * \param Id          string to identify report type through
 *                    SystemC report handling
 * \param MsgTemplate Template for message creation.
 *
 * The message template can contain "%s" placeholders, that
 * can be filled via the % operator during the message creation.
 *
 * Example:
 * \code
 *
 * OSSS_DEFINE_REPORT_MSG( example_rpt, "Example/Category",
 *                         "This is a %s template!\n"
 *                         "Replacers are type-safe: %s"
 * );
 * //...
 * OSSS_REPORT_INFO( osss::report::example_rpt )
 *   % "fancy"               // fill placeholder
 *   << "Some more text."    // append some text
 *   % something_streamable; // fill another placeholder (from a variable)
 *
 * \endcode
 *
 * \see report_base, report_msgs.h
 */
#define OSSS_DEFINE_REPORT_MSG( Type, Id, MsgTemplate )      \
  struct Type : ::osss::report::report_base                  \
  {                                                          \
    OSSS_IMPL_REPORT_CTOR_( Type, MsgTemplate "" );          \
    static const char * const id()                           \
      { return Id; }                                         \
  protected:                                                 \
    const char * const get_id() const { return Type::id(); } \
  }

/// internal helper macro
#define OSSS_IMPL_REPORT_CTOR_( Type, MsgTemplate )     \
  Type() : base_type( MsgTemplate ) {}                  \
  Type( base_type::severity s )                         \
    : base_type( MsgTemplate, s ) {}                    \
  Type( base_type::severity s,                          \
        const char * const  file, int line )            \
    : base_type( MsgTemplate, s, file, line ) {}

#endif /* O3S_UTILS_REPORT_MACROS_H_INCLUDED */
/* Taf!
 * $Id: macros_def.h 2392 2008-07-01 20:45:11Z philipph $
 * :flavour: (osss,h) (osss_recon,h) (osss_full,h)
 */
