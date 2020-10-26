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

#include "o3s/structural/app/osss_type_descriptor_registry.h"

namespace osss {
namespace osssi {

osss_type_descriptor_registry::osss_type_descriptor_registry()
{}

osss_type_descriptor_registry::~osss_type_descriptor_registry()
{}

osss::osss_type_idx
osss_type_descriptor_registry::getNumberOfTypes() const
{
  return m_type_descriptor_table.size();
}

osss_type_descriptor *
osss_type_descriptor_registry::findType(type_id rtti)
{
  // Get an iterator
  std::vector<osssi::osss_type_descriptor>::iterator it = m_type_descriptor_table.begin();
  // See, if names match
  while (it != m_type_descriptor_table.end())
  {
    if (it->match(rtti))
    {
      return &(*it);
    }
    ++it;
  }
  return NULL;
}

void
osss_type_descriptor_registry::addType(type_id rtti
                                      ,sc_core::sc_time attribute_copy_time)
{
  // See, if there already is a type descriptor
  osss_type_descriptor * td = findType(rtti);
  if (NULL == td)
  {
    // No, we add one
    m_type_descriptor_table.push_back( osss_type_descriptor( rtti, attribute_copy_time ) );
    td = &(m_type_descriptor_table.back());
  }
  else
  {
    // Yes, there already is one! If the user specifies this twice,
    // there probably is something going wrong...
    std::cerr << "ERROR! Attribute copy time for type named \""
              << rtti << "\" already declared" << std::endl;
    OSSS_ABORT();
  }
}

// use "for_recon_object" == NULL for device-wide settings
// use "source_rtti" == type_id() for non-shortcut-timing (normal case)
void
osss_type_descriptor_registry::addReconfigurationTime( type_id  target_rtti
                                                     , type_id  source_rtti
                                                     , sc_core::sc_time shorter_reconfiguration_time
                                                     , const osss_recon_base * for_recon_object)
{
  // Find the type descriptor
  osss_type_descriptor * target_descriptor = findType(target_rtti);
  if (NULL == target_descriptor)
  {
    // None! Alert!
    std::cerr << "ERROR! Attribute copy time for type named \""
              << target_rtti << "\" not yet declared" << std::endl;
    OSSS_ABORT();
  }
  osss_type_descriptor * source_descriptor = NULL;
  if (type_id::none != source_rtti)
  {
    source_descriptor = findType(source_rtti);
  }
  // Make the information known to the descriptor
  target_descriptor->addReconfigurationTime(for_recon_object, source_descriptor, shorter_reconfiguration_time);
}

void
osss_type_descriptor_registry::writeXMLStringOn(std::ostream & os) const
{
   // dump all descriptors
   for (unsigned int i = 0; m_type_descriptor_table.size() > i; ++i)
   {
     m_type_descriptor_table[i].writeXMLStringOn(os);
   }
}

} // end namespace osssi
} // end namespace osss

// $Id: osss_type_descriptor_registry.cpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
