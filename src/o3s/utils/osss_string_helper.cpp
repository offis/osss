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


#include "o3s/utils/osss_string_helper.h"
#include "o3s/utils/string_tokenizer.h"

// TODO: this DEFINE should be either set global in the make process
//       or configured at runtime (e.g. elaboration phase)
#define OSSS_SIMPLE_GUARDS

using std::string;
using std::vector;

namespace osss {
namespace osssi {

/*
 * transforms the string str into tokens separated by user
 * defined delimiters. The resulting tokens are returned by the tokens vector
 */
void tokenize(const string& str,
              vector<string>& tokens,
              const string& delimiters /* = " " */ )
{

  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}

//------------------------------------------------------------------------------

/*
 * counts the number of characters ch contained in string str
 */
unsigned int countChar(const string& str, const char& ch)
{
  unsigned int count = 0;
  for(unsigned int i=0; i<str.size(); i++)
  {
    if (str[i] == ch)
      count++;
  }
  return count;
}

//------------------------------------------------------------------------------

void StrSub(string& cp, 
	    const string& sub_this, 
	    const string& for_this, 
	    const int num_times)
{
  int i,loc;
  if (cp.empty())
  {
    cp = sub_this;
    return;
  }
  for (i = 0; i != num_times; i++)
  {
    loc = cp.find(for_this, 0);
    if (loc >= 0) 
      cp.replace(loc, for_this.length(), sub_this);
    else return;
  }
}

string removeClassName(const string& fullSignature, 
                       const string& fullClassName)
{
  string toBeRemoved = fullClassName + "::";
  string tmp_string  = fullSignature;
  StrSub(tmp_string, "", toBeRemoved, -1);
  return tmp_string;
}

//------------------------------------------------------------------------------

string transformToGuard(const string& fullSignature, 
                        const string& fullClassName,
                        const string& methodName)
{
  string_tokenizer strtok( removeClassName(fullSignature, fullClassName)
                         , methodName + string("(") );

  //std::cout << "Number Of Tokens: " << strtok.countTokens()     << std::endl;
  //std::cout << "String:           " << strtok.remainingString() << std::endl;

  // replace return type by "bool"
  string return_type = strtok.next();
  return_type = string("bool");

  // replace method name _stub_<...> by _guarded_stub_<...>
  std::string new_method_name = methodName;
  std::string old_prefix = "_stub_";
  std::string new_prefix = "_guarded_stub_";
  new_method_name.erase(0, old_prefix.length());
  new_method_name = new_prefix + new_method_name;
  
  #if defined(OSSS_SIMPLE_GUARDS)
  string parameter_list = string("()");
  #else
  string parameter_list = string("(") + strtok.next();
  #endif
  
  // build signature of guarded method
  return return_type + string(" ") + new_method_name + parameter_list;
}

//------------------------------------------------------------------------------

/*
 * excpects a trimmed method signature (see method above) and extracts
 * the type of the parameters.
 *
 * Trimmed parameter lists are of the form:
 *   (TypeName0*, TypeName1*, ..., TypeNameN*)
 * whereas parameters are always from pointer type.
 * Up to now no templated types are allowed for parameters.
 *
 * The above parameter list results in a vector of strings containing:
 *  0: TypeName0
 *  1: TypeName1
 *     ...
 *  N: TypeNameN
 *
 */
vector<string> extractTypesFromParameterList(const string& str)
{
  vector<string> type_vector;

  if (str == "()")
    return type_vector;

  // ignore brackets of c-style casts like " (unsigned int)xy"
  string::size_type firstPos = str.find_first_of("(");
  string parameter_list = str;
  const char& ch = str.at(firstPos-1);
  const char* ws = " ";
  if (ch == *ws)
  {
    while (firstPos != string::npos)
    {
      parameter_list = parameter_list.substr(firstPos + 1, 
                                             parameter_list.size());

      firstPos = parameter_list.find_first_of("(");
      const char& ch = parameter_list.at(firstPos-1);
      if (ch != *ws)
        break;
    }
  }

  // remove leading ( and closing ) tokens
  firstPos = parameter_list.find_first_of("(");
  string::size_type lastPos  = parameter_list.find_last_not_of(")");
  parameter_list = parameter_list.substr(firstPos + 1, lastPos - firstPos);

  string_tokenizer strtok( parameter_list
                         , "osss::osssi::osss_serialisable_wrapper<" );

  //std::cout << "Number Of Tokens: " << strtok.countTokens()     << std::endl;
  //std::cout << "String:           " << strtok.remainingString() << std::endl;

  while( strtok.has_next() )
  {
    string tempStr = strtok.next();

    // FIXME: Is it allowed to search for all chars at once?
    string::size_type
      end = tempStr.find_last_not_of( ' ' );
      end = tempStr.find_last_not_of( ',', end );
      end = tempStr.find_last_not_of( ' ', end );
      end = tempStr.find_last_not_of( '*', end );

    if( end != std::string::npos )
      tempStr.erase( end+1 );

    tempStr = "osss::osssi::osss_serialisable_wrapper<" + tempStr;
    type_vector.push_back(tempStr);
  }
  return type_vector;
}

} // namespace osssi

} //namespace osss

// $Id: osss_string_helper.cpp 2586 2008-08-26 13:01:24Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
