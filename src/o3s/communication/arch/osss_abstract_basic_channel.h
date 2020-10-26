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

#ifndef OSSS_ABSTRACT_BASIC_CHANNEL_HH_INCLUDED
#define OSSS_ABSTRACT_BASIC_CHANNEL_HH_INCLUDED

#include <vector>
#include <string>

namespace osss {
class osss_no_address_type {};

template<class AddressType = osss_no_address_type,
         unsigned int writeDataWidth = 8, 
	 unsigned int readDataWidth = 8, 
	 bool biDirectional = false,
	 bool inOut = false>
class osss_abstract_basic_channel { };

//------------------------------------------------------------------------------

template<unsigned int writeDataWidth>
class osss_abstract_basic_channel<osss_no_address_type,
                                  writeDataWidth, 
				  writeDataWidth, 
				  false, 
				  false> :
  public osss_basic_channel
{
public:
  typedef std::vector<sc_dt::sc_biguint<writeDataWidth> > data_chunk;
  static std::string dump_data_chunk(const data_chunk& data);

  explicit osss_abstract_basic_channel(std::string name) : 
    osss_basic_channel( name )
  {}

  virtual ~osss_abstract_basic_channel()
  {}
  
  class abstract_write_if : public virtual osss_signal_if
  {
  public:
    virtual 
    bool 
    write_blocking(data_chunk& data) = 0;

    virtual 
    bool 
    write_blocking(osss_serialisable_object& ser_obj) = 0;
  };

  class abstract_read_if : public virtual osss_signal_if
  {
  public:
    virtual 
    bool 
    read_blocking(data_chunk& data) = 0;

    virtual 
    bool 
    read_blocking(osss_serialisable_object& ser_obj) = 0;
  };
  
};

//------------------------------------------------------------------------------

template<unsigned int writeDataWidth>
class osss_abstract_basic_channel<osss_no_address_type,
                                  writeDataWidth, 
				  writeDataWidth, 
				  false, 
				  true> :
    public osss_abstract_basic_channel<osss_no_address_type,
				       writeDataWidth, 
				       writeDataWidth, 
				       false, 
				       false>
{
 public:
  typedef osss_abstract_basic_channel<osss_no_address_type,
                                      writeDataWidth, 
                                      writeDataWidth, 
                                      false, 
                                      false> base_type;

  typedef std::vector<sc_dt::sc_biguint<writeDataWidth> > data_chunk;
  static std::string dump_data_chunk(const data_chunk& data);

  explicit osss_abstract_basic_channel(std::string name) : 
    base_type( name )
  {}

  virtual ~osss_abstract_basic_channel()
  {}

};

//------------------------------------------------------------------------------

template<class AddressType,
	 unsigned int writeDataWidth,
	 unsigned int readDataWidth>
class osss_abstract_basic_channel<AddressType,
                                  writeDataWidth, 
				  readDataWidth, 
				  true,
				  false> :
  public osss_basic_channel
{
public:

  typedef enum {NO_ACTION, READ_ACTION, WRITE_ACTION} action_type;

  typedef AddressType address_type;

  template<class data_chunk_type> 
  static
  std::string dump_data_chunk(const data_chunk_type& data);

  explicit osss_abstract_basic_channel(std::string name) : 
    osss_basic_channel( name )
  {}

  virtual ~osss_abstract_basic_channel()
  {}

  class abstract_write_if : public virtual osss_signal_if
  {
  public:
    typedef sc_dt::sc_biguint<writeDataWidth> WritePort_t;
    typedef sc_dt::sc_biguint<readDataWidth>  ReadPort_t;

    typedef std::vector< WritePort_t > write_data_chunk;
    typedef std::vector< ReadPort_t >  read_data_chunk;

    virtual
    void
    wait_for_action(address_type& address, 
		    action_type& action) = 0;

    /** 
     * Reads a data chunk from the channel. 
     * 
     * @param address The address, where the chunk should be read from.
     * @param data The chunk, that had been read.
     * @param size The size of the chunk to be read in bytes.
     * 
     * @return \c true if the reading was successfull. Else \c false is returned.
     */
    virtual 
    bool
    read_blocking(address_type& address,
                  read_data_chunk& data,
                  unsigned int size) = 0;

    /** 
     * Reads a serialisable object from the channel. 
     * 
     * @param address The address, where the object should be read from.
     * @param data The object, that had been read.
     * @param size The size of the object to be read in bytes.
     * 
     * @return \c if the reading as successful. Else \c false is returned.
     */
    virtual 
    bool
    read_blocking(address_type& address,
                  osss_serialisable_object& ser_obj,
                  unsigned int size) = 0;

    /** 
     * Reads a serialisable archive from the channel. 
     * 
     * @param address The address, where the archive should be read from.
     * @param data The archive, that had been read.
     * @param size The size of the archive to be read in bytes.
     * 
     * @return \c if the reading as successful. Else \c false is returned.
     */
    virtual 
    bool
    read_blocking(address_type& address,
                  osss_serialisable_archive& ser_arch,
                  unsigned int size) = 0;
    
    virtual 
    bool 
    write_blocking(address_type& address, 
		   const write_data_chunk& data) = 0;

    virtual
    bool
    write_blocking(address_type& address,
		   osss_serialisable_object& ser_obj) = 0;

    virtual
    bool
    write_blocking(address_type& address,
		   osss_serialisable_archive& ser_arch) = 0;

  };

  class abstract_read_if : public virtual osss_signal_if
  {
  public:
    typedef sc_dt::sc_biguint<readDataWidth>  WritePort_t;
    typedef sc_dt::sc_biguint<writeDataWidth> ReadPort_t;

    typedef std::vector< WritePort_t > write_data_chunk;
    typedef std::vector< ReadPort_t >  read_data_chunk;

    virtual
    void
    wait_for_action(address_type& address, 
		    action_type& action) = 0;

    virtual 
    bool 
    read_blocking(address_type& address,
		  read_data_chunk& data) = 0;

    virtual
    bool
    read_blocking(address_type& address,
		  osss_serialisable_object& ser_obj) = 0;

    virtual
    bool
    read_blocking(address_type& address,
		  osss_serialisable_archive& ser_arch) = 0;

    virtual 
    bool 
    write_blocking(address_type& address, 
		   const write_data_chunk& data) = 0;

    virtual
    bool
    write_blocking(address_type& address,
		   osss_serialisable_object& ser_obj) = 0;

    virtual
    bool
    write_blocking(address_type& address,
		   osss_serialisable_archive& ser_arch) = 0;

  };
   
};

//------------------------------------------------------------------------------

template<unsigned int writeDataWidth>
class osss_abstract_basic_channel<osss_no_address_type,
				  writeDataWidth, 
				  writeDataWidth, 
				  true,
				  true> :
  public osss_basic_channel
{
public:
  typedef std::vector<sc_dt::sc_biguint<writeDataWidth> > data_chunk;
  static std::string dump_data_chunk(const data_chunk& data);

  explicit osss_abstract_basic_channel(std::string name) : 
    osss_basic_channel( name )
  {}

  virtual ~osss_abstract_basic_channel()
  {}
  
  class abstract_read_write_if : public virtual osss_signal_if
  {
  public:
    virtual 
    bool 
    write_blocking(data_chunk& data) = 0;

    virtual 
    bool 
    write_blocking(osss_serialisable_object& ser_obj) = 0;

    virtual 
    bool 
    read_blocking(data_chunk& data) = 0;

    virtual 
    bool 
    read_blocking(osss_serialisable_object& ser_obj) = 0;
  };
  
};

#include "o3s/communication/arch/osss_abstract_basic_channel.tpp"

} // namespace osss

#endif // OSSS_ABSTRACT_BASIC_CHANNEL_HH_INCLUDED
// $Id: osss_abstract_basic_channel.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
