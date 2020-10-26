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

//----------------------------------------------------------------------
// master transactor
//----------------------------------------------------------------------
      
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::master::
reset()
{
  signal_if::IN_PARAMS.write(0);
  signal_if::CL_STROBE.write(false);
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::master::
wait_for_action(address_type& address, 
		action_type& action)
{
  std::cerr << "wait_for_action method of Point to point channel"
            << " should never be called!" << std::endl;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::master:: 
write_blocking(address_type& slave_base_addr,
	       const in_params_data_chunk& data)
{
  if (data.size() == 0)
  {
    signal_if::CL_STROBE.write(true);
    signal_if::IN_PARAMS.write(0);
    sc_core::wait();
    signal_if::CL_STROBE.write(false);
  }
  else
  {
    // write data
    signal_if::CL_STROBE.write(true);
    for(unsigned int i=0; i<data.size(); i++)
    {
      signal_if::IN_PARAMS.write(data[i]);
      sc_core::wait();
    }
    signal_if::CL_STROBE.write(false);
  }
  this->reset();
  sc_core::wait();
  return true;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::master:: 
write_blocking(address_type& slave_base_addr,
	       osss_serialisable_object& ser_obj)
{
  return this->write_blocking(slave_base_addr, 
    serialisable_object_to_data_chunk<in_params_data_chunk>(ser_obj));
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::master:: 
write_blocking(address_type& slave_base_addr,
	       osss_serialisable_archive& ser_arch)
{
  return this->write_blocking(slave_base_addr, 
    serialisable_archive_to_data_chunk<in_params_data_chunk>(ser_arch));
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::master:: 
read_blocking(address_type& slave_base_addr,
	      return_params_data_chunk& data,
              unsigned int size)
{
  while (signal_if::SO_STROBE.read() != true)
    sc_core::wait();
  for(unsigned int i=0; i < size; i++)
  {
    data.push_back(signal_if::RETURN_VAL.read());
    sc_core::wait();
  }
  return true;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::master:: 
read_blocking(address_type& slave_base_addr,
	      osss_serialisable_object& ser_obj,
              unsigned int size)
{
  return_params_data_chunk data;
  bool return_value = this->read_blocking(slave_base_addr, data, size);
  data_chunk_to_serialisable_object<return_params_data_chunk>(data, ser_obj);
  return return_value;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::master:: 
read_blocking(address_type& slave_base_addr,
	      osss_serialisable_archive& ser_arch,
              unsigned int size)
{
  return_params_data_chunk data;
  bool return_value = this->read_blocking(slave_base_addr, data, size);
  data_chunk_to_serialisable_archive<return_params_data_chunk>(data, ser_arch);
  return return_value;
}

//----------------------------------------------------------------------
// slave transactor
//----------------------------------------------------------------------
      
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void 
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::slave::
reset()
{
  signal_if::RETURN_VAL.write(0);
  signal_if::SO_STROBE.write(false);
}


//HK: this method should not be used in case of a P2P Channel
template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
void
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::slave::
wait_for_action(address_type& base_address,
		action_type& action)
{
  std::cerr << "wait_for_action method of Point to point channel"
            << " should never be called!" << std::endl;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool 
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::slave::
read_blocking(address_type& address,
	      in_params_data_chunk& data)
{
  data.clear();
  while(!signal_if::CL_STROBE.read())
  {
    sc_core::wait();
  }
  while(signal_if::CL_STROBE.read())
  {
    data.push_back(signal_if::IN_PARAMS.read());
    sc_core::wait();
  }
  return true;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::slave::
read_blocking(address_type& address,
	      osss_serialisable_object& ser_obj)
{
  in_params_data_chunk data;
  bool return_value = this->read_blocking(address, data);
  data_chunk_to_serialisable_object<in_params_data_chunk>(data, ser_obj);
  return return_value;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::slave::
read_blocking(address_type& address,
	      osss_serialisable_archive& ser_arch)
{
  in_params_data_chunk data;
  bool return_value = this->read_blocking(address, data);
  data_chunk_to_serialisable_archive<in_params_data_chunk>(data, ser_arch);
  return return_value;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool 
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::slave::
write_blocking(address_type& address,
	       const return_params_data_chunk& data)
{
  signal_if::SO_STROBE.write(true);
  if (data.size() == 0)
  {
    signal_if::RETURN_VAL.write(0);
    sc_core::wait();
  }
  else
  {
    for(unsigned int i=0; i<data.size(); i++)
    {
      signal_if::RETURN_VAL.write(data[i]);
      sc_core::wait();
    }
  }
  this->reset();
  //sc_core::wait();
  return true;
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::slave::
write_blocking(address_type& address, 
	       osss_serialisable_object& ser_obj)
{
  return this->write_blocking(
    address,
    serialisable_object_to_data_chunk<return_params_data_chunk>(ser_obj));
}

template<unsigned int inDataWidth, unsigned int returnDataWidth>
inline
bool
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::slave::
write_blocking(address_type& address,
	       osss_serialisable_archive& ser_arch)
{
  return this->write_blocking(
    address,
    serialisable_archive_to_data_chunk<return_params_data_chunk>(ser_arch));
}

//------------------------------------------------------------------------------
// helper functions
//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
template<class data_chunk_type>
inline
data_chunk_type
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::
serialisable_object_to_data_chunk(osss_serialisable_object& ser_obj)
{
  data_chunk_type _chunk;

  // prepares the object ser_obj for serialisation
  ser_obj.serialise_obj();
     
  // splits the ser_obj into chunks of size of type 
  // base_type::in_params_data_chunk::value_type
  while (!ser_obj.complete())
  {
    _chunk.push_back(ser_obj.read_chunk<
		     typename data_chunk_type::value_type>());
  }
  return _chunk;
}

//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
template<class data_chunk_type>
inline
data_chunk_type
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::
serialisable_archive_to_data_chunk(osss_serialisable_archive& ser_arch)
{
  data_chunk_type _chunk;

  // prepares the osss_serialisable_archive for serialisation
  ser_arch.prepare_read();

  // writes the whole osss_serialisable_archive in chunks of size
  // of the type base_type::in_params_data_chunk::value_type
  while (!ser_arch.complete())
  {
    _chunk.push_back(ser_arch.read_chunk<
		     typename data_chunk_type::value_type>());
  }
  return _chunk;
}

//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
template<class data_chunk_type>
inline
void
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::
data_chunk_to_serialisable_object( data_chunk_type data,
				   osss_serialisable_object& ser_obj)
{
  if (!data.empty())
  {
    while (!ser_obj.complete())
    {
      ser_obj.write_chunk<typename data_chunk_type::value_type>( 
	data.front() );
      data.erase(data.begin());
    }
  
    // finallises the deserialisation of ser_obj
    ser_obj.deserialise_obj();
  }
}

//------------------------------------------------------------------------------

template<unsigned int inDataWidth, unsigned int returnDataWidth>
template<class data_chunk_type>
inline
void
osss_rmi_point_to_point_channel<inDataWidth, returnDataWidth>::
data_chunk_to_serialisable_archive( data_chunk_type data,
				    osss_serialisable_archive& ser_arch)
{
  if (!data.empty())
  {
    ser_arch.prepare_write();
    while (!data.empty())
    {
      ser_arch.write_chunk<typename data_chunk_type::value_type>( 
	data.front() );
      data.erase(data.begin());
    }
    ser_arch.prepare_rebuild();
  }
}

//------------------------------------------------------------------------------
// $Id: osss_rmi_point_to_point_channel.tpp 2991 2009-01-23 14:49:29Z henning $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
