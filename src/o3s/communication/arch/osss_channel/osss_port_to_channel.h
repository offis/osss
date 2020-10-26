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

#ifndef OSSS_PORT_TO_CHANNEL_HH
#define OSSS_PORT_TO_CHANNEL_HH

#include "o3s/communication/arch/osss_channel/osss_port_channel.h"

namespace osss {

template< class InterfaceType >
class osss_port_to_channel
  : public osss_port<osss_channel_if<InterfaceType> >
{
public:

  // typedefs
  
  typedef osss_channel_if<InterfaceType>      if_type;
  typedef osss_port<if_type>                  base_type;
  typedef osss_port_to_channel<InterfaceType> this_type;

public:

  // constructors

  osss_port_to_channel()
    : base_type()
    {}

  explicit osss_port_to_channel( const char* name_ )
    : base_type(name_)
    {}

  explicit osss_port_to_channel( if_type& interface_ )
    : base_type(interface_)
    {}

  osss_port_to_channel( const char* name_, if_type& interface_ )
    : base_type(name_, interface_)
    {}

  explicit osss_port_to_channel( base_type& parent_ )
    : base_type(parent_)
    {}

  osss_port_to_channel( const char* name_, base_type& parent_ )
    : base_type(name_, parent_)
    {}

  osss_port_to_channel( this_type& parent_ )
    : base_type(parent_)
    {}

  osss_port_to_channel( const char* name_, this_type& parent_ )
    : base_type(name_, parent_)
    {}

  // destructor
  virtual ~osss_port_to_channel()
    {}

private:
  // disabled copy-constructor
  osss_port_to_channel( const this_type& ) {}
 
};

//------------------------------------------------------------------------------
// partial specialisation for
// osss_rmi_channel_client_if
//------------------------------------------------------------------------------

template<>
class osss_port_to_channel<osss_rmi_channel_client_if>
  : public osss_port<osss_channel_if<osss_rmi_channel_client_if> >
{
public:

  // typedefs
  
  typedef osss_channel_if<>                                       if_type;
  typedef osss_port<osss_channel_if<osss_rmi_channel_client_if> > base_type;
  typedef osss_port_to_channel<osss_rmi_channel_client_if>        this_type;

public:

  // constructors

  osss_port_to_channel()
    : base_type()
    {}

  explicit osss_port_to_channel( const char* name_ )
    : base_type(name_)
    {}

  explicit osss_port_to_channel( if_type& interface_ )
    : base_type(interface_)
    {}

  osss_port_to_channel( const char* name_, if_type& interface_ )
    : base_type(name_, interface_)
    {}

  explicit osss_port_to_channel( base_type& parent_ )
    : base_type(parent_)
    {}

  osss_port_to_channel( const char* name_, base_type& parent_ )
    : base_type(name_, parent_)
    {}

  osss_port_to_channel( this_type& parent_ )
    : base_type(parent_)
    {}

  osss_port_to_channel( const char* name_, this_type& parent_ )
    : base_type(name_, parent_)
    {}

  // destructor
  virtual ~osss_port_to_channel()
    {}

private:
  // disabled copy-constructor
  osss_port_to_channel( const this_type& ) : base_type() {}
 
};

//------------------------------------------------------------------------------
// partial specialisation for
// osss_rmi_channel_server_if
//------------------------------------------------------------------------------

template<>
class osss_port_to_channel<osss_rmi_channel_server_if>
  : public osss_port<osss_channel_if<osss_rmi_channel_server_if> >
{
public:

  // typedefs
  
  typedef osss_channel_if<>                                       if_type;
  typedef osss_port<osss_channel_if<osss_rmi_channel_server_if> > base_type;
  typedef osss_port_to_channel<osss_rmi_channel_server_if>        this_type;

public:

  // constructors

  osss_port_to_channel()
    : base_type()
    {}

  explicit osss_port_to_channel( const char* name_ )
    : base_type(name_)
    {}

  explicit osss_port_to_channel( if_type& interface_ )
    : base_type(interface_)
    {}

  osss_port_to_channel( const char* name_, if_type& interface_ )
    : base_type(name_, interface_)
    {}

  explicit osss_port_to_channel( base_type& parent_ )
    : base_type(parent_)
    {}

  osss_port_to_channel( const char* name_, base_type& parent_ )
    : base_type(name_, parent_)
    {}

  osss_port_to_channel( this_type& parent_ )
    : base_type(parent_)
    {}

  osss_port_to_channel( const char* name_, this_type& parent_ )
    : base_type(name_, parent_)
    {}

  // destructor
  virtual ~osss_port_to_channel()
    {}

private:
  // disabled copy-constructor
  osss_port_to_channel( const this_type& ) : base_type() {}
 
};

} // namespace osss

#endif

// $Id: osss_port_to_channel.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
