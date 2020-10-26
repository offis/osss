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
#ifndef OSSS_O3S_UTILS_BLOCK_WRAPPER_H
#define OSSS_O3S_UTILS_BLOCK_WRAPPER_H
/**
 * \file block_wrapper.h
 *
 * \brief source-code annotation infrastructure
 *
 * $Date: 2008-07-07 18:12:52 +0200 (Mon, 07 Jul 2008) $
 * $Revision: 2423 $
 */

#include "osss_common.h"

#define OSSS_IMPL_BLOCK_WRAPPER_NAME_ \
  OSSS_IMPL_CONCAT_( osss_impl_block_wrapper_ , __LINE__ )

/**
 * \brief block annotation helper macro
 *
 * This class can be used to easily implement new
 * block-based source-code annotations.
 *
 * \param Type The type of the specific class, that
 *             performs the handling of the code annotation
 *             (see osss::osssi::block_wrapper).
 *
 * Any additional arguments are passed to the constructor
 * of the given type.
 *
 * \see osss::osssi::block_wrapper
 */
#define OSSS_IMPL_BLOCK_( Type, ... ) \
  if( Type OSSS_IMPL_BLOCK_WRAPPER_NAME_ \
        = Type( __VA_ARGS__ ) ) {} else

namespace osss {
namespace osssi {

/**
 * \brief helper class for source-code annotations
 *
 * This helper class can be used as a base class for
 * source-code annotations.
 *
 * On entrance of the annotated block (with some specific
 * annotation macro, see OSSS_IMPL_BLOCK_()), the constructor
 * of the concrete annotation class is executed an can perform its
 * task.  Once, the control flow leaves the block, the destructor
 * of the object can perform its house-keeping.
 *
 * Example:
 * \code
 *   class some_annotation_helper
 *     : public osss::osssi::block_wrapper
 *   {
 *     // constructor
 *     some_annotation_helper( arguments ) {
 *       // executed at beginning of annotated block
 *     }
 *     ~some_annotation_helper() {
 *       // executed at end of block
 *     }
 *   };
 *   #define OSSS_ANNOTATE( Arg1, Arg2 ) \
 *     OSSS_IMPL_BLOCK_( some_annotation_helper, Arg1, OtherArg, Arg2 )
 *
 *   // ... somewhere inside the user code
 *   OSSS_ANNOTATE( 20, SC_NS ) {
 *     // ... annotated source code here
 *   }
 * \endcode
 *
 * \see OSSS_IMPL_BLOCK_()
 */
class block_wrapper
{
  /// local (undefined) class for boolean operator
  class boolean_type_holder;
public:
  /// boolean return type of implicit conversion
  typedef boolean_type_holder* boolean_type;

  /**
   * \brief implicit conversion to false
   *
   * This operator enables the variable definition
   * inside an if condition.
   */
  operator boolean_type() const { return (boolean_type) 0; }
protected:
  /// protected constructor
  block_wrapper(){}
  /// protected destructor
  ~block_wrapper(){}
};

struct delegate_block_wrapper
  : block_wrapper
{
  struct handle
  {
    virtual void on_entry()       = 0;
    virtual void on_exit()        = 0;
    virtual ~handle(){}
  };

  template< typename T, void (T::*EntryFun)(), void (T::*ExitFun)() >
  struct handle_t
    : public handle
  {
    handle_t( T& dest )
      : dest_( dest ){}
    virtual void on_entry()
      { (dest_.*EntryFun)(); }
    virtual void on_exit()
      { (dest_.*ExitFun)(); }
  protected:
    T& dest_;
  };

protected:
  delegate_block_wrapper( handle* const );
  ~delegate_block_wrapper();
private:
  handle* const hnd_;
};

} // namespace osssi
} // namespace osss

#endif /* OSSS_O3S_UTILS_BLOCK_WRAPPER_H */
/* Taf!
 * $Id: block_wrapper.h 2423 2008-07-07 16:12:52Z philipph $
 * :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
 */
