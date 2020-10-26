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
#ifndef O3S_UTILS_OSSS_REPORT_H_INCLUDED
#define O3S_UTILS_OSSS_REPORT_H_INCLUDED

/**
 * \file  osss_report.h
 * \brief reporting errors, warnings and infos
 *
 * This file contains the  internal error-handling macros
 * for the OSSS library.  These facilities basically provide
 * a thin layer on top of the standardised reporting API
 * of SystemC.
 *
 * $Date: 2009-03-04 23:55:26 +0100 (Wed, 04 Mar 2009) $
 * $Revision: 3023 $
 *
 */

namespace osss {
/**
 * \namespace osss::report
 * \brief namespace wrapper for the error/debug reporting utilities
 *
 * This namespace encapsulates the error-handling and
 * reporting facilities in OSSS.  In particular, this includes
 * the different report-id classes, which are derived from the
 * report_base class.
 *
 * \see report_base
 *
 * The reporting itself is done through various macros.
 * A basic printf()-like placefolder syntax is supported.
 *
 * \see OSSS_DEFINE_REPORT_MSG(), OSSS_REPORT_FATAL(),
 *      OSSS_REPORT_ERROR(), OSSS_REPORT_WARNING(),
 *      OSSS_REPORT_INFO(), OSSS_DEBUG_MSG(), OSSS_MESSAGE()
 */
namespace report { /* for documentation only */ }
} // namespace osss

/* ensure, that OSSS_DEBUGLEVEL is set */
#if !defined(OSSS_DEBUGLEVEL)
# define OSSS_DEBUGLEVEL 0
#endif
#if ( OSSS_DEBUGLEVEL > 7 )
// invalid debug level set
# error Only 0 <= OSSS_DEBUGLEVEL <= 7 is supported.
#endif /* OSSS_DEBUGLEVEL */

#include "osss_common.h"
#include "report/report_base.h"
#include "report/report_msgs.h"

/* ------------------------- reporting macros ------------------------- */

/**
 * print a message to stdout
 *
 * \deprecated Do not use this macro in new designs!
 *
 *  TODO: remove this macro or rename it to sth. like
 *        OSSS_PRINT_MSG()
 */
#define PRINT_MSG(msg) \
  std::cout << ::sc_core::sc_time_stamp() << "\t: " \
            << name() << "(line: " << __LINE__ << ")\t" \
            << msg << std::endl;

/**
 * \def OSSS_MESSAGE()
 * \brief conditional message
 *
 * This macro can be used to conditionally report raw messages.
 * The macro can be used as a simple stream, with a condition parameter,
 * that decides if the report should be issued.
 *
 * Additionally, the current simulation time, the calling process,
 * and the current method name are printed, too.
 *
 * \param ConditionFlag If ConditionFlag is \c true, the following message
 *                      is reported.
 *
 * Example usage:
 * \code
 *   OSSS_MESSAGE( my_condition )
 *     << "The condition is true.";
 * \endcode
 *
 * \see   osss::osss_report
 */
#define OSSS_MESSAGE( ConditionFlag, OldStyleMessage... )     \
  if ( !(ConditionFlag) ) { ((void)0); } else                 \
   ::osss::osss_report< ::osss::report::plain_msg >::type()   \
       .fill( __PRETTY_FUNCTION__ ) << "" OldStyleMessage

/**
 * \def OSSS_REPORT_FATAL
 * \brief report fatal errors
 *
 * This macro can be used to report fatal, non-recoverable errors.
 * The given report id needs to be present in report_msgs.h (or
 * another included collection of error-messages).
 * As a result, a fatal error is reported to the SystemC kernel's
 * error handling facilities.
 *
 * Example usage:
 * \code
 *   OSSS_REPORT_FATAL( osss::report::internal_error )
 *     << "This is a fatal error.";
 * \endcode
 *
 * \param id Type of the report (derived from report_base)
 * \see OSSS_SVRTY_FATAL_, report_msgs.h, OSSS_IMPL_REPORT_STREAM_()
 */
#define OSSS_REPORT_FATAL( id ) \
  OSSS_IMPL_REPORT_STREAM_( ::osss::report::OSSS_SVRTY_FATAL_, id )

#define OSSS_ABORT() \
  OSSS_REPORT_FATAL( osss::report::abort_called ); \
  std::unexpected() /* avoid warnings about missing return */

