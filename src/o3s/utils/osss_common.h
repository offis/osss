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
#ifndef OSSS_COMMON_H_INCLUDED_
#define OSSS_COMMON_H_INCLUDED_
/**
 * \file osss_common.h
 *
 * \brief common include file for OSSS library
 *
 * This file should be included first in every OSSS source, resp.
 * header file. Some compiler detections and other preliminary
 * things are done here.
 *
 * $Date: 2009-03-27 14:10:49 +0100 (Fri, 27 Mar 2009) $
 * $Revision: 3080 $
 */

/* ------------------------ library version ----------------------- */

/**
 * \brief small helper macro for version comparisons
 *
 * returns a (potentially preprocessor checkable) constant
 * for a given version triple (numbers only)
 *
 * \note Each part of the version is has to be smaller
 *       than 256, otherwise the returned constant is no
 *       longer unique.
 */
#define OSSS_MAKE_VERSION(x,y,z) \
  (((a)<<16) + ((b)<<8) + (c))

// should be changed the following to in the future:
// #include "osss_version.h"

/* --------------------------- platform --------------------------- */

/* are we doing hardware synthesis? */
#if defined( SC_SYNTHESIS )

# define  OSSS_SYNTHESIS_ SC_SYNTHESIS

/* Linux */
#elif defined(__linux__)
# define OSSS_LINUX_ __linux__

/* Sun OS */
#elif defined(sun) || defined( __sun__ )
# define OSSS_SUNOS_ __sun__

/* CygWin */
#elif defined(__CYGWIN__) || defined(__CYGWIN32)
# define OSSS_CYGWIN_ 1

/* 32-bit Windows */
#elif defined(_WIN32)
# define OSSS_WIN32_ _WIN32
#elif defined(WIN32)
# define OSSS_WIN32_ WIN32

#endif /* platform selection */

/* --------------------------- compiler --------------------------- */

/* Intel's compiler */
#if defined(__INTEL_COMPILER)
# define OSSS_ICC_ __INTEL_COMPILER
#elif defined(__ICC)
# define OSSS_ICC_ __ICC
#endif

/* MinGW */
#if defined(__MINGW32__)
# define OSSS_MINGW_ __MINGW32__
# if !defined(OSSS_WIN32_)
#  define OSSS_WIN32_ OSSS_MINGW_
# endif
#endif

/* GCC C++ compiler (gets set by Intel compiler as well) */
#if defined(__GNUC__)
# define OSSS_GCC_ \
   OSSS_MAKE_VERSION(__GNUC__,__GNUC_MAJOR__,__GNUC_PATCHLEVEL__)
#endif

/* Microsoft Visual C++ compiler */
#if defined(_MSC_VER)
# define OSSS_MSC_ _MSC_VER
#elif defined(MSC_VER)
# define OSSS_MSC_ MSC_VER
#endif
#if defined(OSSSI_MSC_)
# if ( OSSS_MSC_ >= 1310 ) /* Visual C++ 7.0, resp. .NET 2003 */
#  define OSSS_MSC_NET_ OSSS_MSC_
# else
#  error Microsoft Visual C++ prior to version 7.1 (.NET 2003) is \
         unsupported. Please upgrade or choose another platform.
# endif
#endif

/* ---------------------------- debug ----------------------------- */

/* build optimized version */
#ifdef NDEBUG
/* disable debugging, if not requested explicitly */
# ifndef OSSS_DEBUGLEVEL
#  define OSSS_DEBUGLEVEL 0
# endif
#endif /* NDEBUG */

/* build debug version */
#ifdef DEBUG
/* enable minimum debugging, if not requested explicitly */
# ifndef OSSS_DEBUGLEVEL
#   define OSSS_DEBUGLEVEL 1
# endif
#endif /* DEBUG */

/* ---------------------- branch prediction ----------------------- */

#if !defined( OSSS_GCC_ ) || (OSSS_GCC_ < OSSS_MAKE_VERSION(2,96,0))
// no branch prediction available here

  /**
   * \def osss_likely()
   * \brief branch prediction helper
   *
   * This macro can be used to give a hint to some compilers, that
   * the truth value of the given expression is more likely to
   * be \c true. The compiler can than generate appropriate
   * branch-prediction friendly code.
   *
   * \note Currently, this has no effect on compilers other than
   *       GNU C++ Compiler >= 2.96.
   *
   * \param   x Expression to be checked
   * \return  truth value of x
   *
   * This technique was inspired by the Linux kernel macros
   * likely() and unlikely().
   *
   * \see osss_unlikely(),
   *      http://www.geocities.com/kedarsovani/pubs/likely_unlikely.html
   */
