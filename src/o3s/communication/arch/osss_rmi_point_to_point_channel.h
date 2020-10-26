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

#ifndef OSSS_RMI_POINT_TO_POINT_CHANNEL_HH
#define OSSS_RMI_POINT_TO_POINT_CHANNEL_HH

#include "o3s/communication/arch/osss_abstract_basic_channel.h"
#include "o3s/structural/arch/interconnection/osss_p2p.h"

namespace osss {


struct osss_rmi_point_to_point_channel_address
{
  OSSS_ClientID_t client_id;
  OSSS_ObjectID_t object_id;
  OSSS_MethodID_t method_id;
};


template< unsigned int inDataWidth = 8
        , unsigned int returnDataWidth = 8
        >
class osss_rmi_point_to_point_channel
  : public osss_p2p
  , public osss_abstract_basic_channel
           < osss_rmi_point_to_point_channel_address
           , inDataWidth
           , returnDataWidth
           , true
           , false
           >
{
public:

  typedef osss_abstract_basic_channel
          < osss_rmi_point_to_point_channel_address
          , inDataWidth
          , returnDataWidth
          , true
          , false
          > base_type;

  typedef 
  typename base_type::abstract_write_if::WritePort_t InParamsPort_t;
  
  typedef 
  typename base_type::abstract_write_if::ReadPort_t  ReturnParamsPort_t;

  typedef 
  typename base_type::address_type address_type;

  typedef typename base_type::action_type action_type;
  
  typedef 
  typename 
  base_type::abstract_write_if::write_data_chunk in_params_data_chunk;
  
  typedef
  typename 
  base_type::abstract_write_if::read_data_chunk return_params_data_chunk;

  explicit
  osss_rmi_point_to_point_channel( sc_core::sc_module_name name )
    : osss_p2p( name, inDataWidth, returnDataWidth )
    , base_type( osss_p2p::name() )
  {}

protected:

  virtual void before_end_of_elaboration()
  {
    base_type::check_clock_reset_port();
  }

public:
 
  //------------------------------------------------------------------
  // client signal interface
  //------------------------------------------------------------------
  class client_signal_if
    : public base_type::abstract_write_if
  {
  public:

    sc_core::sc_out< InParamsPort_t > IN_PARAMS;
    sc_core::sc_out< bool > CL_STROBE;
  
    sc_core::sc_in< ReturnParamsPort_t > RETURN_VAL;
    sc_core::sc_in< bool > SO_STROBE;

    client_signal_if() : 
      base_type::abstract_write_if(),
      IN_PARAMS("client_IN_PARAMS_port"),
      CL_STROBE("client_CL_STROBE_port"),
      RETURN_VAL("client_RETURN_VAL_port"),
      SO_STROBE("client_SO_STROBE_port")
    {}

    OSSS_GENERATE {
      //out connections
      this->osss_connect(osss_reg_port(IN_PARAMS), 
			 this->osss_shared_signal("IN_PARAMS"));
      this->osss_connect(osss_reg_port(CL_STROBE), 
			 this->osss_shared_signal("CL_STROBE"));

      //in connections
      this->osss_connect(this->osss_shared_signal("RETURN_VAL"), 
			 osss_reg_port(RETURN_VAL));
      this->osss_connect(this->osss_shared_signal("SO_STROBE"), 
			 osss_reg_port(SO_STROBE));
    }
  };
  
  //------------------------------------------------------------------
  // server signal interface
  //------------------------------------------------------------------
  class server_signal_if
    : public base_type::abstract_read_if 
  {
  public:

    sc_core::sc_in< InParamsPort_t > IN_PARAMS;
    sc_core::sc_in< bool > CL_STROBE;
  
    sc_core::sc_out< ReturnParamsPort_t > RETURN_VAL;
    sc_core::sc_out< bool > SO_STROBE;

    server_signal_if()
      : base_type::abstract_read_if()
      , IN_PARAMS("server_IN_PARAMS_port")
      , CL_STROBE("server_CL_STROBE_port")
      , RETURN_VAL("server_RETURN_VAL_port")
      , SO_STROBE("server_SO_STROBE_port")
    {}

    OSSS_GENERATE {
      //in connections
      this->osss_connect(this->osss_shared_signal("IN_PARAMS"), 
			 osss_reg_port(IN_PARAMS));
      this->osss_connect(this->osss_shared_signal("CL_STROBE"), 
			 osss_reg_port(CL_STROBE));
    
      //out connections
      this->osss_connect(osss_reg_port(RETURN_VAL), 
			 this->osss_shared_signal("RETURN_VAL"));
      this->osss_connect(osss_reg_port(SO_STROBE), 
			 this->osss_shared_signal("SO_STROBE"));
    }
  };

       
  class master
    : public sc_core::sc_module
    , public client_signal_if
    , public osss_method_if
  {
  public:

    typedef client_signal_if signal_if;

    explicit
    master(sc_core::sc_module_name name
             = sc_core::sc_gen_unique_name( "master") )
      : sc_core::sc_module(name)
      , client_signal_if()
    {}

    virtual void reset();

    virtual
    void
    wait_for_action(address_type& address, 
		    action_type& action);

    virtual 
    bool 
    write_blocking(address_type& slave_base_addr,
		   const in_params_data_chunk& data);

    virtual
    bool
    write_blocking(address_type& slave_base_addr,
		   osss_serialisable_object& ser_obj);

    virtual
    bool
    write_blocking(address_type& slave_base_addr,
		   osss_serialisable_archive& ser_arch);


    /**
     *  @param size is unused here
     *
     */
    virtual 
    bool 
    read_blocking(address_type& slave_base_addr,
		  return_params_data_chunk& data,
                  unsigned int size);

    /**
     *  @param size is unused here
     *
     */
    virtual
    bool
    read_blocking(address_type& slave_base_addr,
		  osss_serialisable_object& ser_obj,
                  unsigned int size);

    /**
     *  @param size is unused here
     *
     */
    virtual
    bool
    read_blocking(address_type& slave_base_addr,
		  osss_serialisable_archive& ser_arch,
                  unsigned int size);
  
  };

  class slave
    : public sc_core::sc_module
    , public server_signal_if
    , public osss_method_if
  {
  public:

    typedef server_signal_if signal_if;

    explicit
    slave(sc_core::sc_module_name name
            = sc_core::sc_gen_unique_name( "slave" ))
      : sc_core::sc_module(name)
      , server_signal_if()
    {}

    virtual void reset();

    virtual
    void
    wait_for_action(address_type& base_address,
		    action_type& action);

    virtual 
    bool 
    read_blocking(address_type& address,
		  in_params_data_chunk& data);

    virtual
    bool
    read_blocking(address_type& address,
		  osss_serialisable_object& ser_obj);

    virtual
    bool
    read_blocking(address_type& address,
		  osss_serialisable_archive& ser_arch);

    virtual 
    bool 
    write_blocking(address_type& address,
		   const return_params_data_chunk& data);

    virtual
    bool
    write_blocking(address_type& address,
		   osss_serialisable_object& ser_obj);

    virtual
    bool
    write_blocking(address_type& address,
		   osss_serialisable_archive& ser_arch);
   
  };

private:

  template<class data_chunk_type>
  static
  data_chunk_type
  serialisable_object_to_data_chunk(osss_serialisable_object& ser_obj);

  template<class data_chunk_type>
  static
  data_chunk_type
  serialisable_archive_to_data_chunk(osss_serialisable_archive& ser_arch);

  template<class data_chunk_type>
  static
  void
  data_chunk_to_serialisable_object(data_chunk_type data,
				    osss_serialisable_object& ser_obj);

  template<class data_chunk_type>
  static
  void
  data_chunk_to_serialisable_archive(data_chunk_type data,
				     osss_serialisable_archive& ser_arch);
};

#include "o3s/communication/arch/osss_rmi_point_to_point_channel.tpp"

} // namespace osss

#endif
// $Id: osss_rmi_point_to_point_channel.h 2991 2009-01-23 14:49:29Z henning $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
