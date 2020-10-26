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

#ifndef OSSS_ADDRESS_MAP_HH
#define OSSS_ADDRESS_MAP_HH

#include <map>
#include <string>

namespace osss {

using std::map;
using std::string;

class osss_address_range_base
{
public:
  osss_address_range_base()
  {}

  virtual ~osss_address_range_base()
  {}

};

template< class ADDRESS >
class osss_address_range : public osss_address_range_base
{
public:
  osss_address_range( const ADDRESS &low , const ADDRESS &high ) 
    : osss_address_range_base() , from( low ) , to( high ) 
  {}

  osss_address_range ( const osss_address_range<ADDRESS> &a )
                   : osss_address_range_base() 
  {
    from = a.from;
    to = a.to;
  }

  osss_address_range () 
    : from( 0 ) , to( 0 ) 
  {}
 
  bool decode( const ADDRESS &before , ADDRESS &after ) const 
  {
    if( from <= before && before < to ) 
    {
      after = before - from;
      return true;
    }
    return false;
  }
  
  bool is_in_range( const ADDRESS& addr ) const
  {
    return ((from <= addr) && (addr <= to));
  }

  ADDRESS low()
  {
    return from;
  }

  ADDRESS high()
  {
    return to;
  }

  bool valid()
  {
    return !((from == 0) && (to == 0));
  }

protected:
  ADDRESS from , to;

};

//----------------------------------------------------------------------------

class osss_address_map_base
{
public:
  osss_address_map_base()
  {}

  virtual ~osss_address_map_base()
  {}

};

template< class ADDRESS >
class osss_address_map : public osss_address_map_base
{
public:

  bool decode( const ADDRESS &before , ADDRESS &after , int &id ) const;

  bool decode( ADDRESS addr, int &id ) const
  {
    return decode(addr, addr, id);
  }

  void add_to_map( const ADDRESS &from , const ADDRESS &to , int id ) 
  {
    address_range_map[id] = osss_address_range<ADDRESS>( from , to );
  }

  void add_to_map( const osss_address_range< ADDRESS > &am, int id)
  {
    address_range_map[id] = osss_address_range<ADDRESS>(am);
  }

  unsigned int size()
  {
    return address_range_map.size();
  }

private:
  map< int , osss_address_range<ADDRESS> > address_range_map;

};

template<typename ADDRESS>
bool osss_address_map<ADDRESS>::decode( const ADDRESS &before , 
					ADDRESS &after , int &id ) const
{

  typename map< int , osss_address_range<ADDRESS> >::const_iterator i;

  for( i = address_range_map.begin();
       i != address_range_map.end();
       ++i ) 
  {
    if( (*i).second.decode( before , after ) ) 
    {
      id = (*i).first;
      return true;
    }
  }

  return false;
}

} // namespace osss

#endif
// $Id: osss_address_map.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
