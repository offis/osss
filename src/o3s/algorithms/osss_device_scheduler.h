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

#ifndef OSSS_DEVICE_SCHEDULER_H
#define OSSS_DEVICE_SCHEDULER_H

#include "o3s/utils/osss_systemc.h"

#include "o3s/datatypes/osss_vector.h"
#include "o3s/utils/osss_device_job_descriptor.h"

namespace osss {

class osss_device;

////////////////////////////////////////////////////////////////////////////
///
/// \brief This is the scheduler interface class
///
/// This class provides the base class for all schedulers that shall be
///  used together with device objects. Every scheduler that shall be used
///  with a device object must directly or indirectly be derived from this
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

class osss_device_scheduler
{
public:

  //! \brief Constructor.
  //!
  //! The only allowed constructor for all scheduler classes.
  //! Every derived class must provide this constructor and only this
  //! constructor. When calling the this constructor on an ancestor class,
  //! the identifier of the only constructor parameter must be passed
  //! unchanged to the constructor of the ancestor class.
  osss_device_scheduler();

  virtual
  ~osss_device_scheduler();

  /** This method returns the given cycle count value as a polymorphic object
      representing the same number. The number is converted to an instance of
      the cycle count class used within the device object. Therefore it might
      loose precision. */
  osss_polymorphic< osss_cycle_count > getCycleValue( sc_dt::uint64 value ) const;

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
  osss_device_job_idx
  schedule() = 0;

protected:
  class osss_job_array
  {
    /// to allow constructor call
    friend class osss_device_scheduler;

  public:
    const osss_device_job_descriptor&
    operator[](const osss_device_job_idx index) const;

    private:
    osss_device_scheduler * m_scheduler;
    osss_job_array(osss_device_scheduler * sched);
  };
  /// to allow direct access to m_for_device
  friend class osss_job_array;
  /** This allows the derived classes' schedule() method to access
      the job table. Since it is to be used by the designer, there is no
      m_ prefix */
  osss_job_array jobs;

  /// These are the methods for job access
  const bool          isConfigurationControllerBusy() const;
  osss_device_job_idx getNumberOfReconObjects() const;

  private:
  osss_device *  m_for_device;

  /** This var has to be set according to the desired
     value which is to be written into m_for_device
     member attribute in the constructor. */
  static osss_device * current_device_id;

  public:

  /** Use this abstract factory type to pass around
      references to factories */
  class factory_base
  {
    public:
    virtual ~factory_base(){};
    virtual osss_device_scheduler * getInstance(osss_device * id) = 0;
  };

  /** Use instances of this factory template class to obtain
      instances of device schedulers */
  template< class T >
  class factory : public factory_base
  {
    public:
    factory(){}
    virtual ~factory(){};

    virtual
    osss_device_scheduler * getInstance(osss_device * id)
    {
      current_device_id = id;
      osss_device_scheduler * scheduler = new T();
      current_device_id = NULL;
      return scheduler;
    }
  }; // factory
}; // recon scheduler class

} // namespace osss

#endif // OSSS_DEVICE_SCHEDULER_H
// $Id: osss_device_scheduler.h 2394 2008-07-01 20:45:28Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
