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

#ifndef OSSS_RMI_TYPES_HH
#define OSSS_RMI_TYPES_HH

#include "o3s/utils/osss_process_handle.h"
#include "o3s/datatypes/osss_serialisable_archive.h"
#include "o3s/communication/arch/osss_channel/osss_address_map.h"

namespace osss {

  class osss_object_socket_if;

  typedef osss_serialisable_archive OSSS_Archive_t;
  typedef unsigned int OSSS_MethodID_t;
  typedef unsigned int OSSS_ObjectID_t;
  typedef osssi::OSSS_Process_ID_t OSSS_ClientID_t;
  
  namespace osss_rmi {
    
    typedef enum{start_id   = 0,
		 end_id     = 4,
                 submit_id  = 8,
                 receive_id = 12,
   	         error_id   = 16} Command_ID_t;

    typedef enum{idle = 0,
		 wait_for_access, 
		 access_granted, 
		 method_executing, 
		 method_execution_done} server_state;

    struct server_info
    {
      OSSS_ObjectID_t object_id;
      std::string     object_name;
      void*           server_pointer;
      unsigned int    num_clients;
    };

    struct client_info
    {
      std::string     module_name;
      bool            is_sw_task;
      void*           port_pointer;
      std::string     port_name;
      void*           server_pointer;
      OSSS_ClientID_t client_id;
    };

  }

  class osss_rmi_channel_registry_if
  {
    public:
      virtual void add_server_info(osss_rmi::server_info& server_info) = 0;
      virtual void add_client_info(osss_rmi::client_info& client_info) = 0;
      virtual void update_client_info(void* port_addr, 
				      const OSSS_ClientID_t& client_id) = 0;

      virtual ~osss_rmi_channel_registry_if() {}
  };

  class osss_rmi_channel_information_if
  {
    public:
      virtual std::string print_server_info() = 0;
      virtual std::string print_client_info() = 0;
      virtual std::string print_address_map() = 0;
      virtual std::string print_client_port_to_offset_map() = 0;

      virtual unsigned long get_base_address() = 0;
      virtual unsigned long get_high_address() = 0;

      virtual osss_address_range<unsigned long> 
      get_address_range_as_long(osss_object_socket_if* obj_socket_ptr) = 0;
    
      virtual ~osss_rmi_channel_information_if() {}
  };

  class osss_port_rmi_information_if
  {
    public:
      //! Returns the channel instance that is bound to this RMI port.
      //! When no channel is bound, NULL is returned.
      virtual sc_core::sc_object* bound_channel() const = 0;

      //! Returns the Object Socket instance that is bound to this RMI port.
      //! When no Object Socket is bound, NULL is returned.
      virtual sc_core::sc_object* bound_object_socket() const = 0;

      //! Returns whether this RMI port is a hierarchical port.
      //! All RMI ports that are bound directly to other RMI ports
      //! are hierarchical RMI ports. Thus the only non-hierarchical
      //! RMI port is directly connected to an Object Socket.
      virtual bool is_hierarchical() const = 0;

      //! Returns whether this RMI port is directly used by a process to
      //! access the content of an Object Socket. Usually this port is
      //! the most deepest one in a port to port binding sequence.
      virtual bool is_accessor() const = 0;

      virtual ~osss_port_rmi_information_if() {}
  };

  inline
  std::ostream &
  operator<<( std::ostream & os, const osss_rmi::Command_ID_t& cmd )
  {
    switch(cmd)
    {
      case osss_rmi::start_id : os << "start_id"; break;
      case osss_rmi::end_id : os << "end_id"; break;
      case osss_rmi::submit_id : os << "submit_id"; break;
      case osss_rmi::receive_id : os << "receive_id"; break;
      case osss_rmi::error_id : os << "error_id"; break;
    }
    return os;
  }

  inline
  std::ostream &
  operator<<( std::ostream & os, const osss_rmi::server_state& state )
  {
    switch(state)
    {
      case osss_rmi::idle : os << "idle"; break;
      case osss_rmi::wait_for_access : os << "wait_for_access"; break;
      case osss_rmi::access_granted : os << "access_granted"; break;
      case osss_rmi::method_executing : os << "method_executing"; break;
      case osss_rmi::method_execution_done : os << "method_execution_done"; break;
    }
    return os;
  }