/**
 * \def OSSS_REPORT_ERROR
 * \brief report errors
 *
 * This macro can be used to report "regular" errors.
 * The given report id needs to be present in report_msgs.h (or
 * another included collection of error-messages).
 * As a result, an error is reported to the SystemC kernel's
 * error handling facilities.
 *
 * Example usage:
 * \code
 *   OSSS_REPORT_ERROR( osss::report::internal_error )
 *     << "This is an error.";
 * \endcode
 *
 * \param id Type of the report (derived from report_base)
 * \see OSSS_SVRTY_ERROR_, report_msgs.h, OSSS_IMPL_REPORT_STREAM_()
 */
#define OSSS_REPORT_ERROR( id ) \
  OSSS_IMPL_REPORT_STREAM_( ::osss::report::OSSS_SVRTY_ERROR_, id )

/**
 * \def OSSS_REPORT_WARNING
 * \brief report warnings
 *
 * This macro can be used to report warning messages.
 * The given report id needs to be present in report_msgs.h (or
 * another included collection of error-messages).
 * As a result, a warning is reported to the SystemC kernel's
 * error handling facilities.
 *
 * Example usage:
 * \code
 *   OSSS_REPORT_WARNING( osss::report::plain_msg )
 *     << "This is a warning.";
 * \endcode
 *
 * \param id Type of the report (derived from report_base)
 * \see OSSS_SVRTY_WARNING_, report_msgs.h, OSSS_IMPL_REPORT_STREAM_()
 */
#define OSSS_REPORT_WARNING( id ) \
  OSSS_IMPL_REPORT_STREAM_( ::osss::report::OSSS_SVRTY_WARNING_, id )

/**
 * \def OSSS_REPORT_INFO
 * \brief report informational messages
 *
 * This macro can be used to report informational messages.
 * The given report id needs to be present in report_msgs.h (or
 * another included collection of error-messages).
 * As a result, an informational message is reported to the SystemC
 * kernel's error handling facilities.
 *
 * Example usage:
 * \code
 *   OSSS_REPORT_INFO( osss:report::plain_msg )
 *     << "This is an info.";
 * \endcode
 *
 * \note OSSS_DEBUG_MSG() is provided to report debug messages,
 *       OSSS_MESSAGE() can be used for contextual information
 *       output.
 *
 * \param id Type of the report (derived from report_base)
 * \see OSSS_SVRTY_INFO_, report_msgs.h, OSSS_IMPL_REPORT_STREAM_()
 */
#define OSSS_REPORT_INFO( id ) \
  OSSS_IMPL_REPORT_STREAM_( ::osss::report::OSSS_SVRTY_INFO_, id )

/**
 * \def OSSS_IMPL_REPORT_STREAM_()
 * \brief internal reporting macro
 *
 * This helper macro is used by the public reporting macros
 * to really issue the requested report at the appropriate
 * severity.
 *
 * \note It should not be used directly. Use the severity
 *       specific macros OSSS_REPORT_FATAL(),
 *       OSSS_REPORT_ERROR() etc. instead.
 *
 * \see osss::osss_report,
 *      osss::report::osss_severity
 */
#define OSSS_IMPL_REPORT_STREAM_( severity, id )                         \
  /* create temporary report object and return its stream */             \
  ::osss::osss_report< id >::type( severity, __FILE__, __LINE__ ).fill()

namespace osss {
namespace report {

/**
 * \brief internal check for validity of given error id
 *
 * \todo This is not yet implemented as a static assert!
 */
template< typename ReportType >
struct valid_report_type
{
  /// TODO: implement static inheritance check
  static const bool value = true;
};

} // namespace report

/**
 * \brief internal error reporting helper template
 *
 * This template class is used, to statically check the given
 * error ID in the various reporting macros.
 *
 * \note It should not be used directly. Use the severity
 *       specific macros OSSS_REPORT_FATAL(),
 *       OSSS_REPORT_ERROR() etc. instead.
 */
template< typename ReportType
        , bool Valid = report::valid_report_type<ReportType>::value >
struct osss_report;

template< typename ReportType >
struct osss_report<ReportType, true>
{
  typedef ReportType       type;
  static const bool        valid = true;
  static const char* const id()
    { return type::id(); }
};

} // namespace osss

#endif /* O3S_UTILS_OSSS_REPORT_H_INCLUDED */
/* Taf!
 * $Id: osss_report.h 3023 2009-03-04 22:55:26Z philipph $
 * :flavour: (osss,h) (osss_recon,h) (osss_full,h)
 */
