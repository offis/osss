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

#ifndef OSSS_CONTEXT_BASE_H_INCLUDED
#define OSSS_CONTEXT_BASE_H_INCLUDED

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_recon_types.h"
#include "o3s/utils/object_container.h"

namespace osss {

class osss_object;

namespace osssi {

class osss_recon_base;
class osss_type_descriptor;
class osss_process_handle;

/// interface class for named contexts
class osss_context_base
  : public object_container
  , public sc_core::sc_object
{
  friend class osss_recon_base;

  /**
   * \todo  This is a quick hack, to get the pure-virtual
   *        object_container::content() method working
   *        without having to change this beast of a class
   *        too much.
   */
  object_container::object_handle content() const
    { return this->getOSSSObjectPtr(); }

protected:

  /// named constructor
  osss_context_base( const char *
                       = sc_core::sc_gen_unique_name( "osss_context" ) );

  virtual time_consumer_base* time_consumer() const;

  using object_container::wait;

public:

  virtual ~osss_context_base();

  // This method clears the context. That is, it does not contain
  // a valid content anymore. It has to be assigned one before next
  // use.
  virtual
  void reset() = 0;

  /*
  virtual
  void
  prefetch() = 0;
  */

  virtual
  void
  printOn(std::ostream & os) const = 0;

  virtual
  void
  writeXMLStringOn(std::ostream & os) const = 0;

  // This method is used to obtain access permission
  virtual
  osssi::osss_user_process_descriptor *
  requestPermission() = 0;

  // This method is used to return access permission
  virtual
  void
  returnPermission(osssi::osss_user_process_descriptor *) = 0;


  /** This method returns true (support for OSSS_KEEP_ENABLED() macro).
      It lockes a named context into a enabled state.
      Locks cannot be nested.
   */
  virtual
  void
  lock() = 0;

  /** This method reverses lock(). It always returns false. */
  virtual
  void
  unlock() = 0;

  /** Start lazy-return-of-named-context-permission mode */
  virtual
  void startLazy() = 0;

  /** End lazy mode */
  virtual
  void endLazy() = 0;

  /** Test of laziness */
  virtual
  bool isLazy() = 0;

  // Returns a ptr to the internal structure describing the
  // content's actual type (on that device type).
  virtual
  const osss_type_descriptor *
  getTypePtr() const = 0;

  // Returns a ptr to the reconfigurable object this context
  // was bound to.
  virtual
  osss_recon_handle
  getReconPtr() const = 0;

  // Returns a ptr to the content object. For internal use
  // within osss_lib, you may disable NULL checking by setting
  // the argument to false. For use within user macros, always
  // leave it at its default value.
  virtual
  osss_object *
  getOSSSObjectPtr(bool check_for_null = true) const = 0;

  // This method enables osss_recon to update the type descriptor
  // of a context.
  virtual
  void
  updateType(const osss_type_descriptor * new_type) = 0;

  // This method enables externally updating our content.
  virtual
  void
  updateContent(osss_object * new_context) = 0;

  // Returns the internal lock state. This will call the
  // evaluation method on the content object.
  virtual
  bool
  evaluateInternalLock() const = 0;

  // This registers the client as a user of the context.
  // The call is fed to the osss_recon object (which handles
  // such information).
  virtual void
  signInUserProcess( osss_process_handle const & client_process,
                     osss_priority               priority );

  // Implemented by sc_object on "real" context objects
  virtual
  const char * name () const = 0;

  /** Written by single-cycle-thread:
      Array index is the user process id (from pool).
      The fields flag the external lock state
      with resprect to the user processes.
   */
  std::vector<bool> m_external_locks;

  /** Here we store the results of the placement decision.
      This does _not_ tell, if the context currently is in that slot.
      It says where it would go, if the scheduler this would select
      this context to be enabled.
    */
  osss_slot_idx     m_suitable_slot_index_precalc;

protected:
  /** This attribute stores the information,
   *  to which recon object this context is bound.
   *  Is NULL unless the binding is done. When set,
   *  it points to the reconfigurable object.
   */
  osss_recon_base * m_recon_object_ptr;

  /**
   *  Here we buffer using() calls until we get bound to a
   *  recon object. Pair: first is client process, second is priority.
   */
  class pending_client_registration_list;
  pending_client_registration_list* pending_client_registrations;

  void do_bind( osss_recon_base& );

};

inline
::std::ostream & operator<< ( ::std::ostream & os,
                              osss_context_base const & object )
{
  object.printOn(os);
  return os;
}

// For use in the designer's SC_CTOR. It is to be used just like
// reset_signal_is() just after a SC_CTHREAD statement. The thread
// is registered as a user for the reconfigurable object this context
// will be bound to.

void uses( osss_context_base & , osss_priority = 0 );
void osss_uses( osss_context_base &, osss_priority = 0 );

} // end namespace osssi

using osssi::osss_uses;

} // end namespace osss

#endif // OSSS_CONTEXT_BASE_H_INCLUDED
// $Id: osss_context_base.h 2879 2008-12-09 14:01:05Z andreas $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
