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

// NO "REAL" INCLUDE GUARDS - multiple inclusion allowed

/**
 * \file  osss_assert.h
 * \brief run-time assertions
 *
 * This file contains the OSSS_ASSERT() macro for run-time
 * assertions. This is similar to the C-style assert() macro,
 * and even more similar to the SystemC macro sc_assert().
 *
 * \todo add support for compile-time assertions
 *
 * This file can be included multiple times.  Each time, the
 * OSSS_ASSERT() macro is activated or deactivated according
 * to the OSSS_NDEBUG switch.
 *
 * With this mechanism, certain performance critical sections
 * can be wrapped in a block like in the following example.
 *
 * Example:
 * \code
 *
 * // this section is performance critical
 * #if !defined( OSSS_DEBUG )
 * #  define OSSS_NDEBUG // disable assertions in non-debug build
 * #endif
 * #include "o3s/utils/osss_assert.h"
 *
 * void performance_critical_code_with_assertions()
 * {
 *   //...
 *   OSSS_ASSERT( false ); // no check here -> no error reported
 * }
 *
 * #undef OSSS_NDEBUG // re-enable assertions
 * #include "o3s/utils/osss_assert.h"
 *
 * \endcode
 *
 * \note This file depends on osss_report.h being included
 *       separately, if assertions are enabled.
 *       It is NOT included from this file, though.  This allows the
 *       inclusion of this file at any place in the source code.
 *
 * $Date: 2008-12-04 16:58:31 +0100 (Thu, 04 Dec 2008) $
 * $Revision: 2852 $
 *
 * \see osss::osss_report, OSSS_REPORT_FATAL(), sc_assert(), assert()
 */

#ifdef O3S_UTILS_OSSS_ASSERT_H_INCLUDED_

# undef OSSS_ASSERT
# undef O3S_UTILS_OSSS_ASSERT_H_INCLUDED_

#endif

#define O3S_UTILS_OSSS_ASSERT_H_INCLUDED_

/* ---------------------------- assertions ---------------------------- */

#if !defined( OSSS_NDEBUG ) && !defined( OSSS_SYNTHESIS_ )

/**
 * \def   OSSS_ASSERT()
 * \brief run-time assertions
 *
 * This macro can be used for C-style run-time assertions.
 * The assertions are omitted, if the preprocessor symbol \c OSSS_NDEBUG
 * is defined during the inclusion of the osss_assert.h header file.
 *
 * Otherwise, if the given expression is \c false, a fatal error
 * is reported via an OSSS_REPORT_FATAL() call.
 *
 * \param expr  expression, that is required to be true.
 *
 * Example usage:
 * \code
 *   // ...
 *   OSSS_ASSERT( ultimate_answer == 42 ); // should hold...
 * \endcode
 *
 * \warning Do \em NOT rely on any side-effects within the
 *          asserted expression, since these might be elided in an
 *          optimised build!
 *
 * \note    You should only check internal invariants using this
 *          assertion mechanism.  Errors, that are expected (e.g. due
 *          to invalid user-code) should be checked explicitly and
 *          reported through the regular error-handling mechanisms.
 *
 * \see osss_assert.h, OSSS_REPORT_FATAL()
 */
# define OSSS_ASSERT( expr )                                               \
 ((void)(osss_likely(expr) ? 0 :                                           \
 ( (OSSS_REPORT_FATAL( ::osss::report::assertion_failed ) % #expr ) , 0 )))

#else // assertions are disabled

# define OSSS_ASSERT( expr ) \
    ((void) 0) /* ignore */

#endif /* OSSS_NDEBUG */

/* Taf!
 * $Id: osss_assert.h 2852 2008-12-04 15:58:31Z philipph $
 * :flavour: (osss,H) (osss_recon,h) (osss_icodes,h) (osss_full,h)
 */
