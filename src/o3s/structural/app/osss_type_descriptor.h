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

#ifndef OSSS_TYPE_DESCRIPTOR_H
#define OSSS_TYPE_DESCRIPTOR_H

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/type_id.h"
#include "osss_recon_base.h"

#include <map>
#include <string>

namespace osss {
namespace osssi {

class osss_type_descriptor
{
  private:
  /// ID of the type (as given by typeid)
  type_id m_type_info;

  /// Remember the time to save and restore attributes
  sc_core::sc_time m_attribute_copy_time;

  /** This nested class stores the reconfiguration
      times depending on the reconfigurable object.
      The entries for the object "NULL" refer
      to device wide information */
  class LocationDescriptor
  {
    public:
    /** Create a detail page. */
    explicit LocationDescriptor(const osss_recon_base * for_location)
    {
      m_location = for_location;
    }

    /// Target reconfigurable object (or NULL for device wide specification)
    const osss_recon_base * m_location;

    /** This subclass stores all timing for shortcut
        reconfiguration (differential bitstreams) as
        well as ordinary partial configuration.
        Differential reconfiguration is indicated by
        using NULL for src */
    class SourceClassDescriptor
    {
      public:
      SourceClassDescriptor(const osss_type_descriptor* src, sc_core::sc_time t)
      {
        m_source = src;
        m_duration = t;
      }

      /// Source type for shortcuts (or NULL for complete reconf.)
      const osss_type_descriptor* m_source;
      ///  Logic reconfiguration time
      sc_core::sc_time m_duration;

      // Size type for m_time_table
      typedef std::vector< SourceClassDescriptor >::size_type size_type;
    };

    /// Here all timing entries are lined up
    std::vector< SourceClassDescriptor > m_time_table;

    /** Pass NULL for old_class if not looking for shortcuts.
        Returns table size iff not found .*/
    std::vector< SourceClassDescriptor >::size_type findTimeIndex(const osss_type_descriptor * old_class) const;

    // Size type for m_location_table
    typedef std::vector< LocationDescriptor >::size_type size_type;
  };

  /// Here all timing infos for all locations on the device can be found
  std::vector< LocationDescriptor > m_location_table;

  /** Pass NULL for location if device wide settings are wanted.
      Returns table size iff not found */
  std::vector< LocationDescriptor >::size_type findLocationIndex(const osss_recon_base * for_location) const;

public:
  osss_type_descriptor( type_id          type
                      , sc_core::sc_time attribute_copy_time);

  /** Returns true if this descriptor represents a type of the type */
  bool match( type_id ) const;

  /** Dumps the whole descriptor in XML syntax */
  void writeXMLStringOn(std::ostream & os) const;

  /** Lookup configuration timing.
      Pass NULL for location if device-type-wide setting is requested
      (that is, the recon object instance is not known).
      Pass NULL for descriptor if non-shortcut reconfiguration is selected.
      It is save to pass the source class, even if there is no such
      entry. You will receive the same value as if you passed NULL here.
      Passing "this" for old class will give you SC_ZERO_TIME.
  */
  sc_core::sc_time getConfigurationTime(const osss_recon_base * for_location,
                               const osss_type_descriptor * old_class) const;

  /** Provide this class with reconfiguration time info. You need
      to pass "reconf_time". You may leave "for_location" and
      "from_old_class" at NULL.
      Passing "for_location" will indicate that the timing applies
      to that recon object only (some sort of specialisation).
      NULL indicates, that the value is valid device-wide.
      Passing "from_old_class" enables you to specify that this
      is a time for differential reconfiguration. Passing NULL
      here indicates plain partial reconfiguration.
    */
  void
  addReconfigurationTime(const osss_recon_base * for_location,
                         const osss_type_descriptor * from_old_class,
                         const sc_core::sc_time       reconf_time);

  /** Returns save 'n restore time */
  inline
  sc_core::sc_time
  getAttributeCopyTime() const
  {
    return m_attribute_copy_time;
  }

  /** Returns RTTI type name for the described class */
  inline
  const std::string
  getTypeName() const
  {
    return m_type_info.name();
  }

  /** Returns RTTI type info struct for the described class */
  inline
  type_id getTypeInfo() const
  {
    return m_type_info;
  }
};

inline
std::ostream & operator<<( std::ostream & os,
                           const osss_type_descriptor & object)
{
  object.writeXMLStringOn(os);
  return os;
}

} // end namespace osssi
} // end namespace osss


#endif // OSSS_TYPE_DESCRIPTOR_H
// $Id: osss_type_descriptor.h 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
