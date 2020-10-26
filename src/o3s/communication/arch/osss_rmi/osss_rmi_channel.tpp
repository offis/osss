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

#include <ios>

//#define DEBUG_RMI

#ifdef DEBUG_RMI
#define DEBUG_OSSS_RMI_CHANNEL_MASTER( _methodID_ ) \
  if ((_CLIENT_ID_) == 24 ) \
  { \
    std::cout << "---------------------------------------" << std::endl; \
    std::cout << " master " << std::endl; \
    std::cout << "---------------------------------------" << std::endl; \
    std::cout << "at " << sc_core::sc_time_stamp() << " " \
         << __FUNCTION__ << ": " << std::endl; \
    std::cout << "  objectID: " << objectID << std::endl; \
    std::cout << "  methodID: " << (_methodID_) << std::endl; \
    std::cout << "  client_id: " << _CLIENT_ID_ << std::endl; \
  } \
  else \
  { \
    std::cout << "\t\t\t\t ---------------------------------------" << std::endl; \
    std::cout << "\t\t\t\t  master " << std::endl; \
    std::cout << "\t\t\t\t ---------------------------------------" << std::endl; \
    std::cout << "\t\t\t\t at " << sc_core::sc_time_stamp() << " " \
         << __FUNCTION__ << ": " << std::endl; \
    std::cout << "\t\t\t\t   objectID: " << objectID << std::endl; \
    std::cout << "\t\t\t\t   methodID: " << (_methodID_) << std::endl; \
    std::cout << "\t\t\t\t   client_id: " << _CLIENT_ID_ << std::endl; \
  }
#else
#define DEBUG_OSSS_RMI_CHANNEL_MASTER( _methodID_ ) 
#endif

//------------------------------------------------------------------------------
// [begin] address map related methods
//

template<class ChannelClass>
inline
void
osss_rmi_channel<ChannelClass>::
add_server_info(osss_rmi::server_info& server_info)
{
  m_list_of_servers.push_back(server_info);
}

template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::
add_client_info(osss_rmi::client_info& client_info)
{
  m_list_of_clients.push_back(client_info);
}

template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::
update_client_info(void* port_addr, const OSSS_ClientID_t& client_id)
{
  // update the list of clients
  for(std::vector<osss_rmi::client_info>::size_type ci = 0;
      ci < m_list_of_clients.size(); ci++)
  {
    if(m_list_of_clients[ci].port_pointer == port_addr)
      m_list_of_clients[ci].client_id = client_id;
  }

  // build client ID -> port addr &&
  //       port addr -> client ID map
  m_client_id_to_port_addr_map[client_id] = port_addr;
  m_port_addr_to_client_id_map[port_addr] = client_id;
}

template<class ChannelClass>
inline
osss_address_range<typename ChannelClass::address_type>
osss_rmi_channel<ChannelClass>::
get_address_range(OSSS_ObjectID_t obj_id)
{
  for(typename std::vector<address_map_item>::size_type i = 0; 
      i < m_address_map.size(); i++)
  {
    if(m_address_map[i].object_id == obj_id)
      return m_address_map[i].address_range;
  }
  // if the requested obj_id is not in the m_address_map an
  // invalid address range (from 0 to 0) is returned
  return osss_address_range<typename ChannelClass::address_type>(0,0);
}

template<class ChannelClass>
inline
typename ChannelClass::address_type 
osss_rmi_channel<ChannelClass>::
get_base_address(OSSS_ObjectID_t& obj_id)
{
  return this->get_address_range(obj_id).low();
}

template<class ChannelClass>
inline
typename ChannelClass::address_type 
osss_rmi_channel<ChannelClass>::
get_high_address(OSSS_ObjectID_t& obj_id)
{
  return this->get_address_range(obj_id).high(); 
}

template<class ChannelClass>
inline
OSSS_ObjectID_t 
osss_rmi_channel<ChannelClass>::
get_object_id(typename ChannelClass::address_type& base_addr)
{
  for(typename std::vector<address_map_item>::size_type i = 0; 
      i < m_address_map.size(); i++)
  {
    if(m_address_map[i].address_range.low() == base_addr)
      return m_address_map[i].object_id;
  }
  //if the requested base address is not assigned to any object in
  //m_address_map 0 is returned
  return 0;
}

template<class ChannelClass>
inline
OSSS_ObjectID_t 
osss_rmi_channel<ChannelClass>::
get_object_id(
  osss_address_range<typename ChannelClass::address_type>& addr_range)
{
  for(typename std::vector<address_map_item>::size_type i = 0; 
      i < m_address_map.size(); i++)
  {
    if(m_address_map[i].address_range == addr_range)
      return m_address_map[i].object_id;
  }
  //if the requested address range is not assigned to any object in 
  //m_address_map an invalid address range (from 0 to 0) is returned
  return osss_address_range<typename ChannelClass::address_type>(0,0);
}

//TODO: throw address_offset_not_found exception
template<class ChannelClass>
inline
unsigned int
osss_rmi_channel<ChannelClass>::
get_address_offset(const OSSS_ClientID_t& client_id)
{
  //client_id -> port_addr
  std::map<const OSSS_ClientID_t, void*>::const_iterator client_id_iter =
    m_client_id_to_port_addr_map.find(client_id);
  if (client_id_iter == m_client_id_to_port_addr_map.end())
  {
    std::cout << "---> client_id (" << client_id << ") -> port_addr not found" << std::endl;
    return 0xFFFFFFFF; //throw exception
  }

  void* port_addr = (*client_id_iter).second;

  //port_addr -> offset
  typename std::map<const void*, 
    typename std::pair<address_map_item*, unsigned int> >::const_iterator 
    port_addr_iter = m_client_port_to_offset_map.find(port_addr);
  if(port_addr_iter == m_client_port_to_offset_map.end())
  {
    std::cout << "---> port_addr (" << port_addr << ") -> offset not found" << std::endl;
    return 0xFFFFFFFF; //throw exception
  }
  
  return (*port_addr_iter).second.second;
}

