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
#ifndef OSSS_VECTOR_H_INCLUDED_
#define OSSS_VECTOR_H_INCLUDED_

#include "o3s/utils/osss_common.h"
#include "o3s/datatypes/osss_array.h"

/** \namespace osss */
namespace osss {

/**
 * \class osss_vector
 *
 * \brief This class provides a synthesisable vector.
 *
 *        The size is determined during elaboration phase
 *        and cannot be modified afterwards.
 *
 * \note  Creating such vectors is allowed during elaboration
 *        phase only. This allows statically determining the size.
 *
 */
template< class T >
class osss_vector
  : public osssi::osss_array_base< T >
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
  //! Creates an empty vector of size initialSize.
  explicit
  osss_vector( size_type initialSize = 0 );

  //! \brief Resizes the vector to the given size.
  //!
  //! If the new size is greater
  //! than the original size every added entry is set to a default value.
  //! The original values of the other entries are kept. If the new size is
  //! smaller then the original size, all values of entries with indices
  //! greater than or equal to the new size are lost. The original values of
  //! the other entries are kept.
  void
  resize( size_type newSize );

  //! Allows to assign values to instances of class osss_vector like to
  //! any other vector. If a value is assigned to an index which is not in the
  //! range of the actual vector, the vector is automatically resized. All
  //! original values are kept, newly added entries are set to their defaults,
  //! except for the indexed entry, whose value is set to the assigned value.
  reference
  operator[]( size_type index );

  //! Allows to read from instances of class osss_vector like from every
  //! other vector.
  const_reference
  operator[]( size_type index ) const;

};

} // namespace osss

namespace sc_core {
template< class T >
void
sc_trace( sc_trace_file *
        , ::osss::osss_vector< T > const &
        , ::std::string const & );
}

#include "osss_vector.tpp"

#endif // OSSS_VECTOR_H_INCLUDED
// $Id: osss_vector.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
