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
#ifndef OSSS_ENUM_HH
#define OSSS_ENUM_HH

#include "o3s/datatypes/osss_serialisable_object.h"
#include "o3s/datatypes/osss_serialisable_archive.h"

namespace osss {

template<typename E>
class osss_enum : public osss_serialisable_object
{
public:
    CLASS(osss_enum, osss_serialisable_object);

    /**
     * The actual value hold by this instance
     */
    E value;

public:
    /**
     * default constructor.
     */
    CONSTRUCTOR(public, osss_enum, ())
    {
#ifdef OSSS_GREEN
      serialise_obj();
#endif
    }

   /**
    * initialize with Enumeration given.
    */
    CONSTRUCTOR(public, osss_enum, (E init)) : value(init)
    {
#ifdef OSSS_GREEN
      serialise_obj();
#endif
    }
    
    /** THIS CONSTRUCTOR IS VERY IMPORTANT AND HAS TO BE IN EACH 
     *  SERIALISABLE_OBJECT
     *
     *  Special constructor for rebuilding an Enum out of an 
     *  osss_serialisable_archive
     */
    CONSTRUCTOR(public, osss_enum, (osss_serialisable_archive* arch))
    {
#ifdef OSSS_GREEN
      arch->rebuild(*this);
#endif
    }

    virtual void serialise()
    {
#ifdef OSSS_GREEN
      int int_rep = value;
      ::osss::osss_serialisable_object::store_element(int_rep);
#endif
    }

    virtual void deserialise()
    {
#ifdef OSSS_GREEN
      int int_rep;
      ::osss::osss_serialisable_object::restore_element(int_rep);
      value = static_cast<E>(int_rep);
#endif
    }

    /**
     * assign an Enumeration of this type. In debug
     * version, assert, that it is really in the Range of
     * this Enumeration.
     */
    inline osss_enum& operator = (E setval) 
    {
      value = setval;
      return *this;
    }

    inline osss_enum& operator = (const osss_enum& rhs) 
    {
      if (&rhs != this)
        value = rhs.value;
      return *this;
    }

    /**
    * returns the enumeration value hold with this
    * enum.
    */
    inline operator E () const { return value; }

}; // osss_enum

} // namespace osss

#endif /* OSSS_BASIC_HH */
// $Id: osss_enum.h 2394 2008-07-01 20:45:28Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h) (osss_icodes,h)
