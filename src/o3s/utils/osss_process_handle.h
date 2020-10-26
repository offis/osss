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

/**
 * \file  osss_systemc.h
 * \brief common SystemC inclusion header
 *
 * Include this file, iff you want to use SystemC inside the OSSS library.
 *
 * \note  This file does not introduce any SystemC construct into the
 *        top-level namespace. You need to explicitly prefix SystemC
 *        types etc. with their namespace.
 */
#ifndef OSSS_PROCESS_HANDLE_H
#define OSSS_PROCESS_HANDLE_H

#include "o3s/utils/osss_systemc.h"

namespace osss {
namespace osssi {

/* ------------------------------------------------------------ */

#if defined(OSSS_SYSTEMC_2_2)

/**
 * \class osss_process_handle
 * \brief process handle abstraction
 *
 * \note needed for SystemC 2.1 compatibility
 */
class osss_process_handle
    : public sc_core::sc_process_handle
{
public:

  typedef sc_core::sc_process_handle base_type;
  typedef osss_process_handle        this_type;
  typedef sc_core::sc_process_b*     handle_type;
  typedef sc_core::sc_method_handle  method_type;
  typedef sc_core::sc_thread_handle  thread_type;
  typedef sc_core::sc_cthread_handle cthread_type;
  typedef unsigned int               id_type;

  osss_process_handle()
    : base_type()
  {}

  osss_process_handle( const base_type& src )
    : base_type( src )
  {}

  osss_process_handle( const this_type& src )
    : base_type( src )
  {}

  explicit osss_process_handle( handle_type h )
    : base_type( h )
  {}

  osss_process_handle& 
  operator= (const osss_process_handle &src)
  {
    base_type::operator=(src);
    return *this;
  }

  id_type get_proc_id() const;

  /* for convenience */
  inline operator const handle_type() const;

  /** needed for sort */
  inline bool operator<(  const osss_process_handle& that ) const; 

  static
  osss_process_handle last_created();

  static inline
  osss_process_handle current();

  static inline
  bool wait( unsigned int = 1 );

  static inline
  bool wait( sc_core::sc_time const & );

}; // osss_process_handle

typedef osss_process_handle osss_method_handle;
typedef osss_process_handle osss_thread_handle;
typedef osss_process_handle osss_cthread_handle;

#elif defined(OSSS_SYSTEMC_2_1)

/**
 * \class osss_process_handle
 * \brief process handle abstraction
 *
 * \note needed for SystemC 2.1 compatibility
 */
class osss_process_handle
{
public:

  typedef osss_process_handle         this_type;
  typedef sc_core::sc_process_b*      handle_type;
  typedef unsigned int               id_type;

  friend bool operator == ( const this_type& left, const this_type& right );
  friend bool operator != ( const this_type& left, const this_type& right );

  /** default constructor */
  inline osss_process_handle() :
      m_handle(0)
  {}

  /** constructor */
  inline explicit osss_process_handle( handle_type h )
    : m_handle(h)
  {}

  osss_process_handle( const this_type& src)
    : m_handle(src.m_handle)
  {}

  osss_process_handle& 
  operator= (const osss_process_handle &src)
  {
    m_handle = src.m_handle;
    return *this;
  }

  /* all public methods from sc_process_handle */
  inline bool dynamic() const
  { 
    std::cerr << "Error: Method " << __PRETTY_FUNCTION__ 
              << " is not implemented "
              << "in SystemC-2.1.v1" << std::endl;
    OSSS_ABORT();
    return false;
  }

  inline const std::vector<sc_object*>& get_child_objects() const
  {
    return m_handle ? m_handle->get_child_objects() :
      osss_process_handle::empty_vector;
  }

  inline sc_object* get_parent_object() const
  { 
    return m_handle ? 
      m_handle->host->simcontext()->hierarchy_pop() : 
      (sc_core::sc_object*)0; 
  }

  inline sc_object* get_process_object() const
    { return m_handle; }

  inline const char* name() const
    { return m_handle ? m_handle->name() : ""; }

  inline sc_curr_proc_kind proc_kind() const
  { 
    return m_handle ? 
      m_handle->host->simcontext()->get_curr_proc_info()->kind : 
      SC_NO_PROC_;  
  }

  inline sc_event& terminated_event()
  { 
    std::cerr << "Error: Method " << __PRETTY_FUNCTION__ 
              << " is not implemented "
              << "in SystemC-2.1.v1" << std::endl;
    OSSS_ABORT();
    return osss_process_handle::non_event;
  }

  inline bool terminated() const
  {
    std::cerr << "Error: Method " << __PRETTY_FUNCTION__ 
              << " is not implemented "
              << "in SystemC-2.1.v1" << std::endl;
    OSSS_ABORT();
    return false;
  }

  /** is this a valid handle? */
  inline bool valid() const
    { return ( m_handle != 0 ); }

  id_type get_proc_id() const;

  /* for convenience */
  inline operator const handle_type() const;

  /** needed for sort */
  inline bool operator<(  const osss_process_handle& that ) const;

  /** convert to "raw" process handle */
  inline operator handle_type();

