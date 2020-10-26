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

#ifndef OSSS_RECON_H_INCLUDED
#define OSSS_RECON_H_INCLUDED

#include "o3s/utils/osss_recon_types.h"
#include "o3s/structural/app/osss_context.h"
#include "o3s/structural/app/osss_recon_base.h"
#include "o3s/utils/osss_call_wrapper.h"

namespace osss {

template< class InterfaceT = osss::osss_object >
class osss_recon
  : public osssi::osss_recon_base
{
  friend class osssi::osss_call_wrapper< osss_recon< InterfaceT > >;
public:
  typedef InterfaceT InterfaceClass;
  typedef osssi::osss_call_wrapper< osss_recon< InterfaceT > > CallWrapperClass;

private:
  // Deactivated since prefetching infrastructure is not intact again
  // It worked in v1 and has to be reworked for v2
  //osssi::osss_context_base * m_prefetch_named_context_ptr;

public:
  /// Constructor
  explicit
  osss_recon( sc_core::sc_module_name =
                sc_core::sc_gen_unique_name("osss_recon") );

  /// (empty) destructor
  virtual ~osss_recon(){}

#if 0 /* no references yet */
  template < class DerivedT >
  osss_recon< InterfaceT, SchedulerClass, CycleCountClass >&
  operator=( const osss_reference< DerivedT > &);
#endif

  // Assignment operators

  /// Assignment operator for identical class
  void operator=( osss_recon< InterfaceT > &);

  /// Assignment operator for recon objects
  template< class DerivedT >
  void operator=( osss_recon< DerivedT > &);

  /// Assignment operator for other contexts
  template < class DerivedT >
  void operator=( osss_context< DerivedT > &);

  /// Assignment operator for polymorphic objects
  template < class DerivedT >
  void operator=( const osss_polymorphic< DerivedT > &);

  /// Assignment operator for plain user classes
  void operator=( const InterfaceT &);

  /** This is for external use only if check_for_null is left
      at its default value "true". It returns the content of
      the currently enabled context. Never call this method
      on a multi-slot object!
    */
  InterfaceT *
  getContentPtr(bool check_for_null = true);

  /**
   * \brief call method on contained object
   *
   * To call a member function on the anonymous context object,
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

#if 0
  /** This method is to be used by the user process to obtain
      access permission.
      It returns a ptr to the context for which permission was granted.
      a) If calling_named_context_ptr==NULL (that is, anonymous context),
         it is resolved to a real context.
      b) If new_content_ptr != NULL then the context assigned this
    */
  virtual
  osssi::osss_context_base *
  requestPermission(osssi::osss_context_base * calling_named_context_ptr = NULL,
                           osss_object       * new_content_ptr = NULL);

  /** Counterpart of requestPermission(). Returns the access permission.
      Pass the value you received by requestPermission(). */
  virtual
  void
  returnPermission(osssi::osss_context_base * calling_named_context_ptr);
#endif

  /** This method performs a default constructor call on the recon object.
      If the argument is NULL, it is done on the anonymous context.
      If it is non-NULL, the call is executed on the given context. */
  template< class RHSType >
  void
  construct(osssi::osss_context_base * calling_named_context_ptr = NULL);

  virtual
  std::string const & if_typename() const;

  virtual
  bool check_runtime_type( osss_object* ) const;

#if 0
  /** Always returns true (for easier use in OSSS_KEEP_ENABLED macro).
      This method establises an external lock on an context.
      Never use it on the anonymous context (by passing NULL as arg)!
      Locks cannot be nested!
    */
  virtual
  void
  lock(osssi::osss_context_base * nc_ptr);

  /** Returns locking state of a context. More precisely,
      it tells the caller wether he (as a user process)
      did establish an external lock. This is to be used
      in OSSS_KEEP_ENABLED() macros only.
    */
  virtual
  bool
  isExternallyLocked(osssi::osss_context_base * nc_ptr);

  /** Always returns false (for easier use in OSSS_KEEP_ENABLED macro).
      Obviously, it reverses lock().
    */
  virtual
  void
  unlock(osssi::osss_context_base * nc_ptr);

#endif

  //-----------------------------------------------------------------------
  /** A type conversion that enables assignments from reconfigurable objects
      to normal objects (but only, iff assignment compatible). */
  inline
  operator osss_polymorphic< InterfaceT > ();

  //! A type-cast function for potentially compatible assignments from a
  //! polymorphic object.
  template< class TargetType >
  TargetType cast();

  void end_of_elaboration();

}; // osss_recon

} // end namespace osss

#include "osss_recon.tpp"

#endif  // OSSS_RECON_H_INCLUDED
// $Id: osss_recon.h 3021 2009-03-04 22:55:21Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
