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

template< class ObjectType >
osss_polymorphic< ObjectType >::osss_polymorphic()
  : m_objectPointer(new ObjectType) // Implies, that every class being
                                    // passed to a osss_polymorphic<> must
                                    // have a default constructor.
{ }

//----------------------------------------------------------------------------//

template< class ObjectType >
osss_polymorphic< ObjectType >::osss_polymorphic( const ObjectType & source )
  : m_objectPointer(0)
{
  operator=( source );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
template< class SourceType >
osss_polymorphic< ObjectType >::
osss_polymorphic( const osss_polymorphic< SourceType > & source )
  : osss_polymorphic_if( source ),
    m_objectPointer(0)
{
  operator=( source );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
osss_polymorphic< ObjectType >::
osss_polymorphic( const osss_polymorphic & source )
  : osss_polymorphic_if( source ),
    m_objectPointer(0)
{
  operator=( source );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
osss_polymorphic< ObjectType >::~osss_polymorphic()
{
  if ( m_objectPointer )
  {
    delete( m_objectPointer );
    m_objectPointer = 0;
  }
}

//----------------------------------------------------------------------------//

#if 0 /* disabled */
template< class ObjectType >
ObjectType *
osss_polymorphic< ObjectType >::_getClone_() const
{
  if ( m_objectPointer )
  {
    return( m_objectPointer->_getClone_() );
  }
  else
  {
    std::cerr << "The polymorphic object does not contain any object "
              << "(m_objectPointer is NULL)" << std::endl;
    OSSS_ABORT();
  }
}
#endif /* disabled _getClone_() */

//----------------------------------------------------------------------------//

template< class ObjectType >
template< class SourceType >
inline
osss_polymorphic< ObjectType > &
osss_polymorphic< ObjectType >::operator=( const SourceType & source )
{
#ifdef __FAST_SIMULATION__
  if ( !m_objectPointer )
  {
    std::cerr << "The polymorphic object does not contain any object "
              << "(m_objectPointer is NULL)" << std::endl;
    OSSS_ABORT();
  }
  if ( typeid( *m_objectPointer ) == typeid( source ) )
  {
    *m_objectPointer = source;
  }
  else
#endif
  {
    // We've to be careful, since it could be "&source == m_objectPointer"
    SourceType * replacement =
        osssi::osss_class_proxy_registry::instance().getClone( source );
    // delete does nothing, if arg is NULL
    delete m_objectPointer;
    m_objectPointer = replacement;
  }
  return( *this );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
osss_polymorphic< ObjectType > &
osss_polymorphic< ObjectType >::
operator=( const osss_polymorphic< ObjectType > & source )
{
  ObjectType * replacement =
        osssi::osss_class_proxy_registry::instance().getClone
           ( source.cast_to_const() );
  if( !replacement ) {
    std::cerr << "Could not get clone of object of type "
              << osssi::type_id::from< ObjectType >() << std::endl;
  }
  if( m_objectPointer ) {
    delete( m_objectPointer );
  }
  m_objectPointer = replacement;
  return( *this );
}

template< class ObjectType >
template< class SourceObjectType >
inline
osss_polymorphic< ObjectType > &
osss_polymorphic< ObjectType >::
operator=( const osss_polymorphic< SourceObjectType > & source )
{
  /* due to specialized operator for osss_polymorhic<ObjectType>
   * no check if this == &source is necessary */

  SourceObjectType* source_clone =
        osssi::osss_class_proxy_registry::instance().getClone
           ( source.cast_to_const() );
  if( !source_clone ) {
    std::cerr << "Could not get clone of object of type "
              << osssi::type_id::from< SourceObjectType>() << std::endl;
  }
  ObjectType* replacement  = dynamic_cast<ObjectType*>( source_clone );
  if( !replacement ) {
    std::cerr << "Assignment between incompatible types: "
              << osssi::type_id::from< ObjectType >() << " = "
              << osssi::type_id::from< SourceObjectType >()
              << std::endl;
    OSSS_ABORT();
  }
  if ( m_objectPointer ) {
    delete( m_objectPointer );
  }
  m_objectPointer = replacement;
  return( *this );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
template< class TargetType >
TargetType
osss_polymorphic< ObjectType >::cast()
{
  if ( !m_objectPointer )
  {
    std::cerr << "Can not cast, the polymorphic object does not contain any object "
              << "(m_objectPointer is NULL)" << std::endl;
    OSSS_ABORT();
  }
  if ( !dynamic_cast< TargetType * >( m_objectPointer ) )
  {
    std::cerr << "Can not cast the polymorphic object of type "
              << osssi::type_id::from< ObjectType >()
              << "to the specified target type "
              << osssi::type_id::from< TargetType >()
              << std::endl;
    OSSS_ABORT();
  }
  return( *( dynamic_cast< TargetType * >( m_objectPointer ) ) );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
const ObjectType &
osss_polymorphic< ObjectType >::cast_to_const () const
{
  return ( *m_objectPointer );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
ObjectType &
osss_polymorphic< ObjectType >::plugged_object()
{
  return ( *m_objectPointer );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
ObjectType *
osss_polymorphic< ObjectType >::operator->() const
{
  return( m_objectPointer );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
sc_core::sc_interface *
osss_polymorphic< ObjectType >::get_interface() const
{
  return ( dynamic_cast<sc_core::sc_interface*>( m_objectPointer ) );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
bool
osss_polymorphic< ObjectType >::operator==( const ObjectType & object ) const
{
  if ( !m_objectPointer )
  {
    std::cerr << "Can not compare, "
              << "the polymorphic object does not contain any object "
              << "(m_objectPointer is NULL)" << std::endl;
    OSSS_ABORT();
  }
  if ( typeid( *m_objectPointer ) != typeid( object ) )
  {
    return( false );
  }
  return( m_objectPointer->operator==( object ) );
}

//----------------------------------------------------------------------------//

template< class ObjectType >
inline
bool
osss_polymorphic< ObjectType >::
operator==( const osss_polymorphic< ObjectType > & object ) const
{
  if ( !object.m_objectPointer )
  {
    std::cerr << "Can not compare, "
              << "the source polymorphic object does not contain any object "
              << "(object.m_objectPointer is NULL)" << std::endl;
    OSSS_ABORT();
  }
  return( operator==( *( object.m_objectPointer ) ) );
}

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Binary operations on polymorphic content
//----------------------------------------------------------------------------//

#define OSSS_IMPLEMENT_POLYMORPHIC_ARITHMETICS( method, op )          \
  template< class ObjectType >                                        \
  template< class OtherType >                                         \
  osss_polymorphic< ObjectType >                                      \
  osss_polymorphic< ObjectType >:: method (                        \
        const osss_polymorphic< OtherType > & other) const            \
  {                                                                   \
    return osss_polymorphic< ObjectType >( (*this->m_objectPointer)   \
                                       op (*other.m_objectPointer) ); \
  }

OSSS_IMPLEMENT_POLYMORPHIC_ARITHMETICS( add, + )
OSSS_IMPLEMENT_POLYMORPHIC_ARITHMETICS( sub, - )
OSSS_IMPLEMENT_POLYMORPHIC_ARITHMETICS( mul, * )
OSSS_IMPLEMENT_POLYMORPHIC_ARITHMETICS( div, / )
OSSS_IMPLEMENT_POLYMORPHIC_ARITHMETICS( mod, % )

#undef OSSS_IMPLEMENT_POLYMORPHIC_ARITHMETICS

//----------------------------------------------------------------------------//

template< class ObjectType >
template< class OtherType >
bool
osss_polymorphic< ObjectType >::lt( const osss_polymorphic< OtherType > & other) const
{
  return (*this->m_objectPointer) < (*other.m_objectPointer);
}

//----------------------------------------------------------------------------//
// Additional stuff required by SystemC for proper functionality:
//----------------------------------------------------------------------------//

template< class T >
inline
::std::ostream &
operator<<( ::std::ostream & os, const osss_polymorphic< T > & object )
{
  os << object.cast_to_const();
  return( os );
}

} // namespace osss

//----------------------------------------------------------------------------//

namespace sc_core {

template< class T >
inline
void
sc_trace ( sc_trace_file * tr,
           const osss::osss_polymorphic< T > & object,
           const std::string & str )
{
  std::cerr << "Polymorphic objects can not be traced" << std::endl;
  OSSS_ABORT();
}

} // namespace sc_core

//----------------------------------------------------------------------------//
// Template functions for infix operators
//----------------------------------------------------------------------------//
namespace osss {

#define OSSS_IMPLEMENT_POLYMORPHIC_OPERATOR( op, method )       \
  template< class FirstType, class SecondType >                 \
  osss_polymorphic< FirstType >                                 \
  operator op ( const osss_polymorphic< FirstType > & first,    \
                const osss_polymorphic< SecondType > & second ) \
  { return first.method(second); }

OSSS_IMPLEMENT_POLYMORPHIC_OPERATOR( + , add )
OSSS_IMPLEMENT_POLYMORPHIC_OPERATOR( - , sub )
OSSS_IMPLEMENT_POLYMORPHIC_OPERATOR( * , mul )
OSSS_IMPLEMENT_POLYMORPHIC_OPERATOR( / , div )
OSSS_IMPLEMENT_POLYMORPHIC_OPERATOR( % , mod )

#undef OSSS_IMPLEMENT_POLYMORPHIC_OPERATOR

// lt (less than, <)
template< class FirstType, class SecondType >
bool
operator< ( const osss_polymorphic< FirstType >  & first,
            const osss_polymorphic< SecondType > & second)
{
  return first.lt(second);
}

} /* namespace osss */

//----------------------------------------------------------------------------//
// $Id: osss_polymorphic.tpp 3021 2009-03-04 22:55:21Z philipph $
// :flavour:(all,h)
