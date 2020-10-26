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

#ifndef OSSS_OBJECT_REGISTRY_HH
#define OSSS_OBJECT_REGISTRY_HH

#include "o3s/structural/app/osss_shared_if.h"
#include "o3s/utils/osss_registry_shared.h"

#include "o3s/utils/osss_string_helper.h"

//C++ reflection library
#include "o3s/extern/reflection/refcls.h"

namespace osss {
namespace osssi {

struct method_registry_item_t
{
  unsigned int methodID;
  bool hasParameters;
  bool hasReturnValue;
};

class osss_object_registry;

class osss_method_registry
{

  friend class osss_object_registry;
  friend std::ostream& operator<< (std::ostream& ostr,
                                   const osss_method_registry& mreg);

public:
  osss_method_registry()
  {
    m_index = 1;
    m_class_name = std::string("???");
  }

  std::string getSignature(unsigned int methodID)
  {
    for (std::map<const std::string, method_registry_item_t>::iterator it =
           m_method_map.begin();
         it != m_method_map.end();
         ++it)
    {
      if ((*it).second.methodID == methodID)
        return (*it).first;
    }
    return std::string("");
  }

  bool hasParameters(unsigned int methodID)
  {
    for (std::map<const std::string, method_registry_item_t>::iterator it =
           m_method_map.begin();
         it != m_method_map.end();
         ++it)
    {
      if ((*it).second.methodID == methodID)
        return (*it).second.hasParameters;
    }
    //we should never get here
    return false;
  }

  bool isVoid(unsigned int methodID)
  {
    for (std::map<const std::string, method_registry_item_t>::iterator it =
           m_method_map.begin();
         it != m_method_map.end();
         ++it)
    {
      if ((*it).second.methodID == methodID)
        return !(*it).second.hasReturnValue;
    }
    //we should never get here
    return true;
  }

  unsigned int getMethodID(std::string signature)
  {
    return (*m_method_map.find(signature)).second.methodID;
  }

protected:
  template<class T>
  void addClass(T *object)
  {
    using namespace agm::reflection;

    m_method_map.clear();
    m_index = 1;

    m_class_name = object->getClass().getFullName();

    //
    // CAUTION: this is only experimental code !!!
    //
    if (object->getClass().hasSuper())
    {
      const Class* super_class = object->getClass().super();
      std::string super_class_name = super_class->getFullName();
      Class::MethodList methodList = super_class->getMethods();
      for (Class::MethodList::const_iterator iter = methodList.begin();
           iter != methodList.end();
           ++iter)
      {
        addSignature(removeClassName((*iter)->getSignature(), super_class_name),
                     ((*iter)->getArgsCount()>0),
                     (*iter)->hasReturnType());
      }
    }


    Class::MethodList methodList = object->getClass().getMethods();
    for (Class::MethodList::const_iterator iter = methodList.begin();
         iter != methodList.end();
         ++iter)
    {
      std::string signature = 
        removeClassName((*iter)->getSignature(), m_class_name);
      bool hasParameters = ((*iter)->getArgsCount()>0);
      bool hasReturnValue = (*iter)->hasReturnType();
      addSignature(signature, hasParameters, hasReturnValue);
    }
  }

private:
  void addSignature(std::string signature, 
                    bool hasParameters, 
                    bool hasReturnValue)
  {
    method_registry_item_t item;
    item.methodID = m_index;
    item.hasParameters = hasParameters;
    item.hasReturnValue = hasReturnValue;
    m_method_map[signature] = item;
    m_index++;
  }

  unsigned int m_index;
  std::string m_class_name;
  std::map<const std::string, method_registry_item_t> m_method_map;
};

inline
std::ostream& operator<< (std::ostream& ostr,
                          const osss_method_registry& mreg)
{
  ostr << "registred methods for class: " << mreg.m_class_name << std::endl;
  ostr << "method signature -> methodID" << std::endl;
  ostr << "----------------------------" << std::endl;
  for (std::map<const std::string, method_registry_item_t>::const_iterator it =
         mreg.m_method_map.begin();
       it != mreg.m_method_map.end();
       ++it)
  {
    ostr << (*it).first << " -> " << (*it).second.methodID << std::endl;
  }

  return ostr;
}

//------------------------------------------------------------------------------

class osss_object_registry
{
  friend std::ostream& operator<< (std::ostream& ostr,
                                   const osss_object_registry& oreg);

public:
  osss_object_registry() :
    m_index(1),
    m_methodID_map(),
    m_objectID_map()
  {}

  ~osss_object_registry()
  {
    for (methodID_map_iterator_t iter = m_methodID_map.begin();
         iter != m_methodID_map.end();
         ++iter)
    {
      delete iter->second;
    }
  }