//TODO: throw client_id_not_found exception
template<class ChannelClass>
inline
OSSS_ClientID_t
osss_rmi_channel<ChannelClass>::
get_client_id(const OSSS_ObjectID_t& obj_id, 
	      const unsigned int& addr_offset)
{
  //obj_id && addr_offset -> port_addr
  address_map_item* address_map_item_ptr = NULL;
  for(typename std::vector<address_map_item>::size_type ami = 0;
      ami < m_address_map.size(); ami++)
  {
    if(m_address_map[ami].object_id == obj_id)
    {
      address_map_item_ptr = &(m_address_map[ami]);
      break;
    }
  }

  if (address_map_item_ptr)
  {
    typename std::pair<address_map_item*, unsigned int> 
      search_pair(address_map_item_ptr, addr_offset);
    typename std::map<const std::pair<address_map_item*, unsigned int>, 
                      void* >::const_iterator iter = 
      m_offset_to_client_port_map.find(search_pair);
    if(iter == m_offset_to_client_port_map.end())
    {
      std::cout << "---> iter == m_offset_to_client_port_map.end()" << std::endl;
      return 0xFFFFFFFF; //throw exception
    }

    void* port_addr = (*iter).second;

    //port_addr -> client_id
    std::map<const void*, OSSS_ClientID_t>::const_iterator 
      port_addr_iter = m_port_addr_to_client_id_map.find(port_addr);
    if(port_addr_iter == m_port_addr_to_client_id_map.end())
    {
      std::cout << "---> port_addr_iter == m_port_addr_to_client_id_map.end()" << std::endl;
      return 0xFFFFFFFF; //throw exception
    }

    return (*port_addr_iter).second;
  }
  else
  {
    std::cout << "---> address_map_item_ptr == NULL" << std::endl;
    return 0xFFFFFFFF; //throw exception
  }
}

template<class ChannelClass>
inline
void
osss_rmi_channel<ChannelClass>::
complete()
{
  // this call of complete builds the inner structure of the OSSS-Channel
  ChannelClass::complete();
 
  build_address_map();
  build_offset_maps();
  assign_address_ranges();
}

template<class ChannelClass>
inline
void
osss_rmi_channel<ChannelClass>::
build_address_map()
{
  //determine the number of clients connected logically to each server
  //and calculate the address map
  for(std::vector<osss_rmi::server_info>::size_type si = 0;
      si < m_list_of_servers.size(); si++)
  {
    for(std::vector<osss_rmi::client_info>::size_type ci = 0;
	ci < m_list_of_clients.size(); ci++)
    {
      if(m_list_of_servers[si].server_pointer ==
	 m_list_of_clients[ci].server_pointer)
	m_list_of_servers[si].num_clients++;
    }

    //address map calculation for RMI protocol
    if (m_list_of_servers[si].num_clients > 0)
    {
      typename ChannelClass::address_type base_addr;
      if (m_address_map.size() == 0)
      {
	base_addr = 0;
      }
      else
      {
	base_addr = m_address_map.back().address_range.high() + 1;
      }
      typename ChannelClass::address_type high_addr = 
	base_addr + 16 * m_list_of_servers[si].num_clients - 1;
      address_map_item addr_map_item;
      addr_map_item.object_id = m_list_of_servers[si].object_id;
      addr_map_item.address_range = 
	osss_address_range<typename ChannelClass::address_type>(
	  base_addr, high_addr);
    
      m_address_map.push_back(addr_map_item);
    }
  }
}

template<class ChannelClass>
inline
void
osss_rmi_channel<ChannelClass>::
build_offset_maps()
{
  for(std::vector<osss_rmi::server_info>::size_type si = 0;
      si < m_list_of_servers.size(); si++)
  {
    unsigned int offset = 0;
    //calculate "port address" -> "address map offset"
    for(std::vector<osss_rmi::client_info>::size_type ci = 0;
	ci < m_list_of_clients.size(); ci++)
    {
      if(m_list_of_servers[si].server_pointer ==
	 m_list_of_clients[ci].server_pointer)
      {
	OSSS_ObjectID_t obj_id = m_list_of_servers[si].object_id;
	//search in address map for matching object ID
	for(typename std::vector<address_map_item>::size_type ami = 0;
	    ami < m_address_map.size(); ami++)
	{
	  if(m_address_map[ami].object_id == obj_id)
	  {
	    address_map_item* addr_map_item_ptr = &(m_address_map[ami]);
	    std::pair<address_map_item*, unsigned int> 
	      offset_pair(addr_map_item_ptr, offset);
	    m_client_port_to_offset_map[m_list_of_clients[ci].port_pointer] = 
	      offset_pair;
	    m_offset_to_client_port_map[offset_pair] = 
	      m_list_of_clients[ci].port_pointer;
	    offset++;
	  }
	}
      }
    }
  }
}

template<class ChannelClass>
inline
void
osss_rmi_channel<ChannelClass>::
assign_address_ranges()
{
  typename std::vector<address_map_item>::size_type ami = 0;
  for(std::vector< osss_signal_if* >::size_type ti = 0;
      ti < ChannelClass::m_transactorList.size(); ti++)
  {
    
    //since each server_transactor implements a slave_if this is the more
    //reasonable way
    server_transactor* st = 
      dynamic_cast<server_transactor*>(ChannelClass::m_transactorList[ti]);
    if (st)
    {
      st->setAddressRange//<typename ChannelClass::address_type>
	( m_address_map[ami].address_range.low(), 
	  m_address_map[ami].address_range.high() );
      ami++;
    }

    /*
    osss_slave_if* slave_if = 
      dynamic_cast<osss_slave_if*>(ChannelClass::m_transactorList[ti]);
    if (slave_if)
    {
      slave_if->setAddressRange<typename ChannelClass::address_type>
	( m_address_map[ami].address_range.low(), 
	  m_address_map[ami].address_range.high() );
      ami++;
    }
    */
  }
}

template<class ChannelClass>
inline
std::string
osss_rmi_channel<ChannelClass>::
print_server_info()
{
  std::ostringstream ostr;

  ostr << "Servers attached to osss_rmi_channel: "
       << ChannelClass::sc_object::name() << std::endl;
  ostr << "=====================================" << std::endl;
  for(typename std::vector<osss_rmi::server_info>::size_type i = 0;
      i < m_list_of_servers.size(); i++)
  {
    ostr << "Server Nr. " << i << std::endl;
    ostr << "\t\t Object ID: " 
         << m_list_of_servers[i].object_id << std::endl;
    ostr << "\t\t Object Name: " 
         << m_list_of_servers[i].object_name << std::endl;
    ostr << "\t\t Number of Clients connected by this channel: " 
         << m_list_of_servers[i].num_clients << std::endl;
    ostr << "\t\t Server Pointer: "
         << m_list_of_servers[i].server_pointer << std::endl;
  }
  ostr << std::endl;

  return ostr.str();
}

