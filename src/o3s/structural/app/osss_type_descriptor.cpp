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

#include "o3s/structural/app/osss_type_descriptor.h"
#include "o3s/utils/osss_log.h"

namespace osss {
namespace osssi {

osss_type_descriptor::osss_type_descriptor(type_id          rtti_type,
                                           sc_core::sc_time attribute_copy_time)
    : m_type_info( rtti_type ),
      m_attribute_copy_time( attribute_copy_time )
{}

void
osss_type_descriptor::writeXMLStringOn( std::ostream & os ) const
{
  // establish a string buffer for modifications
  std::string buffer;

  // create a sequence of invalid characters which are
  // to be replaced during modification
  char invalid_chars[4];
  strcpy(invalid_chars, "_<>");
  invalid_chars[0] = '"';
  invalid_chars[3] = '\0';

  // initialise buffer with name as given by RTTI
  buffer = getTypeName();

  // replace every invalid char with '_'
  for (unsigned int i = 0; i < buffer.length(); ++i)
  {
    for (unsigned int n = 0; strlen(invalid_chars) > n; ++n)
    {
      buffer[i] = (buffer[i] == invalid_chars[n] ? '_' : buffer[i]);
    }
  }

  // ...and print things
  os << "<class_type name=\"" << buffer
     << "\"><attribute_copy_time ";
  osss_log::writeXMLTimeAttributesOn(m_attribute_copy_time, os);
  os << "/> <logic_reconfiguration_time ";
  osss_log::writeXMLTimeAttributesOn(getConfigurationTime(NULL, NULL), os);
  os << "/></class_type>" << std::endl;
}

bool
osss_type_descriptor::match( type_id rtti ) const
{
  return ( rtti == m_type_info);
}

osss_type_descriptor::LocationDescriptor::size_type
osss_type_descriptor::findLocationIndex(const osss_recon_base * for_location) const
{
  LocationDescriptor::size_type index = 0;
  LocationDescriptor::size_type table_size = m_location_table.size();
  while (index < table_size)
  {
    if (m_location_table[index].m_location == for_location)
    {
      return index;
    }
    ++index;
  }
  return table_size;
}

// Pass NULL for descriptor if non-shortcut reconfiguration is selected.
// Passing "this" for old class will give you SC_ZERO_TIME.
sc_core::sc_time
osss_type_descriptor::getConfigurationTime(const osss_recon_base *      for_location,
                                           const osss_type_descriptor * old_class) const
{
  if (this == old_class)
  {
    return sc_core::SC_ZERO_TIME;
  }
  // Try to find detail table, assuming that the user did pass NULL as
  // he wanted or a valid value as he wanted.
  LocationDescriptor::size_type location_table_size = m_location_table.size();
  LocationDescriptor::size_type location_index = findLocationIndex(for_location);
  if ((location_table_size == location_index) && (for_location != NULL))
  {
    // Recon-object specific search failed. Retry with device-wide settings.
    // We assume, the user passed NULL...
    location_index = findLocationIndex(NULL);
  }
  // Is the index valid by now?
  if (location_table_size != location_index)
  {
    // Yes, success.
    // Now see if there is a shortcut. Again, we assumed that the
    // user passed exactly what he wanted.
    LocationDescriptor::SourceClassDescriptor::size_type time_index;
    LocationDescriptor::SourceClassDescriptor::size_type time_table_size = m_location_table[location_index].m_time_table.size();
    time_index = m_location_table[location_index].findTimeIndex( old_class );
    if ((old_class != NULL) && (time_index == time_table_size))
    {
      // Shortcut lookup failed. Retry without shortcut. Fallback.
      time_index = m_location_table[location_index].findTimeIndex( NULL );
    }
    // Do we have an entry now?
    if (time_index != time_table_size)
    {
      // Yes, success! Satisfy the user.
      return m_location_table[location_index].m_time_table[time_index].m_duration;
    }
  }
  // There was something going wrong. Complain.
  std::cerr << "ERROR! Cannot lookup configuration time for type \"" << getTypeName() << "\"." << std::endl;
  OSSS_ABORT();
  return sc_core::SC_ZERO_TIME;
}

void
osss_type_descriptor::addReconfigurationTime(const osss_recon_base *      for_location,
                                             const osss_type_descriptor * from_old_class,
                                             const sc_core::sc_time       reconf_time)
{
  // Is there a detail page for the specified location?
  LocationDescriptor::size_type location_index = findLocationIndex(for_location);
  LocationDescriptor::size_type location_table_size = m_location_table.size();
  if (location_table_size == location_index)
  {
    // There is no such detail page yet. Create one.
    m_location_table.push_back(LocationDescriptor(for_location));
    // Now its definitely there
    location_index = findLocationIndex(for_location);
  }
  // Is there a timing entry as specified by source class (valid or NULL)?
  LocationDescriptor::SourceClassDescriptor::size_type time_index;
  LocationDescriptor::SourceClassDescriptor::size_type time_table_size = m_location_table[location_index].m_time_table.size();
  time_index = m_location_table[location_index].findTimeIndex( from_old_class );
  if (time_index == time_table_size)
  {
    // Not present, add the detail page.
    m_location_table[location_index].m_time_table.push_back(LocationDescriptor::SourceClassDescriptor(from_old_class, reconf_time));
  }
  else
  {
    // Yes, already present. So we got that information earlier.
    // That means, the user specified it twice... something is wrong here...
    std::cerr << "ERROR! Configuration time for \"" << getTypeName() << "\" (";
    if (NULL != for_location)
    {
      std::cerr << "device-wide";
    }
    else
    {
      std::cerr << "osss_recon=" << for_location;
    }
    if (NULL != from_old_class)
    {
      std::cerr << ", differential configuration from class \"" << from_old_class->getTypeName() << "\"";
    }
    else
    {
      std::cerr << ", normal configuration";
    }
    std::cerr << ") is already defined" << std::endl;
    OSSS_ABORT();
  }
}

osss_type_descriptor::LocationDescriptor::SourceClassDescriptor::size_type
osss_type_descriptor::LocationDescriptor::findTimeIndex(const osss_type_descriptor * old_class) const
{
  SourceClassDescriptor::size_type index = 0;
  SourceClassDescriptor::size_type table_size = m_time_table.size();
  while (index < table_size)
  {
    if (m_time_table[index].m_source == old_class)
    {
      return index;
    }
    ++index;
  }
  return table_size;
}


} // end namespace osssi
} // end namespace osss

// $Id: osss_type_descriptor.cpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
