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

#ifndef OSSS_P2P_H_INCLUDED
#define OSSS_P2P_H_INCLUDED

#include "o3s/structural/arch/osss_architecture_object.h"

namespace osss {

class osss_p2p
  : public osss_architecture_object
{
  typedef osss_architecture_object base_type;

protected:

  osss_p2p( unsigned int             in_data_width
          , unsigned int             return_data_width
          , sc_core::sc_module_name  /* unused */ )
    : base_type()
    , m_in_data_width(in_data_width)
    , m_return_data_width(return_data_width)
  {}

  osss_p2p( sc_core::sc_module_name  name
          , unsigned int             in_data_width
          , unsigned int             return_data_width )
    : base_type(name)
    , m_in_data_width(in_data_width)
    , m_return_data_width(return_data_width)
  {}

public:

  virtual const char* kind() const { return "osss_p2p"; }

  unsigned int in_data_width()     const { return m_in_data_width; }
  unsigned int return_data_width() const { return m_return_data_width; }

private:

  unsigned int m_in_data_width;
  unsigned int m_return_data_width;
};

} // namespace osss

#endif // OSSS_P2P_H_INCLUDED
// $Id: osss_p2p.h 2840 2008-12-04 09:30:05Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