template<class ChannelClass>
inline
std::string
osss_rmi_channel<ChannelClass>::
print_client_info()
{
  std::ostringstream ostr;

  ostr << "Clients attached to osss_rmi_channel: "
       << ChannelClass::sc_object::name() << std::endl;
  ostr << "=====================================" << std::endl;
  for(typename std::vector<osss_rmi::client_info>::size_type i = 0;
      i < m_list_of_clients.size(); i++)
  {
    ostr << "Client Nr. " << i << std::endl;
    ostr << "\t\t Module Name: "
	      << m_list_of_clients[i].module_name << std::endl;
    ostr << "\t\t SW Task: "
         << m_list_of_clients[i].is_sw_task << std::endl;
    ostr << "\t\t Port Pointer: " 
         << m_list_of_clients[i].port_pointer << std::endl;
    ostr << "\t\t Port Name: " 
         << m_list_of_clients[i].port_name << std::endl;
    ostr << "\t\t Server Pointer: "
         << m_list_of_clients[i].server_pointer << std::endl;
    ostr << "\t\t Client ID: ";
    if (m_list_of_clients[i].client_id == 0)
      ostr << "N/A";
    else
      ostr << m_list_of_clients[i].client_id; 
    ostr << std::endl;
  }
  ostr << std::endl;

  return ostr.str();
}

template<class ChannelClass>
inline
std::string
osss_rmi_channel<ChannelClass>::
print_address_map()
{
  std::ostringstream ostr;

  ostr << "Address Map of osss_rmi_channel: "
       << ChannelClass::sc_object::name() << std::endl;
  ostr << "================================" << std::endl;
  for(typename std::vector<address_map_item>::size_type i = 0;
      i < m_address_map.size(); i++)
  {
    ostr << "Server Nr. " << i << std::endl;
    ostr << "\t\t Object ID: "
         << m_address_map[i].object_id << std::endl;
    ostr << "\t\t Low: " << std::hex << m_address_map[i].address_range.low() 
         << " (" << std::dec << m_address_map[i].address_range.low() << ")"
         << " High: " << std::hex << m_address_map[i].address_range.high() 
         << " (" << std::dec << m_address_map[i].address_range.high() << ")"
         << std::endl;
  }
  ostr << std::endl;

  return ostr.str();
}

template<class ChannelClass>
inline
std::string
osss_rmi_channel<ChannelClass>::
print_client_port_to_offset_map()
{
  std::ostringstream ostr;

  ostr << "port address -> (address_map_item, offset) map: "
       << ChannelClass::sc_object::name() << std::endl
       << "===============================================" << std::endl;
  for(typename std::map<const void*, 
	       std::pair<address_map_item*, 
                         unsigned int> >::const_iterator iter =
	m_client_port_to_offset_map.begin(); 
      iter != m_client_port_to_offset_map.end(); iter++)
  {
    ostr << (*iter).first << " -> "
         << "( [" 
         << " obj_id: " << (*iter).second.first->object_id << " " 
         << " low: " << (*iter).second.first->address_range.low() << " "
         << " high: " << (*iter).second.first->address_range.high() << " ] "
         << ", " << (*iter).second.second << " )" << std::endl;
  }
  ostr << std::endl;

  return ostr.str();
}

template<class ChannelClass>
inline
osss_address_range<unsigned long> 
osss_rmi_channel<ChannelClass>::
get_address_range_as_long(osss_object_socket_if* obj_socket_ptr)
{
  osssi::osss_object_registry* obj_reg = this->getObjectRegistry();
  
  osss_address_range<typename ChannelClass::address_type> tmp_range =
    this->get_address_range(
      (OSSS_ObjectID_t)(obj_reg->getObjectID(
                          obj_socket_ptr->get_shared_if())));

  osss_address_range<unsigned long> range(tmp_range.low(), tmp_range.high());
  return range;
}

template<class ChannelClass>
inline
unsigned long 
osss_rmi_channel<ChannelClass>::
get_base_address()
{
  unsigned long tmp_base_address = 0;

  for(typename std::vector<address_map_item>::size_type i = 0; 
      i < m_address_map.size(); i++)
  {
    if(i == 0)
      tmp_base_address = m_address_map[0].address_range.low();
    else
    {
      if(m_address_map[i].address_range.low() <= tmp_base_address)
        tmp_base_address = m_address_map[i].address_range.low();
    }
  }
 
  return tmp_base_address;
}

template<class ChannelClass>
inline
unsigned long 
osss_rmi_channel<ChannelClass>::
get_high_address()
{
  unsigned long tmp_high_address = 0;

  for(typename std::vector<address_map_item>::size_type i = 0; 
      i < m_address_map.size(); i++)
  {
    if(i == 0)
      tmp_high_address = m_address_map[0].address_range.high();
    else
    {
      if(m_address_map[i].address_range.high() >= tmp_high_address)
        tmp_high_address = m_address_map[i].address_range.high();
    }
  }
 
  return tmp_high_address;
}

//
// [end] address map related methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// client_transactor (master)
//------------------------------------------------------------------------------

template<class ChannelClass>
inline
void
osss_rmi_channel<ChannelClass>::client_transactor::
reset()
{
  ChannelClass::master::reset();
}

template<class ChannelClass>
inline
OSSS_Archive_t 
osss_rmi_channel<ChannelClass>::client_transactor::
call_function(OSSS_ClientID_t clientID,
	      OSSS_ObjectID_t objectID, 
	      OSSS_MethodID_t methodID,
	      OSSS_Archive_t archive,
              unsigned int size)

{
  #ifdef DEBUG_RMI
  std::cout << "at: " << sc_core::sc_time_stamp() << " " 
       << __PRETTY_FUNCTION__
       << " of client: " << _CLIENT_ID_ 
       << " called" << std::endl;

  std::cout << "clientID: " << clientID
       << " objectID: " << objectID 
       << " methodID: " << methodID 
       << " size: " << size
       << std::endl;
  archive.print_vector();
  #endif

  if (archive.empty())
  {
    #ifdef DEBUG_RMI
    std::cout << "#client " << clientID
              << " function call with empty archive" << std::endl;
    #endif
  }

  this->request_for_method(objectID, methodID);
  if(!archive.empty())
  {
    this->wait_for_grant(objectID, methodID);
    #ifdef DEBUG_RMI
      std::cout << "#client " << clientID
                << " got grant, will submit parameters" << std::endl;
    #endif
    this->submit_parameters(objectID, archive);
  }
  #ifdef DEBUG_RMI
    std::cout << "#client " << clientID
              << " is waiting for method completion" << std::endl;
  #endif
  //general case, we need to poll if the call is completed yet
  //see comment on partial specialisation for point to point channel below
  this->wait_for_completion(objectID);
  #ifdef DEBUG_RMI
    std::cout << "#client " << clientID
              << " call completed, reading return value!" << std::endl;
  #endif
  assert(size>0);

  // receive return parameter
  OSSS_Archive_t return_archive;
  this->receive_return_parameter(objectID, return_archive, size);

  #ifdef DEBUG_RMI
    std::cout << "#client " << clientID
              << " return value was " << return_archive << std::endl;
  #endif

  // announces the end of return parameters reception
  // HK: Do we really need this? I don't think so, hence deactivated
  //this->end_of_transfer(objectID, methodID, true);

  return return_archive;
}

