// :external source:
/*
    AGM::LibReflection is a C++ reflection library.
    Version 0.8.
    Copyright (C) 2004 Achilleas Margaritis

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef _REFLECTION_DEFINE_HH_
#define _REFLECTION_DEFINE_HH_

#include "o3s/extern/reflection/reflection.h"
#include <memory>

namespace agm { namespace reflection {

#define __TYPE2TYPE_ARG__(N)  Type2Type<T##N> t##N


#define __REFLECTIVE_CLASS_CONSTRUCTOR__(N) \
  __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __TEMPLATE_DECLARE_BEGIN__, __TEMPLATE_END__) \
  ReflectiveClass & constructor (__REPEAT(N, __TYPE2TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__ )) \
  { \
    __callable__ *cb = __create_static_callable__(& new_instance##N <T __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__,  __NOTHING__)> ); \
    __callable__ *placement_cb = __create_static_callable__(__create_new_instance_ptr__(& new_instance##N <T __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__,  __NOTHING__)>) ); \
      std::string targ = getArgString(cb);\
    __register_constructor_method__ r(cb, placement_cb, m_cls_.get(), ACCESS_PUBLIC, cls_name(), cls_name(), targ.c_str()); \
      return *this; \
  }

#define __REFLECTIVE_CLASS_NEW_INSTANCE__(N) \
  __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __TEMPLATE_DECLARE_BEGIN__, __TEMPLATE_END__) \
  static T* constructor_new##N (__REPEAT(N, __ARG__, __COMMA__, __NOTHING__, __NOTHING__)) \
  { \
    return new T (__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__)); \
  }

#define __NEW_INSTANCE__(N) \
  template <class T  __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__) >\
  T* new_instance##N (__REPEAT(N, __ARG__, __COMMA__, __NOTHING__, __NOTHING__)) \
  { \
    return new T (__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__)); \
  }

    __NEW_INSTANCE__(0)
    __NEW_INSTANCE__(1)
    __NEW_INSTANCE__(2)
    __NEW_INSTANCE__(3)
    __NEW_INSTANCE__(4)
    __NEW_INSTANCE__(5)
    __NEW_INSTANCE__(6)
    __NEW_INSTANCE__(7)
    __NEW_INSTANCE__(8)
    __NEW_INSTANCE__(9)
    __NEW_INSTANCE__(10)
    __NEW_INSTANCE__(11)
    __NEW_INSTANCE__(12)
    __NEW_INSTANCE__(13)
    __NEW_INSTANCE__(14)
    __NEW_INSTANCE__(15)
    __NEW_INSTANCE__(16)
    __NEW_INSTANCE__(17)
    __NEW_INSTANCE__(18)
    __NEW_INSTANCE__(19)
    __NEW_INSTANCE__(20)

  template <typename T>
  struct Type2Type
  {
    typedef T ResultT;
  };
    
  template <typename T>
  class ReflectiveClass
  {
  private:
  template <typename SuperT>
    static void * __super_cast(void *p)
    {
      return dynamic_cast<T *>(static_cast<SuperT *>(p));
    }
  
    template <typename SuperT>
    static const void * __super_cast_const(const void *p)
    {
      return dynamic_cast<const T *>(static_cast<const SuperT *>(p));
    }
  


  public:
    ReflectiveClass()
    : m_cls_()
    {
      init(cls_name(),
           0, /* no super */
           &ReflectiveClass::__null_super_cast,
           &ReflectiveClass::__null_super_cast_const);
    }

    ReflectiveClass(const std::string& name)
    : m_cls_()
    {
      init(name.c_str(),
           0, /* no super */
           &ReflectiveClass::__null_super_cast,
           &ReflectiveClass::__null_super_cast_const);
    }

    template <typename SuperT >
    ReflectiveClass(const SuperT *, const Class* supercls = 0)
    : m_cls_()
    {
      supercls = resolve( (SuperT*)0, supercls);
      init(cls_name(),
           supercls,
           & __super_cast<SuperT>,
           & __super_cast_const<SuperT>);
    }

    template <typename SuperT >
    ReflectiveClass(const std::string &name, const SuperT*, const Class *supercls = 0)
    : m_cls_()
    {
      supercls = resolve( (SuperT*)0, supercls);
      init(name.c_str(),
           supercls,
           &__super_cast<SuperT>,
           &__super_cast_const<SuperT>);
    }

    ReflectiveClass(ReflectiveClass & that) /* transfer ownership */
    : m_cls_(that.m_cls_)
    {
    }

    inline const Class * get() const
    {
      return m_cls_.get();
    }

    inline const Class & operator *() const
    {
      return *m_cls_;
    }

    inline const Class * operator -> () const
    {
      return get();
    }

    template <typename C>
    ReflectiveClass & field(const C T::* fieldptr, const char *name, ACCESS_TYPE acctype = ACCESS_PUBLIC)
    {
      unsigned long offset = ((unsigned long)&(((const T*)1024) ->* fieldptr)) - 1024;
      std::string tname = demangle<C>();
      __register_field__ r(offset, typeid(C), m_cls_.get(), acctype, tname.c_str(), name);
      return *this;
    }

    template <typename C>
    ReflectiveClass & static_field(const C *fieldptr, const char *name, ACCESS_TYPE acctype = ACCESS_PUBLIC)
    {
      std::string tname = demangle<C>();
      __register_static_field__ r(const_cast<C*>(fieldptr), typeid(C), m_cls_.get(), acctype, tname.c_str(), name);
      return *this;
    }

  

    template <typename M>
    ReflectiveClass & method(M methodptr, const char *name, ACCESS_TYPE acctype = ACCESS_PUBLIC)
    {
      __callable__ *cb = __create_callable__(methodptr);
      std::string tname = unmangle(cb->get_ret_type().name());
      std::string targ = getArgString(cb);
      __register_method__ r(cb, m_cls_.get(), acctype, tname.c_str(), name, targ.c_str(), "");
        return *this;
    }

    template <typename M>
    ReflectiveClass & static_method(M methodptr, const char *name, ACCESS_TYPE acctype = ACCESS_PUBLIC)
    {
      __callable__ *cb = __create_static_callable__(methodptr);
      std::string tname = unmangle(cb->get_ret_type().name());
       std::string targ = getArgString(cb);
     __register_static_method__ r(cb, m_cls_.get(), acctype, tname.c_str(), name, targ.c_str());
        return *this;
    }

    template <typename C>
    ReflectiveClass & property(C (T::*getter)() const, void (T::*setter)(C), const char *name, ACCESS_TYPE acctype = ACCESS_PUBLIC)
    {
      std::string tname = demangle<C>();
      __register_property__ r(__create_property__<T, C>(getter, setter), this->m_cls_.get(), tname.c_str(), name, typeid(C) );
      return *this;
    }
    
    template <typename M>
    ReflectiveClass & constructor (M methodptr, ACCESS_TYPE acctype = ACCESS_PUBLIC)
    {
      __callable__ *cb = __create_static_callable__( methodptr );
      __callable__ *placement_cb = __create_static_callable__( __create_new_instance_ptr__ (methodptr));
      std::string targ = getArgString(cb);
      __register_constructor_method__ r(cb, placement_cb, m_cls_.get(), acctype, cls_name(), cls_name(), targ.c_str());
      return *this;
    }

    template <typename M1, typename M2>
    ReflectiveClass & constructor (M1 methodptr, M2 placement_methodptr, ACCESS_TYPE acctype = ACCESS_PUBLIC)
    {
      __callable__ *cb = __create_static_callable__( methodptr );
      __callable__ *placement_cb = __create_static_callable__( placement_methodptr );
      std::string targ = getArgString(cb);
      __register_constructor_method__ r(cb, placement_cb, m_cls_.get(), acctype, cls_name(), cls_name(), targ.c_str());
      return *this;
    }

    __REFLECTIVE_CLASS_CONSTRUCTOR__(0)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(1)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(2)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(3)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(4)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(5)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(6)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(7)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(8)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(9)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(10)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(11)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(12)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(13)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(14)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(15)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(16)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(17)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(18)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(19)
    __REFLECTIVE_CLASS_CONSTRUCTOR__(20)

  private:
  
