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

//---------------------------------------------------------------------------
// unidirectional point-to-point channel
//---------------------------------------------------------------------------

template<unsigned int dataWidth >
class osss_simple_point_to_point_channel<dataWidth, false> : 
    public osss_abstract_basic_channel<osss_no_address_type,
				       dataWidth, 
				       dataWidth, 
				       false, 
				       false>
{ 
public:

  typedef osss_abstract_basic_channel<osss_no_address_type,
				      dataWidth, 
				      dataWidth, 
				      false, 
				      false> base_type;

  typedef typename base_type::data_chunk data_chunk;

  osss_simple_point_to_point_channel(std::string name) : 
    base_type( name )
  {}

  /**
   * transactor which declares two sc_out ports
   * and how these ports are connected to the shared signals inside the channel
   */
  class writer : public base_type::abstract_write_if,
                 public osss_method_if
  {
  public:
    sc_core::sc_out< sc_dt::sc_uint< dataWidth > > data_out;
    sc_core::sc_out<bool> valid_out;

    OSSS_GENERATE {
      this->osss_connect(osss_reg_port(data_out), 
			 this->osss_shared_signal("data_lines"));
      this->osss_connect(osss_reg_port(valid_out), 
			 this->osss_shared_signal("valid_line"));
    }
    
    virtual void reset();
   
    virtual bool write_blocking(data_chunk& data);
   
    virtual bool write_blocking(osss_serialisable_object& ser_obj);
  
  private:
    
    void start_transaction();

  };

  /**
   * transactor which declares two sc_in ports
   * and how these ports are connected to the shared signals inside the channel
   */
  class reader : public base_type::abstract_read_if,
                 public osss_method_if
  {
  public:
    sc_core::sc_in< sc_dt::sc_uint< dataWidth > > data_in; 
    sc_core::sc_in<bool> valid_in;

    OSSS_GENERATE {
      this->osss_connect(this->osss_shared_signal("data_lines"), 
			 osss_reg_port(data_in));
      this->osss_connect(this->osss_shared_signal("valid_line"), 
			 osss_reg_port(valid_in));
    }

    virtual void reset();

    virtual bool read_blocking(data_chunk& data);
   
    virtual bool read_blocking(osss_serialisable_object& ser_obj);
   
  private:
    
    void wait_for_transaction();

  };
      
};

//---------------------------------------------------------------------------
// bidirectional point-to-point channel
//---------------------------------------------------------------------------

template<unsigned int dataWidth >
class osss_simple_point_to_point_channel<dataWidth, true> :
  public osss_abstract_basic_channel<osss_no_address_type,
				     dataWidth, 
				     dataWidth, 
				     true, 
				     true>
{ 
public:

  typedef osss_abstract_basic_channel<osss_no_address_type,
				      dataWidth, 
				      dataWidth, 
				      true, 
				      true> base_type;
  typedef typename base_type::data_chunk data_chunk;

  osss_simple_point_to_point_channel(std::string name) : 
    base_type( name )
  {}
 
  class read_writer : public base_type::abstract_read_write_if,
                      public osss_method_if
  {
  public:
    sc_core::sc_inout< sc_dt::sc_uint< dataWidth > > data_inout;
    sc_core::sc_inout<bool> valid_inout;

    OSSS_GENERATE {
      this->osss_connect(osss_reg_port(data_inout), 
			 this->osss_shared_signal("data_lines"));
      this->osss_connect(osss_reg_port(valid_inout), 
			 this->osss_shared_signal("valid_line"));
    }
    
    virtual void reset();
   
    virtual bool write_blocking(data_chunk& data);
  
    virtual bool write_blocking(osss_serialisable_object& ser_obj);
  
    virtual bool read_blocking(data_chunk& data);
   
    virtual bool read_blocking(osss_serialisable_object& ser_obj);
  
  private:
    
    void start_transaction();
  
    void wait_for_transaction();
  
  };

};

#include "o3s/communication/arch/osss_simple_point_to_point_channel_impl.tpp"
// $Id: osss_simple_point_to_point_channel.tpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