//------------------------------------------------------------------------------

template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::client_transactor::
call_procedure(OSSS_ClientID_t clientID,
	       OSSS_ObjectID_t objectID,
	       OSSS_MethodID_t methodID,
	       OSSS_Archive_t archive)
{
  #ifdef DEBUG_RMI
  std::cout << "at: " << sc_core::sc_time_stamp() << " " 
       << __FUNCTION__
       << " of client: " << _CLIENT_ID_ 
       << " called" << std::endl;
	
  std::cout << "clientID: " << clientID 
       << " objectID: " << objectID 
       << " methodID: " << methodID << std::endl;
  archive.print_vector();
  #endif

  if (archive.empty())
  {
    #ifdef DEBUG_RMI
    std::cout << "#client " << clientID 
              << " procedure call with empty archive" << std::endl;
    #endif
  }
  
  this->request_for_method(objectID, methodID);
  //HK: if we have no Parameters, we do not need to wait for the grant,
  //instead, we just wait for the Method to complete.
  if(!archive.empty())
  {
    #ifdef DEBUG_RMI
      std::cout << "#client " << clientID
                << " is waiting for grant" << std::endl;
    #endif
    this->wait_for_grant(objectID, methodID);
    #ifdef DEBUG_RMI
      std::cout << "#client " << clientID
                << " got grant, will submit parameters" << std::endl;
    #endif
    this->submit_parameters(objectID, archive);
  }
  #ifdef DEBUG_RMI
    std::cout << "#client " << clientID
              << " is waiting for method completion" << std::endl;
  #endif
  this->wait_for_completion(objectID);
  #ifdef DEBUG_RMI
    std::cout << "#client " << clientID
              << " call completed, no return value!" << std::endl;
  #endif

  // receive return parameter
  // HK: This is a procedure call, so there are no return values
  //     to read, so for now I just skip this step.
  //OSSS_Archive_t return_archive;
  //this->receive_return_parameter(objectID, return_archive, 1);

  // announces the end of a wait for completion of the method call
  // HK; Not needed (see above)
  //this->end_of_transfer(objectID, methodID, true);
}

//--------------------------------------------------------------------

template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::client_transactor::
request_for_method(OSSS_ObjectID_t& objectID,
		   OSSS_MethodID_t& methodID)
{
  DEBUG_OSSS_RMI_CHANNEL_MASTER(methodID);

  this->start_parameter_transfer(objectID, methodID);
}

template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::client_transactor::
start_parameter_transfer(OSSS_ObjectID_t& objectID,
			 OSSS_MethodID_t& methodID)
{
  typename ChannelClass::address_type 
    /*
    start_parameter_transfer_address(objectID + 
				     osss_rmi::start_id + 
				     16 * _CLIENT_ID_);
    */
    start_parameter_transfer_address(
      m_rmi_channel_ptr->get_base_address(objectID) + 
      osss_rmi::start_id + 
      16 * m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_));

#ifdef DEBUG_RMI
  std::cout << std::endl;
  std::cout << "start_parameter_transfer:" << std::endl
	    << "=========================" << std::endl;
  std::cout << "obj ID: " << objectID 
	    << " baddr: " << m_rmi_channel_ptr->get_base_address(objectID)
	    << " cmd: " << osss_rmi::start_id
	    << " client ID: " << _CLIENT_ID_ 
	    << " addr offset: " << m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_)
	    << std::endl << "raw addr: " << start_parameter_transfer_address
	    << std::endl << std::endl;
  std::cout << "methodID: " << methodID << std::endl;
#endif

  typename ChannelClass::data_chunk start_parameter_transfer_chunk;
  start_parameter_transfer_chunk.push_back(methodID);
 
  ChannelClass::master::write_blocking(start_parameter_transfer_address,
				       start_parameter_transfer_chunk);
#ifdef DEBUG_RMI
  std::cout << std::endl;
  std::cout << "start_parameter_transfer:" << std::endl
	    << "=========================" << std::endl;
  std::cout << "MID sent!" << std::endl;
#endif
}
 

//TODO: remove end_of_transfer 
template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::client_transactor:: 
end_of_transfer(OSSS_ObjectID_t& objectID,
		OSSS_MethodID_t& methodID,
		bool last_transfer)
{
  std::cerr << __FUNCTION__ << std::endl;
  std::cerr << "  Error: This method should never be called!" << std::endl;
}
  
template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::client_transactor::
idle()
{
  std::cerr << __FUNCTION__ << std::endl;
  std::cerr << "  Error: This method should never be called!" << std::endl;
}
  
template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::client_transactor::
submit_parameters(OSSS_ObjectID_t& objectID,
		  OSSS_Archive_t& archive)
{
  DEBUG_OSSS_RMI_CHANNEL_MASTER("no methodID");
  
  typename ChannelClass::address_type 
    /*
    submit_parameters_address(objectID + 
			      osss_rmi::submit_id + 
			      16 * _CLIENT_ID_);
    */
    submit_parameters_address(
      m_rmi_channel_ptr->get_base_address(objectID) + 
      osss_rmi::submit_id + 
      16 * m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_));

  /*
  std::cout << std::endl;
  std::cout << "submit_parameters:" << std::endl
	    << "==================" << std::endl;
  std::cout << "obj ID: " << objectID 
	    << " baddr: " << m_rmi_channel_ptr->get_base_address(objectID)
	    << " cmd: " << osss_rmi::submit_id
	    << " client ID: " << _CLIENT_ID_ 
	    << " addr offset: " << m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_)
	    << std::endl << "raw addr: " << submit_parameters_address
	    << std::endl << std::endl;
  */
  ChannelClass::master::write_blocking(submit_parameters_address,
					 archive);
}

