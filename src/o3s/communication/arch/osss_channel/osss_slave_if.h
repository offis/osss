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

#ifndef OSSS_SLAVE_IF_HH
#define OSSS_SLAVE_IF_HH

#include "o3s/utils/osss_systemc.h"
#include "o3s/communication/arch/osss_channel/osss_multi_signal_if.h"
#include "o3s/communication/arch/osss_channel/osss_address_map.h"

namespace osss {

class osss_slave_if : public osss_multi_signal_if
{
public:

  osss_slave_if();
  virtual ~osss_slave_if();

  virtual bool isMaster();
  virtual bool isSlave();

  template< class ADDRESS >
  void setAddressRange( const ADDRESS &low, const ADDRESS &high)
  {
    if (m_address_range == NULL)
      m_address_range = new osss_address_range<ADDRESS>(low, high);
    else
    {
      std::cout << "Warning: Address range has already been specified!"
                << std::endl
                << "Old address range will be overwritten."
                << std::endl;

      delete m_address_range;
      m_address_range = new osss_address_range<ADDRESS>(low, high);
    }
  }

  template< class ADDRESS >
  osss_address_range< ADDRESS > * getAddressRange()
  {
    if (m_address_range != NULL)
    {
      return dynamic_cast<osss_address_range< ADDRESS > * >(m_address_range);
    }
    else
      return NULL;
  }

protected:

  void wait()        const { sc_core::wait();    }
  void wait( int n ) const { sc_core::wait( n ); }

  osss_address_range_base * m_address_range;

};

} // namespace osss

#endif
// $Id: osss_slave_if.h 2841 2008-12-04 09:30:09Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
