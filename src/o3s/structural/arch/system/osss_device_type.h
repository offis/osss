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

#ifndef OSSS_DEVICE_TYPE_H
#define OSSS_DEVICE_TYPE_H

#include <vector>
#include <string>
#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/is_derived.h"
#include "o3s/structural/app/osss_type_descriptor_registry.h"

namespace osss {

/** This class is used to remember all device types
    (e.g. different FPGA families). One instance for
    each device type. */
class osss_device_type
  : public osssi::osss_type_descriptor_registry
{
  private:
  /// User-defined name string
  std::string m_name;

  public:
  /// Constructor with user-defined name string.
  explicit osss_device_type(const std::string name);
  ~osss_device_type();

  void writeXMLStringOn(std::ostream & os) const;
  //void printOn(ostream & os) const;
};

inline
::std::ostream &
operator<<( ::std::ostream & os,
            const osss_device_type & object )
{
  //object.printOn(os);
  object.writeXMLStringOn(os);
  return os;
}

template< typename TargetType, typename SourceType /*=void*/ >
void osss_declare_time( osss_device_type&              dev_type
                      , sc_core::sc_time               attr_copy_time
                      , sc_core::sc_time               recon_time
                      , osssi::osss_recon_base const * recon_object = NULL )
{

  // static check, if TargetType is derived from osss_object
  typedef typename osssi::is_derived< TargetType >::type target_type;

  /// \todo add meta-function to check if void, or derived from osss_object
  typedef SourceType source_type;

  dev_type.addType( osssi::type_id::from< target_type >()
                  , attr_copy_time );

  dev_type.addReconfigurationTime( osssi::type_id::from< target_type >()
                                 , osssi::type_id::from< source_type >()
                                 , recon_time, recon_object );
}

} // end namespace osss

#endif // OSSS_DEVICE_TYPE_H
// $Id: osss_device_type.h 3026 2009-03-04 22:55:31Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
