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

#include <algorithm>
#include <ctime>
#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_log.h"
#include "o3s/structural/arch/system/osss_device_type.h"
#include "o3s/structural/arch/system/osss_device.h"
#include "o3s/utils/osss_process_registry.h"

namespace osss{
namespace osssi{


osss_log::osss_log() :
    m_log_file(NULL), first_event(true)
{}

osss_log::~osss_log()
{
  if (isOpen())
  {
    close();
  }
}

void osss_log::open(const std::string filename)
{
  if ( isOpen() )
    close();

  m_log_file = new std::ofstream(filename.c_str());
  if (!m_log_file)
  {
    std::cerr  << "ERROR: Cannot open log file \""
          << filename << "\"" << std::endl;
    OSSS_ABORT();
  }
  first_event = true;
}

void osss_log::close()
{
  if (!m_log_file)
  {
    std::cerr  << "WARNING: attempt to close an closed logfile"
               << std::endl;
  }
  else
  {
    writeFooter(*m_log_file);
    m_log_file->close();
    delete m_log_file;
    m_log_file = NULL;
  }
}

bool osss_log::isOpen()
{
  return (m_log_file != NULL);
}

void osss_log::addDevice(osss::osss_device * new_device_ptr)
{
  if (m_log_file != NULL)
  {
    std::cerr << "WARNING: Log::addDevice() adding device "
                 "while log is open produces incorrect results" << std::endl;
  }
  m_device_instance_table.push_back(new_device_ptr);
  // The next two statements are inefficient but since they
  // are rarely executed...
  m_device_type_table.push_back(new_device_ptr->getDeviceTypePtr());
  unique(m_device_type_table.begin(), m_device_type_table.end());
}

void osss_log::writeHeader(std::ostream & os) const
{
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
  time_t now = time(NULL);
  char * tmp = asctime(localtime(&now));
  // tmp may contain \n or \r, truncate that...
  unsigned int len = strlen(tmp);
  char * buffer = new char[len+1];
  strncpy(buffer, tmp, len);
  do
  {
    buffer[len] = '\0';
    --len;
  }
  while ((buffer[len] == '\n') || (buffer[len] == '\r'));

  os << "<simulation xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"OSSS+R_simulation_schema.xsd\" date=\""
     << buffer << "\">" << std::endl;
  os << "<types>" << std::endl;
  for (size_t dev_nr = 0; m_device_type_table.size() > dev_nr; dev_nr++)
  {
    m_device_type_table[dev_nr]->writeXMLStringOn(os);
    os << std::endl;
  }
  os << "</types><ressources>" << std::endl;
  for (size_t dev_nr = 0; m_device_instance_table.size() > dev_nr; dev_nr++)
  {
    m_device_instance_table[dev_nr]->writeXMLStringOn(os);
    os << std::endl;
  }
  os << "</ressources><trace>" << std::endl;
}

void osss_log::writeFooter(std::ostream & os) const
{
  os << "</trace><processes>" << std::endl;
  for (unsigned int proc_nr = 0; m_process_table.size() > proc_nr; proc_nr++)
  {
    (*m_log_file) << "<process id=\"" << m_process_table[proc_nr]
                  << "\" name=\""
                  << osss_process_registry::get_proc_name(m_process_table[proc_nr])
                  << "\"/>";
  }
  os << "</processes></simulation>" << std::endl;
}

void osss_log::write(const MajorGroup               major_group,
                     const MinorGroup               minor_group,
                     const MicroGroup               micro_group,
                     const osss::osss_device      * hw_device,
                     const osss_recon_handle        recon_object,
                     const osss_slot_descriptor   * slot_desc_ptr,
                     const std::string              context_name,
                     const OSSS_Process_ID_t        originator)
{
  if (m_log_file)
  {
    if (first_event) {
      // first event, so we start with writing the header
      writeHeader(*m_log_file);
      first_event = false;
    }

    // who
    OSSS_Process_ID_t process =
        ::osss::osssi::osss_process_handle::current().get_proc_id();
    (*m_log_file) << "<event this_process_ref=\"" << process
                  // on whom
                  << "\" slot_ref=\"" << slot_desc_ptr
                  << "\" named_context_ref=\"" << context_name
                  << "\" content_ref=\"0"
                  << "\" originator_process_ref=\"" << originator
                  << "\"> <time ";
    // when
    writeXMLTimeAttributesOn(sc_core::sc_time_stamp(), *m_log_file);
    // what
    (*m_log_file) << "/><group major=\"";
    switch (major_group)
    {
    case attrSaveEvent:                   (*m_log_file) << "attribute save"; break;
    case configurationEvent:              (*m_log_file) << "partial configuration"; break;
    case attrRestoreEvent:                (*m_log_file) << "attribute restore"; break;
    case accessEvent:                     (*m_log_file) << "access";   break;
    case contentEvent:                    (*m_log_file) << "content lifetime"; break;
    case prefetchEvent:                   (*m_log_file) << "prefetch"; break;
    default:                              (*m_log_file) << "unknown";  break;
    }
    (*m_log_file) << "\" minor=\"";
    switch (minor_group)
    {
    case requestEvent: (*m_log_file) << "request"; break;
    case executeEvent: (*m_log_file) << "execute"; break;
    case returnEvent:  (*m_log_file) << "return"; break;
    case createEvent:  (*m_log_file) << "create"; break;
    case destroyEvent: (*m_log_file) << "destroy"; break;
    default:           (*m_log_file) << "unknown"; break;
    }
    (*m_log_file) << "\" micro=\"";
    switch (micro_group)
    {
    case startEvent:   (*m_log_file) << "start"; break;
    case stopEvent:    (*m_log_file) << "stop"; break;
    default:           (*m_log_file) << "unknown"; break;
    }
    (*m_log_file) << "\"/>";
    (*m_log_file) << "</event>" << std::endl;
  } // if logging on
}

void osss_log::writeXMLTimeAttributesOn(const sc_core::sc_time data,
                                        std::ostream & os)
{
    // to_string delivers something like "12 ns".
    // we copy it to buffer and let separator
    // point to the blank in between
    char buffer[64];
    strncpy(buffer, data.to_string().c_str(), 63);
    buffer[63]='\0';
    int separator = 0;
    while ((separator < 63) && (buffer[separator] != ' '))
    {
      ++separator;
    }
    if (separator < 63)
    {
      buffer[separator] = '\0';
      os << "value=\"" << buffer
         << "\" unit=\"" << &(buffer[separator+1]) << "\"";
    }
    else
    {
      // ups there was something wrong with the string
      std::cerr << "WARNING! Cannot write time \"" << data
                << "\" on stream. Internal error." << std::endl;
      os << "<!-- " << data << "-->";
    }
}

void
osss_log::addUserProcess(osssi::OSSS_Process_ID_t user_process, int priority)
{
  m_process_table.push_back(user_process);
  sort(m_process_table.begin(), m_process_table.end());
  unique(m_process_table.begin(), m_process_table.end());
}

} // end namespace osssi
} // end namespace osss

// our instance
osss::osssi::osss_log osss_event_log;
// $Id: osss_log.cpp 2394 2008-07-01 20:45:28Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