  template<class T>
  void addClass(T* object)
  {
    osss_method_registry *mreg = new osss_method_registry();
    mreg->addClass(object);
    // get osss_shared_if<>* of surrounding SO
    osss_shared_if<sc_core::sc_interface>* sif =
      osssi::osss_registry_shared::get_shared_object((void*)object);
    if (sif != NULL)
    {
      m_methodID_map[sif] = mreg;
      m_objectID_map[sif] = m_index;
      m_index++;
    }
    else
    {
      std::cerr << "Error in "
                << __PRETTY_FUNCTION__
                << " the user class @addr: " << (void*)object
                << " is not wrapped by a Shared Object" << std::endl;
      OSSS_ABORT();
    }
  }


  template<class T>
  unsigned int getObjectID(T *object)
  {
    // check whether T is a Shared Object
    osss_shared_if<>* sif = dynamic_cast<osss_shared_if<>*>(object);
    if (sif == NULL)
    {
      // check for user object plugged inside a Shared Object
      sif = osssi::osss_registry_shared::get_shared_object((void*)object);
    }

    if (sif != NULL)
    {
      objectID_map_iterator_t iter = m_objectID_map.find(sif);
      if (iter != m_objectID_map.end())
        return iter->second;
    }
    return 0; // this indicates an invalid ObjectID
  }

  const void* getObject(unsigned int objectID)
  {
    for (objectID_map_iterator_t iter = m_objectID_map.begin();
         iter != m_objectID_map.end();
         ++iter)
    {
      if (iter->second == objectID)
      {
        const osss_shared_if<sc_core::sc_interface>* sif = iter->first;
        const void* user_obj_ptr = 
          osssi::osss_registry_shared::get_plugged_object(sif);
        if (user_obj_ptr != NULL)
          return user_obj_ptr;
      }
    }
    return NULL;
  }

  template<class T>
  osss_method_registry* getMethodRegistry(T *object)
  {
    // check whether T is a Shared Object
    osss_shared_if<>* sif = dynamic_cast<osss_shared_if<>*>(object);
    if (sif == NULL)
    {
      // check for user object plugged inside a Shared Object
      sif = osssi::osss_registry_shared::get_shared_object((void*)object);
    }

    if (sif != NULL)
    {
      methodID_map_iterator_t iter = m_methodID_map.find(sif);
      if (iter != m_methodID_map.end())
        return iter->second;
    }
    return 0; // this indicates an invalid ObjectID
  }

  osss_method_registry* getMethodRegistry(unsigned int objectID)
  {
    methodID_map_iterator_t method_iter = m_methodID_map.begin();
    for (objectID_map_iterator_t obj_iter = m_objectID_map.begin();
         obj_iter != m_objectID_map.end();
         ++obj_iter)
    {
      if (obj_iter->second == objectID)
        return method_iter->second;
      else
        method_iter++;
    }
    return NULL;
  }

private:
  // this registry can NOT be copied
  osss_object_registry(const osss_object_registry&);
  osss_object_registry& operator=(const osss_object_registry&);

  unsigned int m_index;
  std::map<const osss_shared_if<sc_core::sc_interface>*,
           osss_method_registry*> m_methodID_map;

  typedef std::map<const osss_shared_if<sc_core::sc_interface>*,
                   osss_method_registry*>::iterator methodID_map_iterator_t;

  typedef std::map<const osss_shared_if<sc_core::sc_interface>*,
    osss_method_registry*>::const_iterator
    methodID_map_const_iterator_t;

  std::map<const osss_shared_if<sc_core::sc_interface>*,
           unsigned int> m_objectID_map;

  typedef std::map<const osss_shared_if<sc_core::sc_interface>*,
                   unsigned int>::iterator objectID_map_iterator_t;

  typedef std::map<const osss_shared_if<sc_core::sc_interface>*,
                   unsigned int>::const_iterator objectID_map_const_iterator_t;
};

inline
std::ostream& operator<< (std::ostream& ostr,
                          const osss_object_registry& oreg)
{
  ostr << "object registry:" << std::endl;
  ostr << "================" << std::endl;
  osss_object_registry::objectID_map_const_iterator_t oit =
    oreg.m_objectID_map.begin();
  for (osss_object_registry::methodID_map_const_iterator_t mit =
         oreg.m_methodID_map.begin();
       mit != oreg.m_methodID_map.end();
       ++mit)
  {
    ostr << "object addr: " << oit->first << " -> " << oit->second << std::endl;
    ostr << *(mit->second);
    ostr << std::endl;
    oit++;
  }

  return ostr;
}

} // namespace osssi
} // namespace osss

#endif
// $Id: osss_object_registry.h 2991 2009-01-23 14:49:29Z henning $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
