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

#ifndef OSSS_RMI_CHANNEL_HH_INCLUDED
#define OSSS_RMI_CHANNEL_HH_INCLUDED

#include "o3s/communication/arch/osss_rmi/osss_transactor_accessor.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_master_if.h"
#include "o3s/communication/arch/osss_rmi_point_to_point_channel.h"

// Experimental feature
#if defined (USE_GREENBUS)
#  include "o3s/communication/arch/greenBus/green_bus.h"
#endif

namespace osss {

//--------------------------------------------------------------------
// OSSS-RMI-Channel
//--------------------------------------------------------------------

template<class ChannelClass>
class osss_rmi_channel : 
  public ChannelClass,
  public osss_rmi_channel_registry_if,
  public osss_rmi_channel_information_if
{
public:

  typedef ChannelClass ChannelClass_t;

  osss_rmi_channel()
    : ChannelClass(),
      m_list_of_servers(),
      m_list_of_clients(),
      m_address_map(),
      m_client_port_to_offset_map(),
      m_offset_to_client_port_map(),
      m_client_id_to_port_addr_map(),
      m_port_addr_to_client_id_map(),
      m_transactor_accessor_list()
  {
    /*
    std::cout << "Warning: You are using the default constructor of "
              << "osss_rmi_channel<...>." << std::endl
              << "For synthesis it is required that the name of this RMI Channel "
              << "matches its identifier. " << std::endl
              << "Please use osss_rmi_channel( const char* ) constructor for "
              << "osss_rmi_channel " << ChannelClass::name() << " instead."<< std::endl
              << std::endl;
    */
  }

  explicit osss_rmi_channel(sc_core::sc_module_name module_name) 
    : ChannelClass(module_name),
      m_list_of_servers(),
      m_list_of_clients(),
      m_address_map(),
      m_client_port_to_offset_map(),
      m_offset_to_client_port_map(),
      m_client_id_to_port_addr_map(),
      m_port_addr_to_client_id_map(),
      m_transactor_accessor_list()
  {}

  //----------------------------------------------------------------------------
  // [begin] address map related methods

  struct address_map_item
  {
    OSSS_ObjectID_t object_id;
    osss_address_range<typename ChannelClass::address_type> address_range;
  };

  //this method is called during osss_object_socket_port to RMI-Channel binding
  virtual void add_server_info(osss_rmi::server_info& server_info);
  //this method is called during osss_port_rmi to RMI-Channel binding
  virtual void add_client_info(osss_rmi::client_info& client_info);
  //this method is called during simulation when the operator->() of an 
  //osss_rmi_port is called for the first time
  virtual void update_client_info(void* port_addr, 
				  const OSSS_ClientID_t& client_id);

  osss_address_range<typename ChannelClass::address_type> 
    get_address_range(OSSS_ObjectID_t obj_id);
  typename ChannelClass::address_type get_base_address(OSSS_ObjectID_t& obj_id);
  typename ChannelClass::address_type get_high_address(OSSS_ObjectID_t& obj_id);

  OSSS_ObjectID_t get_object_id(
    typename ChannelClass::address_type& base_addr);
  OSSS_ObjectID_t get_object_id(
    osss_address_range<typename ChannelClass::address_type>& addr_range);

  unsigned int get_address_offset(const OSSS_ClientID_t& client_id);
  OSSS_ClientID_t get_client_id(const OSSS_ObjectID_t& obj_id,
				const unsigned int& addr_offset);
  
  virtual void complete();

 private:

  void build_address_map();
  void assign_address_ranges();
  void build_offset_maps();

 public:

  //---------------------------------------------------------
  // methods from osss_rmi_channel_information_if
  //---------------------------------------------------------
  virtual unsigned long get_base_address();
  virtual unsigned long get_high_address();

  virtual osss_address_range<unsigned long> 
  get_address_range_as_long(osss_object_socket_if* obj_socket_ptr);

  virtual std::string print_server_info();
  virtual std::string print_client_info();
  virtual std::string print_address_map();
  virtual std::string print_client_port_to_offset_map();

  // [end] address map related methods
  //----------------------------------------------------------------------------

  void register_transactor_accessor(transactor_accessor_base* tab)
  {
    m_transactor_accessor_list.push_back(tab);
  }

  //------------------------------------------------------------------
  // client transactor
  //------------------------------------------------------------------

  typedef osss_rmi_channel_client_if client_if;

  class client_transactor : public virtual client_if,
			    public virtual osss_rmi_master_if,
                            public virtual ChannelClass::master
  {
  public:

    client_transactor() : 
      ChannelClass::master(sc_core::sc_gen_unique_name("client_transactor")),
      m_rmi_channel_ptr( NULL )
    {}

    explicit client_transactor(osss_rmi_channel<ChannelClass>* rmi_channel_ptr) : 
      ChannelClass::master(sc_core::sc_gen_unique_name("client_transactor")),
      m_rmi_channel_ptr(rmi_channel_ptr)
    {}

    virtual void reset();

    /** 
     * Calls a remote function and receives the return parameter.
     *
     * The parameter \a size is required in order to determin how many bytes
     * must be read from the channel in order to receive the whole return
     * parameter.
     * 
     * @param clientID
     * @param objectID
     * @param methodID 
     * @param archive All parameters packed as an archive.
     * @param size The size of the paramter to return.
     * @return The return parameter packet as an archive
     */
    virtual 
    OSSS_Archive_t 
    call_function(OSSS_ClientID_t clientID,
                  OSSS_ObjectID_t objectID,
                  OSSS_MethodID_t methodID,
                  OSSS_Archive_t archive,
                  unsigned int size);
  
    /** 
     * Calls a remote procedure (has no return type in contrast to 
     *                           @see call_function).
     * 
     * @param clientID
     * @param objectID
     * @param methodID 
     * @param archive All parameters packed as an archive.
     */
    virtual
    void 
    call_procedure(OSSS_ClientID_t clientID,
		   OSSS_ObjectID_t objectID,
		   OSSS_MethodID_t methodID,
		   OSSS_Archive_t archive);

  protected:
    
    //----------------------------------------------------------------
    // helper methods from osss_rmi_master_if
    //----------------------------------------------------------------
    
    virtual
    void
    request_for_method(OSSS_ObjectID_t& objectID,
		       OSSS_MethodID_t& methodID);

    virtual
    void
    wait_for_grant(OSSS_ObjectID_t& objectID,
		   OSSS_MethodID_t& methodID);
    

    virtual 
    void 
    start_parameter_transfer(OSSS_ObjectID_t& objectID,
			     OSSS_MethodID_t& methodID);
  
    virtual 
    void 
    end_of_transfer(OSSS_ObjectID_t& objectID,
		    OSSS_MethodID_t& methodID,
		    bool last_transfer);
  
    virtual 
    void 
    idle();
  
    virtual 
    void 
    submit_parameters(OSSS_ObjectID_t& objectID,
		      OSSS_Archive_t& archive);
  
    virtual 
    void 
    wait_for_completion(OSSS_ObjectID_t& objectID);
  
    virtual 
    void 
    receive_return_parameter(OSSS_ObjectID_t& objectID,
			     OSSS_Archive_t& return_archive,
                             unsigned int size);
 
    osss_rmi_channel<ChannelClass>* m_rmi_channel_ptr;

  };

  typedef osss_rmi_channel_server_if server_if;
    
  class server_transactor : public virtual server_if,
			    public virtual ChannelClass::slave
  {
  public:

    server_transactor() : 
      ChannelClass::slave(sc_core::sc_gen_unique_name("server_transactor")),
      m_rmi_channel_ptr( NULL )
    {}

    explicit server_transactor(osss_rmi_channel<ChannelClass>* rmi_channel_ptr) : 
      ChannelClass::slave(sc_core::sc_gen_unique_name("server_transactor")),
      m_rmi_channel_ptr( rmi_channel_ptr )
    {}

    virtual void reset();

    virtual
    void 
    listen_for_action(OSSS_ClientID_t &clientID,
		      OSSS_ObjectID_t &objectID,
		      OSSS_MethodID_t &methodID,
		      osss_rmi::Command_ID_t &command_id);

    virtual
    void
    return_grant(OSSS_ClientID_t clientID, bool granted, bool hasParameters);

    virtual
    bool 
    receive_in_params(OSSS_Archive_t &arch);

    virtual
    void 
    return_params_idle(OSSS_ClientID_t clientID,
		       bool is_busy,
                       bool is_void);

    virtual
    bool 
    provide_return_params(OSSS_ClientID_t clientID,
			  OSSS_Archive_t &arch);

  protected:
    void
    resolve_address(typename ChannelClass::address_type raw_address,
		    OSSS_ObjectID_t objectID,
		    OSSS_ClientID_t &clientID,
		    osss_rmi::Command_ID_t &cmd_id);

    osss_rmi_channel<ChannelClass>* m_rmi_channel_ptr;

  };

 protected:

  void end_of_elaboration()
  {
    osss_basic_channel::register_trace();
    osss_basic_channel::register_trace_all();
    osss_basic_channel::print_channel_graph_on();
  }

  void before_end_of_elaboration()
  {
    ChannelClass::before_end_of_elaboration();
    for(unsigned int i=0; i<m_transactor_accessor_list.size(); i++)
    {
      client_transactor_accessor* cta = 
        dynamic_cast<client_transactor_accessor*>(
          m_transactor_accessor_list[i]);
      // client_transactor_accessor found
      if(cta != NULL)
      {
        client_transactor* t = new client_transactor(this);
        cta->set_transactor(t);
        cta->set_channel(this);
        // regsiter transactor at osss_basic_channel
        ChannelClass::m_transactorList.push_back(t);
        // build connection to interconnection network
        t->buildConnectionList();
        t->connect(*this);
        
        //check for requested traces
        if(cta->simple_trace_requested())
        {
          t->trace_simple(cta->get_simple_trace_name());
        }
        if(cta->trace_requested())
        {
          t->trace_in_file(cta->get_trace_file(), cta->get_trace_name());
        }
        
        // set priority
        if (osss_master_if* master_if = dynamic_cast< osss_master_if *>(t))
        {
          master_if->setPriority(cta->get_priority());
        }
      }
      else
      {
        server_transactor_accessor* sta = 
          dynamic_cast<server_transactor_accessor*>(
            m_transactor_accessor_list[i]);
        // server_transactor_accessor found
        if(sta != NULL)
        {
          server_transactor* t = new server_transactor(this);
          sta->set_transactor(t);
          sta->set_channel(this);
          // regsiter transactor at osss_basic_channel
          ChannelClass::m_transactorList.push_back(t);
          // build connection to interconnection network
          t->buildConnectionList();
          t->connect(*this);
        
          //check for requested traces
          if(sta->simple_trace_requested())
          {
            t->trace_simple(sta->get_simple_trace_name());
          }
          if(sta->trace_requested())
          {
            t->trace_in_file(sta->get_trace_file(), sta->get_trace_name());
          }
        
          // set address range
          // this is only useful when the designer sets the address range
          // explicitly through the port that is bound to the OSSS-RMI Channel.
          // Since the address map is calculated completely automatically this
          // feature is not used here.
          /*
          if (osss_slave_if* slave_if = dynamic_cast< osss_slave_if *>(t))
          {
            slave_if->setAddressRange<typename ChannelClass::address_type>(
              sta->get_base_address(), sta->get_high_address());
          }
          */
        }
        else
        {
          std::cerr << "Error in: " << __PRETTY_FUNCTION__
                    << " found illegal transactor accessor type!" << std::endl;
          OSSS_ABORT();
        }
      }
    }
    this->complete();
  }

  std::vector<osss_rmi::server_info> m_list_of_servers;
  std::vector<osss_rmi::client_info> m_list_of_clients;
  std::vector<address_map_item> m_address_map;

  std::map<const void*, std::pair<address_map_item*, unsigned int> > 
    m_client_port_to_offset_map;

  std::map<const std::pair<address_map_item*, unsigned int>, void* >
    m_offset_to_client_port_map;

  std::map<const OSSS_ClientID_t, void*> m_client_id_to_port_addr_map;
  std::map<const void*, OSSS_ClientID_t> m_port_addr_to_client_id_map;

  std::vector<transactor_accessor_base*> m_transactor_accessor_list;
};

//=========================================================================
// partial specialisation for
//  osss_rmi_point_to_point_channel
//=========================================================================

template<unsigned int inDataWidth, unsigned int returnDataWidth>
class osss_rmi_channel< osss_rmi_point_to_point_channel<inDataWidth, 
							returnDataWidth> > : 
  public osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>,
  public osss_rmi_channel_registry_if,
  public osss_rmi_channel_information_if
{
public:

  typedef osss_rmi_point_to_point_channel<inDataWidth, 
					  returnDataWidth> ChannelClass;
  typedef ChannelClass ChannelClass_t;

  osss_rmi_channel() : 
    ChannelClass(),
    m_transactor_accessor_list()
  {}

  explicit osss_rmi_channel(sc_core::sc_module_name module_name) : 
    ChannelClass(module_name),
    m_transactor_accessor_list()
  {}

  virtual void add_server_info(osss_rmi::server_info& server_info);
  virtual void add_client_info(osss_rmi::client_info& client_info);
  virtual void update_client_info(void* port_addr, 
				  const OSSS_ClientID_t& client_id);
  OSSS_ClientID_t get_client_id(const OSSS_ObjectID_t& obj_id,
				const unsigned int& addr_offset);

  //---------------------------------------------------------
  // methods from osss_rmi_channel_information_if
  //---------------------------------------------------------
  virtual std::string print_server_info();
  virtual std::string print_client_info();

  virtual std::string print_address_map() 
    { return ""; /* Not implemented */ }
  virtual std::string print_client_port_to_offset_map() 
    { return ""; /* Not implemented */ } 

  virtual unsigned long get_base_address() { return 0; }
  virtual unsigned long get_high_address() { return 0; } 

  virtual osss_address_range<unsigned long> 
  get_address_range_as_long(osss_object_socket_if* obj_socket_ptr)
  { return osss_address_range<unsigned long>(0,0); }


  void register_transactor_accessor(transactor_accessor_base* tab)
  {
    m_transactor_accessor_list.push_back(tab);
  }



  typedef osss_rmi_channel_client_if client_if;

  class client_transactor : public virtual client_if,
			    public virtual osss_rmi_master_if,
                            public virtual ChannelClass::master
  {
  public:
    
    client_transactor() : 
      ChannelClass::master(sc_core::sc_gen_unique_name("client_transactor")),
      m_rmi_channel_ptr( NULL )
    {}

    explicit client_transactor(osss_rmi_channel<ChannelClass>* rmi_channel_ptr) : 
      ChannelClass::master(sc_core::sc_gen_unique_name("client_transactor")),
      m_rmi_channel_ptr( rmi_channel_ptr )
    {}

    virtual void reset();

    virtual
    OSSS_Archive_t
    call_function(OSSS_ClientID_t clientID,
                  OSSS_ObjectID_t objectID, 
                  OSSS_MethodID_t methodID,
                  OSSS_Archive_t archive,
                  unsigned int size);

    virtual
    void 
    call_procedure(OSSS_ClientID_t clientID,
		   OSSS_ObjectID_t objectID,
		   OSSS_MethodID_t methodID,
		   OSSS_Archive_t archive);

  protected:
    
    //----------------------------------------------------------------
    // helper methods from osss_rmi_master_if
    //----------------------------------------------------------------
    
    virtual
    void
    request_for_method(OSSS_ObjectID_t& objectID,
		       OSSS_MethodID_t& methodID);

    virtual
    void
    wait_for_grant(OSSS_ObjectID_t& objectID,
		   OSSS_MethodID_t& methodID);

    virtual 
    void 
    start_parameter_transfer(OSSS_ObjectID_t& objectID,
			     OSSS_MethodID_t& methodID);
  
    virtual 
    void 
    end_of_transfer(OSSS_ObjectID_t& objectID,
		    OSSS_MethodID_t& methodID,
		    bool last_transfer);
  
    virtual 
    void 
    idle();
  
    virtual 
    void 
    submit_parameters(OSSS_ObjectID_t& objectID,
		      OSSS_Archive_t& archive);
  
    virtual 
    void 
    wait_for_completion(OSSS_ObjectID_t& objectID);
  
    /** 
     * Receives the return parameter of a function call.
     *
     * The parameter \a size is required in order to determin who many bytes
     * must be read from the channel in order to receive the whole return
     * parameter.
     * 
     * @param objectID 
     * @param return_archive The returned parameter as an archive.
     * @param size The size of the paramter to return.
     */
    virtual void 
    receive_return_parameter(OSSS_ObjectID_t& objectID,
                             OSSS_Archive_t& return_archive,
                             unsigned int size);

    osss_rmi_channel<ChannelClass>* m_rmi_channel_ptr;

  };

  typedef osss_rmi_channel_server_if server_if;
    
  class server_transactor : public virtual server_if,
			    public virtual ChannelClass::slave
  {
  public:
    
    server_transactor() : 
      ChannelClass::slave(sc_core::sc_gen_unique_name("server_transactor")),
      m_rmi_channel_ptr( NULL ),
      state(osss_rmi::end_id),
      return_params_written(false),
      wait_for_grant(false),
      return_grant_written(false)
    {}

    explicit server_transactor(osss_rmi_channel<ChannelClass>* rmi_channel_ptr) :
      ChannelClass::slave(sc_core::sc_gen_unique_name("server_transactor")),
      m_rmi_channel_ptr( rmi_channel_ptr ),
      state(osss_rmi::end_id),
      return_params_written(false),
      wait_for_grant(false),
      return_grant_written(false)
    {}

    virtual void reset();

    virtual
    void 
    listen_for_action(OSSS_ClientID_t &clientID,
		      OSSS_ObjectID_t &objectID,
		      OSSS_MethodID_t &methodID,
		      osss_rmi::Command_ID_t &command_id);

    virtual
    void
    return_grant(OSSS_ClientID_t clientID, bool granted, bool hasParameters);

    virtual
    bool 
    receive_in_params(OSSS_Archive_t &arch);

    virtual
    void 
    return_params_idle(OSSS_ClientID_t clientID,
		       bool is_busy,
                       bool is_void);

    virtual
    bool 
    provide_return_params(OSSS_ClientID_t clientID,
			  OSSS_Archive_t &arch);

  protected:
    osss_rmi_channel<ChannelClass>* m_rmi_channel_ptr;

  private:
    osss_rmi::Command_ID_t state;
    bool return_params_written;
    bool wait_for_grant;
    bool return_grant_written;

  };

 protected:

  void end_of_elaboration()
  {
    osss_basic_channel::register_trace();
    osss_basic_channel::register_trace_all();
    osss_basic_channel::print_channel_graph_on();
  }

  void before_end_of_elaboration()
  {
    ChannelClass::before_end_of_elaboration();
    for(unsigned int i=0; i<m_transactor_accessor_list.size(); i++)
    {
      client_transactor_accessor* cta = 
        dynamic_cast<client_transactor_accessor*>(
          m_transactor_accessor_list[i]);
      // client_transactor_accessor found
      if(cta != NULL)
      {
        client_transactor* t = new client_transactor(this);
        cta->set_transactor(t);
        cta->set_channel(this);
        // regsiter transactor at osss_basic_channel
        ChannelClass::m_transactorList.push_back(t);
        // build connection to interconnection network
        t->buildConnectionList();
        t->connect(*this);
        
        //check for requested traces
        if(cta->simple_trace_requested())
        {
          t->trace_simple(cta->get_simple_trace_name());
        }
        if(cta->trace_requested())
        {
          t->trace_in_file(cta->get_trace_file(), cta->get_trace_name());
        }
        
        // set priority
        if (osss_master_if* master_if = dynamic_cast< osss_master_if *>(t))
        {
          master_if->setPriority(cta->get_priority());
        }
      }
      else
      {
        server_transactor_accessor* sta = 
          dynamic_cast<server_transactor_accessor*>(
            m_transactor_accessor_list[i]);
        // server_transactor_accessor found
        if(sta != NULL)
        {
          server_transactor* t = new server_transactor(this);
          sta->set_transactor(t);
          sta->set_channel(this);
          // regsiter transactor at osss_basic_channel
          ChannelClass::m_transactorList.push_back(t);
          // build connection to interconnection network
          t->buildConnectionList();
          t->connect(*this);
        
          //check for requested traces
          if(sta->simple_trace_requested())
          {
            t->trace_simple(sta->get_simple_trace_name());
          }
          if(sta->trace_requested())
          {
            t->trace_in_file(sta->get_trace_file(), sta->get_trace_name());
          }
        }
        else
        {
          std::cerr << "Error in: " << __PRETTY_FUNCTION__
                    << " found illegal transactor accessor type!" << std::endl;
          OSSS_ABORT();
        }
      }
    }
    this->complete();
  }

  osss_rmi::server_info m_server_info;
  osss_rmi::client_info m_client_info;

  std::vector<transactor_accessor_base*> m_transactor_accessor_list;
  
};

#include "o3s/communication/arch/osss_rmi/osss_rmi_channel.tpp"

} // namespace osss

#endif // OSSS_RMI_CHANNEL_HH_INCLUDED
// $Id: osss_rmi_channel.h 2991 2009-01-23 14:49:29Z henning $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
