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
#ifndef O3S_UTILS_REPORT_BASE_H_INCLUDED
#define O3S_UTILS_REPORT_BASE_H_INCLUDED

/**
 * \file  report_base.h
 * \brief debugging and error handling
 *
 * This file contains the implementation of the basic debugging and
 * error handling utilities for the OSSS library.
 *
 * $Date: 2008-07-01 22:45:11 +0200 (Tue, 01 Jul 2008) $
 * $Revision: 2392 $
 *
 * \see osss::report
 */
#include "message.h"

#include <string>
#include <sstream>

/* ensure, that OSSS_DEBUGLEVEL is set */
#if !defined(OSSS_DEBUGLEVEL)
# define OSSS_DEBUGLEVEL 0
#endif
#if ( OSSS_DEBUGLEVEL > 7 )
// invalid debug level set
# error Only 0 <= OSSS_DEBUGLEVEL <= 7 is supported.
#endif /* OSSS_DEBUGLEVEL */

namespace osss {
namespace report {

/**
 * \brief OSSS error message severities
 *
 * Internal enumeration used for error handling.
 * The values correspond to those defined in SystemC' sc_severity
 *
 * \see sc_core::sc_severity
 */
enum osss_severity {
  OSSS_SVRTY_FATAL_  = 0, ///< unrecoverable errors, \see SC_FATAL
  OSSS_SVRTY_ERROR_  = 1, ///< error, maybe recoverable, \see SC_ERROR
  OSSS_SVRTY_WARNING_= 2, ///< possible problem, \see SC_WARNING
  OSSS_SVRTY_INFO_   = 4, ///< informational message, \see SC_INFO
  OSSS_SVRTY_DEBUG_  = 8, ///< debugging output (without prefix)
  /// largest severity value
  OSSS_SVRTY_LAST_   = ( OSSS_SVRTY_DEBUG_ | OSSS_DEBUGLEVEL ) + 1
};

/* ----------------------- report helper class ------------------------ */

/**
 * \class report_base
 * \brief error and debug messages
 *
 * This class handles the error reporting and debugging
 * of the OSSS library.  It should not be used directly.
 * Use the various provided macros instead.
 *
 * \see OSSS_DEFINE_REPORT_MSG(), OSSS_REPORT_FATAL(),
 *      OSSS_REPORT_ERROR(), OSSS_REPORT_WARNING(),
 *      OSSS_REPORT_INFO(), OSSS_DEBUG_MSG(), OSSS_MESSAGE()
 */
class report_base
{
protected:
  typedef report_base base_type;

  static const char* const file_unknown; ///< used, if __FILE__ macro is missing
  static const int         line_unknown; ///< used, if __LINE__ macro is missing
  static const char* const lib_prefix;   ///< prefix for message ids

public:
  /// shortcut for report severity
  typedef osss_severity severity;

protected:
  /**
   * \brief constructor to prepare report
   *
   * This constructor is provided to set up a new report. It is 
   * used within the reporting macros to pass the current
   * file and line to the report.
   *
   * \param msg_tpl The report's basic message template. Can contain
   *                "%s" placeholders, that can then be filled via the
   *                % operator.
   *
   * \param s       The report's severity, see osss::report::severity.
   * \param file    The current file, usually called with __FILE__ macro.
   * \param line    The current line, usually called with __LINE__ macro.
   *
   * \see ~report_base(), OSSS_DEFINE_REPORT_MSG()
   */
  explicit
  report_base( const char * const     msg_tpl,
               severity s             = OSSS_SVRTY_DEBUG_,
               const char* const file = file_unknown,
               int line               = line_unknown );

public:
  /**
   * \brief stream helper
   *
   * This method returns a stream, that can be conveniently
   * used to append more information to the current report.
   *
   * \param  context  This parameter can be used with macros
   *                  like __PRETTY_FUNCTION__. If it is given,
   *                  the current simulation context (calling
   *                  function, current process, simulation time)
   *                  is added to the report.
   * \see    OSSS_MESSAGE(), reduce_function()
   *
   * \return stream_t   reference to output stream
   * \see ~report_base
   */
  message& fill( const char* context = 0 );


  /**
   * \brief issue report
   *
   * This method passes the current report to the SystemC error
   * reporting facilities.  If a message is pending, this method
   * is called during destruction of this instance, at the latest.
   *
   * \warning This method may throw an exception, depending on the
   *          current sc_core::sc_actions set for the current message
   *          ID.
   */
  void trigger() const;

  /**
   * \brief cancel current report
   *
   * This method disables the current report. It will not trigger
   * automatically on destruction.
   *
   * \note Cancellation can not be revoked.
   * \see trigger()
   */
  void cancel();

protected:
  /**
   * \brief return current report ID
   *
   * This method dispatches to the static implementation
   * of the id() method in the derived classes.
   *
   * \see OSSS_DEFINE_REPORT_MSG()
   */
  virtual const char* const get_id() const = 0;

  /**
   * \brief destructor (fires report, if necessary)
   *
   * If the current report object represents a  pending report,
   * it is passed through to the SystemC error reporting on destruction.
   *
   * \warning This destructor may throw an exception, if SystemC
   *          error reporting does for the represented report.
   *          Do NOT use this class as a member unless you know
   *          what you are doing.
   */
   virtual ~report_base();

  /** disabled copy constructor */
  report_base( const report_base& );
  /** disabled new operator - only stack allocation allowed */
  void* operator new( size_t );

private:

  /**
   * \brief function name reduction helper
   *
   * This method reduces a string given by macros like
   * __PRETTY_FUNCTION__ to a shorter fraction: "methodname()".
   * It is used, if the \c context parameter is given to 
   * the stream() method.
   *
   */
  static
  std::string reduce_function( const char* );

  message      msg_;
  bool         active_;
  severity     sev_;
  const char * const file_;
  unsigned int       line_;
  const char * id_;

}; /* class report_base */

} /* namespace report */
} /* namespace osss  */

#endif /* O3S_UTILS_REPORT_BASE_H_INCLUDED */
/* Taf!
 * $Id: report_base.h 2392 2008-07-01 20:45:11Z philipph $
 * :flavour: (osss,h) (osss_recon,h) (osss_full,h)
 */
