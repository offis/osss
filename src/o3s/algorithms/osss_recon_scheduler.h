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

#ifndef OSSS_RECON_SCHEDULER_H
#define OSSS_RECON_SCHEDULER_H

#include "o3s/utils/osss_systemc.h"

#include "o3s/utils/osss_recon_types.h"
#include "o3s/datatypes/osss_vector.h"
#include "o3s/utils/osss_recon_job_descriptor.h"

namespace osss {

namespace osssi {
class osss_recon_base;
}

////////////////////////////////////////////////////////////////////////////
///
/// \brief This is the scheduler interface class
///
/// This class provides the base class for all schedulers that shall be
///  used together with reconfigurable objects. Every scheduler that shall be used
///  with a reconfigurable object must directly or indirectly be derived from this
///  class.
///
/// \note Interface class.
///       Every dervied class must implement the schedule method correctly.
///       Every derived class must be able to be copied by the built-in
///       assignment operator and must not overload the assignment operator.
///       Every derived class must only provide the constructor, that is given
///       by this class.
///
////////////////////////////////////////////////////////////////////////////

class osss_recon_scheduler
{
  friend class osssi::osss_recon_base;
public:

  osss_recon_scheduler();

  virtual
  ~osss_recon_scheduler();

public:
  //! This scheduler differs from those for the shared objects in
  //! some aspects:
  //! a) The jobs are NOT sorted accoring to their priority.
  //!    However, the sort order does not change between
  //!    schedule() calls, that is, the job numbers statically
  //!    refer to certain user processes.
  //! b) Instead of a request vector there is a set of methods
  //!    available for the schedule() method which provide
  //!    information on the possible jobs to be scheduled.
  //! c) The schedule method must return a job number between 0
  //!    and getNumberOfJobs()-1. The number of jobs never
  //!    changes during runtime. The returned number indicates
  //!    which request is to be served next.
  //! d) The returned job number must indicate a valid job.
  //!    The schedule() method is never called when there
  //!    are no valid job candidates.
  //! The schedule() method may rely on being called for each
  //! decision, even if there is one valid job only.
  virtual
  osss_recon_job_idx schedule() = 0;

private:
  const osss_recon_job_descriptor&
  getJob( osss_recon_job_idx index ) const;

protected:
  class osss_job_array
  {
    /// to allow constructor call
    friend class osss_recon_scheduler;

  public:
    const osss_recon_job_descriptor&
    operator[](osss_recon_job_idx index) const;
    // osss_job_index_type size() const; re-enable when fossy
    // knows osss_vector

  private:
    osss_recon_scheduler& m_scheduler;
    explicit
    osss_job_array(osss_recon_scheduler& sched);
  };

  /** This allows the derived classes' schedule() method to access
      the job table. Since it is to be used by the designer, there is no
      m_ prefix */
  osss_job_array jobs;

  // These are the methods for job access
  osss_context_idx   getNumberOfContexts() const;
  osss_type_idx      getNumberOfClasses() const;
  osss_recon_job_idx getNumberOfUserProcesses() const; // get rid of it,
                                // when fossy knows osss_vector<>
  bool               isConfigurationControllerBusy() const;

  /** This method returns the given cycle count value as a polymorphic object
      representing the same number. The number is converted to an instance of
      the cycle count class used within the device object. Therefore it might
      loose precision. */
  osss_polymorphic< osss_cycle_count > getCycleValue( sc_dt::uint64 value ) const;

private:
  /// Remembers, to which recon object we belong.
  osss_recon_handle m_for_recon_object;

  /** This var has to be set according to the desired
   *  value which is to be written into m_for_recon_object
   *  member attribute in the constructor.
   *
   * \warning This is not MT safe!
   * \see factory_base
   */
  static osss_recon_handle current_recon_object_handle;

  /** Use this abstract factory base type when
      passing around pointers to factories that
      are able to create osss_recon_scheduler objects. */
  class factory_base
  {
  public:
    virtual ~factory_base(){}
    virtual osss_recon_scheduler * getInstance() const = 0;
    virtual std::string getName() const = 0;
  protected:
    factory_base( osss_recon_handle owner )
      : owner_( owner )
    {}
    osss_recon_handle owner_;
  };

  /** Use instances of this template class to construct
      real schedulers. */
  template< class T >
  class factory : public factory_base
  {
  public:
    explicit factory( osss_recon_handle owner )
      : factory_base( owner )
    {}

    virtual
    osss_recon_scheduler * getInstance() const
    {
      // prepare global handle
      osss_recon_scheduler::current_recon_object_handle = this->owner_;
      // Create scheduler of type T
      osss_recon_scheduler * scheduler = new T;
      // reset global handle
      osss_recon_scheduler::current_recon_object_handle = 0;

      // return created scheduler
      return scheduler;
    }

    /// Return class name of scheduler which this object can create.
    virtual std::string getName() const
    {
      return osssi::type_id::from<T>().name();
    }
  }; // class factory
}; // class osss_recon_scheduler

} // namespace osss

#endif // OSSS_RECON_SCHEDULER_H
// $Id: osss_recon_scheduler.h 3022 2009-03-04 22:55:24Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
