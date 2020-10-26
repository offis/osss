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

#ifndef OSSS_ELABORATION_HELPER_H
#define OSSS_ELABORATION_HELPER_H 1

#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_systemc.h"
#include "o3s/datatypes/osss_vector.h"

namespace osss {

class osss_system;

namespace osssi {

class osss_system_partition;

/**
 * \class osss_elaboration_helper
 * \brief helper class to reduce coupling between osss_system
 *        and elaboration classes
 *
 * This class is a simple wrapper around elaboration-specific
 * information originally contained in the osss_system class.
 * It has been introduced to reduce the dependencies between
 * osss_system and the elaboration classes, in particular the
 * elaborator base class osss_elab_base.
 *
 * \todo This class is only a quick solution. The
 *       elaboration mechanism and the associated information
 *       should be reworked at some time.
 *
 * \see osss_system, osss_elab_base
 */
class osss_elaboration_helper
{
public:
  typedef std::vector<sc_core::sc_port_base*>       ports_vec_t;
  typedef osss_vector<osssi::osss_system_partition> partition_vec_t;

  explicit
  osss_elaboration_helper( const osss_system& );

  const osss_system *     m_system;
  const ports_vec_t &     m_externalPorts;
  const partition_vec_t&  m_custom_ip_vec;
  const partition_vec_t&  m_software_subsystem_vec;
  const partition_vec_t&  m_channel_ip_vec;
};

} // namespace osssi
} // namespace osss

#endif // OSSS_ELABORATION_HELPER_H

// $Id: osss_elaboration_helper.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
