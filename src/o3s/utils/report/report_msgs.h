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
/**
 * \file report_msgs.h
 *
 * This file contains information about the different library (error)
 * messages. It is assumed, that this file contains only invocations
 * of the OSSS_DEFINE_REPORT_MSG() macro, which maps message ids to
 * their library subsystem and an appropriate message prefix.
 *
 * \see osss_report.h
 *
 * $Date: 2009-03-27 14:10:42 +0100 (Fri, 27 Mar 2009) $
 * $Revision: 3078 $
 */

#ifndef O3S_UTILS_REPORT_MSGS_H_INCLUDED
#define O3S_UTILS_REPORT_MSGS_H_INCLUDED

#include "o3s/utils/report/macros_def.h"
#include "o3s/utils/report/report_base.h"

namespace osss {
OSSS_REPORT_BEGIN_DEFINITION_

// OSSS_DEFINE_REPORT_MSG( Type, Id [, MsgTemplate] )

/* ---------------- core library --------------- */

OSSS_DEFINE_REPORT_MSG( abort_called, "Core/Abort called",
  "OSSS_ABORT() has been called due to an unrecoverable error."
);

OSSS_DEFINE_REPORT_MSG( assertion_failed,   "Core/Assertion failed",
  "Expression '%s' is unexpectedly false!\n"
  "If this seems to be a bug in the OSSS library, please contact\n"
  "osss-devel@offis.de."
);

OSSS_DEFINE_REPORT_MSG( internal_error, "Core/Internal error",
  "An internal error has occured: %s\n"
  "If this seems to be a bug in the OSSS library, please contact\n"
  "osss-devel@offis.de."
);

OSSS_DEFINE_REPORT_MSG( not_implemented, "Core/Not implemented",
  "%s is not implemented yet\n."
  "If you urgently need this functionality, please contact\n"
  "osss-devel@offis.de."
);

/* ----------------- datatypes ----------------- */

OSSS_DEFINE_REPORT_MSG( index_out_of_bounds,
                        "Datatype/Index out of bounds",
  "%s: Invalid field index detected: %s >= %s (=size)!"
);

/* ---------------- elaboration ---------------- */

OSSS_DEFINE_REPORT_MSG( called_after_elaboration,
                        "Elaboration/Simulation started",
  "'%s' called after end of elaboration phase."
);

/* ---------------- user objects --------------- */

OSSS_DEFINE_REPORT_MSG( invalid_user_class, "Object/Invalid user-class",
  "Given user-class '%s' is not supported.\n"
  "Please ensure, that your user-class is properly derived \n"
  "from 'osss_object'."
);

OSSS_DEFINE_REPORT_MSG( pure_virtual_call, "Object/Pure-virtual call",
  "Call to pure-virtual method\n'%s' detected.\n"
  "Please ensure, that your polymorphic object/context is\n"
  "properly initialised."
);

/* --------------- shared objects -------------- */

OSSS_DEFINE_REPORT_MSG( access_before_reset, "Shared Object/Access before reset",
  "A Shared Object has not been reset before first access\n"
  "(client: %s).  Access will be blocked until reset is finished."
);

/* ---------------- arch objects --------------- */

OSSS_DEFINE_REPORT_MSG( channel_select_signal_invalid,
                        "Channel/Invalid select signal",
  "The select signal of an '%s' has to be of boolean type!"
);

OSSS_DEFINE_REPORT_MSG( channel_boolean_signal_invalid,
                        "Channel/Invalid boolean signal",
  "Invalid attempt to read signal '%s' of type '%s' as boolean!"
);


OSSS_DEFINE_REPORT_MSG( ram_read_too_early, "osss_ram/Read access",
  "Read data from an osss_ram is only valid two "
  "clock cycles after prepare_read() was called."
);

OSSS_DEFINE_REPORT_MSG( ram_init_overflow, "osss_ram/Initialisation",
  "Too many initialisation values given via osss_ram::init()\n"
  "for RAM module %s."
);

/* -------------------------------------------- */

OSSS_DEFINE_REPORT_MSG(
  plain_msg,
  "Core/Plain",
  /* empty message */
);

OSSS_REPORT_END_DEFINITION_
} // namespace osss

#include "o3s/utils/report/macros_undef.h"

#endif // O3S_UTILS_REPORT_MSGS_H_INCLUDED
/* Taf!
 * $Id: report_msgs.h 3078 2009-03-27 13:10:42Z philipph $
 * :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
 */
