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
#ifndef OSSS_ELABORATION_H
#define OSSS_ELABORATION_H 1

#include <deque>

namespace osss {

/**
 * \def   OSSS_ELABORATE
 * \brief switch to activate elaboration classes
 *
 * If this preprocessor switch is defined, the registered
 * elaborator classes are run from within
 *   osss_system::end_of_elaboration()
 *
 * \see osss_elaboration, osss_system, osss_start
 *
 */

/**
 * \def   OSSS_ELABORATE_ONLY
 * \brief switch to perform elaboration \em only
 *
 * If this preprocessor switch is defined, the simulation
 * is stopped directly after the elaboration phase.
 *
 * \note For this to work, you need to use osss_start()
 *       instead of sc_start() in your sc_main().
 *
 * \see osss_start(), OSSS_ELABORATE, osss_elaboration
 */

// ensure definition of OSSS_ELABORATE,
// if OSSS_ELABORATE_ONLY is defined
#if defined( OSSS_ELABORATE_ONLY ) && !defined( OSSS_ELABORATE )
#  define OSSS_ELABORATE 1
#endif



class osss_system;

namespace osssi {

class osss_elaboration_helper;

/**
 * \class osss_elab_base
 * \brief base class for elaborator classes
 *
 * This class provides the common interface for the elaborator
 * classes.  You should derive from it, if you create your own
 * elaborator.
 *
 * \see osss_elaboration, OSSS_REGISTER_ELABORATOR
 */
class osss_elab_base
{
public:
  osss_elab_base() : helper_(0) { }
  /**
   * \brief interface to osss_elaboration singleton
   *
   * This method is called from osss_elaboration::elaborate().
   * Since it should not be overridden, it is not virtual.
   *
   * From within this function, the protected, pure-virtual
   * elaborate() function is called.  The derived class
   * has access to the contents of the osss_system instance
   * via the \c helper_ member.
   *
   */
  void elaborate( const osss_system& );
  virtual ~osss_elab_base();
protected:
  /**
   * \brief pure-virtual elaboration function
   *
   * Implement this function in your derived elaborator.
   * Information about the current \c osss_system can
   * be found in the \c helper_ member.
   *
   * \see osss_elaboration, OSSS_ELABORATE, osss_system
   */
  virtual void elaborate() = 0;
  /**
   * \brief information about current osss_system
   *
   * During the elaborate() call, this member holds
   * the information about the current osss_system
   * instance.
   *
   */
  const osss_elaboration_helper* helper_;
};

} /* namespace osssi */


/**
 * \class osss_elaboration
 * \brief OSSS elaboration mechanism
 *
 * This class is the main interface for the OSSS elaboration
 * mechanism.  It's a singleton, with which special elaborator
 * classes can be registered.  The registration is done with
 * the OSSS_REGISTER_ELABORATOR macro.
 *
 * During osss_system::end_of_elaboration() the member function
 * elaborate() calls the specific function of all statically
 * registered elaborator classes.
 *
 * In addition to the general SystemC API for hierarchy traversal
 * etc., the elaborators are given the main osss_system instance,
 * from which they are called.
 *
 * \see osss_system, osssi::osss_elab_base, OSSS_ELABORATE
 */
class osss_elaboration {

  typedef osssi::osss_elab_base  elab_t;
  typedef std::deque<elab_t*>    container_t;
  typedef container_t::iterator  iterator_t;

public:
  static osss_elaboration& instance();

  void elaborate( const osss_system& );
  void append( elab_t& );
  void release();
  ~osss_elaboration();

private:
  bool        done_;
  container_t elabs_;

  osss_elaboration();
  // disabled copy constructor
  osss_elaboration( const osss_elaboration& );
  // disabled assignment operator
  osss_elaboration& operator=( const osss_elaboration& );

}; /* osss_elaboration */

namespace osssi {

/**
 * \class osss_register_elaborator
 * \brief internal elaborator registration helper
 *
 * This class is an internal helper to enable the
 * registration of elaborator classes for the
 * execution from osss_elaboration.
 *
 * You should not need to use this class directly.
 * Register your elaborators with the
 * OSSS_REGISTER_ELABORATOR() macro.
 *
 * \see osss_elaboration, OSSS_REGISTER_ELABORATOR
 */
template< typename Elaborator >
class osss_register_elaborator
{
public:
  typedef Elaborator type;
private:
  osss_register_elaborator( );
  static  osss_register_elaborator reg_;
};

} // namespace osssi
} // namespace osss

/**
 * \def OSSS_REGISTER_ELABORATOR
 * \brief register an elaborator class with elaboration framework
 *
 * This macro registers an elaborator class with the main
 * elaboration singleton osss_elaboration.  Every registered
 * elaborator gets run during the main osss_system elaboration.
 *
 * \param Elab  class name of the elaborator
 *
 * \note  The registration has to be done within the public
 *        headers of the library, since otherwise the linker
 *        is allowed to omit the elaborator class in the
 *        resulting simulation binary.
 *
 * \see osss_elaboration, osss_register_elaborator
 *      OSSS_ELABORATE, OSSS_ELABORATE_ONLY
 */
#if defined( OSSS_ELABORATE )
#  define OSSS_REGISTER_ELABORATOR( Elab )            \
     template class                                   \
       ::osss::osssi::osss_register_elaborator< Elab >
#else
#  define OSSS_REGISTER_ELABORATOR( Elab ) \
     /* nothing */
#endif

#include "osss_elaboration.tpp"

#endif /* OSSS_ELABORATION_H */
// $Id: osss_elaboration.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