#  define osss_likely(x)   !!(x)
  /**
   * \def osss_unlikely()
   * \brief branch prediction helper
   *
   * This macro can be used to give a hint to some compilers, that
   * the truth value of the given expression is more likely to
   * be \c false. The compiler can than generate appropriate
   * branch-prediction friendly code.
   *
   * \note Currently, this has no effect on compilers other than
   *       GNU C++ Compiler >= 2.96.
   *
   * \param   x Expression to be checked
   * \return  truth value of x
   *
   * This technique was inspired by the Linux kernel macros
   * likely() and unlikely().
   *
   * \see osss_likely(),
   *      http://www.geocities.com/kedarsovani/pubs/likely_unlikely.html
   */
#  define osss_unlikely(x) !!(x)
#else
// use the optimized expressions
#  define osss_likely(x)   __builtin_expect(!!(x),1)
#  define osss_unlikely(x) __builtin_expect(!!(x),0)
#endif

/* ---------------------- token concatenation --------------------- */

/**
 * \def OSSS_IMPL_CONCAT_( First, Second )
 * \brief concatenates two arguments
 *
 * Internal macro to concatenate two preprocessor tokens.
 *
 * \return arguments joined to a single token
 */
#define OSSS_IMPL_CONCAT_( First, Second ) \
  OSSS_IMPL_CONCAT_DEFERRED_( First, Second )

/**
 * \def OSSS_IMPL_CONCAT_DEFERRED_( First, Second )
 * \brief (first) concatenation helper
 *
 * Internal macro, do not use directly.
 *
 * \see OSSS_IMPL_CONCAT_( First, Second )
 */
#define OSSS_IMPL_CONCAT_DEFERRED_( First, Second ) \
  OSSS_IMPL_CONCAT_EVEN_MORE_DEFERRED_( First, Second )

/**
 * \def OSSS_IMPL_CONCAT_EVEN_MORE_DEFERRED_( First, Second )
 * \brief (second) concatenation helper
 *
 * Internal macro, do not use directly.
 *
 * \see OSSS_IMPL_CONCAT_( First, Second )
 */
#define OSSS_IMPL_CONCAT_EVEN_MORE_DEFERRED_( First, Second ) \
  First ## Second

/* ---------------------------- misc ------------------------------ */

#if defined (OSSS_MSC_)

/* In Visual C++ __PRETTY_FUNCTION__ and __func__ are not available,
   so we use __FUNCTION__ here instead */
#  define __func__ __FUNCTION__
#  define __PRETTY_FUNCTION__ __FUNCTION__

/* C++ exception specification ignored except to indicate
 * a function is not __declspec(nothrow) */
#pragma warning( disable : 4290 ) 

#endif /* OSSS_MSC_ */

/**
 * \namespace osss
 * \brief main OSSS library namespace
 *
 * This is the public namespace of the OSSS simulation library.
 * All elements of this library are encaspulated within this namespace.
 *
 * The only exception are source files from external vendors, like Boost,
 * or OSCI (of course).  In this case, the upstream classes remain in
 * their original namespace (provided they where shipped with such a
 * namespace).
 *
 * As a rule of thumb, the elements of the \c osss namespace can be
 * considered as the public API of the OSSS simulation library.
 * Implementation-specific elements are usually moved to the
 * nested namespace \c osss::osssi.
 *
 * \warning Since this library is currently under development, please
 *          do not rely on the contents of this namespace.
 *          For explicitly public elements, please refer to the
 *          exported symbols from within osss.h.
 *
 * \see osss.h
 */
namespace osss { /* here for documentation only */

/**
 * \namespace osss::osssi
 * \brief internal OSSS library namespace
 *
 * This namespace contains implementation-specific elements of
 * the OSSS simulation library.  Usually, you should not need to
 * bother with its contents.
 *
 * \todo    The migration of all purely implementation-specific
 *          elements to this namespace is not complete, yet
 *          (see \ticket{27})
 *
 * \warning Do not use any element of this namespace in any OSSS model
 *          directly, since new versions of the OSSS library may change
 *          any element within this namespace at any time.
 *
 */
namespace osssi { /* here for documentation only */ }

} /* namespace osss */


/* --------------------- hardware synthesis ----------------------- */

#if defined( OSSS_SYNTHESIS_ )

#define OSSS_SYNTHESISABLE_BEGIN_ \
    namespace fossy_synthesisable {

#define OSSS_SYNTHESISABLE_END_   \
    } /* end namespace fossy_synthesisable */

#define OSSS_SYNTHESISABLE_EXPORT_( Symbol ) \
    using fossy_synthesisable:: ## Symbol

#else /* ! OSSS_SYNTHESIS_ */

#define OSSS_SYNTHESISABLE_BEGIN_            /* ignore */
#define OSSS_SYNTHESISABLE_END_              /* ignore */
#define OSSS_SYNTHESISABLE_EXPORT_( Symbol ) /* ignore */

#endif /* OSSS_SYNTHESIS_ */

#endif /* OSSS_COMMON_H_INCLUDED_ */

/* Taf!
 * $Id: osss_common.h 3080 2009-03-27 13:10:49Z philipph $
 * :flavour: (all,H)
 */
