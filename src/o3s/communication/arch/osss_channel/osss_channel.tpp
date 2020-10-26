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

namespace osss {

template< class ArbiterClass, class AddressDecoderClass >
inline
osss_channel< ArbiterClass, AddressDecoderClass >::
osss_channel( std::string const & name )
  : osss_basic_channel( name )
  , m_numMasters(0)
  , m_numSlaves(0)
  , m_arbiterSelect(0)
  , m_addressDecoderSelect(0)
  , m_arbiter(NULL)
  , m_addressDecoder(NULL)
{}

template< class ArbiterClass, class AddressDecoderClass >
void
osss_channel< ArbiterClass, AddressDecoderClass >::complete()
{
  if (!m_complete_method_called)
  {
    // build arbiter
    if (m_arbiter != NULL)
    {
      std::cout << "Warning: "
                << "Arbiter of OSSS-Channel: " << this->name()
                << " has already been created. "
                << "Keeping the foremost created arbiter."
                << std::endl;
    }

    m_arbiter = new ArbiterClass();
    osss_arbiter_if *tmp_arbiter = 
      dynamic_cast< osss_arbiter_if* >(m_arbiter);
    if (tmp_arbiter == NULL)
    {
      std::cerr << "ArbiterClass is not of type osss_arbiter_if" 
                << std::endl;
      OSSS_ABORT();
    }

    /// FIXME: the delta delay is a hack and should not be required!
    osss_clock_reset_if::forward_ports_delayed( *m_arbiter );
    // osss_clock_reset_if::forward_ports( *m_arbiter );
    //m_arbiter->reset_port( osss_basic_channel::reset_port_port );
    //m_arbiter->clock_port( osss_basic_channel::clock_port_port );
    m_arbiter->buildConnectionList();
    m_arbiter->connect(*this);
    m_arbiter->__osss_buildProcess();

    // build address decoder
    if (m_addressDecoder != NULL)
    {
      std::cout << "Warning: "
                << "Address decoder of OSSS-Channel: " << this->name()
                << " has already been created. "
                << "Keeping the foremost created address decoder."
                << std::endl;
    }

    m_addressDecoder = new AddressDecoderClass();
    osss_address_decoder_if *tmp_addressDecoder = 
      dynamic_cast< osss_address_decoder_if* >(m_addressDecoder);
    if (tmp_addressDecoder == NULL)
    {
      std::cerr << "AddressDecoderClass is not of type osss_address_decoder_if" 
                << std::endl;
      OSSS_ABORT();
    }

    /// FIXME: the delta delay is a hack and should not be required!
    osss_clock_reset_if::forward_ports_delayed( *m_addressDecoder );
    // osss_clock_reset_if::forward_ports( *m_addressDecoder );
    //m_addressDecoder->reset_port( osss_basic_channel::reset_port_port );
    //m_addressDecoder->clock_port( osss_basic_channel::clock_port_port );
    if (! m_addressDecoder->isDummy() )
    {
      m_addressDecoder->buildConnectionList();
      m_addressDecoder->connect(*this);
      m_addressDecoder->__osss_buildProcess();
    }

    m_complete_method_called = true;
  }
  else
  {
    std::cout << "Warning: complete() method of channel "
              << this->name() << " has been called twice."
              << " The second call does not have any effect."
              << std::endl;
  }
}

//-------------------------------------------------------------------------//

template< class ArbiterClass, class AddressDecoderClass >
inline
osss_channel< ArbiterClass, AddressDecoderClass >::~osss_channel()
{
  delete m_arbiter;
  delete m_addressDecoder;
}

//-------------------------------------------------------------------------//


template< class ArbiterClass, class AddressDecoderClass >
inline
ArbiterClass *
osss_channel< ArbiterClass, AddressDecoderClass >::getArbiter()
{
  return m_arbiter;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
AddressDecoderClass *
osss_channel< ArbiterClass, AddressDecoderClass >::getAddressDecoder()
{
  return m_addressDecoder;
}

//-------------------------------------------------------------------------//

template< class ArbiterClass, class AddressDecoderClass >
inline
std::vector< std::vector< osss_signal_list_element_base * > * > *
osss_channel< ArbiterClass, AddressDecoderClass >::getMultiSignalList()
{
  return &m_multiSignalList;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
std::vector< osss_via_list_element * > *
osss_channel< ArbiterClass, AddressDecoderClass >::getMuxViaList()
{
  return &m_muxViaList;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
std::vector< osss_via_list_element * > *
osss_channel< ArbiterClass, AddressDecoderClass >::getOrViaList()
{
  return &m_orViaList;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
std::vector< osss_via_list_element * > *
osss_channel< ArbiterClass, AddressDecoderClass >::getAndOrViaList()
{
  return &m_andOrViaList;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
std::vector< osss_via_list_element * > *
osss_channel< ArbiterClass, AddressDecoderClass >::getAndViaList()
{
  return &m_andViaList;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
sc_core::sc_signal<unsigned int> * 
osss_channel< ArbiterClass, AddressDecoderClass >::getArbiterSelect()
{
  return &m_arbiterSelect;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
sc_core::sc_signal<unsigned int> * 
osss_channel< ArbiterClass, AddressDecoderClass >::getAddressDecoderSelect()
{
  return &m_addressDecoderSelect; 
}

template< class ArbiterClass, class AddressDecoderClass >
inline
unsigned int 
osss_channel< ArbiterClass, AddressDecoderClass >::getNumTransactors()
{
  return m_numMasters + m_numSlaves;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
unsigned int 
osss_channel< ArbiterClass, AddressDecoderClass >::getNumMasters()
{
  return m_numMasters;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
void 
osss_channel< ArbiterClass, AddressDecoderClass >::incNumMasters()
{
  m_numMasters++;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
unsigned int 
osss_channel< ArbiterClass, AddressDecoderClass >::getNumSlaves()
{
  return m_numSlaves;
}

template< class ArbiterClass, class AddressDecoderClass >
inline
void 
osss_channel< ArbiterClass, AddressDecoderClass >::incNumSlaves()
{
  m_numSlaves++;
}

} // end namespace osss
//
// $Id: osss_channel.tpp 2842 2008-12-04 09:30:13Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
