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

#include "osss_system.h"
#include "osss_system_partition_interface.h"

#include "o3s/structural/app/osss_software_task.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_if.h"
#include "o3s/communication/arch/osss_rmi/osss_object_socket_if.h"
#include "o3s/structural/arch/user_defined_hardware/osss_module.h"
#include "o3s/structural/arch/sw_processor/osss_processor_base.h"

namespace osss {
namespace osssi {

class osss_interface_attribute
  : public sc_core::sc_attr_base
{
public:

  // constructors
  osss_interface_attribute() : 
    sc_attr_base( "osss_interface_attribute" ),
    interface_vec()
  {}

  osss_interface_attribute( const osss_interface_attribute& a ) : 
    sc_attr_base( a.name() ),
    interface_vec() // do NOT copy interface vector (?)
  {}

public:

  // public data member; for easy access
  std::vector<std::pair<const sc_core::sc_port_base*,
                        const osss_port_rmi_information_if*> > interface_vec;
private:
  // disabled
  osss_interface_attribute& operator = ( const osss_interface_attribute& );

}; /* class osss_interface_attribute */

//------------------------------------------------------------------------------

void
osss_system_partition::build_interface_vector()
{
  unsigned int interface_counter = 0;

  // go through all objects of this system partition
  for(unsigned int i=0;i<m_object_vec.size(); i++) {
    sc_core::sc_attr_base* attrib = 
      m_object_vec[i]->get_attribute("osss_interface_attribute");
    // if osss_interface_attribute found
    if (attrib) {
      osss_interface_attribute* if_attrib =
        dynamic_cast<osss_interface_attribute*>(attrib);
      // go through the interface vector of this osss_interface_attribute
      for(unsigned j=0; j<if_attrib->interface_vec.size(); j++) {

        std::ostringstream os;
        os << m_name
           << "_IF_"
           << interface_counter;

        osss_system_partition_interface* part_if =
          new osss_system_partition_interface( os.str() );

        part_if->m_module_ptr = m_object_vec[i];

        part_if->m_port_ptr = 
          dynamic_cast<const sc_core::sc_object*>(
            if_attrib->interface_vec[j].first);

        part_if->m_channel_ptr = 
          if_attrib->interface_vec[j].second->bound_channel();

        m_interface_vec.push_back(part_if);
        ++interface_counter;
      } // for (interfaces)
    } // if (attrib)
  } // for (objects)
}

} // namespace osssi

//------------------------------------------------------------------------------

void
osss_system::end_of_elaboration()
{
    //find IP blocks and partition the system accordingly
    this->partition_system();

    osss_elaboration::instance().elaborate( *this );

    //for debugging
#if defined OSSS_SYSTEM_DEBUG
    std::cout << this->print_custom_IP_vec();
    std::cout << this->print_software_subsystem_vec();
    std::cout << this->print_channel_ip_vec();
#endif
}

void 
osss_system::partition_system()
{
  //Important limitation: The system partitioning does not work
  //hierarchical. I.e. all IP blocks need to be visible in the
  //top-level module
  std::vector<sc_core::sc_object*> system_objs(get_child_objects());

  this->extractExternalPorts(system_objs);
  this->build_custom_ip_partitions(system_objs);
  this->build_software_subsystem_partitions(system_objs);
  this->build_channel_ip_partitions(system_objs);
}

//----------------------------------------------------------------------------
// Ports on the top level represent external ports of the design
//----------------------------------------------------------------------------
void
osss_system::extractExternalPorts(std::vector<sc_core::sc_object*>& system_objs)
{
  using sc_core::sc_port_base;

  for(unsigned int i=0; i<system_objs.size(); i++) {
    if (sc_port_base * pb = dynamic_cast<sc_port_base*>(system_objs[i])) {
        m_externalPorts.push_back(pb);
    }
  }
}

//----------------------------------------------------------------------------
// replace tag IDs to merge partitions
//----------------------------------------------------------------------------
void 
osss_system::replace_tags(std::vector<sc_core::sc_object*>& system_objs,
                          const std::string& tag_name,
                          unsigned int ID0, unsigned int ID1)
{
  if (ID0 == ID1)
    return;

  unsigned int old_ID;
  unsigned int new_ID;
  if (ID0 < ID1)
  {
    old_ID = ID1;
    new_ID = ID0;
  }
  else
  {
    old_ID = ID0;
    new_ID = ID1;
  }

  for(unsigned int i=0; i<system_objs.size(); i++)
  {
    if(sc_core::sc_attr_base* attr_b = system_objs[i]->
       get_attribute(tag_name))
    {
        sc_core::sc_attribute<unsigned int>* attrib =
          dynamic_cast<sc_core::sc_attribute<unsigned int>*>(attr_b);
        if (attrib && attrib->value == old_ID)
          attrib->value = new_ID;
    }
  }
}


//----------------------------------------------------------------------------
// custom IP helper methods
//----------------------------------------------------------------------------

void 
osss_system::add_to_custom_ip_vec(const unsigned int& ID,
                                  sc_core::sc_object* obj_ptr)
{
  if (m_custom_ip_vec[ID].name() != std::string())
    m_custom_ip_vec[ID].add_object(obj_ptr);
  else
  {
    m_custom_ip_vec[ID] = 
      osssi::osss_system_partition(
        std::string(sc_core::sc_gen_unique_name("osss_custom_ip")));
    m_custom_ip_vec[ID].add_object(obj_ptr);
  }
}

void 
osss_system::build_custom_ip_vec(std::vector<sc_core::sc_object*>& system_objs)
{
  for(unsigned int i=0; i<system_objs.size(); i++)
  {
    if(sc_core::sc_attr_base* attr_b = system_objs[i]->
       get_attribute(std::string("osss_custom_ip")))
    {
      sc_core::sc_attribute<unsigned int>* attrib =
        dynamic_cast<sc_core::sc_attribute<unsigned int>*>(attr_b);
      if (attrib)
        this->add_to_custom_ip_vec(attrib->value, system_objs[i]);
    }
  }
}

void 
osss_system::add_custom_ip_tag(const unsigned int& ID,
                               sc_core::sc_object* obj_ptr)
{
  sc_core::sc_attribute<unsigned int>* tag = 
    new sc_core::sc_attribute<unsigned int>(std::string("osss_custom_ip"), 
                                            ID);
  obj_ptr->add_attribute(*tag);
}

void
osss_system::add_custom_ip_interface_tag
    (const sc_core::sc_port_base* pb,
     const osss_port_rmi_information_if* port_iif,
     sc_core::sc_object* obj_ptr)
{
  sc_core::sc_attr_base* attrib =
    obj_ptr->get_attribute("osss_interface_attribute");
  if (attrib) // attribute found
  {
    osssi::osss_interface_attribute* if_attrib = 
      dynamic_cast<osssi::osss_interface_attribute*>(attrib); 
    if_attrib->interface_vec.push_back(
      std::pair<const sc_core::sc_port_base*,
                const osss_port_rmi_information_if*>(pb, port_iif));
  }
  else
  {
    osssi::osss_interface_attribute* tag = 
      new osssi::osss_interface_attribute();
    tag->interface_vec.push_back(
      std::pair<const sc_core::sc_port_base*,
                const osss_port_rmi_information_if*>(pb, port_iif));
    obj_ptr->add_attribute(*tag);
  }
}

void
osss_system::build_custom_ip_block_interfaces()
{
  for(unsigned int i=0; i<m_custom_ip_vec.size(); i++)
  {
    if(m_custom_ip_vec[i].name() != std::string())
      m_custom_ip_vec[i].build_interface_vector();
  }
}


void
osss_system::build_custom_ip_partitions(std::vector<sc_core::sc_object*>& system_objs)
{
    unsigned int custom_ip_block_counter = 0;

    // 1st run: 
    // --------
    // finds all partitions of sc_modules and osss_object_sockets
    // that are connected by OSSS point-to-point channels
    for(unsigned int i=0; i<system_objs.size(); i++)
    {
      sc_core::sc_module* scm = NULL;

      // check for SystemC Modules
      if(sc_core::sc_module* sm = 
         dynamic_cast<sc_core::sc_module*>(system_objs[i]))

      {
        //pure "SC Modules" only
        if (std::string(sm->kind()) == std::string("sc_module"))
          scm = sm;
      }
      // check for OSSS Modules
      if(osss_module* om =
         dynamic_cast<osss_module*>(system_objs[i]))
      {
        //pure "OSSS Modules" only
        if (std::string(om->kind()) == std::string("osss_module"))
          scm = om;
      }

      if (scm != NULL)
      {
        this->add_custom_ip_tag(custom_ip_block_counter, scm);

        //extract all ports of this module
        std::vector<sc_core::sc_object*> child_objs(scm->get_child_objects());
        for(unsigned int j = 0; j<child_objs.size(); j++)
        {
          // check for ports
          if(sc_core::sc_port_base* pb =
             dynamic_cast<sc_core::sc_port_base*>(child_objs[j]))
          {
            // we only consider osss_port<osss_rmi_if<...> > here
            if (std::string(pb->kind()) == std::string("osss_port_rmi"))
            {
              //which OSSS Channel and object socket is this OSSS RMI port 
              //bound to?
              osss_port_rmi_information_if* iif =
                dynamic_cast<osss_port_rmi_information_if*>(child_objs[j]);
              if (iif)
              {
                sc_core::sc_object* ch_ptr = iif->bound_channel();
                if(ch_ptr)
                {
                  if (std::string(ch_ptr->kind()) == std::string("osss_p2p"))
                  {
                    this->add_custom_ip_tag(custom_ip_block_counter, ch_ptr);

                    //check for connected SO
                    sc_core::sc_object* object_socket_ptr = 
                      iif->bound_object_socket();
                    if (object_socket_ptr)
                    {
                      //check if this object has already been tagged
                      if (object_socket_ptr->num_attributes() == 0)
                        this->add_custom_ip_tag(custom_ip_block_counter, 
                                                object_socket_ptr);
                      else
                      {
                        if(sc_core::sc_attr_base* attr_b = object_socket_ptr->
                           get_attribute(std::string("osss_custom_ip")))
                        {
                          sc_core::sc_attribute<unsigned int>* attrib =
                            dynamic_cast<sc_core::sc_attribute<unsigned int>*>
                            (attr_b);
                          if (attrib)
                            this->replace_tags(system_objs, 
                                               std::string("osss_custom_ip"),
                                               custom_ip_block_counter,
                                               attrib->value);
                        }
                      }
                    }
                  }
                  else
                  {
                    // we have found an IP channel
                    // mark this module as an external interface
                    this->add_custom_ip_interface_tag(pb, iif, scm);
                  } // check for channel kind (osss_p2p, ...)
                } // bound to channel ?
              } // osss_port_rmi_information_if ?
            } // osss_port_rmi ?
            // TODO: We have to consider connections via an sc_signal. 
            // Current version: If one module is ONLY connected via an
            // sc_signal to another module it is identified as an own
            // custom_ip without an interface to a bus.
            // This causes trouble with the MHS generation
          } // sc_core::sc_port_base ?
        } // for all child objects of sc_module

        custom_ip_block_counter++;

      } // scm != NULL ?
    } // for all sc_objects (end 1st run)

    // 2nd run: 
    // --------
    // finds all single osss_object_sockets that are connected to
    // IP channels only
    // AND
    // mark the ports of all osss_object_sockets that are connected
    // to IP channels (external interface)
    for(unsigned int i=0; i<system_objs.size(); i++)
    {
      if(sc_core::sc_module* scm = 
         dynamic_cast<sc_core::sc_module*>(system_objs[i]))
      {
        if(std::string(scm->kind()) == std::string("osss_object_socket"))
        {
          //check for objects that have not already been tagged
          if (scm->num_attributes() == 0)
          {
            this->add_custom_ip_tag(custom_ip_block_counter, scm);
            //new tag needed next time
            custom_ip_block_counter++;
          }

          //check for connections to channels
          osss_object_socket_if* obj_socket_if = 
            dynamic_cast<osss_object_socket_if*>(scm);
          if (obj_socket_if)
          {
            const std::vector<sc_core::sc_port_base*> obj_socket_ports = 
              obj_socket_if->get_ports();
            for(unsigned int j=0; j<obj_socket_ports.size(); j++)
            {
              osss_port_rmi_information_if* iif =
                dynamic_cast<osss_port_rmi_information_if*>(obj_socket_ports[j]);

              if (std::string(iif->bound_channel()->kind()) !=
                  std::string("osss_p2p"))
              {
                // we have found an IP channel
                // mark this module as an external interface
                this->add_custom_ip_interface_tag(obj_socket_ports[j], 
                                                  iif, 
                                                  scm);
              }
            }
          }
        }
      }
    }

    this->build_custom_ip_vec(system_objs);
    this->build_custom_ip_block_interfaces();
  }

//----------------------------------------------------------------------------
// software subsystem helper methods
//----------------------------------------------------------------------------

void 
osss_system::add_to_software_subsystem_vec
  (const unsigned int& ID, sc_core::sc_object* obj_ptr)
{
  if (m_software_subsystem_vec[ID].name() != std::string())
    m_software_subsystem_vec[ID].add_object(obj_ptr);
  else
  {
    m_software_subsystem_vec[ID] = 
      osssi::osss_system_partition(
        std::string(sc_core::sc_gen_unique_name("osss_software_subsystem")));
    m_software_subsystem_vec[ID].add_object(obj_ptr);
  }
}

void 
osss_system::build_software_subsystem_vec
  (std::vector<sc_core::sc_object*>& system_objs)
{
  for(unsigned int i=0; i<system_objs.size(); i++)
  {
    if(sc_core::sc_attr_base* attr_b = system_objs[i]->
       get_attribute(std::string("osss_software_subsystem")))
    {
      sc_core::sc_attribute<unsigned int>* attrib =
        dynamic_cast<sc_core::sc_attribute<unsigned int>*>(attr_b);
      if (attrib)
        this->add_to_software_subsystem_vec(attrib->value, system_objs[i]);
    }
  }
}

void 
osss_system::add_software_subsystem_tag
  (const unsigned int& ID, sc_core::sc_object* obj_ptr)
{
  sc_core::sc_attribute<unsigned int>* tag = 
    new sc_core::sc_attribute<unsigned int>(
      std::string("osss_software_subsystem"), ID);
  obj_ptr->add_attribute(*tag);
} 

void 
osss_system::build_software_subsystem_partitions
  ( std::vector<sc_core::sc_object*>& system_objs )
{
  unsigned int software_subsystem_counter = 0;
  
  // 1st run: 
  // --------
  // finds all software tasks, determine which processor they are
  // bound on and check which channel (processor bus) the processor
  // is bound to
  for(unsigned int i=0; i<system_objs.size(); i++)
  {
    
    // check for OSSS Software Tasks
    if(osss_software_task* swt = 
       dynamic_cast<osss_software_task*>(system_objs[i]))
    {
    
      this->add_software_subsystem_tag(software_subsystem_counter, swt);
      
      if (osssi::osss_processor_base* pb =swt->mapped_on_processor())
      {
        if (sc_core::sc_module* pb_module_ptr =
            dynamic_cast<sc_core::sc_module*>(pb))
        {
          //check if this processor has already been tagged
          if (pb_module_ptr->num_attributes() == 0)
            this->add_software_subsystem_tag(software_subsystem_counter, 
                                             pb_module_ptr);
          else
          {
            if(sc_core::sc_attr_base* attr_b = pb_module_ptr->
               get_attribute(std::string("osss_software_subsystem")))
            {
              sc_core::sc_attribute<unsigned int>* attrib =
                dynamic_cast<sc_core::sc_attribute<unsigned int>*>(attr_b);
              if (attrib)
                this->replace_tags(system_objs,
                                   std::string("osss_software_subsystem"),
                                   software_subsystem_counter,
                                   attrib->value);
            }
          }

          if (sc_core::sc_object* ch_ptr = pb->bound_channel())
          {
            if (std::string(ch_ptr->kind()) == std::string("osss_p2p"))
            {
              std::cerr << "Error: Processor " << pb_module_ptr->name() 
                        << " can not be bount to point-to-point channel "
                        << ch_ptr->name() << "." << std::endl;
              OSSS_ABORT();
            }
            else
            {
              //check if this channel already been tagged
              if (ch_ptr->num_attributes() == 0)
                this->add_software_subsystem_tag(software_subsystem_counter, 
                                                 ch_ptr);
              else
              {
                if(sc_core::sc_attr_base* attr_b = ch_ptr->
                   get_attribute(std::string("osss_software_subsystem")))
                {
                  sc_core::sc_attribute<unsigned int>* attrib =
                    dynamic_cast<sc_core::sc_attribute<unsigned int>*>(attr_b);
                  if (attrib)
                    this->replace_tags(system_objs,
                                       std::string("osss_software_subsystem"),
                                       software_subsystem_counter,
                                       attrib->value);
                }
              }
            }
          }
          else // processor has not been bound to any channel
          {
            // Should that really be an error ???
            /*
            std::cerr << "Error: Processor " << pb_module_ptr->name() 
                      << " has not been bound to any channel." << std::endl;
            OSSS_ABORT();
            */
          }
        }
      }
      else // software task has not been mapped on any processor
      {
        std::cerr << "Error: Software Task " << swt->name() 
                  << " has not been mapped onto any processor." << std::endl;
        OSSS_ABORT();
      }
      
      software_subsystem_counter++;
        
    } // check for OSSS Software Task
  } // for all sc_objects (end 1st run)

  // 2nd run: 
  // --------
  // finds all processors that no software task is mapped onto
  for(unsigned int i=0; i<system_objs.size(); i++)
  {
    if(osssi::osss_processor_base* pb = 
       dynamic_cast<osssi::osss_processor_base*>(system_objs[i]))
    {
      //check for processors that have not already been tagged
      if (system_objs[i]->num_attributes() == 0)
      {
        this->add_software_subsystem_tag(software_subsystem_counter, 
                                         system_objs[i]);
        
        std::cout << "Warning: Processor " << system_objs[i]->name()
                  << "has no software task mapped onto." << std::endl;

        if (sc_core::sc_object* ch_ptr = pb->bound_channel())
        {
          if (std::string(ch_ptr->kind()) == std::string("osss_p2p"))
          {
            std::cerr << "Error: Processor " << system_objs[i]->name() 
                      << " can not be bount to point-to-point channel "
                      << ch_ptr->name() << "." << std::endl;
            OSSS_ABORT(); 
          }
          else
          {
            //check if this channel already been tagged
            if (ch_ptr->num_attributes() == 0)
              this->add_software_subsystem_tag(software_subsystem_counter, 
                                               ch_ptr);
            else
            {
              if(sc_core::sc_attr_base* attr_b = ch_ptr->
                 get_attribute(std::string("osss_software_subsystem")))
              {
                sc_core::sc_attribute<unsigned int>* attrib =
                  dynamic_cast<sc_core::sc_attribute<unsigned int>*>(attr_b);
                if (attrib)
                  this->replace_tags(system_objs,
                                     std::string("osss_software_subsystem"),
                                     software_subsystem_counter,
                                     attrib->value);
              }
            }
          }
        }
        else // processor has not been bound to any channel
        {
          std::cerr << "Error: Processor " << system_objs[i]->name() 
                    << " has not been bound to any channel." << std::endl;
          OSSS_ABORT();
        }

        software_subsystem_counter++;
      }
    }
  }

  this->build_software_subsystem_vec(system_objs);
}


//----------------------------------------------------------------------------
// channel IP helper methods
//----------------------------------------------------------------------------

void 
osss_system::add_to_channel_ip_vec
    (const unsigned int& ID, sc_core::sc_object* obj_ptr)
{
    if (m_channel_ip_vec[ID].name() != std::string())
      m_channel_ip_vec[ID].add_object(obj_ptr);
    else
    {
      m_channel_ip_vec[ID] = 
        osssi::osss_system_partition(
          std::string(sc_core::sc_gen_unique_name("osss_channel_ip")));
      m_channel_ip_vec[ID].add_object(obj_ptr);
    }
}

void 
osss_system::build_channel_ip_vec
    (std::vector<sc_core::sc_object*>& system_objs)
{
  for(unsigned int i=0; i<system_objs.size(); i++)
  {
    if(sc_core::sc_attr_base* attr_b = system_objs[i]->
       get_attribute(std::string("osss_channel_ip")))
    {
      sc_core::sc_attribute<unsigned int>* attrib =
        dynamic_cast<sc_core::sc_attribute<unsigned int>*>(attr_b);
      if (attrib)
        this->add_to_channel_ip_vec(attrib->value, system_objs[i]);
    }
  }
}

void 
osss_system::add_channel_ip_tag
    (const unsigned int& ID, sc_core::sc_object* obj_ptr)
{
  sc_core::sc_attribute<unsigned int>* tag = 
    new sc_core::sc_attribute<unsigned int>(std::string("osss_channel_ip"), 
                                            ID);
  obj_ptr->add_attribute(*tag);
} 

void 
osss_system::build_channel_ip_partitions
    (std::vector<sc_core::sc_object*>& system_objs)
{
  unsigned int channel_ip_counter = 0;
  std::string channel_name;

  // finds all non osss point-to-point channels that have not
  // been tagged until now
  for(unsigned int i=0; i<system_objs.size(); i++)
  {
   if(dynamic_cast<osss_basic_channel*>(system_objs[i]))
    {
      // only non p2p channels
      if (std::string(system_objs[i]->kind()) != std::string("osss_p2p"))
      {
         if (system_objs[i]->num_attributes() == 0)
          { 
           this->add_channel_ip_tag(channel_ip_counter, system_objs[i]);
            channel_ip_counter++;
          }

      }
    }
  }

  this->build_channel_ip_vec(system_objs);

}

} // namespace osss


// $Id: osss_system.cpp 2737 2008-11-27 09:50:25Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
