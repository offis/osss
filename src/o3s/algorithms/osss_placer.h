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

#ifndef OSSS_PLACER_H_INCLUDED
#define OSSS_PLACER_H_INCLUDED

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/type_id.h"

#include "o3s/utils/osss_recon_types.h"
//#include "o3s/datatypes/osss_polymorphic.h"
#include "o3s/datatypes/osss_vector.h"
//#include "o3s/algorithms/osss_cycle_count.h"
#include "o3s/utils/osss_slot_descriptor.h"
#include "o3s/utils/osss_context_descriptor.h"

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

class osss_placer
{
  friend class osssi::osss_recon_base;
public:

  //! \brief Constructor.
  //!
  osss_placer();

  virtual
  ~osss_placer();

public:
  virtual
  osss_slot_idx
  place( const osss_context_descriptor& context_to_be_placed) = 0;

protected:
  class osss_slot_array
  {
    /// to allow constructor call
    friend class osss_placer;

    public:
    const osss_slot_descriptor&
    operator[](const osss_slot_idx index) const;

    private:
    osss_placer * m_placer;
    osss_slot_array(osss_placer * placer);
  };

  /** This allows the derived classes' place() method to access
      the slot table. */
  osss_slot_array slots;

  // These are the methods for job access
  const osss_context_idx      getNumberOfContexts() const;
  const osss_type_idx         getNumberOfClasses() const;
  const osss_slot_idx         getNumberOfSlots() const;
  const bool                  isConfigurationControllerBusy() const;

private:
  /// Remembers, to which recon object we belong.
  osss_recon_handle m_for_recon_object;

  /** This var has to be set according to the desired
      value which is to be written into m_for_recon_object
      member attribute in the constructor. */
  static osss_recon_handle current_recon_object_handle;

  /** Use this abstract factory base type when
      passing around pointers to factories that
      are able to create osss_placer objects. */
  class factory_base
  {
  public:
    virtual ~factory_base(){};
    virtual osss_placer * getInstance() const = 0;
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
    virtual ~factory(){};
    virtual osss_placer * getInstance() const
    {
      // Create the placer with the information
      // to which recon object it belongs
      osss_placer::current_recon_object_handle = this->owner_;
      // Create
      osss_placer * placer = new T();
      // Clear the value to avoid dangling pointers
      // possibly making debugging difficult.
      osss_placer::current_recon_object_handle = NULL;
      return placer;
    }

    /// Return class name of scheduler which this object can create.
    virtual std::string getName() const
    {
      return osssi::type_id::from<T>().name();
    }
  }; // class factory
}; // class osss_placer

} // namespace osss

#endif // OSSS_PLACER_H_INCLUDED

// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
