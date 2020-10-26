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

#ifndef OSSS_TYPE_DESCRIPTOR_REGISTRY_H
#define OSSS_TYPE_DESCRIPTOR_REGISTRY_H

#include "o3s/structural/app/osss_type_descriptor.h"

namespace osss {
namespace osssi {

/** This class provides a registry of type descriptors
    for user classes. These descriptors contain OSSS+R
    relevant timing information. And here is the source
    to search/create/modify/read them */
class osss_type_descriptor_registry
{
  private:
  /// This table contains all the descriptors
  std::vector<osssi::osss_type_descriptor> m_type_descriptor_table;

  public:
  osss_type_descriptor_registry();
  ~osss_type_descriptor_registry();

  /** Determine amount of registered types */
  osss_type_idx
  getNumberOfTypes() const;


  /** Make a new type known. This will cause a new descriptor
      to be created. Don't do this twice for the same type,
      the method will complain.
      The name is the string to be returned by the C++ RTTI
      [typeid(T).name()].
    */
  void
  addType( type_id rtti, sc_core::sc_time attribute_copy_time);

  /** Use this method to make reconfiguration time known to
      the registry. You need to specify the type id for
      the target class (as given by typeid()).
      Additionally, the reconfguration time is required.
      Optionally, you can provide a source type id. This
      marks the time as relevant for shortcuts from
      source to target cast (differential bitstream).
      Provide NULL for source class if you want to indicate
      a normal partial reconfiguration (not differential).
      If you provide "for_recon_object", the entry does
      apply to the recon object only. If you pass NULL here,
      the entry is remembered for the whole device.
   */
  void
  addReconfigurationTime(type_id target_rtti,
                         type_id source_rtti,
                         sc_core::sc_time reconfiguration_time,
                         const osss_recon_base * for_recon_object);

  /** Looks up a type descriptor for a given type. You may modify
      but not delete it... It returns NULL if not found. */
  osssi::osss_type_descriptor * findType(type_id rtti);

  /** Write the type registry on stream "os" as XML tags. */
  void
  writeXMLStringOn(std::ostream & os) const;
};

} // end namespace osssi
} // end namespace osss

#endif // OSSS_TYPE_DESCRIPTOR_REGISTRY_H
// $Id: osss_type_descriptor_registry.h 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
