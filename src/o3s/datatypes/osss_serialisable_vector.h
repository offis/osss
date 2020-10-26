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
#ifndef OSSS_SERIALISABLE_VECTOR_HH
#define OSSS_SERIALISABLE_VECTOR_HH

#include "o3s/datatypes/osss_vector.h"
#include "o3s/datatypes/osss_serialisable_archive.h"

namespace osss {

/**
 *
 * \class osss_serialisable_vector
 *
 * \brief This class provides a synthesisable AND serialisable vector of  
 *        dynamic but bounded size. 
 *
 * \note  All types T inside this class need to be either
 *        C++ built-in types, SystemC data typaes or user-defined 
 *        serialisable objects.
 *        The size is determined during elaboration time and cannot be
 *        changed during simulation.
 *
 */
template< class T >
class osss_serialisable_vector
  : public osss_vector<T>
  , public osss_serialisable_object
{
public:
  typedef osss_vector<T> base_type;

  typedef typename base_type::reference       reference;
  typedef typename base_type::const_reference const_reference;
  typedef typename base_type::size_type       size_type;

  OSSS_IS_SERIALISABLE( osss_serialisable_vector );

  //! \brief Constructor. 
  //!
  //! Creates an empty vector of size 0
  OSSS_SERIALISABLE_CTOR(osss_serialisable_vector, ());

  //! \brief Constructor. 
  //!
  //! Creates an empty vector of the specified size
  OSSS_SERIALISABLE_CTOR(osss_serialisable_vector, (size_type size));

  //! \brief Copy constructor. 
  //!
  //! Creates a new vector as exact copy of the passed one.
  OSSS_SERIALISABLE_CTOR(osss_serialisable_vector,
                         (const osss_serialisable_vector<T> & sourceVector));

  ~osss_serialisable_vector();
  
  //! Copies the contents of the source vector to the target vector.
  osss_serialisable_vector &
  operator=( const osss_serialisable_vector<T> & sourceVector );

  using base_type::size;

  void used_size(size_type s);
  size_type used_size() const;

  virtual void serialise();
  virtual void deserialise();

private:

  size_type m_used_size;

};

} // namespace osss

#include "o3s/datatypes/osss_serialisable_vector.tpp"

#endif // OSSS_SERIALISABLE_VECTOR_HH
// $Id: osss_serialisable_vector.h 2740 2008-11-27 09:50:41Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