template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::client_transactor::
wait_for_grant(OSSS_ObjectID_t& objectID,
	       OSSS_MethodID_t& methodID)
{
  DEBUG_OSSS_RMI_CHANNEL_MASTER(methodID);

  typename ChannelClass::address_type 
    /*
    wait_for_grant_address(objectID + 
			   osss_rmi::receive_id + 
			   16 * _CLIENT_ID_);
    */
    wait_for_grant_address(
      m_rmi_channel_ptr->get_base_address(objectID) + 
      osss_rmi::receive_id + 
      16 * m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_));

  /*
  std::cout << std::endl;
  std::cout << "wait_for_grant:" << std::endl
	    << "===============" << std::endl;
  std::cout << "obj ID: " << objectID 
	    << " baddr: " << m_rmi_channel_ptr->get_base_address(objectID)
	    << " cmd: " << osss_rmi::receive_id
	    << " client ID: " << _CLIENT_ID_ 
	    << " addr offset: " << m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_)
	    << std::endl << "raw addr: " << wait_for_grant_address
	    << std::endl << std::endl;
  */

  //HK: This is the general case, so we need to poll.
  typename ChannelClass::data_chunk wait_for_grant_result;
  wait_for_grant_result.push_back(0);

  while (wait_for_grant_result[0] == 0)
  {
    //wait_for_grant_result.clear();
    wait_for_grant_result[0] = 0;

    ChannelClass::master::read_blocking(wait_for_grant_address,
                                        wait_for_grant_result, 1);
 
    if (wait_for_grant_result.size() != 1)
    {
      std::cerr << "Error in " << __FUNCTION__ << std::endl;
      std::cerr << "wait_for_grant_result.size(): " 
		<< wait_for_grant_result.size() << std::endl;
    }
  }
}
  
template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::client_transactor::
wait_for_completion(OSSS_ObjectID_t& objectID)
{
  DEBUG_OSSS_RMI_CHANNEL_MASTER("no methodID");

  typename ChannelClass::address_type 
    /*
    wait_for_completion_address(objectID + 
				osss_rmi::receive_id + 
				16 * _CLIENT_ID_);
    */
    wait_for_completion_address(
      m_rmi_channel_ptr->get_base_address(objectID) + 
      osss_rmi::receive_id + 
      16 * m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_));

#ifdef DEBUG_RMI
  std::cout << std::endl;
  std::cout << "wait_for_completion:" << std::endl
	    << "====================" << std::endl;
  std::cout << "obj ID: " << objectID 
	    << " baddr: " << m_rmi_channel_ptr->get_base_address(objectID)
	    << " cmd: " << osss_rmi::receive_id
	    << " client ID: " << _CLIENT_ID_ 
	    << " addr offset: " << m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_)
	    << std::endl << "raw addr: " << wait_for_completion_address
	    << std::endl << std::endl;
#endif

  typename ChannelClass::data_chunk wait_for_completion_result;
  wait_for_completion_result.push_back(0);

  while (wait_for_completion_result[0] == 0)
  {
    //wait_for_completion_result.clear();
    wait_for_completion_result[0] = 0;

    ChannelClass::master::read_blocking(wait_for_completion_address,
					wait_for_completion_result, 1);

    if (wait_for_completion_result.size() != 1)
    {
      std::cerr << "Error in " << __FUNCTION__ << std::endl;
      std::cerr << "wait_for_completion_result.size(): " 
		<< wait_for_completion_result.size() << std::endl;
    }
  }
}

template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::client_transactor::
receive_return_parameter(OSSS_ObjectID_t& objectID,
			 OSSS_Archive_t& return_archive,
                         unsigned int size)
{
  DEBUG_OSSS_RMI_CHANNEL_MASTER("no methodID");

  typename ChannelClass::address_type 
    /*
    return_parameters_address(objectID + 
			      osss_rmi::receive_id + 
			      16 * _CLIENT_ID_);
    */
    return_parameters_address(
      m_rmi_channel_ptr->get_base_address(objectID) + 
      osss_rmi::receive_id + 
      16 * m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_));

#ifdef DEBUG_RMI
  std::cout << std::endl;
  std::cout << "receive_return_parameter:" << std::endl
	    << "=========================" << std::endl;
  std::cout << "obj ID: " << objectID 
	    << " baddr: " << m_rmi_channel_ptr->get_base_address(objectID)
	    << " cmd: " << osss_rmi::receive_id
	    << " client ID: " << _CLIENT_ID_ 
	    << " addr offset: " << m_rmi_channel_ptr->get_address_offset(_CLIENT_ID_)
	    << std::endl << "raw addr: " << return_parameters_address
	    << std::endl << "size of data: "<< size << std::endl << std::endl;
#endif 

  assert(size>0);

  ChannelClass::master::read_blocking(return_parameters_address,
				      return_archive, size);
}

//------------------------------------------------------------------------------
// server_transactor (slave)
//------------------------------------------------------------------------------


template<class ChannelClass>
inline
void
osss_rmi_channel<ChannelClass>::server_transactor::
resolve_address(typename ChannelClass::address_type raw_address,
		OSSS_ObjectID_t objectID,
                OSSS_ClientID_t &clientID,
		osss_rmi::Command_ID_t &cmd_id)
{
  sc_dt::sc_uint<32> shift_reg(raw_address - 
			       m_rmi_channel_ptr->get_base_address(objectID));
  clientID = m_rmi_channel_ptr->get_client_id(objectID, 
					      shift_reg.range(31, 4).to_uint());
  switch (shift_reg.range(3, 0))
  {
    case 0  : cmd_id = osss_rmi::start_id; break;
    case 4  : cmd_id = osss_rmi::end_id;   break;
    case 8  : cmd_id = osss_rmi::submit_id; break;
    case 12 : cmd_id = osss_rmi::receive_id; break;
    default : cmd_id = osss_rmi::error_id; break;
  }

  /*
  std::cout << std::endl;
  std::cout << "resolve_address:" << std::endl
	    << "================" << std::endl;
  std::cout << "obj ID: " << objectID 
	    << " baddr: " << m_rmi_channel_ptr->get_base_address(objectID)
	    << " cmd: " << cmd_id
	    << " client ID: " << clientID
	    << " addr offset: " << shift_reg.range(31, 4).to_uint()
	    << std::endl << "raw address: " << raw_address
	    << std::endl << std::endl;
  */
}

template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::server_transactor::
reset()
{
  ChannelClass::slave::reset();
}
    
