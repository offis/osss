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

#ifndef OSSS_PORT_RMI_HH
#define OSSS_PORT_RMI_HH

#include "o3s/communication/arch/osss_channel/osss_port_to_channel.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_channel_client_if.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_if.h"

namespace osss {

template<class IF>
class osss_port<osss_rmi_if<IF> > : 
  public osss_port< osss_channel_if<osss_rmi_channel_client_if> >,
  public osss_port_rmi_information_if
{
public:

  typedef osss_port<osss_channel_if<osss_rmi_channel_client_if> > base_type;
  typedef osss_port<osss_rmi_if<IF> > this_type;

  // transactor dummies (needed for port-to-port binding)
  class server_transactor {};
  class client_transactor {};
    
  osss_port()
    : base_type(),
      m_obj_ptr( NULL ),
      m_shared_obj_ptr( NULL ),
      m_priority( 0 ),
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_bound_to_object_socket( false ),
      m_parent_port( NULL ),
      m_is_accessor( true ),
      m_bound_channel( NULL ),
      m_bound_object_socket( NULL )
  {
    std::cout << "Warning: You are using the default constructor of "
              << "osss_port<osss_rmi_if<...> >." << std::endl
              << "For synthesis it is required that the name of this port "
              << "matches its identifier. " << std::endl
              << "Please use osss_port( const char* ) constructor for osss_port "
              << name() << " instead."<< std::endl
              << std::endl;

    m_obj_stub_ptr = new osss_rmi_if<IF>(this);
  }

  explicit osss_port(const char* name)
    : base_type( name ),
      m_obj_ptr( NULL ),
      m_shared_obj_ptr( NULL ),
      m_priority( 0 ),
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_bound_to_object_socket( false ),
      m_parent_port( NULL ),
      m_is_accessor( true ),
      m_bound_channel( NULL ),
      m_bound_object_socket( NULL )
  {
    m_obj_stub_ptr = new osss_rmi_if<IF>(this);
  }
  
  osss_rmi_channel_client_if* getTransactor()
  {
    return base_type::operator->();
  }

  virtual sc_core::sc_object* bound_channel() const
  {
    return m_bound_channel;
  }

  virtual sc_core::sc_object* bound_object_socket() const
  {
    return m_bound_object_socket;
  }

  void setPriority( osss_priority priority )
  {
    m_priority = priority;
  }

  osss_priority getPriority()
  {
    return m_priority;
  }

  // WARNING: this should be called during simulation only !!!
  osss_rmi_if<IF>* operator->()
  {
    /*
    if (!sc_core::sc_start_of_simulation_invoked())
      std::cout << " ERROR: ";
    */

    if ((m_obj_ptr != NULL) && (m_obj_stub_ptr != NULL))
    {
      if (m_firstAccess)
      {
        m_proc_id = osssi::osss_process_handle::current().get_proc_id();

	//--------------------------------------------------
        // special treatment for shared object:
	//   port registry
	//--------------------------------------------------
	if (boundToSharedObject())
	{
          m_shared_obj_ptr->registerPortUsage(this, m_proc_id);

	  //debug
	  //m_shared_obj_ptr->printPortMap();
	}
	if (base_type::m_rmi_channel_ptr)
	{
	  // TODO: unify with port registry of shared object ???
          base_type::m_rmi_channel_ptr->update_client_info(this, m_proc_id);
	}
	
	m_firstAccess = false;
      }
      else
      {
	if ( m_proc_id != osssi::osss_process_handle::current().get_proc_id() )
	{
	  std::cerr << "Error: This " << kind() << " "
		    << base_type::name()
		    << " has been accessed from within another process before." 
		    << std::endl;
	  // stop simulation due to error
	  sc_core::sc_stop();
	}
      }

      return m_obj_stub_ptr;
    }
    else
    {
      std::cerr << "Error in " << kind() << " "
                << base_type::name()
                << " port is not bound to an object" << std::endl;
      OSSS_ABORT();
    }
    return NULL;
  }
  

  // this binding operator is intended to be used with OSSS RMI-Ports that are
  // inside sc_modules (HW) that are no osss_software_task
  template< class Channel, class Object >
  void operator()(Channel& channel, Object& object)
  {
    m_bound_to_object_socket = true;
    m_bound_channel = &channel;
    m_bound_object_socket = &object;

    // check if the channel parameter is derived from osss_basic_channel
    osss_basic_channel* basic_channel = 
      dynamic_cast<osss_basic_channel*>(&channel);

    // check of the object parameter is derived from osss_object_socket_if
    osss_object_socket_if* object_socket =
      dynamic_cast<osss_object_socket_if*>(&object);
    
    if ((basic_channel != NULL) && (object_socket != NULL))
    {
      //base_type::operator()(*basic_channel);

      if (! dynamic_cast<osss_software_task*>(this->get_parent()))
      {
	base_type::operator()<Channel>(channel);
      }

      IF* obj_if = 
	dynamic_cast<IF*>(&(object.plugged_object()));
      
      if (obj_if == NULL)
      {
	//check for shared object
	m_shared_obj_ptr = 
	  dynamic_cast<osss_shared_if<>*>(&(object.plugged_object()));
	
	m_obj_ptr = dynamic_cast<IF*>(object.plugged_object().operator->());
	
	if ((m_shared_obj_ptr == NULL) || (m_obj_ptr == NULL))
	{
	  std::cerr << "Error in " << kind() << " " 
		    << base_type::name()
		    << " binding failed. " 
		    << "Linked object is neither normal nor shared object." 
		    << std::endl; 
	  OSSS_ABORT();
	}
      }
      else
      {
	m_obj_ptr = obj_if;
      }
    }
    else
    {
      std::cerr << "Error in " << kind() << " "
                << base_type::name()
                << " binding failed" << std::endl; 
      OSSS_ABORT();
    }

    if (! dynamic_cast<osss_software_task*>(this->get_parent()))
    {
      this->register_at_rmi_channel(base_type::m_rmi_channel_ptr);
    }
  }

  // this binding operator is intended to be used with OSSS RMI-Ports that are
  // inside osss_software_task only !!!
  template< class Object >
  void operator()(Object& object)
  {
    m_bound_to_object_socket = true;
    m_bound_object_socket = &object;

    // check of the object parameter is derived from osss_object_socket_if
    osss_object_socket_if* object_socket =
      dynamic_cast<osss_object_socket_if*>(&object);

    if (object_socket != NULL)
    {
      if (! dynamic_cast<osss_software_task*>(this->get_parent()))
      {
	std::cerr << "Error: The binding operator: "
		  << __PRETTY_FUNCTION__ << " is intended to be used with OSSS"
		  << " RMI-Ports that are inside OSSS Software Tasks only!" 
		  << std::endl;
	OSSS_ABORT();
      }

      IF* obj_if = 
	dynamic_cast<IF*>(&(object.plugged_object()));
      
      if (obj_if == NULL)
      {
	//check for shared object
	m_shared_obj_ptr = 
	  dynamic_cast<osss_shared_if<>*>(&(object.plugged_object()));
	
	m_obj_ptr = dynamic_cast<IF*>(object.plugged_object().operator->());
	
	if ((m_shared_obj_ptr == NULL) || (m_obj_ptr == NULL))
	{
	  std::cerr << "Error in " << kind() << " " 
		    << base_type::name()
		    << " binding failed. " 
		    << "Linked object is neither normal nor shared object." 
		    << std::endl; 
	  OSSS_ABORT();
	}
      }
      else
      {
	m_obj_ptr = obj_if;
      }
    }
    else
    {
      std::cerr << "Error in " << kind() << " "
                << base_type::name()
                << " binding failed" << std::endl; 
      OSSS_ABORT();
    }
  }
 
  // port to port binding
  void operator()(this_type& port)
  {
    base_type::bind_to_port(port);

    //we have a parent port due to port->port binding
    m_parent_port = &port;
  }

  virtual bool is_hierarchical() const
  {
    return (!m_bound_to_object_socket && m_parent_port);
  }

  // warning: this method can only be called when osss_rmi_port elaboration
  //          has been completed
  virtual bool is_accessor() const
  {
    return m_is_accessor;
  }

  virtual const char* kind() const
  { 
    return "osss_port_rmi"; 
  }

  unsigned int getObjectID()
  {
    if (m_obj_ptr != NULL)
    {
//      std::cout << "name: " << name() << std::endl;
//      std::cout << "getObjectID: m_shared_obj_ptr = " << m_shared_obj_ptr << std::endl;
      return base_type::getObjectRegistry()->getObjectID(m_shared_obj_ptr);
    }
    else
    {
      std::cerr << "Error in " << kind() << " " 
                << base_type::name()
                << " port is not bound to an object" << std::endl;
      OSSS_ABORT();
    }
    return 0;
  }

  unsigned int getMethodID(std::string methodName)
  {
    if (m_obj_ptr != NULL)
    {
//      std::cout << "name: " << name() << std::endl;
//      std::cout << "getMethodID: m_shared_obj_ptr = " << m_shared_obj_ptr << std::endl;
      return base_type::getObjectRegistry()->
        getMethodRegistry(m_shared_obj_ptr)->getMethodID(methodName);
    }
    else
    {
      std::cerr << "Error in " << kind() << " "
                << base_type::name()
                << " port is not bound to an object" << std::endl;
      OSSS_ABORT();
    }
    return 0;
  }

protected:

  virtual void end_of_elaboration()
  {
    
    if (m_parent_port)
    {
      // the parent port can not be the accessor port
      // maybe we are the accessor port :)
      m_parent_port->m_is_accessor = false;

      // this information is obtained from the parent port
      m_obj_ptr = m_parent_port->m_obj_ptr;
      m_shared_obj_ptr = m_parent_port->m_shared_obj_ptr;
      delete m_obj_stub_ptr; 
      m_obj_stub_ptr = NULL;
      m_obj_stub_ptr = m_parent_port->m_obj_stub_ptr;
      m_priority = m_parent_port->m_priority;

      m_bound_channel = m_parent_port->m_bound_channel;
      m_bound_object_socket = m_parent_port->m_bound_object_socket;
      //if the channel pointer is not set yet, we need to set it now
      //to propagate the binding to an upper level Channel to the port
      //on the lower level!
      //TODO: check if this really works over multiple hierarchy levels
      if(!base_type::m_rmi_channel_ptr)
      {
        this->register_at_rmi_channel(m_parent_port->base_type::m_rmi_channel_ptr);
        base_type::m_rmi_channel_ptr = 
          m_parent_port->base_type::m_rmi_channel_ptr;
      }

      // this can be omitted because these values are assigned during simulation
      //m_firstAccess = port.m_firstAccess;
      //m_proc_id = port.m_proc_id;
    }

    //--------------------------------------------------
    // special treatment for shared object:
    //  subscription
    //--------------------------------------------------
    if (boundToSharedObject())
    {
      m_shared_obj_ptr->subscribe( this, m_priority );
    }

    // debug information
    /*
    std::cout << "name: " << base_type::name() 
              << " port address: " << this 
              << " priority: " << m_priority 
              << std::endl;
    */
  }

  virtual
  void 
  register_at_rmi_channel(osss_rmi_channel_registry_if* rmi_channel_pointer)
  {
    if (rmi_channel_pointer)
    {
      osss_rmi::client_info c_info;
      c_info.module_name = this->get_parent()->name();
      if (dynamic_cast<osss_software_task*>(this->get_parent()))
	c_info.is_sw_task = true;
      else
	c_info.is_sw_task = false;

      c_info.port_pointer = this;
      c_info.port_name = name();
      c_info.server_pointer = m_shared_obj_ptr;
      c_info.client_id = 0; // the client id is unknown up to now
      // this value will be updated at the first rmi port
      // access during simulation
      
      rmi_channel_pointer->add_client_info(c_info);
    }
  }

  bool boundToSharedObject()
  {
    return ( m_shared_obj_ptr != NULL );
  }

private:
  // disabled copy constructor
  osss_port(const osss_port&);
  osss_port& operator=(const osss_port&);

  IF *m_obj_ptr;
  osss_shared_if<>* m_shared_obj_ptr;
  osss_rmi_if<IF> *m_obj_stub_ptr;
  osss_priority m_priority;
  bool m_firstAccess;
  osssi::OSSS_Process_ID_t m_proc_id;
  bool m_bound_to_object_socket;
  this_type* m_parent_port;
  bool m_is_accessor;

  // holds the binding information for architectural context extraction
  sc_core::sc_object* m_bound_channel;
  sc_core::sc_object* m_bound_object_socket;
};

} // namespace osss

#endif
// $Id: osss_port_rmi.h 2992 2009-01-28 15:50:28Z henning $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
