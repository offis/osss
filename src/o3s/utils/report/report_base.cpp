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

#include "report_base.h"
#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_assert.h"
#include "o3s/utils/osss_report.h"
#include "o3s/utils/osss_process_handle.h"
#include <iostream>

namespace osss {
namespace report {

/// local severity conversion function
inline static
sc_core::sc_severity
osss_severity_to_sc_severity( report_base::severity svrty );

report_base::report_base( const char* const msg_tpl,
                          severity s,
                          const char* const file, int line )
  : msg_( msg_tpl )
  , active_(true)
  , sev_( s )
  , file_( file )
  , line_( line )
  , id_( "<unknown>" )
{}

message&
report_base::fill( const char* context )
{
  // cache current ID - would be lost during trigger() called from destructor
  id_ = this->get_id();
  // include simulation context in message
  if ( context ) {
    msg_
      << ::sc_core::sc_time_stamp()
      << "~" << ::sc_core::sc_delta_count() << " " \
      << ::osss::osssi::osss_process_handle::current().name()
      << " @ " << reduce_function( context )
      << ": ";
  }
  return msg_;
}

void
report_base::cancel()
{
  active_ = false;
}

report_base::~report_base()
{
  // cancel current message, another exception is on its way
  if ( std::uncaught_exception() ) {
    cancel();
  }
  this->trigger();
}

void
report_base::trigger() const
{
  if( osss_likely(active_) ) {
    using std::cerr;
    using std::endl;

    // is it a debug message?
    if( sev_ & OSSS_SVRTY_DEBUG_ ) {
      unsigned level = ( sev_ ^ OSSS_SVRTY_DEBUG_ );
      if (level) { // write prefix, only if level is set
        cerr << lib_prefix << "DEBUG/" << level << ": ";
      }
      // dump message
      cerr << msg_.combine();
      if ( file_ != file_unknown ) {
        cerr << " (file: " << file_
             << ", line: " << line_ << ")";
      }
      cerr << endl;

    } else { // regular message

      sc_core::sc_severity scs =
      osss_severity_to_sc_severity( sev_ );

      std::string msg_type( lib_prefix );
      msg_type += id_;

      // call SystemC report handler
      sc_core::sc_report_handler::report( scs, msg_type.c_str(),
                                          msg_.combine().c_str(),
                                          file_, line_ );
    }
  }
}

inline static
sc_core::sc_severity
osss_severity_to_sc_severity( report_base::severity svrty )
{
   // translate to SystemC severity
  sc_core::sc_severity scs = ::sc_core::SC_MAX_SEVERITY;
   switch(svrty) {
#     define OSSS_TRANSLATE_RPT_SVRTY_HELPER( osss, sysc ) \
        case osss:                                         \
          scs=::sc_core::sysc;                             \
          break

      OSSS_TRANSLATE_RPT_SVRTY_HELPER( OSSS_SVRTY_FATAL_,   SC_FATAL   );
      OSSS_TRANSLATE_RPT_SVRTY_HELPER( OSSS_SVRTY_ERROR_,   SC_ERROR   );
      OSSS_TRANSLATE_RPT_SVRTY_HELPER( OSSS_SVRTY_WARNING_, SC_WARNING );
      OSSS_TRANSLATE_RPT_SVRTY_HELPER( OSSS_SVRTY_INFO_,    SC_INFO    );
      default:
         OSSS_ASSERT( false && "Invalid report severity!" );

#     undef OSSS_TRANSLATE_RPT_SVRTY_HELPER
  }
  return scs;
}

std::string
report_base::reduce_function( const char* char_text )
{
  std::string text( char_text );
  // remove all namespace and class names
  std::string::size_type idx_bra = text.find("(");
  std::string::size_type idx_sep = text.rfind("::", idx_bra);
  std::string working_copy;
  if (idx_sep != std::string::npos)
  {
    working_copy = text.substr(idx_sep+2);
  }
  else
  {
    working_copy = text;
  }
  // remove all arguments
  idx_bra = working_copy.find("(");
  if (idx_bra != std::string::npos)
  {
    working_copy  = working_copy.substr(0, idx_bra);
    working_copy += "()";
  }
  return working_copy;
}

const char* const report_base::lib_prefix   = "/OSSS/";
const char* const report_base::file_unknown = "<unknown>";
const int         report_base::line_unknown = -1;

} /* namespace report */
} /* namespace osss */

/* Taf!
 * $Id: report_base.cpp 2392 2008-07-01 20:45:11Z philipph $
 * :flavour: (osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
 */