template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::server_transactor::
listen_for_action(OSSS_ClientID_t &clientID,
		  OSSS_ObjectID_t &objectID,
		  OSSS_MethodID_t &methodID,
		  osss_rmi::Command_ID_t &command_id)
{
  #ifdef DEBUG_RMI
  std::cout << "at " << sc_core::sc_time_stamp() << " " 
            << __FUNCTION__ << std::endl;
  #endif

  
  typename ChannelClass::address_type listen_for_action_address;
  typename ChannelClass::action_type  listen_for_action_action;
  typename ChannelClass::data_chunk   listen_for_action_data_chunk;

  ChannelClass::slave::wait_for_action(listen_for_action_address,
				       listen_for_action_action);

  OSSS_ClientID_t requesting_client;
  osss_rmi::Command_ID_t cmd_id;

  this->resolve_address(listen_for_action_address, objectID,
			requesting_client, cmd_id);

  switch (listen_for_action_action)
  {
    case ChannelClass::WRITE_ACTION :
    {
      switch (cmd_id)
      {
	case osss_rmi::start_id :
	  ChannelClass::slave::read_blocking(listen_for_action_data_chunk);
	  if (listen_for_action_data_chunk.size() != 1)
	  {
	    std::cerr << "Error [start_id] size != 1" << std::endl;
	    clientID = requesting_client;
	    methodID = 0;
	    command_id = osss_rmi::error_id;
	  }
	  else
	  {
	    clientID = requesting_client;
	    methodID = listen_for_action_data_chunk[0];
	    command_id = osss_rmi::start_id;
	  }
	  break;

	case osss_rmi::end_id :
	  ChannelClass::slave::read_blocking(listen_for_action_data_chunk);
	  if (listen_for_action_data_chunk.size() != 1)
	  {
	    std::cerr << "Error [end_id] size != 1" << std::endl;
	    clientID = requesting_client;
	    methodID = 0;
	    command_id = osss_rmi::error_id;
	  }
	  else
	  {
	    clientID = requesting_client;
	    methodID = listen_for_action_data_chunk[0];
	    command_id = osss_rmi::end_id;
	  }
	  break;

	case osss_rmi::submit_id :
	  clientID = requesting_client;
	  methodID = 0;
	  command_id = osss_rmi::submit_id;
	  break;

	default:
	  std::cerr << "Error: illegal write command. "
		    << "None of [start_id, end_id, submit_id]." << std::endl;
	  clientID = requesting_client;
	  methodID = 0;
	  command_id = osss_rmi::error_id;
	  break;
      }
    }
    break;

    case ChannelClass::READ_ACTION :
    {
      switch (cmd_id)
      {
	case osss_rmi::receive_id :
	  clientID = requesting_client;
	  methodID = 0;
	  command_id = osss_rmi::receive_id;
	  break;

	default :
	  std::cerr << "Error: illegal read command. "
		    << "None of [receive_id]." << std::endl;
	  clientID = requesting_client;
	  methodID = 0;
	  command_id = osss_rmi::error_id;
	  break;
      }
    }
    break;

    default :
      std::cerr << "Housten, we have a problem!" << std::endl;
      clientID = requesting_client;
      methodID = 0;
      command_id = osss_rmi::error_id;
      break;
  }
}

//------------------------------------------------------------------------------
  
template<class ChannelClass>
inline
void
osss_rmi_channel<ChannelClass>::server_transactor::
return_grant(OSSS_ClientID_t clientID, bool granted, bool hasParameters)
{
  typename ChannelClass::data_chunk return_grant_data_chunk;
  if (granted)
    return_grant_data_chunk.push_back(clientID);
  else
    return_grant_data_chunk.push_back(0);

  ChannelClass::slave::write_blocking(return_grant_data_chunk);
}

//------------------------------------------------------------------------------

template<class ChannelClass>
inline
bool 
osss_rmi_channel<ChannelClass>::server_transactor::
receive_in_params(OSSS_Archive_t &arch)
{
  ChannelClass::slave::read_blocking(arch);
  return true;
}

//------------------------------------------------------------------------------

template<class ChannelClass>
inline
bool 
osss_rmi_channel<ChannelClass>::server_transactor::
provide_return_params(OSSS_ClientID_t clientID,
		      OSSS_Archive_t &arch)
{
  if(arch.empty())
  {
    std::cerr << "trying to submit empty archive!" << std::endl; 
  }
  else
    #ifdef DEBUG_RMI
      std::cout << "MASTER: return Value is" << arch << std::endl;
    #endif
    ChannelClass::slave::write_blocking(arch);

  return true;
}

//------------------------------------------------------------------------------

template<class ChannelClass>
inline
void 
osss_rmi_channel<ChannelClass>::server_transactor::
return_params_idle(OSSS_ClientID_t clientID, bool is_busy, bool is_void)
{
  typename ChannelClass::data_chunk idle_result;

  if (is_busy)
    idle_result.push_back(0);
  else
    idle_result.push_back(1);

  ChannelClass::slave::write_blocking(idle_result);
}

