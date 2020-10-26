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

#ifndef OSSS_ABSTRACT_CHANNEL_HH_INCLUDED
#define OSSS_ABSTRACT_CHANNEL_HH_INCLUDED

#include <vector>
#include <string>

#include "o3s/structural/arch/osss_architecture_object.h"

namespace osss {

template<class ArbiterClass,
         class AddressDecoderClass = osss_no_address_decoder,
	 unsigned int MinDataWidth = 8,
	 unsigned int AddressWidth = 32>
class osss_abstract_channel :
  public osss_channel<ArbiterClass, AddressDecoderClass>
{
public:
  typedef std::vector<sc_dt::sc_uint<MinDataWidth> > data_chunk;
  static std::string dump_data_chunk(const data_chunk& data);

  typedef sc_dt::sc_uint<MinDataWidth>    data_type;
  typedef sc_dt::sc_uint<AddressWidth> address_type;
  typedef enum {NO_ACTION, READ_ACTION, WRITE_ACTION} action_type;

  explicit osss_abstract_channel(std::string name) 
    : osss_channel<ArbiterClass, AddressDecoderClass>(name)
  {}
  
  class abstract_master_if : public virtual osss_master_if
  {
  public:
    virtual 
    bool 
    write_blocking(address_type slave_base_addr,
		   const data_chunk& data) = 0;

    virtual
    bool
    write_blocking(address_type slave_base_addr,
		   osss_serialisable_object& ser_obj) = 0;

    virtual
    bool
    write_blocking(address_type slave_base_addr,
		   osss_serialisable_archive& ser_arch) = 0;

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
    read_blocking(address_type slave_base_addr,
                  data_chunk& data,
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
    read_blocking(address_type slave_base_addr,
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
    read_blocking(address_type slave_base_addr,
                  osss_serialisable_archive& ser_arch,
                  unsigned int size) = 0;
  };

  class abstract_slave_if : public virtual osss_slave_if
  {
  public:
    virtual
    void
    wait_for_action(address_type& base_address,
		    action_type& action) = 0;

    virtual 
    bool 
    read_blocking(data_chunk& data) = 0;

    virtual
    bool
    read_blocking(osss_serialisable_object& ser_obj) = 0;

    virtual
    bool
    read_blocking(osss_serialisable_archive& ser_arch) = 0;

    virtual 
    bool 
    write_blocking(const data_chunk& data) = 0;

    virtual
    bool
    write_blocking(osss_serialisable_object& ser_obj) = 0;

    virtual
    bool
    write_blocking(osss_serialisable_archive& ser_arch) = 0;
  };
  
};

#include "o3s/communication/arch/osss_abstract_channel.tpp"

} // namespace osss

#endif // OSSS_ABSTRACT_CHANNEL_HH_INCLUDED
// $Id: osss_abstract_channel.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
