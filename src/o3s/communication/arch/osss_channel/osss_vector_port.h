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

#ifndef OSSS_VECTOR_PORT_H
#define OSSS_VECTOR_PORT_H

#include "o3s/utils/osss_systemc.h"

namespace osss {

// ----------------------------------------------------------------------------
//  CLASS : osss_vector_in<T>
//
// Similar to sc_in, but capable of handling multiple bindings
// ----------------------------------------------------------------------------

template<class T>
class osss_vector_in 
    : public sc_core::sc_port< sc_core::sc_signal_in_if<T>, 0 >
{
public:
  // typedefs

  typedef T                                      data_type;

  typedef sc_core::sc_signal_in_if<data_type>    if_type;
  typedef sc_core::sc_port<if_type,0>            base_type;
  typedef osss_vector_in<data_type>              this_type;

  typedef if_type                                in_if_type;
  typedef base_type                              in_port_type;
  typedef sc_core::sc_signal_inout_if<data_type> inout_if_type;
  typedef sc_core::sc_port<inout_if_type,0>      inout_port_type;

public:

  // constructors

  osss_vector_in()
    : base_type(), m_traces( 0 )
    {}

  explicit osss_vector_in( const char* name_ )
    : base_type( name_ ), m_traces( 0 )
    {}

  explicit osss_vector_in( const in_if_type& interface_ )
    : base_type( CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
    {}

  osss_vector_in( const char* name_, const in_if_type& interface_ )
    : base_type( name_, CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
    {}

  explicit osss_vector_in( in_port_type& parent_ )
    : base_type( parent_ ), m_traces( 0 )
    {}

  osss_vector_in( const char* name_, in_port_type& parent_ )
    : base_type( name_, parent_ ), m_traces( 0 )
    {}

  explicit osss_vector_in( inout_port_type& parent_ )
    : base_type(), m_traces( 0 )
    { sc_core::sc_port_base::bind( parent_ ); }

  osss_vector_in( const char* name_, inout_port_type& parent_ )
    : base_type( name_ ), m_traces( 0 )
    { sc_core::sc_port_base::bind( parent_ ); }

  explicit osss_vector_in( this_type& parent_ )
    : base_type( parent_ ), m_traces( 0 )
    {}

  osss_vector_in( const char* name_, this_type& parent_ )
    : base_type( name_, parent_ ), m_traces( 0 )
    {}


  // destructor

  virtual ~osss_vector_in()
    { remove_traces(); }


  // bind to in interface

  void bind( const in_if_type& interface_ )
    { sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

  void operator () ( const in_if_type& interface_ )
    { sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


  // bind to parent in port

  void bind( in_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }

  void operator () ( in_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }


  // bind to parent inout port

  void bind( inout_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }

  void operator () ( inout_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }


  // interface access shortcut methods

  // get the default event

  /*
  const sc_core::sc_event& default_event() const
  { return (*this)->default_event(); }
  */

  /*
  const std::vector< sc_core::sc_event& >& default_event() const
  {
    std::vector< sc_core::sc_event& > event_vector = 
      new std::vector< sc_core::sc_event& >();

    for (int i=0;i<size(); i++)
      event_vector->push_back((*this)[i]->default_event());

    return *event_vector;
  }
  */

  const sc_core::sc_event& default_event(unsigned int i) const
    { return (*this)[i]->default_event(); }

  // get the value changed event
  
  /*
  const sc_core::sc_event& value_changed_event() const
    { return (*this)->value_changed_event(); }
  */

  /*
  const std::vector< sc_core::sc_event& >& value_changed_event() const
  {
    std::vector< sc_core::sc_event& > event_vector = 
      new std::vector< sc_core::sc_event& >();

    for (int i=0;i<size(); i++)
      event_vector->push_back((*this)[i]->value_changed_event());

    return *event_vector;
  }
  */

  const sc_core::sc_event& value_changed_event(unsigned int i) const
    { return (*this)[i]->value_changed_event(); }


  // read the current value

  /*
  const data_type& read() const
    { return (*this)->read(); }
  */

  const data_type& read(unsigned int i) const
    { return (*this)[i]->read(); }

  /*
  operator const data_type& () const
    { return (*this)->read(); }
  */


  // was there a value changed event?

  bool event(unsigned int i) const
    { return (*this)[i]->event(); }

  bool event() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->event();

    return val_changed; 
  }
  
  // (other) event finder method(s)

  /*
  sc_core::sc_event_finder& value_changed() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::value_changed_event );
    }
  */


  // called when elaboration is done
  /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
  /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

  virtual void end_of_elaboration();

  virtual const char* kind() const
    { return "osss_vector_in"; }


  // called by sc_core::sc_trace
  void add_trace( sc_core::sc_trace_file*, const std::string& ) const;

protected:

  void remove_traces() const;

  mutable sc_core::sc_trace_params_vec* m_traces;

protected:

  // called by pbind (for internal use only)
  virtual int vbind( sc_core::sc_interface& );
  virtual int vbind( sc_core::sc_port_base& );

private:

  // disabled
  osss_vector_in( const this_type& );
  this_type& operator = ( const this_type& );

#ifdef __GNUC__
  // Needed to circumvent a problem in the g++-2.95.2 compiler:
  // This unused variable forces the compiler to instantiate
  // an object of T template so an implicit conversion from
  // read() to a C++ intrinsic data type will work.
  static data_type dummy;
#endif
};

template<typename T>
inline
::std::ostream& operator << ( ::std::ostream& os, const osss_vector_in<T>& a )
{
  for (int i=0; i<a.size(); i++)
  {
    os << i << ": " << a[i].read() << ::std::endl;
  }

  return os;
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// called when elaboration is done

template <class T>
inline
void
osss_vector_in<T>::end_of_elaboration()
{
  if( m_traces != 0 ) {
    for( sc_core::sc_trace_params_vec::iterator i = m_traces->begin(); 
         i != m_traces->end(); i++ ) {
      sc_core::sc_trace_params* p = (*i);
      for (int j=0; j<base_type::size(); j++)
      {
	in_if_type* iface = DCAST<in_if_type*>( this->get_interface(j) );
	std::ostringstream ostr;
	ostr << p->name << j;
	sc_core::sc_trace( p->tf, iface->read(), ostr.str() );
      }
    }
    remove_traces();
  }
}


// called by sc_core::sc_trace

template <class T>
inline
void
osss_vector_in<T>::add_trace( sc_core::sc_trace_file* tf_, const std::string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_core::sc_trace_params_vec;
	}
	m_traces->push_back( new sc_core::sc_trace_params( tf_, name_ ) );
    }
}

template <class T>
inline
void
osss_vector_in<T>::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// called by pbind (for internal use only)

template <class T>
inline
int
osss_vector_in<T>::vbind( sc_core::sc_interface& interface_ )
{
    return sc_core::sc_port_b<if_type>::vbind( interface_ );
}

template <class T>
inline
int
osss_vector_in<T>::vbind( sc_core::sc_port_base& parent_ )
{
    in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_core::sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_core::sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}

// ----------------------------------------------------------------------------
//  CLASS : osss_vector_in<bool>
//
//  Specialization of osss_vector_in<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class osss_vector_in<bool>
    : public sc_core::sc_port<sc_core::sc_signal_in_if<bool>,0> 
{
public:

  // typedefs

  typedef bool                                   data_type;

  typedef sc_core::sc_signal_in_if<data_type>    if_type;
  typedef sc_core::sc_port<if_type,0>            base_type;
  typedef osss_vector_in<data_type>              this_type;

  typedef if_type                                in_if_type;
  typedef base_type                              in_port_type;
  typedef sc_core::sc_signal_inout_if<data_type> inout_if_type;
  typedef sc_core::sc_port<inout_if_type,0>      inout_port_type;

public:

  // constructors

  osss_vector_in()
    : base_type(), m_traces( 0 )
    {}

  explicit osss_vector_in( const char* name_ )
    : base_type( name_ ), m_traces( 0 )
    {}

  explicit osss_vector_in( const in_if_type& interface_ )
    : base_type( CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
    {}

  osss_vector_in( const char* name_, const in_if_type& interface_ )
    : base_type( name_, CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
    {}

  explicit osss_vector_in( in_port_type& parent_ )
    : base_type( parent_ ), m_traces( 0 )
    {}

  osss_vector_in( const char* name_, in_port_type& parent_ )
    : base_type( name_, parent_ ), m_traces( 0 )
    {}

  explicit osss_vector_in( inout_port_type& parent_ )
    : base_type(), m_traces( 0 )
    { sc_core::sc_port_base::bind( parent_ ); }

  osss_vector_in( const char* name_, inout_port_type& parent_ )
    : base_type( name_ ), m_traces( 0 )
    { sc_core::sc_port_base::bind( parent_ ); }

  explicit osss_vector_in( this_type& parent_ )
    : base_type( parent_ ), m_traces( 0 )
    {}
  
  osss_vector_in( const char* name_, this_type& parent_ )
    : base_type( name_, parent_ ), m_traces( 0 )
    {}


  // destructor

  virtual ~osss_vector_in()
    { remove_traces(); }


  // bind to in interface

  void bind( const in_if_type& interface_ )
    { sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

  void operator () ( const in_if_type& interface_ )
    { sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


  // bind to parent in port

  void bind( in_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }

  void operator () ( in_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }


  // bind to parent inout port

  void bind( inout_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }

  void operator () ( inout_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }


  // interface access shortcut methods

  // get the default event

  /*
  const sc_core::sc_event& default_event() const
    { return (*this)->default_event(); }
  */

  const sc_core::sc_event& default_event(unsigned int i) const
    { return (*this)[i]->default_event(); }


  // get the value changed event

  /*
  const sc_core::sc_event& value_changed_event() const
    { return (*this)->value_changed_event(); }
  */

  const sc_core::sc_event& value_changed_event(unsigned int i) const
    { return (*this)[i]->value_changed_event(); }

  // get the positive edge event

  /*
  const sc_core::sc_event& posedge_event() const
    { return (*this)->posedge_event(); }
  */

  const sc_core::sc_event& posedge_event(unsigned int i) const
    { return (*this)[i]->posedge_event(); }

  // get the negative edge event

  /*
  const sc_core::sc_event& negedge_event() const
    { return (*this)->negedge_event(); }
  */

  const sc_core::sc_event& negedge_event(unsigned int i) const
    { return (*this)[i]->negedge_event(); }

  // read the current value

  /*
  const data_type& read() const
    { return (*this)->read(); }
  */

  const data_type& read(unsigned int i)
    { return (*this)[i]->read(); }

  /*
  operator const data_type& () const
    { return (*this)->read(); }
  */


  // use for positive edge sensitivity

  /*
  sc_core::sc_event_finder& pos() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::posedge_event );
    }
  */

  // use for negative edge sensitivity

  /*
  sc_core::sc_event_finder& neg() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::negedge_event );
    }
  */


  // was there a value changed event?

  bool event(unsigned int i) const
    { return (*this)[i]->event(); }

  bool event() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->event();

    return val_changed; 
  }

  // was there a positive edge event?

  bool posedge(unsigned int i) const
    { return (*this)[i]->posedge(); }

  bool posedge() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->posedge();

    return val_changed; 
  }

  // was there a negative edge event?

  bool negedge(unsigned int i) const
    { return (*this)[i]->negedge(); }

  bool negedge() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->negedge();

    return val_changed; 
  }


  // delayed evaluation
  const sc_core::sc_signal_bool_deval& delayed(unsigned int i) const;

  // (other) event finder method(s)

  /*
  sc_core::sc_event_finder& value_changed() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::value_changed_event );
    }
  */


  // called when elaboration is done
  /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
  /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

  virtual void end_of_elaboration();

  virtual const char* kind() const
    { return "osss_vector_in"; }


  // called by sc_core::sc_trace
  void add_trace( sc_core::sc_trace_file*, const std::string& ) const;

protected:

  void remove_traces() const;

  mutable sc_core::sc_trace_params_vec* m_traces;

protected:

  // called by pbind (for internal use only)
  virtual int vbind( sc_core::sc_interface& );
  virtual int vbind( sc_core::sc_port_base& );

private:

  osss_vector_in( const this_type& );
  this_type& operator = ( const this_type& );

#ifdef __GNUC__
  // Needed to circumvent a problem in the g++-2.95.2 compiler:
  // This unused variable forces the compiler to instantiate
  // an object of T template so an implicit conversion from
  // read() to a C++ intrinsic data type will work.
  static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_core::sc_signal_bool_deval&
osss_vector_in<bool>::delayed(unsigned int i) const
{
  const in_if_type* iface = DCAST<const in_if_type*>( get_interface(i) );
  if( iface != 0 ) {
    return RCAST<const sc_core::sc_signal_bool_deval&>( *iface );
  } else {
    // the tricky part
    const sc_core::sc_port_base* pb = this;
    return RCAST<const sc_core::sc_signal_bool_deval&>( *pb );
  }
}

// called when elaboration is done

inline
void
osss_vector_in<bool>::end_of_elaboration()
{
  if( m_traces != 0 ) {
    for( sc_core::sc_trace_params_vec::iterator i = m_traces->begin(); 
         i != m_traces->end(); i++ ) {
      sc_core::sc_trace_params* p = (*i);
      for (int j=0; j<base_type::size(); j++)
      {
	in_if_type* iface = DCAST<in_if_type*>( get_interface(j) );
	std::ostringstream ostr;
	ostr << p->name << j;
	sc_core::sc_trace( p->tf, iface->read(), ostr.str() );
      }
    }
    remove_traces();
  }
}

// called by sc_core::sc_trace

inline
void
osss_vector_in<bool>::add_trace(sc_core::sc_trace_file* tf_, 
				const std::string& name_) const
{
  if( tf_ != 0 ) {
    if( m_traces == 0 ) {
      m_traces = new sc_core::sc_trace_params_vec;
    }
    m_traces->push_back( new sc_core::sc_trace_params( tf_, name_ ) );
  }
}

inline
void
osss_vector_in<bool>::remove_traces() const
{
  if( m_traces != 0 ) {
    for( int i = m_traces->size() - 1; i >= 0; -- i ) {
      delete (*m_traces)[i];
    }
    delete m_traces;
    m_traces = 0;
  }
}


// called by pbind (for internal use only)

inline
int
osss_vector_in<bool>::vbind( sc_core::sc_interface& interface_ )
{
  return sc_core::sc_port_b<if_type>::vbind( interface_ );
}

inline
int
osss_vector_in<bool>::vbind( sc_core::sc_port_base& parent_ )
{
  in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
  if( in_parent != 0 ) {
    sc_core::sc_port_base::bind( *in_parent );
    return 0;
  }
  inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
  if( inout_parent != 0 ) {
    sc_core::sc_port_base::bind( *inout_parent );
    return 0;
  }
  // type mismatch
  return 2;
}

// ----------------------------------------------------------------------------
//  CLASS : osss_vector_in<sc_dt::sc_logic>
//
//  Specialization of osss_vector_in<T> for type sc_dt::sc_logic.
// ----------------------------------------------------------------------------

template <>
class osss_vector_in<sc_dt::sc_logic>
: public sc_core::sc_port<sc_core::sc_signal_in_if<sc_dt::sc_logic>,0>
{
public:

  // typedefs

  typedef sc_dt::sc_logic                        data_type;

  typedef sc_core::sc_signal_in_if<data_type>    if_type;
  typedef sc_core::sc_port<if_type,0>            base_type;
  typedef osss_vector_in<data_type>              this_type;

  typedef if_type                                in_if_type;
  typedef base_type                              in_port_type;
  typedef sc_core::sc_signal_inout_if<data_type> inout_if_type;
  typedef sc_core::sc_port<inout_if_type,0>      inout_port_type;

public:

  // constructors

  osss_vector_in()
    : base_type(), m_traces( 0 )
    {}

  explicit osss_vector_in( const char* name_ )
    : base_type( name_ ), m_traces( 0 )
    {}

  explicit osss_vector_in( const in_if_type& interface_ )
    : base_type( CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
    {}

  osss_vector_in( const char* name_, const in_if_type& interface_ )
    : base_type( name_, CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
    {}

  explicit osss_vector_in( in_port_type& parent_ )
    : base_type( parent_ ), m_traces( 0 )
    {}

  osss_vector_in( const char* name_, in_port_type& parent_ )
    : base_type( name_, parent_ ), m_traces( 0 )
    {}

  explicit osss_vector_in( inout_port_type& parent_ )
    : base_type(), m_traces( 0 )
    { sc_core::sc_port_base::bind( parent_ ); }

  osss_vector_in( const char* name_, inout_port_type& parent_ )
    : base_type( name_ ), m_traces( 0 )
    { sc_core::sc_port_base::bind( parent_ ); }

  explicit osss_vector_in( this_type& parent_ )
    : base_type( parent_ ), m_traces( 0 )
    {}

  osss_vector_in( const char* name_, this_type& parent_ )
    : base_type( name_, parent_ ), m_traces( 0 )
    {}


  // destructor

  virtual ~osss_vector_in()
    { remove_traces(); }


  // bind to in interface

  void bind( const in_if_type& interface_ )
    { sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

  void operator () ( const in_if_type& interface_ )
    { sc_core::sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


  // bind to parent in port

  void bind( in_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }

  void operator () ( in_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }


  // bind to parent inout port

  void bind( inout_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }

  void operator () ( inout_port_type& parent_ )
    { sc_core::sc_port_base::bind( parent_ ); }


  // interface access shortcut methods

  // get the default event

  /*
  const sc_core::sc_event& default_event() const
    { return (*this)->default_event(); }
  */

  const sc_core::sc_event& default_event(unsigned int i) const
    { return (*this)[i]->default_event(); }


  // get the value changed event

  /*
  const sc_core::sc_event& value_changed_event() const
    { return (*this)->value_changed_event(); }
  */

  const sc_core::sc_event& value_changed_event(unsigned int i) const
    { return (*this)[i]->value_changed_event(); }

  // get the positive edge event

  /*
  const sc_core::sc_event& posedge_event() const
    { return (*this)->posedge_event(); }
  */

  const sc_core::sc_event& posedge_event(unsigned int i) const
    { return (*this)[i]->posedge_event(); }

  // get the negative edge event

  /*
  const sc_core::sc_event& negedge_event() const
    { return (*this)->negedge_event(); }
  */

  const sc_core::sc_event& negedge_event(unsigned int i) const
    { return (*this)[i]->negedge_event();}


  // read the current value
  
  /*
  const data_type& read() const
    { return (*this)->read(); }
  */

  const data_type& read(unsigned int i) const
    { return (*this)[i]->read(); }

  /*
  operator const data_type& () const
    { return (*this)->read(); }
  */


  // use for positive edge sensitivity

  /*
  sc_core::sc_event_finder& pos() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::posedge_event );
    }
  */

  // use for negative edge sensitivity

  /*
  sc_core::sc_event_finder& neg() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::negedge_event );
    }
  */


  // was there a value changed event?

  bool event(unsigned int i) const
    { return (*this)[i]->event(); }

  bool event() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->event();

    return val_changed; 
  }

  // was there a positive edge event?

  bool posedge(unsigned int i) const
    { return (*this)[i]->posedge(); }

  bool posedge() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->posedge();

    return val_changed; 
  }

  // was there a negative edge event?

  bool negedge(unsigned int i) const
    { return (*this)[i]->negedge(); }

  bool negedge() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->negedge();

    return val_changed; 
  }

  // delayed evaluation
  const sc_core::sc_signal_logic_deval& delayed(unsigned int i) const;


  // (other) event finder method(s)

  /*
  sc_core::sc_event_finder& value_changed() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::value_changed_event );
    }
  */


  // called when elaboration is done
  /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
  /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

  virtual void end_of_elaboration();

  virtual const char* kind() const
    { return "sc_in"; }


  // called by sc_core::sc_trace
  void add_trace( sc_core::sc_trace_file*, const std::string& ) const;

protected:

  void remove_traces() const;

  mutable sc_core::sc_trace_params_vec* m_traces;

protected:

  // called by pbind (for internal use only)
  virtual int vbind( sc_core::sc_interface& );
  virtual int vbind( sc_core::sc_port_base& );

private:

  // disabled
  osss_vector_in( const this_type& );
  this_type& operator = ( const this_type& );

#ifdef __GNUC__
  // Needed to circumvent a problem in the g++-2.95.2 compiler:
  // This unused variable forces the compiler to instantiate
  // an object of T template so an implicit conversion from
  // read() to a C++ intrinsic data type will work.
  static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_core::sc_signal_logic_deval&
osss_vector_in<sc_dt::sc_logic>::delayed(unsigned int i) const
{
  const in_if_type* iface = DCAST<const in_if_type*>( get_interface(i) );
  if( iface != 0 ) {
    return RCAST<const sc_core::sc_signal_logic_deval&>( *iface );
  } else {
    // the tricky part
    const sc_core::sc_port_base* pb = this;
    return RCAST<const sc_core::sc_signal_logic_deval&>( *pb );
  }
}

// called when elaboration is done

inline
void
osss_vector_in<sc_dt::sc_logic>::end_of_elaboration()
{
  if( m_traces != 0 ) {
    for( sc_core::sc_trace_params_vec::iterator i = m_traces->begin(); 
         i != m_traces->end(); i++ ) {
      sc_core::sc_trace_params* p = (*i);
      for (int j=0; j<base_type::size(); j++)
      {
	in_if_type* iface = DCAST<in_if_type*>( get_interface(j) );
	std::ostringstream ostr;
	ostr << p->name << j;
	sc_core::sc_trace( p->tf, iface->read(), ostr.str() );
      }
    }
    remove_traces();
  }
}


// called by sc_core::sc_trace

inline
void
osss_vector_in<sc_dt::sc_logic>::add_trace( sc_core::sc_trace_file* tf_, 
					    const std::string& name_ ) const
{
  if( tf_ != 0 ) {
    if( m_traces == 0 ) {
      m_traces = new sc_core::sc_trace_params_vec;
    }
    m_traces->push_back( new sc_core::sc_trace_params( tf_, name_ ) );
  }
}

inline
void
osss_vector_in<sc_dt::sc_logic>::remove_traces() const
{
  if( m_traces != 0 ) {
    for( int i = m_traces->size() - 1; i >= 0; -- i ) {
      delete (*m_traces)[i];
    }
    delete m_traces;
    m_traces = 0;
  }
}


// called by pbind (for internal use only)

inline
int
osss_vector_in<sc_dt::sc_logic>::vbind( sc_core::sc_interface& interface_ )
{
  return sc_core::sc_port_b<if_type>::vbind( interface_ );
}

inline
int
osss_vector_in<sc_dt::sc_logic>::vbind( sc_core::sc_port_base& parent_ )
{
  in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
  if( in_parent != 0 ) {
    sc_core::sc_port_base::bind( *in_parent );
    return 0;
  }
  inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
  if( inout_parent != 0 ) {
    sc_core::sc_port_base::bind( *inout_parent );
    return 0;
  }
  // type mismatch
  return 2;
}

// ----------------------------------------------------------------------------
//  CLASS : osss_vector_inout<T>
//
// ----------------------------------------------------------------------------

template <class T>
class osss_vector_inout
  : public sc_core::sc_port<sc_core::sc_signal_inout_if<T>,0>
{
public:

  // typedefs

  typedef T                             data_type;

  typedef sc_core::sc_signal_inout_if<data_type> if_type;
  typedef sc_core::sc_port<if_type,0>            base_type;
  typedef osss_vector_inout<data_type>  this_type;

  typedef sc_core::sc_signal_in_if<data_type>    in_if_type;
  typedef sc_core::sc_port<in_if_type,0>         in_port_type;
  typedef if_type                       inout_if_type;
  typedef base_type                     inout_port_type;

public:

  // constructors

  osss_vector_inout()
    : base_type(), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( const char* name_ )
    : base_type( name_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( inout_if_type& interface_ )
    : base_type( interface_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  osss_vector_inout( const char* name_, inout_if_type& interface_ )
    : base_type( name_, interface_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( inout_port_type& parent_ )
    : base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  osss_vector_inout( const char* name_, inout_port_type& parent_ )
    : base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( this_type& parent_ )
    : base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  osss_vector_inout( const char* name_, this_type& parent_ )
    : base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}


  // destructor

  virtual ~osss_vector_inout();


  // interface access shortcut methods

  // get the default event

  /*
  const sc_core::sc_event& default_event() const
    { return (*this)->default_event(); }
  */

  const sc_core::sc_event& default_event(unsigned int i) const
    { return (*this)[i]->default_event(); }


  // get the value changed event

  /*
  const sc_core::sc_event& value_changed_event() const
    { return (*this)->value_changed_event(); }
  */

  const sc_core::sc_event& value_changed_event(unsigned int i) const
    { return (*this)[i]->value_changed_event(); }

  // read the current value

  /*
  const data_type& read() const
    { return (*this)->read(); }
  */

  const data_type& read(unsigned int i) const
    { return (*this)[i]->read(); }

  /*
  operator const data_type& () const
    { return (*this)->read(); }
  */


  // was there a value changed event?

  bool event(unsigned int i) const
    { return (*this)[i]->event(); }

  bool event() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->event();

    return val_changed; 
  }


  // write the new value

  void write( const data_type& value_ )
  { 
    for(int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( value_ ); 
    }
  }

  void write( const unsigned int pos, const data_type& value_ )
  {
    (*this)[pos]->write( value_ );
  }

  this_type& operator = ( const data_type& value_ )
  { 
    for (int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( value_ ); 
    }
    return *this;
  }

  this_type& operator = ( const in_if_type& interface_ )
  { 
    for (int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( interface_.read() );
    }
    return *this;
  }

  this_type& operator = ( const in_port_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this; 
  }

  this_type& operator = ( const inout_port_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this;     
  }

  this_type& operator = ( const this_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this; 
  }


  // set initial value (can also be called when port is not bound yet)

  void initialize( const data_type& value_ );

  void initialize( const in_if_type& interface_ )
    { initialize( interface_.read() ); }


  // called when elaboration is done
  /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
  /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

  virtual void end_of_elaboration();


  // (other) event finder method(s)

  /*
  sc_core::sc_event_finder& value_changed() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::value_changed_event );
    
    }
  */

  virtual const char* kind() const
    { return "osss_vector_inout"; }

protected:

  data_type* m_init_val;

public:

  // called by sc_core::sc_trace
  void add_trace( sc_core::sc_trace_file*, const std::string& ) const;

protected:

  void remove_traces() const;

  mutable sc_core::sc_trace_params_vec* m_traces;

private:

  // disabled
  osss_vector_inout( const this_type& );

#ifdef __GNUC__
  // Needed to circumvent a problem in the g++-2.95.2 compiler:
  // This unused variable forces the compiler to instantiate
  // an object of T template so an implicit conversion from
  // read() to a C++ intrinsic data type will work.
  static data_type dummy;
#endif
};

template<typename T>
::std::ostream& operator << ( ::std::ostream& os, const osss_vector_inout<T>& a )
{
  for (int i=0; i<a.size(); i++)
  {
    os << i << ": " << a[i].read() << ::std::endl;
  }

  return os;
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// destructor

template <class T>
inline
osss_vector_inout<T>::~osss_vector_inout()
{
  if( m_init_val != 0 ) {
    delete m_init_val;
  }
  remove_traces();
}


// set initial value (can also be called when port is not bound yet)

template <class T>
inline
void
osss_vector_inout<T>::initialize( const data_type& value_ )
{
  for (int i=0; i<base_type::size(); i++)
  {
    inout_if_type* iface = DCAST<inout_if_type*>( this->get_interface(i) );
    if( iface != 0 ) {
      iface->write( value_ );
    } else {
      if( m_init_val == 0 ) {
	m_init_val = new data_type;
      }
      *m_init_val = value_;
    }
  }
}


// called when elaboration is done

template <class T>
inline
void
osss_vector_inout<T>::end_of_elaboration()
{
  if( m_init_val != 0 ) {
    write( *m_init_val );
    delete m_init_val;
    m_init_val = 0;
  }
  if( m_traces != 0 ) {
    for( sc_core::sc_trace_params_vec::iterator i = m_traces->begin(); 
         i != m_traces->end(); i++ ) 
    {
      sc_core::sc_trace_params* p = (*i);
      for (int j=0; j<base_type::size(); j++)
      {
        in_if_type* iface = DCAST<in_if_type*>( this->get_interface(j) );
        std::ostringstream ostr;
        ostr << p->name << j;
        sc_core::sc_trace( p->tf, iface->read(), ostr.str() );
      }
    }
    remove_traces();
  }
}


// called by sc_core::sc_trace

template <class T>
inline
void
osss_vector_inout<T>::add_trace( sc_core::sc_trace_file* tf_,
                                 const std::string& name_) const
{
  if( tf_ != 0 ) {
    if( m_traces == 0 ) {
      m_traces = new sc_core::sc_trace_params_vec;
    }
    m_traces->push_back( new sc_core::sc_trace_params( tf_, name_ ) );
  }
}

template <class T>
inline
void
osss_vector_inout<T>::remove_traces() const
{
  if( m_traces != 0 ) {
    for( int i = m_traces->size() - 1; i >= 0; -- i ) {
      delete (*m_traces)[i];
    }
    delete m_traces;
    m_traces = 0;
  }
}



// ----------------------------------------------------------------------------
//  CLASS : osss_vector_inout<bool>
//
//  Specialization of osss_vector_inout<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class osss_vector_inout<bool>
  : public sc_core::sc_port<sc_core::sc_signal_inout_if<bool>,0>
{
public:

  // typedefs

  typedef bool                                   data_type;

  typedef sc_core::sc_signal_inout_if<data_type> if_type;
  typedef sc_core::sc_port<if_type,0>            base_type;
  typedef osss_vector_inout<data_type>           this_type;

  typedef sc_core::sc_signal_in_if<data_type>    in_if_type;
  typedef sc_core::sc_port<in_if_type,0>         in_port_type;
  typedef if_type                                inout_if_type;
  typedef base_type                              inout_port_type;

public:

  // constructors

  osss_vector_inout()
    : base_type(), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( const char* name_ )
    : base_type( name_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( inout_if_type& interface_ )
    : base_type( interface_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  osss_vector_inout( const char* name_, inout_if_type& interface_ )
    : base_type( name_, interface_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( inout_port_type& parent_ )
    : base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  osss_vector_inout( const char* name_, inout_port_type& parent_ )
    : base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( this_type& parent_ )
    : base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  osss_vector_inout( const char* name_, this_type& parent_ )
    : base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}


  // destructor

  virtual ~osss_vector_inout();


  // interface access shortcut methods

  // get the default event

  /*
  const sc_core::sc_event& default_event() const
    { return (*this)->default_event(); }
  */

  const sc_core::sc_event& default_event(unsigned int i) const
    { return (*this)[i]->default_event(); }


  // get the value changed event

  /*
  const sc_core::sc_event& value_changed_event() const
    { return (*this)->value_changed_event(); }
  */

  const sc_core::sc_event& value_changed_event(unsigned int i) const
    { return (*this)[i]->value_changed_event(); }

  // get the positive edge event

  /*
  const sc_core::sc_event& posedge_event() const
    { return (*this)->posedge_event(); }
  */

  const sc_core::sc_event& posedge_event(unsigned int i) const
    { return (*this)[i]->posedge_event(); }

  // get the negative edge event

  /*
  const sc_core::sc_event& negedge_event() const
    { return (*this)->negedge_event(); }
  */

  const sc_core::sc_event& negedge_event(unsigned int i) const
    { return (*this)[i]->negedge_event(); }

  // read the current value

  /*
  const data_type& read() const
    { return (*this)->read(); }
  */

  const data_type& read(unsigned int i) const
    { return (*this)[i]->read(); }

  /*
  operator const data_type& () const
    { return (*this)->read(); }
  */


  // use for positive edge sensitivity

  /*
  sc_core::sc_event_finder& pos() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::posedge_event );
    }
  */

  // use for negative edge sensitivity

  /*
  sc_core::sc_event_finder& neg() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::negedge_event );
    }
  */


  // was there a value changed event?

  bool event(unsigned int i) const
    { return (*this)[i]->event(); }

  bool event() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->event();

    return val_changed; 
  }

  // was there a positive edge event?

  bool posedge(unsigned int i) const
    { return (*this)[i]->posedge(); }
  
  bool posedge() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->posedge();

    return val_changed; 
  }

  // was there a negative edge event?

  bool negedge(unsigned int i) const
    { return (*this)[i]->negedge(); }

  bool negedge() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->negedge();

    return val_changed; 
  }


  // delayed evaluation
  const sc_core::sc_signal_bool_deval& delayed(unsigned int i) const;

  // write the new value

  void write( const data_type& value_ )
  { 
    for(int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( value_ ); 
    }
  }

  void write( const unsigned int pos, const data_type& value_ )
  {
    (*this)[pos]->write( value_ );
  }

  this_type& operator = ( const data_type& value_ )
  { 
    for (int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( value_ ); 
    }
    return *this;
  }

  this_type& operator = ( const in_if_type& interface_ )
  { 
    for (int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( interface_.read() );
    }
    return *this;
  }

  this_type& operator = ( const in_port_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this; 
  }

  this_type& operator = ( const inout_port_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this;     
  }

  this_type& operator = ( const this_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this; 
  }


  // set initial value (can also be called when port is not bound yet)

  void initialize( const data_type& value_ );

  void initialize( const in_if_type& interface_ )
    { initialize( interface_.read() ); }


  // called when elaboration is done
  /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
  /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

  virtual void end_of_elaboration();


  // (other) event finder method(s)

  /*
  sc_core::sc_event_finder& value_changed() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::value_changed_event );
    }
  */

  virtual const char* kind() const
    { return "osss_vector_inout"; }

protected:

  data_type* m_init_val;

public:

  // called by sc_core::sc_trace
  void add_trace( sc_core::sc_trace_file*, const std::string& ) const;

protected:

  void remove_traces() const;

  mutable sc_core::sc_trace_params_vec* m_traces;

private:

  // disabled
  osss_vector_inout( const this_type& );

#ifdef __GNUC__
  // Needed to circumvent a problem in the g++-2.95.2 compiler:
  // This unused variable forces the compiler to instantiate
  // an object of T template so an implicit conversion from
  // read() to a C++ intrinsic data type will work.
  static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_core::sc_signal_bool_deval&
osss_vector_inout<bool>::delayed(unsigned int i) const
{
  const in_if_type* iface = DCAST<const in_if_type*>( get_interface(i) );
  if( iface != 0 ) {
    return RCAST<const sc_core::sc_signal_bool_deval&>( *iface );
  } else {
    // the tricky part
    const sc_core::sc_port_base* pb = this;
    return RCAST<const sc_core::sc_signal_bool_deval&>( *pb );
  }
}

// destructor

inline
osss_vector_inout<bool>::~osss_vector_inout()
{
  if( m_init_val != 0 ) {
    delete m_init_val;
  }
  remove_traces();
}


// set initial value (can also be called when port is not bound yet)

inline
void
osss_vector_inout<bool>::initialize( const data_type& value_ )
{
  for (int i=0; i<base_type::size(); i++)
  {
    inout_if_type* iface = DCAST<inout_if_type*>( get_interface(i) );
    if( iface != 0 ) {
      iface->write( value_ );
    } else {
      if( m_init_val == 0 ) {
	m_init_val = new data_type;
      }
      *m_init_val = value_;
    }
  }
}


// called when elaboration is done

inline
void
osss_vector_inout<bool>::end_of_elaboration()
{
  if( m_init_val != 0 ) {
    write( *m_init_val );
    delete m_init_val;
    m_init_val = 0;
  }
  if( m_traces != 0 ) {
    for( sc_core::sc_trace_params_vec::iterator i = m_traces->begin(); 
         i != m_traces->end(); i++ ) 
    {
      sc_core::sc_trace_params* p = (*i);
      for (int j=0; j<base_type::size(); j++)
      {
	in_if_type* iface = DCAST<in_if_type*>( get_interface(j) );
	std::ostringstream ostr;
	ostr << p->name << j;
	sc_core::sc_trace( p->tf, iface->read(), ostr.str() );
      }
    }
    remove_traces();
  }
}

// called by sc_trace

inline
void
osss_vector_inout<bool>::add_trace( sc_core::sc_trace_file* tf_, 
				    const std::string& name_ ) const
{
  if( tf_ != 0 ) {
    if( m_traces == 0 ) {
      m_traces = new sc_core::sc_trace_params_vec;
    }
    m_traces->push_back( new sc_core::sc_trace_params( tf_, name_ ) );
  }
}

inline
void
osss_vector_inout<bool>::remove_traces() const
{
  if( m_traces != 0 ) {
    for( int i = m_traces->size() - 1; i >= 0; -- i ) {
      delete (*m_traces)[i];
    }
    delete m_traces;
    m_traces = 0;
  }
}



// ----------------------------------------------------------------------------
//  CLASS : osss_vector_inout<sc_dt::sc_logic>
//
//  Specialization of osss_vector_inout<T> for type sc_dt::sc_logic.
// ----------------------------------------------------------------------------

template <>
class osss_vector_inout<sc_dt::sc_logic>
  : public sc_core::sc_port<sc_core::sc_signal_inout_if<sc_dt::sc_logic>,0>
{
public:

  // typedefs

  typedef sc_dt::sc_logic                        data_type;

  typedef sc_core::sc_signal_inout_if<data_type> if_type;
  typedef sc_core::sc_port<if_type,0>            base_type;
  typedef osss_vector_inout<data_type>           this_type;

  typedef sc_core::sc_signal_in_if<data_type>    in_if_type;
  typedef sc_core::sc_port<in_if_type,0>         in_port_type;
  typedef if_type                                inout_if_type;
  typedef base_type                              inout_port_type;

public:

  // constructors

  osss_vector_inout()
    : base_type(), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( const char* name_ )
    : base_type( name_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( inout_if_type& interface_ )
    : base_type( interface_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  osss_vector_inout( const char* name_, inout_if_type& interface_ )
    : base_type( name_, interface_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( inout_port_type& parent_ )
    : base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  osss_vector_inout( const char* name_, inout_port_type& parent_ )
    : base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  explicit osss_vector_inout( this_type& parent_ )
    : base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}

  osss_vector_inout( const char* name_, this_type& parent_ )
    : base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
    {}


  // destructor

  virtual ~osss_vector_inout();


  // interface access shortcut methods

  // get the default event
  
  /*
  const sc_core::sc_event& default_event() const
    { return (*this)->default_event(); }
  */

  const sc_core::sc_event& default_event(unsigned int i) const
    { return (*this)[i]->default_event(); }


  // get the value changed event

  /*
  const sc_core::sc_event& value_changed_event() const
    { return (*this)->value_changed_event(); }
  */

  const sc_core::sc_event& value_changed_event(unsigned int i) const
    { return (*this)[i]->value_changed_event(); }

  // get the positive edge event

  /*
  const sc_core::sc_event& posedge_event() const
    { return (*this)->posedge_event(); }
  */

  const sc_core::sc_event& posedge_event(unsigned int i) const
    { return (*this)[i]->posedge_event();}

  // get the negative edge event

  /*
  const sc_core::sc_event& negedge_event() const
    { return (*this)->negedge_event(); }
  */

  const sc_core::sc_event& negedge_event(unsigned int i) const
    { return (*this)[i]->negedge_event(); }


  // read the current value

  /*
  const data_type& read() const
    { return (*this)->read(); }
  */

  const data_type& read(unsigned int i) const
    { return (*this)[i]->read(); }

  /*
  operator const data_type& () const
    { return (*this)->read(); }
  */


  // use for positive edge sensitivity

  /*
  sc_core::sc_event_finder& pos() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::posedge_event );
    }
  */

  // use for negative edge sensitivity

  /*
  sc_core::sc_event_finder& neg() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::negedge_event );
    }
  */


  // was there a value changed event?

  bool event(unsigned int i) const
    { return (*this)[i]->event(); }

  bool event() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->event();

    return val_changed; 
  }

  // was there a positive edge event?

  bool posedge(unsigned int i) const
    { return (*this)[i]->posedge(); }
  
  bool posedge() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->posedge();

    return val_changed; 
  }

  // was there a negative edge event?

  bool negedge(unsigned int i) const
    { return (*this)[i]->negedge(); }

  bool negedge() const
  {
    bool val_changed = false;
    for(int i=0; i<base_type::size(); i++)
      val_changed = val_changed | (*this)[i]->negedge();

    return val_changed; 
  }


  // delayed evaluation
  const sc_core::sc_signal_logic_deval& delayed(unsigned int i) const;

  // write the new value

  void write( const data_type& value_ )
  { 
    for(int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( value_ ); 
    }
  }

  void write( const unsigned int pos, const data_type& value_ )
  {
    (*this)[pos]->write( value_ );
  }

  this_type& operator = ( const data_type& value_ )
  { 
    for (int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( value_ ); 
    }
    return *this;
  }

  this_type& operator = ( const in_if_type& interface_ )
  { 
    for (int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( interface_.read() );
    }
    return *this;
  }

  this_type& operator = ( const in_port_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this; 
  }

  this_type& operator = ( const inout_port_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this;     
  }

  this_type& operator = ( const this_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this; 
  }


  // set initial value (can also be called when port is not bound yet)

  void initialize( const data_type& value_ );

  void initialize( const in_if_type& interface_ )
    { initialize( interface_.read() ); }


  // called when elaboration is done
  /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
  /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

  virtual void end_of_elaboration();


  // (other) event finder method(s)

  /*
  sc_core::sc_event_finder& value_changed() const
    {
      return *new sc_core::sc_event_finder_t<in_if_type>(
	*this, &in_if_type::value_changed_event );
    }
  */

  virtual const char* kind() const
    { return "osss_vector_inout"; }

protected:

  data_type* m_init_val;

public:

  // called by sc_trace
  void add_trace( sc_core::sc_trace_file*, const std::string& ) const;

protected:

  void remove_traces() const;

  mutable sc_core::sc_trace_params_vec* m_traces;

private:

  // disabled
  osss_vector_inout( const this_type& );

#ifdef __GNUC__
  // Needed to circumvent a problem in the g++-2.95.2 compiler:
  // This unused variable forces the compiler to instantiate
  // an object of T template so an implicit conversion from
  // read() to a C++ intrinsic data type will work.
  static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_core::sc_signal_logic_deval&
osss_vector_inout<sc_dt::sc_logic>::delayed(unsigned int i) const
{
  const in_if_type* iface = DCAST<const in_if_type*>( get_interface(i) );
  if( iface != 0 ) {
    return RCAST<const sc_core::sc_signal_logic_deval&>( *iface );
  } else {
    // the tricky part
    const sc_core::sc_port_base* pb = this;
    return RCAST<const sc_core::sc_signal_logic_deval&>( *pb );
  }
}

// destructor

inline
osss_vector_inout<sc_dt::sc_logic>::~osss_vector_inout()
{
  if( m_init_val != 0 ) {
    delete m_init_val;
  }
  remove_traces();
}


// set initial value (can also be called when port is not bound yet)

inline
void
osss_vector_inout<sc_dt::sc_logic>::initialize( const data_type& value_ )
{
  for (int i=0; i<base_type::size(); i++)
  {
    inout_if_type* iface = DCAST<inout_if_type*>( get_interface(i) );
    if( iface != 0 ) {
      iface->write( value_ );
    } else {
      if( m_init_val == 0 ) {
	m_init_val = new data_type;
      }
      *m_init_val = value_;
    }
  }
}


// called when elaboration is done

inline
void
osss_vector_inout<sc_dt::sc_logic>::end_of_elaboration()
{
  if( m_init_val != 0 ) {
    write( *m_init_val );
    delete m_init_val;
    m_init_val = 0;
  }
  if( m_traces != 0 ) {
    for( sc_core::sc_trace_params_vec::iterator i = m_traces->begin(); 
         i != m_traces->end(); i++ )
    {
      sc_core::sc_trace_params* p = (*i);
      for ( int j=0; j<base_type::size(); j++ )
      {
        in_if_type* iface = DCAST<in_if_type*>( get_interface(j) );
        std::ostringstream ostr;
        ostr << p->name << j;
        sc_core::sc_trace( p->tf, iface->read(), ostr.str() );
      }
    }
    remove_traces();
  }
}


// called by sc_trace

inline
void
osss_vector_inout<sc_dt::sc_logic>::add_trace( sc_core::sc_trace_file* tf_,
					       const std::string& name_ ) const
{
  if( tf_ != 0 ) {
    if( m_traces == 0 ) {
      m_traces = new sc_core::sc_trace_params_vec;
    }
    m_traces->push_back( new sc_core::sc_trace_params( tf_, name_ ) );
  }
}

inline
void
osss_vector_inout<sc_dt::sc_logic>::remove_traces() const
{
  if( m_traces != 0 ) {
    for( int i = m_traces->size() - 1; i >= 0; -- i ) {
      delete (*m_traces)[i];
    }
    delete m_traces;
    m_traces = 0;
  }
}

// ----------------------------------------------------------------------------
//  CLASS : osss_vector_out<T>
//
// ----------------------------------------------------------------------------

// osss_vector_out can also read from its port, hence no difference with 
// osss_vector_inout.
// For debugging reasons, a class is provided instead of a define.

template <class T>
class osss_vector_out
  : public osss_vector_inout<T>
{
public:

  // typedefs

  typedef T                                   data_type;

  typedef osss_vector_out<data_type>          this_type;
  typedef osss_vector_inout<data_type>        base_type;

  typedef typename base_type::in_if_type      in_if_type;
  typedef typename base_type::in_port_type    in_port_type;
  typedef typename base_type::inout_if_type   inout_if_type;
  typedef typename base_type::inout_port_type inout_port_type;

public:

  // constructors

  osss_vector_out()
    : base_type()
    {}

  explicit osss_vector_out( const char* name_ )
    : base_type( name_ )
    {}

  explicit osss_vector_out( inout_if_type& interface_ )
    : base_type( interface_ )
    {}

  osss_vector_out( const char* name_, inout_if_type& interface_ )
    : base_type( name_, interface_ )
    {}

  explicit osss_vector_out( inout_port_type& parent_ )
    : base_type( parent_ )
    {}

  osss_vector_out( const char* name_, inout_port_type& parent_ )
    : base_type( name_, parent_ )
    {}

  explicit osss_vector_out( this_type& parent_ )
    : base_type( parent_ )
    {}

  osss_vector_out( const char* name_, this_type& parent_ )
    : base_type( name_, parent_ )
    {}


  // destructor (does nothing)

  virtual ~osss_vector_out()
    {}


  // write the new value

  this_type& operator = ( const data_type& value_ )
  { 
    for (int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( value_ ); 
    }
    return *this;
  }

  this_type& operator = ( const in_if_type& interface_ )
  { 
    for (int i=0; i<base_type::size(); i++)
    {
      (*this)[i]->write( interface_.read() );
    }
    return *this;
  }

  this_type& operator = ( const in_port_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this; 
  }

  this_type& operator = ( const inout_port_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this;     
  }

  this_type& operator = ( const this_type& port_ )
  { 
    if (port_.size() >= base_type::size())
    {
      for (int i=0; i<base_type::size(); i++)
	(*this)[i]->write( port_[i]->read());
    }
    else
    {
      for (int i=0; i<base_type::size(); i++)
      {
	(*this)[i]->write( port_->read() ); 
      }
    }
    return *this; 
  }

  virtual const char* kind() const
    { return "osss_vector_out"; }

private:

  // disabled
  osss_vector_out( const this_type& );
};

} // namespace osss

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// ----------------------------------------------------------------------------
//  FUNCTION : sc_trace
// ----------------------------------------------------------------------------

namespace sc_core {

template <class T>
inline
void
sc_trace( sc_core::sc_trace_file* tf,
          const osss::osss_vector_in<T>& port,
          const std::string& name)
{
  const sc_signal_in_if<T>* iface = 0;
  bool trace_done = true;
  if (sc_get_curr_simcontext()->elaboration_done() )
  {
    for (int i=0; i<port.size(); i++)
    {
      iface = DCAST<const sc_signal_in_if<T>*>( port.get_interface(i) );
      if ( iface )
      {
        std::ostringstream ostr;
        ostr << name << i;
        sc_trace( tf, iface->read(), ostr.str() );
      }
      else 
      {
        trace_done = false;
      }
    }
  }
  else
    trace_done = false;

  if (! trace_done)
  {
    port.add_trace( tf, name );
  }
}

template <class T>
inline
void
sc_trace( sc_trace_file* tf,
          const osss::osss_vector_inout<T>& port, 
          const std::string& name )
{
  const sc_signal_in_if<T>* iface = 0;
  bool trace_done = true;
  if (sc_get_curr_simcontext()->elaboration_done() )
  {
    for (int i=0; i<port.size(); i++)
    {
      iface = DCAST<const sc_signal_in_if<T>*>( port.get_interface(i) );
      if ( iface )
      {
        std::ostringstream ostr;
        ostr << name << i;
        sc_trace( tf, iface->read(), ostr.str() );
      }
      else
      {
        trace_done = false;
      }
    }
  }
  else
    trace_done = false;

  if (! trace_done)
    port.add_trace( tf, name );
}

} // namespace sc_core

// ----------------------------------------------------------------------------
//  FUNCTION : operator<< (for making processes sensitive to osss_vectors)
// ----------------------------------------------------------------------------

namespace osss {

template < class T >
inline
sc_core::sc_sensitive & operator<< (sc_core::sc_sensitive & sens, 
			   const osss_vector_in<T> & vecPort)
{
  for (int i=0; i<vecPort.size(); i++)
    sens << *(vecPort[i]);

  return sens;
}

template < class T >
inline
sc_core::sc_sensitive & operator<< (sc_core::sc_sensitive & sens, 
			   const osss_vector_inout<T> & vecPort)
{
  for (int i=0; i<vecPort.size(); i++)
    sens << *(vecPort[i]);

  return sens;
}

} // namespace osss

#endif
// $Id: osss_vector_port.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
