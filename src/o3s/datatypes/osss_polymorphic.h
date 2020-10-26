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
#ifndef OSSS_POLYMORPHIC_H
#define OSSS_POLYMORPHIC_H

#include <iosfwd>
#include "o3s/datatypes/osss_polymorphic_if.h"
#include "o3s/utils/osss_class_proxy_registry.h"

namespace osss {
template< class ObjectType >
class osss_polymorphic;

template< class T >
::std::ostream &
operator<<( ::std::ostream & os, const osss_polymorphic< T > & object );
}

namespace sc_core {
template< class T >
void
sc_trace ( sc_trace_file * tr,
           const ::osss::osss_polymorphic< T > & object, 
           const ::std::string & str );
}

namespace osss {

/// forward declaration for friendness
template< typename > class osss_socket;

/**
 * \class osss_polymorphic
 *
 * \brief This class must be used to declare polymorphic objects within
 *        OSSS.
 *
 */
template< class ObjectType >
class osss_polymorphic : public osss_polymorphic_if
{
public:

  //! \brief Default constructor.
  osss_polymorphic();

  //! \brief Copy constructor.
  template< class SourceType >
  osss_polymorphic( const osss_polymorphic< SourceType > & source );

  //! \brief Copy constructor.
  osss_polymorphic( const osss_polymorphic & source );

  //! \brief Copy constructor.
  osss_polymorphic( const ObjectType & source );

  ~osss_polymorphic();

  //! Enable assignments from normal objects to polymorphic objects.
  template< class SourceType >
  inline
  osss_polymorphic< ObjectType > &
  operator=( const SourceType & source );

  //! Enable assignments from polymorphic objects to polymorphic
  //! objects of the same base type.
  inline
  osss_polymorphic<ObjectType> &
  operator=( const osss_polymorphic< ObjectType > & source );

  //! Enable assignments from polymorphic objects to polymorphic
  //! objects of a derived base type.
  template< class SourceObjectType >
  inline
  osss_polymorphic<ObjectType> &
  operator=( const osss_polymorphic< SourceObjectType > & source );

  //! A type-cast function for potentially compatible assignments from a
  //! polymorphic object.
  template< class TargetType >
  TargetType
  cast();

  //! A type conversion that enables assignments from polymorphic objects
  //! to normal objects (but only, iff assignment compatible).
  inline
  const ObjectType &
  cast_to_const() const;

  //! Overload the right-arrow operator to enable calls to methods of the
  //! objects, assigned to a polymorphic object, by re-directing these calls.
  inline
  ObjectType *
  operator->() const;

  //! Returns the interface of the object inside the polymorphic object.
  //! If the object has no interface of type sc_core::sc_interface it
  //! returns NULL.
  inline
  sc_core::sc_interface*
  get_interface() const;

  //! The overloading of this operator is absolutely necessary, because this
  //! forces the write method of a signal to always update the current and
  //! future value, when writing a polymorphic object to a signal!
  inline
  bool
  operator==( const ObjectType & object ) const;

  //! The overloading of this operator is required by SystemC. But it
  //! does not have any reasonable functionality for instances of this class,
  //! because processes are not allowed to be sensitive on signals that carry
  //! objects.
  inline
  bool
  operator==( const osss_polymorphic< ObjectType > & object ) const;

  /** These operators perform the operation on their content, therefore
      the "ObjectType" class must support the operations, if you use them. */
  /* binary operations */
  template< class OtherType >
  inline
  osss_polymorphic< ObjectType >
  add( const osss_polymorphic< OtherType > & other ) const;

  template< class OtherType >
  inline
  osss_polymorphic< ObjectType >
  sub( const osss_polymorphic< OtherType > & other ) const;

  template< class OtherType >
  inline
  osss_polymorphic< ObjectType >
  div( const osss_polymorphic< OtherType > & other ) const;

  template< class OtherType >
  inline
  osss_polymorphic< ObjectType >
  mul( const osss_polymorphic< OtherType > & other ) const;

  template< class OtherType >
  inline
  osss_polymorphic< ObjectType >
  mod( const osss_polymorphic< OtherType > & other ) const;

  template< class OtherType >
  bool
  lt( const osss_polymorphic< OtherType > & other ) const;

protected:

  //! This pointer is linked to a copy of a standard C++ object that is
  //! assigned to the polymorphic oject:
  ObjectType * m_objectPointer;

private:

  friend class osss_class_proxy_registry;
  template<class> friend class osss_socket;

  //! A type conversion that provides non-const access to the underlying
  //! object
  inline
  ObjectType &
  plugged_object();

  friend
  ::std::ostream &
  operator<< <>( ::std::ostream & os, const osss_polymorphic< ObjectType > & object );

  friend
  void
  ::sc_core::sc_trace<>( ::sc_core::sc_trace_file * tr,
                         const osss_polymorphic< ObjectType > & object, 
                         const ::std::string & str );
};

// add
template< class FirstType, class SecondType >
inline
osss_polymorphic< FirstType >
operator+ ( const osss_polymorphic< FirstType >  & first,
            const osss_polymorphic< SecondType > & second );
// sub
template< class FirstType, class SecondType >
inline
osss_polymorphic< FirstType >
operator- ( const osss_polymorphic< FirstType >  & first,
            const osss_polymorphic< SecondType > & second );
// div
template< class FirstType, class SecondType >
inline
osss_polymorphic< FirstType >
operator/ ( const osss_polymorphic< FirstType >  & first,
            const osss_polymorphic< SecondType > & second );
// mul
template< class FirstType, class SecondType >
inline
osss_polymorphic< FirstType >
operator* ( const osss_polymorphic< FirstType >  & first,
            const osss_polymorphic< SecondType > & second );
// mod (modulo, %)
template< class FirstType, class SecondType >
inline
osss_polymorphic< FirstType >
operator% ( const osss_polymorphic< FirstType >  & first,
            const osss_polymorphic< SecondType > & second );
// lt (less than, <)
template< class FirstType, class SecondType >
bool
operator< ( const osss::osss_polymorphic< FirstType >  & first,
            const osss::osss_polymorphic< SecondType > & second );

} //namespace osss

#include "o3s/datatypes/osss_polymorphic.tpp"

#endif // OSSS_POLYMORPHIC_H
// $Id: osss_polymorphic.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
