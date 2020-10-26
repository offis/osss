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

#include "message.h"
#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_assert.h"
#include "o3s/utils/osss_report.h"

#include <list>
#include <string>
#include <sstream>
#include <algorithm>

namespace osss {
namespace report {

/* ----------------------- implementation class ------------------------ */

class message::impl
{
public:
  class element;
  class substring;
  class replacer;

  explicit     impl( const char* );
  stream_type& inject();
  stream_type& append();
  std::string  combine() const;
  ~impl();

private:

  typedef std::list<element*>   snippets_type;
  typedef std::list<replacer*>  replacers_type;

  snippets_type                 snippets_;
  replacers_type                replacers_;
  replacers_type::iterator      current_;
  stream_type                   tail_;

  static const char replacer_pattern[];
  static const char replacer_unknown[];

};

const char message::impl::replacer_pattern[] = "%s";
const char message::impl::replacer_unknown[] = "<unknown>";

/* -------------------------- con/destruction -------------------------- */

message::message( const char* msg )
  : impl_( new impl(msg) )
{}

message::~message()
  { delete impl_; }

/* --------------------------- forward calls --------------------------- */

message::stream_type&
message::inject()
  { return impl_->inject();  }

message::stream_type&
message::append()
  { return impl_->append(); }

std::string
message::combine() const
  { return impl_->combine(); }

/* --------------------------- implementation -------------------------- */

class message::impl::element
{
public:
  virtual void print( message::stream_type& ) const = 0;
  virtual ~element(){}

}; // message::impl::element

class message::impl::substring
  : public message::impl::element
{
public:
  substring( const std::string& msg )
    : msg_( msg){}

  substring( const std::string&     msg,
             std::string::size_type from,
             std::string::size_type to    )
    : msg_( msg, from, to-from )
  {}

  virtual void print( message::stream_type& out ) const
    { out << msg_; }
private:
  const std::string msg_;
}; // message::impl::substring

class message::impl::replacer
  : public message::impl::element
{
public:
  replacer()
    : str_()
  {}

  message::stream_type& stream()
  {
    filled_ = true;
    return str_;
  }

  virtual void print( message::stream_type& out ) const
  {
    /// print replaced value, iff set, replacer_unknown otherwise
    if( filled_ ) {
      out << str_.str();
    } else {
      out << replacer_unknown;
    }
  }

private:
  std::stringstream str_;
  bool filled_;

}; // message::impl::replacer

message::impl::impl( char const * const msg_tpl )
  : snippets_()
  , replacers_()
  , current_( replacers_.end() )
  , tail_()
{
  const size_t pat_sz  = sizeof( replacer_pattern ) - 1;

  OSSS_ASSERT( msg_tpl );     // ensure valid message
  std::string msg = msg_tpl;  // temporary string

  std::string::size_type from = 0;
  std::string::size_type to   = 0;
  std::string::size_type last = msg.size();

  while( from < last ) {

    // look for next replacer/substring
    to = msg.find( replacer_pattern, from );
    bool pat_found = ( to != std::string::npos );

    if ( !pat_found ) { // no other pattern
      to = last;        // get remaining string
    }

    // store substring, if present
    if( to - from ) {
      snippets_.push_back( new substring( msg, from, to ) );
    }
    // next search start
    from = to;

    // we found a pattern, store pattern replacer
    if ( pat_found ) {
      replacer* rpl = new replacer();
      snippets_.push_back( rpl );
      replacers_.push_back( rpl ); // remember replacer for inject()
      from += pat_sz;             // skip placeholder
    }
  }
  // reseat beginning position
  current_ = (!replacers_.empty())
                ? replacers_.begin() : replacers_.end();
}

/* anonymous */ namespace {

/// small helper struct to delete a pointer
struct delete_helper
{
  template< typename Type >
  void operator()( const Type * ptr )
    { delete ptr; }
};

/// small helper struct to print from a pointer to a std::ostream
struct print_helper
{
  explicit print_helper( std::stringstream& out ) : out_(out) {}

  template< typename Type >
  void operator() ( const Type* const ptr )
    { ptr->print( out_ ); }
private:
    std::stringstream& out_;
};

} // anonymous namespace

std::string
message::impl::combine() const
{
  std::stringstream result;
  std::for_each( snippets_.begin(), snippets_.end(),
                 print_helper( result ) );
  result << tail_.str();
  return result.str();
}

message::stream_type&
message::impl::inject()
{
  OSSS_ASSERT( current_ != replacers_.end() );
  // return current replacer stream 
  stream_type& result = (*current_)->stream();
  // and increment marker
  ++current_;
  return result;
}


message::stream_type&
message::impl::append()
{
  return tail_;
}

message::impl::~impl()
{
  std::for_each( snippets_.begin(), snippets_.end(),
                 delete_helper() );
}

} // namespace report
} // namespace osss

/* Taf!
 * $Id: message.cpp 2392 2008-07-01 20:45:11Z philipph $
 * :flavour: (osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
 */