//==============================================================================
// partial specialisation for
//  osss_rmi_point_to_point_channel
//==============================================================================
//HK: Since we allready have this partial specialisation for the point to point 
//channel, we might as well use it to implement some protocoll optimisations.
//The protocoll used in the point to point case does not use polling, to
//determine, wether a value read is valid or not. The general implementation of
//the protocoll needs to use polling in order to make sure that a value read
//from an arbitrary address is actually valid.

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
add_server_info(osss_rmi::server_info& server_info)
{
  m_server_info = server_info;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
add_client_info(osss_rmi::client_info& client_info)
{
  m_client_info = client_info;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
update_client_info(void* port_addr, const OSSS_ClientID_t& client_id)
{
  if (m_client_info.port_pointer == port_addr)
    m_client_info.client_id = client_id;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
OSSS_ClientID_t
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
get_client_id(const OSSS_ObjectID_t& obj_id, 
	      const unsigned int& addr_offset)
{
  //this is fairly easy, we have a point to point channel
  //so we have only one client
  //HK: wait for first access by the client (this sets the client_id)
  while(m_client_info.client_id == 0)
  {
    sc_core::wait();
  }
  return m_client_info.client_id;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
std::string
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
print_server_info()
{
  std::ostringstream ostr;

  ostr << "Server attached to osss_rmi_point_to_point_channel: "
       << ChannelClass::sc_object::name() << std::endl;
  ostr << "===================================================" 
       << std::endl;
 
  ostr << "\t\t Object ID: " 
       << m_server_info.object_id << std::endl;
  ostr << "\t\t Object Name: " 
       << m_server_info.object_name << std::endl;
  ostr << "\t\t Number of Clients connected by this channel: " 
       << m_server_info.num_clients << std::endl;
  ostr << "\t\t Server Pointer: "
       << m_server_info.server_pointer << std::endl;
  
  ostr << std::endl;

  return ostr.str();
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
std::string
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
print_client_info()
{
  std::ostringstream ostr; 

  ostr << "Client attached to osss_rmi_point_to_point_channel: "
       << ChannelClass::sc_object::name() << std::endl;
  ostr << "===================================================" 
       << std::endl;
  
  ostr << "\t\t Module Name: "
       << m_client_info.module_name << std::endl;
  ostr << "\t\t SW Task: "
       << m_client_info.is_sw_task << std::endl;
  ostr << "\t\t Port Pointer: " 
       << m_client_info.port_pointer << std::endl;
  ostr << "\t\t Port Name: " 
       << m_client_info.port_name << std::endl;
  ostr << "\t\t Server Pointer: "
       << m_client_info.server_pointer << std::endl;
  ostr << "\t\t Client ID: ";
  if (m_client_info.client_id == 0)
    ostr << "N/A";
  else
    ostr << m_client_info.client_id; 
  ostr << std::endl;
 
  ostr << std::endl;

  return ostr.str();
}

//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
client_transactor::
reset()
{
  ChannelClass::master::reset();
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
OSSS_Archive_t 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
client_transactor::
call_function(OSSS_ClientID_t clientID,
	      OSSS_ObjectID_t objectID, 
	      OSSS_MethodID_t methodID,
	      OSSS_Archive_t archive,
              unsigned int size)
{
  #ifdef DEBUG_RMI
  std::cout << "at: " << sc_core::sc_time_stamp() << " " 
       << __PRETTY_FUNCTION__
       << " of client: " << _CLIENT_ID_ 
       << " called" << std::endl;

  std::cout << "clientID: " << clientID
       << " objectID: " << objectID 
       << " methodID: " << methodID << std::endl;
  archive.print_vector();
  #endif

  if (archive.empty())
  {
    #ifdef DEBUG_RMI
    std::cout << "function call with empty archive" << std::endl;
    #endif
  }

  this->request_for_method(objectID, methodID);
  if(!archive.empty())
  {
    this->wait_for_grant(objectID, methodID);
    #ifdef DEBUG_RMI
      std::cout << "client " << clientID
                << "got grant, will submit parameters" << std::endl;
    #endif
    this->submit_parameters(objectID, archive);
  }
  #ifdef DEBUG_RMI
    std::cout << "client " << clientID
              << "is waiting for method completion" << std::endl;
  #endif
  //general case, we need to poll if the call is completed yet
  //see comment on partial specialisation for point to point channel below
  //this->wait_for_completion(objectID);
  assert(size>0);

  // receive return parameter
  OSSS_Archive_t return_archive;
  this->receive_return_parameter(objectID, return_archive, size);

  // announces the end of return parameters reception
  // HK: Do we really need this? I don't think so, hence deactivated
  //this->end_of_transfer(objectID, methodID, true);

  return return_archive;
}

//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
client_transactor::
call_procedure(OSSS_ClientID_t clientID,
	       OSSS_ObjectID_t objectID,
	       OSSS_MethodID_t methodID,
	       OSSS_Archive_t archive)
{
  #ifdef DEBUG_RMI
  std::cout << "at: " << sc_core::sc_time_stamp() << " " 
       << __PRETTY_FUNCTION__
       << " of client: " << _CLIENT_ID_ 
       << " called" << std::endl;
	
  std::cout << "clientID: " << clientID 
       << " objectID: " << objectID 
       << " methodID: " << methodID << std::endl;
  archive.print_vector();
  #endif

  if (archive.empty())
  {
    #ifdef DEBUG_RMI
    std::cout << "procedure call with empty archive" << std::endl;
    #endif
  }
  
  this->request_for_method(objectID, methodID);
  //HK: if we have no Parameters, we do not need to wait for the grant,
  //instead, we just wait for the Method to complete.
  if(!archive.empty())
  {
    #ifdef DEBUG_RMI
      std::cout << "client " << clientID
                << " is waiting for grant..." << std::endl;
    #endif
    this->wait_for_grant(objectID, methodID);
    #ifdef DEBUG_RMI
      std::cout << "client " << clientID
                << " got grant, will submit parameters" << std::endl;
    #endif
    this->submit_parameters(objectID, archive);
  }
  #ifdef DEBUG_RMI
    std::cout << "client " << clientID
              << " is waiting for method completion" << std::endl;
  #endif
  this->wait_for_completion(objectID);

  // receive return parameter
  // HK: This is a procedure call, so there are no return values
  //     to read, so for now I just skip this step.
  //OSSS_Archive_t return_archive;
  //this->receive_return_parameter(objectID, return_archive, 1);

  // announces the end of a wait for completion of the method call
  // HK; Not needed (see above)
  //this->end_of_transfer(objectID, methodID, true);
}

//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
client_transactor::
request_for_method(OSSS_ObjectID_t& objectID,
		   OSSS_MethodID_t& methodID)
{
  this->start_parameter_transfer(objectID, methodID);
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
client_transactor::
wait_for_grant(OSSS_ObjectID_t& objectID,
	       OSSS_MethodID_t& methodID)
{
  typename ChannelClass::address_type addr;
  typename ChannelClass::return_params_data_chunk data;

  ChannelClass::master::read_blocking(addr, data, 0);
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
client_transactor::
start_parameter_transfer(OSSS_ObjectID_t& objectID,
			 OSSS_MethodID_t& methodID)
{
  typename ChannelClass::address_type addr;
  addr.client_id = _CLIENT_ID_;
  addr.object_id = objectID;
  addr.method_id = methodID;

  typename ChannelClass::in_params_data_chunk data;
  data.push_back(methodID);
  #ifdef DEBUG_RMI
    std::cout << "Sending MID" << std::endl;
  #endif
  ChannelClass::master::write_blocking(addr, data);
}
  
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
client_transactor::
end_of_transfer(OSSS_ObjectID_t& objectID,
		OSSS_MethodID_t& methodID,
		bool last_transfer)
{
  std::cerr << "end_of_transfer method"
            << " should never be called!" << std::endl;
}
 
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >:: 
client_transactor::
idle()
{
  sc_core::wait();
}
  
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
client_transactor::
submit_parameters(OSSS_ObjectID_t& objectID,
		  OSSS_Archive_t& archive)
{
  typename ChannelClass::address_type addr;
  ChannelClass::master::write_blocking(addr, archive);
}
 
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >:: 
client_transactor::
wait_for_completion(OSSS_ObjectID_t& objectID)
{
  typename ChannelClass::address_type addr;
  typename ChannelClass::return_params_data_chunk data;
  ChannelClass::master::read_blocking(addr, data, 0);
}
  
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
client_transactor::
receive_return_parameter(OSSS_ObjectID_t& objectID,
			 OSSS_Archive_t& return_archive,
                         unsigned int size)
{
  typename ChannelClass::address_type addr;
  ChannelClass::master::read_blocking(addr, return_archive, size);
}

//==============================================================================
// server_transactor
//==============================================================================
    
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
server_transactor::
reset()
{
  ChannelClass::slave::reset();
}


//HK: In this special case, we do not use the wait_for_action method of the 
//channel. There are only one client and one object connected to this RMI
//channel so we do not need to extract the corresponding IDs from the 
//address.
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
server_transactor::
listen_for_action(OSSS_ClientID_t &clientID,
		  OSSS_ObjectID_t &objectID,
		  OSSS_MethodID_t &methodID,
		  osss_rmi::Command_ID_t &command_id)
{
  //typename ChannelClass::address_type addr;
  //typename ChannelClass::action_type  action;
  
  //ChannelClass::slave::wait_for_action(addr, action);
  //HK: No need to get this from the Address
  #ifdef DEBUG_RMI
        std::cout  << " lfa_entry @ " << sc_core::sc_time_stamp()
                  << m_rmi_channel_ptr->print_client_info() << std::endl;
  #endif
  clientID =  m_rmi_channel_ptr->get_client_id(objectID, 0);
  #ifdef DEBUG_RMI
        std::cout << clientID << " --> listening for action: " 
                  << sc_core::sc_time_stamp() << std::endl;
      #endif
  //objectID =  m_rmi_channel_ptr->m_server_info.object_id;
  
  /*
    start_id                    request_for_method
    receive_id                  wait_for_grant 
    submit_id (methodID == 0)   submit_parameters
    receive_id (methodID == 0)  wait_for_completion <-- not used here !!!
    receive_id (methodID == 0)  receive_return_parameter
    end_id                      end_of_transfer
  */ 

  if ((clientID == 0) or (objectID == 0))
  {
    command_id = osss_rmi::error_id;
    std::cerr << "Error in: ((addr.client_id == 0) or (addr.object_id == 0)) " 
	      << std::endl;
  }
  else
  {
    //previous call was serviced, waiting for a new one
    if (state == osss_rmi::end_id)
    {
      //HK: What if Method ID needs more than one chunk?
      typename ChannelClass::in_params_data_chunk tmp_data;
      typename ChannelClass::address_type tmp_addr;
      //this will block until we receive a MethodID
      ChannelClass::slave::read_blocking(tmp_addr, tmp_data);
      #ifdef DEBUG_RMI
        std::cout << clientID << " --> start of new transaction: " 
                  << sc_core::sc_time_stamp() << std::endl;
      #endif
      if (tmp_data.size() != 1)
      {
        std::cerr << "Error [start_id] size != 1" << std::endl;
        methodID = 0;
        state = osss_rmi::error_id;
      }
      else
      {
        methodID = tmp_data[0].to_uint();
        #ifdef DEBUG_RMI
          std::cout << clientID << " --> received valid MID: " 
                    << methodID << std::endl;
        #endif
      }

      state = osss_rmi::start_id;
    }
    else if (state == osss_rmi::start_id)
    {
      state = osss_rmi::receive_id;
      #ifdef DEBUG_RMI
        std::cout << clientID << " --> " << state << " (wait_for_grant): "
                  << sc_core::sc_time_stamp() << std::endl;
      #endif
    }
    command_id = state;
  }
}

//------------------------------------------------------------------------------
  
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
server_transactor::
return_grant(OSSS_ClientID_t clientID, bool granted, bool hasParameters)
{
  //grant needs to be transmitted if method has Parameters, otherwise
  //we don't need to do anything
  if (hasParameters)
  {
    if(granted)
    {
      return_grant_written = true;
      typename ChannelClass::address_type addr;
      typename ChannelClass::return_params_data_chunk data;
      #ifdef DEBUG_RMI
        std::cout << "Transmitting grant to client " << clientID << ": " 
                  << sc_core::sc_time_stamp() << std::endl;
      #endif
      data.push_back(1);
      ChannelClass::slave::write_blocking(addr, data);
      //next thing we will do is wait for parameters
      state = osss_rmi::submit_id;
    }
    else
      sc_core::wait();
  }
  else
  {
    if(granted)
    {
      return_grant_written = true;
      //do not wait here, we need to save the cycle!
    }
    else
    {
      sc_core::wait();
    }
  }
}

//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
server_transactor::
receive_in_params(OSSS_Archive_t &arch)
{
  typename ChannelClass::address_type addr;
  
#ifdef DEBUG_RMI
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
  
  bool ret_val = ChannelClass::slave::read_blocking(addr, arch);
  
  #ifdef DEBUG_RMI
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::cout << arch << std::endl;
  #endif
  state = osss_rmi::receive_id;
  return ret_val;
}

//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
server_transactor::
provide_return_params(OSSS_ClientID_t clientID,
		      OSSS_Archive_t &arch)
{
  return_params_written = true;
  typename ChannelClass::address_type addr;
  state = osss_rmi::end_id;
  return ChannelClass::slave::write_blocking(addr, arch);
}

//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_channel<osss_rmi_point_to_point_channel<inDataWidth, 
						 returnDataWidth> >::
server_transactor::
return_params_idle(OSSS_ClientID_t clientID,
		   bool is_busy,
		   bool is_void)
{
  if(is_void && !is_busy)
  {
    //in case return value is void, we have to initiate a transfer to
    //notify the client
    typename ChannelClass::address_type addr;
    typename ChannelClass::return_params_data_chunk data;
    #ifdef DEBUG_RMI
      std::cout << "Notify client, method Execution complete " << clientID
                << ": " << sc_core::sc_time_stamp() << std::endl;
    #endif
    ChannelClass::slave::write_blocking(addr, data);
    state = osss_rmi::end_id;
  }
  else
  {
    //if method has return values, we do not need to notify the client
    //explicitly
    sc_core::wait();
  }
}
// $Id: osss_rmi_channel.tpp 2992 2009-01-28 15:50:28Z henning $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
