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

#ifndef OSSS_CONTEXT_H_INCLUDED
#define OSSS_CONTEXT_H_INCLUDED

#include "osss_context_base.h"
#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/type_id.h"
#include "o3s/utils/osss_call_wrapper.h"

#if 0 /* no references yet */
#include "osssr/osss_reference.h"
#endif

namespace osss {

/** This class is the container for named contexts.
    Instantiate the objects as members of SystemC modules only.
  */
template< class ReconObjectInterfaceT >
class osss_recon;

template< class InterfaceT >
class osss_context
  : public osssi::osss_context_base
{
  friend class osssi::osss_call_wrapper< osss_context< InterfaceT > >;
public:
  typedef osss_context_base base_type;
  typedef InterfaceT        InterfaceClass;
  typedef osssi::osss_call_wrapper< osss_context< InterfaceT > > CallWrapperClass;

private:

  /** This member holds a ptr to the content within container */
  InterfaceT *  m_content_ptr;

  /** This indicates the type descriptor structure for the
      actual type of content (RTTI type). Note that the
      descriptor is dependent on the device type!
    */
  const osssi::osss_type_descriptor * m_type_descriptor;

public:
  /// Binding operator. Binds the context to a reconfigurable object.
  template< class ReconObjectInterfaceT >
  void
  operator()(osss_recon< ReconObjectInterfaceT > &);

  /// Binding operator. Binds the context to a reconfigurable object.
  template< class ReconObjectInterfaceT >
  void
  bind(osss_recon< ReconObjectInterfaceT > &);

  /// Trivial constructor
  osss_context();

  /// Constructor with name
  explicit osss_context(const char *);

  /// Trivial destructor.
  virtual ~osss_context();

#if 0 /* no references yet */
  template < class DerivedInterfaceT >
  osss_context< InterfaceT >& operator=( const osss_reference< DerivedInterfaceT > &);
#endif

  /// Assignment operator for other contexts
  template < class DerivedInterfaceT >
  osss_context< InterfaceT >& operator=( osss_context< DerivedInterfaceT > &);

  /// Assignment operator for recon objects
  template< class DerivedInterfaceT >
  osss_context< InterfaceT >& operator=( osss_recon< DerivedInterfaceT > &);

  /// Assignment operator for polymorphic objects
  osss_context< InterfaceT >& operator=( const osss_polymorphic< InterfaceT > &);

  /// Assignment operator for plain user classes
  osss_context< InterfaceT >& operator=( const InterfaceT &);

  /**
   * \brief call method on contained object
   *
   * To call a member function on the currently contained object,
   * simply use this operator->.  All required house-keeping
   * is performed automatically.
   *
   * Example:
   * \code
   *   osss_recon< my_interface > recon;
   *   // ...
   *   result = recon->my_method( args );
   * \endcode
   *
   * \note osss_call() is no longer needed for this operator
   */
  inline
  CallWrapperClass operator->();

  /// Always returns "this". It obtains access permission to this context. Blocking call.
  virtual
  osssi::osss_user_process_descriptor *
  requestPermission();

  /// Returns the access permission.
  virtual
  void
  returnPermission(osssi::osss_user_process_descriptor * upd);

  /** Start lazy-return-of-named-context-permission mode */
  virtual
  void startLazy();

  /** End lazy mode */
  virtual
  void endLazy();

  /** Test of laziness */
  virtual
  bool isLazy();

  /** This method performs a default constructor call on the context. */
  template< class RHSType >
  void
  construct();

  /** Requests and returns permission. The context is enabled afterwards.
      However, it is not locked, so it may get disabled immediately again!
    */
  virtual
  void
  enable();

  /** Clears (invalidates) the content. The user must assign a new value
      to this object before it gets usable again.
    */
  virtual
  void
  reset();

  virtual std::string const& if_typename() const;

  virtual
  void
  printOn(std::ostream & os) const;

  virtual
  void
  writeXMLStringOn(std::ostream & os) const;

  virtual
  const char * name () const
  {
    return sc_core::sc_object::name();
  }


#if 0 /* no references yet */
  osss_reference< InterfaceT >
  getReference();
#endif

  /** Returns a ptr to the reconfigurable object this
      object is bound to.
    */
  virtual
  osssi::osss_recon_base *
  getReconPtr() const;

  /** Returns the device type dependent type descriptor
      structure for the actual content.
    */
  virtual
  const osssi::osss_type_descriptor *
  getTypePtr() const;

  /** Returns a ptr to the content. For use in user macros,
      always keep the argument at its default value.
      You may give "false" for OSSS lib internal use
      if you know what you are doing.
    */
  virtual
  osss_object *
  getOSSSObjectPtr(bool check_for_null = true) const;

  /**
   * Like getOSSSObjectPtr(), but return type is interface
   * class pointer
   */
  virtual
  InterfaceT *
  getContentPtr(bool check_for_null = true);

  /// To allow osss_recon's reconfiguration thread update our internal state
  virtual
  void
  updateType(const osssi::osss_type_descriptor * new_type);

  /// To allow updates on our internal state
  virtual
  void
  updateContent(osss_object * new_context);

  /* Part of the temporarily disabled prefetching infrastructure.
  virtual
  void
  prefetch();
  */

  /** This method locks this context into enabled state.
      It returns true (support for OSSS_KEEP_ENABLED() macro).
      You cannot nest locks! */
  virtual
  void
  lock();

  /** This method returns current locking state */
  virtual
  bool
  isLocked();

  /** This reverses lock(). It always returns false. */
  virtual
  void
  unlock();

  //-----------------------------------------------------------------------
  /** A type conversion that enables assignments from contexts objects
      to normal objects (but only, iff assignment compatible).
    */
  inline
  operator const osss_polymorphic< InterfaceT > ();

  /** Calls evaluation method on content. Therefore it returns the
      current state of the internal lock.
    */
  virtual
  bool
  evaluateInternalLock() const;
}; // end class osss_context

} // end namespace osss

#include "osss_context.tpp"

#endif // OSSS_CONTEXT_H
// $Id: osss_context.h 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
