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

#ifndef OSSS_LOG_H
#define OSSS_LOG_H

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_process_handle.h"
#include "o3s/structural/app/osss_recon_base.h"

namespace osss{

class osss_device;
class osss_device_type;

namespace osssi {

class osss_recon_base;
class osss_context_base;

/** This class provides logging abilities. It is not
    to be confused with SystemC's trace facility.
    The log file contains information about reconfiguration
    times, grants, etc. */
class osss_log
{
public:
  /// Group, to which an event belongs
  enum MajorGroup {
    attrSaveEvent,
    configurationEvent,
    attrRestoreEvent,
    accessEvent,
    contentEvent,
    prefetchEvent
  };
  /// Subgroup
  enum MinorGroup {
    requestEvent,
    executeEvent,
    returnEvent,
    createEvent,
    destroyEvent
  };
  /// Detailed info: Start (or single) or end?
  enum MicroGroup {
    startEvent,
    stopEvent
  };

  private:
  /// File handle for log file
  std::ofstream * m_log_file;
  std::string     m_buffer[256];

  /// marks the first occuring event after log is opened
  bool first_event;

  /// All devices which shall be logged
  std::vector< const osss_device * >            m_device_instance_table;
  /// All device types which shall be logged
  std::vector< const osss::osss_device_type * > m_device_type_table;
   /// All processes which use reconfiguration things
  std::vector< osssi::OSSS_Process_ID_t >  m_process_table;

  /// Writes XML header
  void writeHeader(::std::ostream & os) const;
  /// Writes XML footer
  void writeFooter(::std::ostream & os) const;

  public:
  /// Constructor. Creates a log object with closed file.
  osss_log();
  ~osss_log();

  /** Open logfile. Advise: Filename should end with
      ".xml", it is not automatically appended. */
  void open(const std::string filename);
  /** Close logfile again */
  void close();
  /** Test method, returns true iff logfile is open */
  bool isOpen();

  /** Writes an event. Provide all the information you got,
      leave unknown things to NULL/0/... */
  void write(const MajorGroup             major_group,
             const MinorGroup             minor_group,
             const MicroGroup             micro_group,
             const osss::osss_device *    hw_device,
             const osss_recon_handle      recon_object,
             const osss_slot_descriptor * slot_desc,
             const std::string            context_name,
             const OSSS_Process_ID_t      originator);

  /** Adds a device to the list of those which should be
      listed in the logfile */
  void addDevice(osss::osss_device * new_device);

  /** Adds a user process to those to be logged */
  void addUserProcess(osssi::OSSS_Process_ID_t user_process, int priority);

  /** Internal helper methods for wiring timing info in XML ways... */
  static void writeXMLTimeAttributesOn(const sc_core::sc_time data,
                                             std::ostream &   os   );
};

} // end namespace osssi
} // end namespace osss

// Moving this symbol into the osssi namespace
// would be desirable but doesn't work for some
// strange reason. Linker failure.
extern osss::osssi::osss_log osss_event_log;

#endif // OSSS_LOG_H
// $Id: osss_log.h 2394 2008-07-01 20:45:28Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