__NEWINSTANCE_PTR_CALLER__(T,0)
__NEWINSTANCE_PTR_CALLER__(T,1)
__NEWINSTANCE_PTR_CALLER__(T,2);
__NEWINSTANCE_PTR_CALLER__(T,3);
__NEWINSTANCE_PTR_CALLER__(T,4);
__NEWINSTANCE_PTR_CALLER__(T,5);
__NEWINSTANCE_PTR_CALLER__(T,6);
__NEWINSTANCE_PTR_CALLER__(T,7);
__NEWINSTANCE_PTR_CALLER__(T,8);
__NEWINSTANCE_PTR_CALLER__(T,9);
__NEWINSTANCE_PTR_CALLER__(T,10);
__NEWINSTANCE_PTR_CALLER__(T,11);
__NEWINSTANCE_PTR_CALLER__(T,12);
__NEWINSTANCE_PTR_CALLER__(T,13);
__NEWINSTANCE_PTR_CALLER__(T,14);
__NEWINSTANCE_PTR_CALLER__(T,15);
__NEWINSTANCE_PTR_CALLER__(T,16);
__NEWINSTANCE_PTR_CALLER__(T,17);
__NEWINSTANCE_PTR_CALLER__(T,18);
__NEWINSTANCE_PTR_CALLER__(T,19);
__NEWINSTANCE_PTR_CALLER__(T,20);

    template <typename SuperT>
    static const Class * resolve(const SuperT *, const Class *supercls)
    {
      if (supercls == 0) {
        const std::type_info& super_tid = typeid(SuperT);
        supercls = Class::forType(super_tid);
        if (supercls == 0) {
          supercls = new Class_Holder(super_tid);
        }
      }
      return supercls;
    }



    static std::string getArgString(__callable__*cb)
    {
      std::string res = "(";
      int count = cb->get_args_count();
      const __callable__::arg_list_type& v = cb->get_args();
      for (int i=0; i<count; i++)
      {
        if (i) {
          res += ", ";
        }
        res += unmangle(v[i]->name());
      }
      res += ')';
      return res;
    }


    void init(const char *name,
              const Class *super,
              Class::_super_cast_fun super_cast,
              Class::_super_cast_const_fun super_cast_const)
    {
      m_cls_.reset(new Class(name,
                             super,
                             sizeof(T),
                             0,
                             super_cast,
                             super_cast_const,
                             typeid(T),
                             typeid(const T),
                             typeid(T*),
                             typeid(const T*),
                             REFLECTION_TYPE_CUSTOM1(T),
                             REFLECTION_TYPE_CUSTOM2(T),
                             REFLECTION_TYPE_CUSTOM3(T),
                             REFLECTION_TYPE_CUSTOM4(T)
                             ));
    }

    static const Class * findClass(const std::type_info& tinfo)
    {
      return Class::forType(tinfo);
    }

    static void * __null_super_cast(void *p)
    {
      return 0;
    }

    static const void * __null_super_cast_const(const void *p)
    {
      return 0;
    }

    static std::string calculateClassName()
    {
      std::string s = unmangle(typeid(T).name());
      // remove namespace
      std::string::size_type pos;
      if ((pos = s.rfind(':')) != std::string::npos)
      {
        s = s.substr(pos + 1);
      }
      // remove template 
      if ((pos = s.find('<')) != std::string::npos)
      {
        s = s.substr(0, pos);
      }
      return s;
    }
    
    static const char *cls_name()
    {
      static std::string s_cls_name = calculateClassName();
      return s_cls_name.c_str();
    }
  

    std::auto_ptr<Class> m_cls_;

  };

}}

#endif
// $Id: refcls.h 1766 2008-01-10 13:38:39Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
