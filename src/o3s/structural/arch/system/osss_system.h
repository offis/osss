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
#ifndef OSSS_SYSTEM_H_INCLUDED
#define OSSS_SYSTEM_H_INCLUDED

#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_elaboration.h"
#include "o3s/datatypes/osss_vector.h"

namespace osss {

// forward declarations (public namespace)
class osss_port_rmi_information_if;

namespace osssi {

// forward declarations
class osss_system_partition_interface;
class osss_elaboration_helper;

class osss_system_partition
{
public:

  typedef sc_core::sc_object               object_t;
  typedef osss_system_partition_interface  interface_t;
  typedef std::vector<object_t*>           object_vector_t;
  typedef std::vector<interface_t*>        interface_vector_t;

  explicit
  osss_system_partition(const std::string& name = "" )
  : m_name(name),
    m_object_vec(),
    m_interface_vec()
  {}

  const std::string&        name()       const { return m_name; }
  const object_vector_t&    objects()    const { return m_object_vec; }
  const interface_vector_t& interfaces() const { return m_interface_vec; }

  void add_object( object_t* obj ) {
    m_object_vec.push_back(obj);
  }

  void build_interface_vector();

protected:
  std::string        m_name;
  object_vector_t    m_object_vec;
  interface_vector_t m_interface_vec;

}; /* class osss_system_partition */

} // namespace osssi

//------------------------------------------------------------------------------

class osss_system
  : public sc_core::sc_module
{
  friend class osssi::osss_elaboration_helper;

  typedef sc_core::sc_module base_type;

protected:

  osss_system()
   : base_type()
   , m_externalPorts()
   , m_custom_ip_vec()
   , m_software_subsystem_vec()
   , m_channel_ip_vec()
  {}

  explicit
  osss_system( sc_core::sc_module_name name )
   : base_type(name)
   , m_externalPorts()
   , m_custom_ip_vec()
   , m_software_subsystem_vec()
   , m_channel_ip_vec()
  {}

public:
  virtual const char* kind() const { return "osss_system"; }

protected:
  virtual void end_of_elaboration();

  /**
   * \note Important limitation: The system partitioning does not work
   *       hierarchical. I.e. all IP blocks need to be visible in the
   *       top-level module.
   */
  void 
  partition_system();

  //----------------------------------------------------------------------------
  // replace tag IDs to merge partitions
  //----------------------------------------------------------------------------

  void 
  replace_tags(std::vector<sc_core::sc_object*>& system_objs,
               const std::string& tag_name, 
               unsigned int ID0, unsigned int ID1);


  //----------------------------------------------------------------------------
  // Ports on the top level represent external ports of the design
  //----------------------------------------------------------------------------
  void
  extractExternalPorts(std::vector<sc_core::sc_object*>& system_objs);

  //----------------------------------------------------------------------------
  // custom IP helper methods
  //----------------------------------------------------------------------------

  void 
  add_to_custom_ip_vec(const unsigned int& ID,
                       sc_core::sc_object* obj_ptr);

  void 
  build_custom_ip_vec(std::vector<sc_core::sc_object*>& system_objs);

  void 
  add_custom_ip_tag(const unsigned int& ID,
                    sc_core::sc_object* obj_ptr);

  void add_custom_ip_interface_tag(const sc_core::sc_port_base* pb,
                                   const osss_port_rmi_information_if* port_iif,
                                   sc_core::sc_object* obj_ptr);

  void build_custom_ip_block_interfaces();

  void 
  build_custom_ip_partitions(std::vector<sc_core::sc_object*>& system_objs);

  //----------------------------------------------------------------------------
  // software subsystem helper methods
  //----------------------------------------------------------------------------

  void 
  add_to_software_subsystem_vec(const unsigned int& ID,
                                sc_core::sc_object* obj_ptr);

  void 
  build_software_subsystem_vec(std::vector<sc_core::sc_object*>& system_objs);

  void 
  add_software_subsystem_tag(const unsigned int& ID,
                             sc_core::sc_object* obj_ptr);

  void 
  build_software_subsystem_partitions(
    std::vector<sc_core::sc_object*>& system_objs);

  //----------------------------------------------------------------------------
  // channel IP helper methods
  //----------------------------------------------------------------------------

  void 
  add_to_channel_ip_vec(const unsigned int& ID,
                        sc_core::sc_object* obj_ptr);

  void 
  build_channel_ip_vec(std::vector<sc_core::sc_object*>& system_objs);

  void 
  add_channel_ip_tag(const unsigned int& ID, sc_core::sc_object* obj_ptr);

  void 
  build_channel_ip_partitions(std::vector<sc_core::sc_object*>& system_objs);


#if 0

  //----------------------------------------------------------------------------
  // Generate ACI (architectural Context Information) 
  // for FOSSY (BETA only)
  //----------------------------------------------------------------------------

  void 
  generate_aci_for(const std::vector<sc_core::sc_object*>& system_objs, 
                   std::ofstream& aci_file);

  std::string
  dump_aci_for(const std::vector<sc_core::sc_object*>& system_objs);

  void 
  build_channel_definitions(osssi::communication_map::CommunicationMap* cmap,
                            const std::vector<sc_core::sc_object*>& system_objs);

  void
  build_channel_mappings(osssi::communication_map::CommunicationMap* cmap,
                         const std::vector<sc_core::sc_object*>& system_objs);


  std::string
  dump_fossy_aci_for(const std::vector<sc_core::sc_object*>& system_objs);

  std::string 
  print_custom_IP_vec();

  std::string
  print_software_subsystem_vec();

  std::string
  print_channel_ip_vec();

  //------------------------------------------------------------------------------
  // print osss_system as dot file
  //------------------------------------------------------------------------------
  void osss_print_system_graph(const std::string& file_name);

#endif

  std::vector<sc_core::sc_port_base*>       m_externalPorts;
  osss_vector<osssi::osss_system_partition> m_custom_ip_vec;
  osss_vector<osssi::osss_system_partition> m_software_subsystem_vec;
  osss_vector<osssi::osss_system_partition> m_channel_ip_vec;

};

} // namespace osss

#endif
// $Id: osss_system.h 2833 2008-12-04 09:29:20Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
