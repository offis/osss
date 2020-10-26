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

#ifndef OSSS_CHANNEL_HH
#define OSSS_CHANNEL_HH

#include <vector>

#include "o3s/communication/arch/osss_channel/osss_basic_channel.h"

namespace osss {

class osss_no_address_decoder;
class osss_arbiter_if;
class osss_address_decoder_if;

template< class ArbiterClass
        , class AddressDecoderClass = osss_no_address_decoder
        >
class osss_channel
  : public osss_basic_channel
{
public:

  explicit osss_channel( std::string const& name 
                           = sc_core::sc_gen_unique_name( "osss_channel" ) );

  virtual const char* kind() const
    { return "osss_channel"; }

  virtual ~osss_channel();

  ArbiterClass * getArbiter();
  AddressDecoderClass * getAddressDecoder();

  virtual std::vector< std::vector< osss_signal_list_element_base * > * > *
    getMultiSignalList();

  virtual std::vector< osss_via_list_element * > *
    getMuxViaList();

  virtual std::vector< osss_via_list_element * > *
    getOrViaList();

  virtual std::vector< osss_via_list_element * > *
    getAndOrViaList();

  virtual std::vector< osss_via_list_element * > *
    getAndViaList();

  virtual sc_core::sc_signal<unsigned int> * getArbiterSelect();
  virtual sc_core::sc_signal<unsigned int> * getAddressDecoderSelect();

  virtual unsigned int getNumTransactors();

  virtual unsigned int getNumMasters();
  virtual void incNumMasters();

  virtual unsigned int getNumSlaves();
  virtual void incNumSlaves();

  virtual void complete();

protected:

  std::vector< std::vector< osss_signal_list_element_base * > * > 
    m_multiSignalList;

  std::vector< osss_via_list_element * > m_muxViaList;
  std::vector< osss_via_list_element * > m_orViaList;
  std::vector< osss_via_list_element * > m_andOrViaList;
  std::vector< osss_via_list_element * > m_andViaList;

  unsigned int m_numMasters;
  unsigned int m_numSlaves;

  sc_core::sc_signal<unsigned int> m_arbiterSelect;
  sc_core::sc_signal<unsigned int> m_addressDecoderSelect;

private:
  ArbiterClass * m_arbiter;
  AddressDecoderClass * m_addressDecoder;

};

} // namespace osss

#include "o3s/communication/arch/osss_channel/osss_channel.tpp"

#endif
// $Id: osss_channel.h 2842 2008-12-04 09:30:13Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