  //------------------------------------------------------------------------------
  // osss_method_call_item
  //------------------------------------------------------------------------------

  struct osss_method_call_item
  {
    osss_method_call_item()
    {
      clear();
    }

    void clear()
    {
      clientID = 0;
      objectID = 0;
      methodID = 0;
      access_granted = false;
      in_params = osss_serialisable_archive();
      in_params_complete = false;
      is_executing = false;
      call_done = false;
      return_params = NULL;
      return_params_complete = false;
    }

    osss_method_call_item(const osss_method_call_item& mci)
    {
      clientID = mci.clientID;
      objectID = mci.objectID;
      methodID = mci.methodID;
      access_granted = mci.access_granted;
      in_params = mci.in_params;
      in_params_complete = mci.in_params_complete;
      is_executing = mci.is_executing;
      call_done = mci.call_done;
      if (mci.return_params != NULL)
      {
	if (return_params)
	  delete return_params;
	return_params = new OSSS_Archive_t(*mci.return_params);
      }
      else
      {
	/*
	  if (return_params != NULL)
	  delete return_params;
	*/
	return_params = NULL;
      }
      return_params_complete = mci.return_params_complete;
    }

    osss_method_call_item& operator=(const osss_method_call_item& mci)
    {
      if (this == &mci)
	return *this;

      clientID = mci.clientID;
      objectID = mci.objectID;
      methodID = mci.methodID;
      access_granted = mci.access_granted;
      in_params = mci.in_params;
      in_params_complete = mci.in_params_complete;
      is_executing = mci.is_executing;
      call_done = mci.call_done;
      if (mci.return_params != NULL)
      {
	if (return_params)
	  delete return_params;
	return_params = new OSSS_Archive_t(*mci.return_params);
      }
      else
      {
	/*
	  if (return_params != NULL)
	  delete return_params;
	*/
	return_params = NULL;
      }
      return_params_complete = mci.return_params_complete;

      return *this;
    }

    ~osss_method_call_item()
    {
      if (return_params != NULL)
	delete return_params;
      return_params = NULL;
    }

    OSSS_ClientID_t        clientID;
    OSSS_ObjectID_t        objectID;
    OSSS_MethodID_t        methodID;
    bool                   access_granted;
    OSSS_Archive_t         in_params;
    bool                   in_params_complete;
    bool                   is_executing;
    bool                   call_done;
    OSSS_Archive_t*        return_params;
    bool                   return_params_complete;
  };

  inline
  std::ostream &
  operator<<( std::ostream & os, const osss_method_call_item& mci )
  {
    os << "clientID: " << mci.clientID 
       << " objectID: " << mci.objectID 
       << " methodID: " << mci.methodID << std::endl;
    
    os << "access granted: " << mci.access_granted << std::endl;
    os << "in params complete: " << mci.in_params_complete << std::endl;
    if (mci.in_params_complete)
      os << "in params: " << std::endl
	 << mci.in_params << std::endl;
    
    os << "is_executing: " << mci.is_executing << std::endl;
    os << "call done: " << mci.call_done << std::endl;

    os << "return params complete: " << mci.return_params_complete << std::endl;
    if (mci.return_params_complete)
      os << "return params: " << std::endl
	 << *mci.return_params;

    return os;
  }

  typedef std::map<const OSSS_ClientID_t, osss_method_call_item> OSSS_MethodID_Record_t;

  inline
  std::ostream &
  operator<<( std::ostream & os, const OSSS_MethodID_Record_t& mir )
  {
    OSSS_MethodID_Record_t::const_iterator iter = mir.begin();
    while(iter != mir.end())
    {
      os << "clientID: " << iter->first << " -> osss_method_call_item" << std::endl;
      os << iter->second << std::endl << std::endl;
      iter++;
    }
    return os;
  }

  typedef std::map<const OSSS_ClientID_t, osss_rmi::server_state> OSSS_RMI_Server_State_t;

  inline
  std::ostream &
  operator<<( std::ostream & os, const OSSS_RMI_Server_State_t& sst )
  {
    OSSS_RMI_Server_State_t::const_iterator iter = sst.begin();
    while(iter != sst.end())
    {
      os << "clientID: " << iter->first << " -> " << iter->second << std::endl;
      iter++;
    }
    return os;
  }

} // namespace osss

#endif
// $Id: osss_rmi_types.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
