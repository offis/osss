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
#ifndef OSSS_ARRAY_H_INCLUDED
#define OSSS_ARRAY_H_INCLUDED

#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_systemc.h"

#include <cstddef>  // std::size_t
#include <vector>   // std::vector<T>
#include <iosfwd>   // std::ostream&

namespace osss {
namespace osssi {

/**
 * \class osss_array_base
 *
 * \brief This class is a base class for synthesisable arrays.
 *
 * \note This class is not intended to be utilised in a user-design
 *       In a user-design use either: osss_array or osss_vector
 *
 * \see osss_array
 * \see osss_vector
 */
template< class T >
class osss_array_base
{
  typedef osss_array_base<T> this_type;
  typedef std::vector<T>     storage_type;
public:
  typedef T item_type;
  typedef typename storage_type::reference       reference;
  typedef typename storage_type::const_reference const_reference;

  // We re-use the size_type of the underlying STL container.
  // STL recommends not to use your own datatypes here.
  typedef typename storage_type::size_type       size_type;

  //! \brief Returns the size of the array.
  //!
  //! The size is determined by the 'Size' template argument
  size_type size() const;

  //! Allows to assign values to instances of class osss_array like to
  //! any other array. If a value is assigned to an index which is not in the
  //! range of the actual array, an error will occur.
  reference
  operator[]( size_type index );

  //! Allows to read from instances of class osss_array like from every
  //! other array. If the reading is performed on an index which is not in the
  //! range of the actual array, an error will occur.
  const_reference
  operator[]( size_type index ) const;

  //! Two instances of class osss_array are only equal, if they are of
  //! the same size and the value of all corresponding entries are equal.
  //! In this case true is returned, otherwise false is returned.
  //! ==operator is necessary for using class osss_array with
  //! sc_signals.
  bool
  operator == ( const this_type & ) const;

protected:

  //! \brief protected constructor.
  //!
  //! Creates an empty array of the specified size
  explicit osss_array_base(size_type size);

  /**
   * \brief protected destructor
   *
   * Since destructor is not virtual, prohibit polymorphic
   * destruction by making it protected.
   */
  ~osss_array_base(){}

  /**
   * \brief resize underlying storage
   *
   * \see osss_vector::resize()
   */
  void resize( size_type );

private:

  //! This member contains the array itself.
  storage_type m_elements;

};


template < typename T >
::std::ostream & operator << ( ::std::ostream &
                             , const osss_array_base< T >& );

} // namespace osssi

/**
 * \class osss_array
 *
 * \brief This class provides a synthesisable array of a fixed size.
 *        The size is determined at compile time and cannot be changed
 *        during runtime (e.g. during the elaboration phase)
 *
 * \note  For an array that can resized during the elaboration phase
 *        please refer to class osss_vector
 *
 * \see osss_vector
 */
template< class T, size_t Size >
class osss_array
  : public osssi::osss_array_base<T>
{
  typedef typename osssi::osss_array_base< T > base_type;

public:

  typedef typename base_type::reference        reference;
  typedef typename base_type::const_reference  const_reference;

  // We re-use the size_type of the underlying STL container.
  // STL recommends not to use your own datatypes here.
  typedef typename base_type::size_type        size_type;

  //! \brief Constructor.
  //!
  //! Creates an empty array of size 'Size'
  osss_array()
    : base_type( Size )
  {}

  size_type size() const
    { return Size; }

};

} //namespace osss

namespace sc_core {

template< typename T, size_t Size >
void
sc_trace( sc_trace_file *
        , const ::osss::osss_array< T, Size > &
        , const ::std::string & );

} // namespace sc_core

#include "o3s/datatypes/osss_array.tpp"

#endif // OSSS_ARRAY_H_INCLUDED
// $Id: osss_array.h 3015 2009-02-26 21:28:51Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