  static
  osss_process_handle last_created();

  static inline
  osss_process_handle current();

  static inline
  bool wait( unsigned int = 1 );

protected:

  /** the real process handle */
  handle_type  m_handle;

protected:
  static std::vector<sc_core::sc_object*>
    empty_vector; // Returned if m_handle == 0.
  static sc_event
    non_event;    // Returned if m_handle == 0.

}; // osss_process_handle

class osss_method_handle
  : public osss_process_handle
{
  typedef osss_process_handle         base_type;
  typedef sc_core::sc_method_handle   method_type;
public:
  osss_method_handle() 
    : base_type(),
      m_method(0)
  {}
  osss_method_handle( handle_type  h,
                      method_type  m )
    : base_type(h),
      m_method(m)
  {}
public:
  /** convert to method handle */
  inline operator method_type()
    { return m_method; }

protected:
  method_type  m_method;
};

class osss_thread_handle
  : public osss_process_handle
{
  typedef osss_process_handle         base_type;
  typedef sc_core::sc_thread_handle   thread_type;
public:
  osss_thread_handle() 
    : base_type(),
      m_thread(0)
  {}
  osss_thread_handle( handle_type  h,
                      thread_type  t )
    : base_type(h),
      m_thread(t)
  {}
  /** convert to thread handle */
  inline operator thread_type()
    { return m_thread; }

protected:
  thread_type  m_thread;
};

class osss_cthread_handle
  : public osss_process_handle
{
  typedef osss_process_handle          base_type;
  typedef sc_core::sc_cthread_handle   cthread_type;
public:
  osss_cthread_handle() 
    : base_type(),
      m_cthread(0)
  {}

  osss_cthread_handle( handle_type  h,
                      cthread_type  t )
    : base_type(h),
      m_cthread(t)
  {}
  /** convert to cthread handle */
  inline operator cthread_type()
    { return m_cthread; }

protected:
  cthread_type  m_cthread;
};

#endif

typedef osss_process_handle::id_type OSSS_Process_ID_t;

/* ------------------------------------------------------------ */

inline
osss_process_handle
osss_process_handle::current()
{
# if defined(OSSS_SYSTEMC_2_1)
    return sc_core::sc_get_curr_process_handle();
# elif defined(OSSS_SYSTEMC_2_2)
    return sc_core::sc_get_current_process_handle();
# endif
}


bool
osss_process_handle::wait( unsigned int n )
{
  osss_process_handle curr_proc = current();
  // is the current process a clocked one?
  if( sc_core::SC_CTHREAD_PROC_ == curr_proc.proc_kind() )
  {
    // wait requested number of cycles
    sc_core::wait( n );
    return true;
  }
  // no clocked process running - do nothing and report this
  return false;
}

bool
osss_process_handle::wait( sc_core::sc_time const & delay )
{
  osss_process_handle curr_proc = current();
  // is the current process a clocked one?
  if( sc_core::SC_THREAD_PROC_ == curr_proc.proc_kind() )
  {
    // wait requested number of cycles
    sc_core::wait( delay );
    return true;
  }
  // no flexibly timed process running - do nothing and report this
  return false;
}

#if defined(OSSS_SYSTEMC_2_1)
inline
osss_process_handle::operator
osss_process_handle::handle_type()
{
    return m_handle;
}
inline
osss_process_handle::operator
const osss_process_handle::handle_type() const
{
    return (const handle_type) m_handle;
}
#elif defined(OSSS_SYSTEMC_2_2)
inline
osss_process_handle::operator
const osss_process_handle::handle_type() const
{
    base_type   tmp( *this );
    handle_type rst = (handle_type) tmp;
    return (const handle_type) rst;
}
#endif

inline
bool
osss_process_handle::operator<( const osss_process_handle& that ) const
{
  return this->get_process_object() < that.get_process_object();
}


#if defined(OSSS_SYSTEMC_2_1)
inline
bool
operator==( const osss_process_handle& left, 
	    const osss_process_handle& right ) 
{
   return (left.m_handle != 0) && (right.m_handle != 0) &&
          (left.m_handle == right.m_handle);
}

inline
bool
operator!=( const osss_process_handle& left,
            const osss_process_handle& right )
{
  return (left.m_handle == 0) || (right.m_handle == 0) ||
         (left.m_handle != right.m_handle);
}
#endif

/* ------------------------------------------------------------ */

extern
osss_method_handle
osss_create_method_process( const char*               name_p,
                            sc_core::SC_ENTRY_FUNC    method_p,
                            sc_core::sc_process_host* host_p );

extern
osss_thread_handle
osss_create_thread_process( const char*               name_p,
                            sc_core::SC_ENTRY_FUNC    method_p,
                            sc_core::sc_process_host* host_p );

extern
osss_cthread_handle
osss_create_cthread_process( const char*               name_p,
                             sc_core::SC_ENTRY_FUNC    method_p,
                             sc_core::sc_process_host* host_p );

} // namespace osssi
} // namespace osss

#endif  // OSSS_PROCESS_HANDLE_H
// $Id: osss_process_handle.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
