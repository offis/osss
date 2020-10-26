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


#ifdef MSVC
#pragma warning (disable: 4786)
#pragma warning (disable: 4003)
#endif

#include <sstream>

#if __GNUC__ > 3 || \
    ( __GNUC__ == 3 && __GNUC_MINOR__ > 0 )
#define GCC_USEDEMANGLE
#endif

#ifdef GCC_USEDEMANGLE
#include <cstdlib>
#include <cxxabi.h>
#endif


namespace agm { namespace reflection {

inline
std::string unmangle(const char * mangledName)
{
#ifdef GCC_USEDEMANGLE
    /*  for whatever reason, the ABI::CXA in gcc doesn't do the demangle if it is 
	single char name
      v void
      c char
      h unsigned char
      a signed char
      i int
      j unsigned int
      i signed int
      s short
      l long
      x long long
      f float
      d double
      e long double
    */
    if (mangledName[0] != '\0' && mangledName[1] == '\0') {
	switch (mangledName[0]) {
	    case 'v': return "void";
	    case 'c': return "char";
	    case 'h': return "unsigned char";
	    case 'a': return "signed char";
	    case 'i': return "int";
	    case 'j': return "unsigned int";
		//	    case 'i': return "signed int";
	    case 's': return "short";
	    case 'l': return "long";
	    case 'x': return "long long";
	    case 'f': return "float";
	    case 'd': return "double";
	    case 'e': return "long double";
	    case 'b': return "bool";
	    default:
		break;
	}
    }

    std::string result;
    int status;
    char* unmangled = abi::__cxa_demangle(mangledName, 0, 0, &status);
    if(status == 0)
    {
        result = std::string(unmangled);
        free(unmangled);
    }
    else
    {
        // Keep mangled name if result is not 0
        result = std::string(mangledName);
    }
    return result;
#else
    return std::string(mangledName);
#endif
}

inline
MemberBase::MemberBase(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name) :
        m_class(pclass),
        m_access(access),
        m_type(type), 
	m_name(name)
{
}

/**assignment operator
 */
inline
MemberBase& MemberBase::operator = (const MemberBase& other) {
    m_class = other.m_class;
    m_access = other.m_access;
    m_type = other.m_type;
    m_name = other.m_name;
    return *this;
}

inline
MemberBase::MemberBase(const MemberBase & other)
: m_class(other.m_class),
      m_access(other.m_access),
      m_type(other.m_type), 
      m_name(other.m_name)
{
}



//default constructor
inline
Field::Field(unsigned long offset, const std::type_info &typeinfo, const Class *pclass, ACCESS_TYPE access, const char *type, const char *name) :
MemberBase(pclass,access,type,name),
    m_typeinfo(typeinfo),
    m_offset(offset) {
}



//default constructor
inline
StaticField::StaticField(void *address, const std::type_info &typeinfo, const Class *pclass, ACCESS_TYPE access, const char *type, const char *name) :
    MemberBase(pclass,access,type,name), 
        m_typeinfo(typeinfo),
	m_address(address) {
    }


/** retrieve argument parameter information
    @return argument parameters
*/
inline
const MethodBase::ArgumentTypeList & MethodBase::getArgsTypes() const {
    if (m_callable == 0) throw InvalidStateError(std::string(getName()) +  " not initialized");
    return m_callable->get_args();
}

inline
int MethodBase::getArgsCount() const {
    if (m_callable == 0) throw InvalidStateError(std::string(getName()) +  " not initialized");
    return m_callable->get_args_count();
}

inline
const std::type_info& MethodBase::getReturnType() const {
    if (m_callable == 0) throw InvalidStateError(std::string(getName()) +  " not initialized");
    return m_callable->get_ret_type();
}

inline
MethodBase& MethodBase::operator = (const MethodBase& other) {
    this->MemberBase::operator = (other);
    m_callable = other.m_callable;
    m_id = other.m_id;
    m_args = other.m_args;
    m_long_id = other.m_long_id;
    m_own_cb = false; /* no ownership */
    return *this;
}

inline
MethodBase::MethodBase(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name, const char *args) :
    MemberBase(pclass,access,type,name),
        m_callable(0),
	m_id(name),
	m_args(args),
	m_long_id(name),
	m_own_cb(false)
        {
        m_id += args;
	}


inline
MethodBase::MethodBase(const MethodBase& other)
: MemberBase(other),
      m_callable(other.m_callable),
      m_id(other.m_id),
      m_args(other.m_args),
      m_long_id(other.m_long_id),
      m_own_cb(false) /* explicit set to false; no ownership of cb pointer */
{
}

inline
void MethodBase::set_callable(__callable__ *cb) {
	typedef __callable__::arg_list_type arglist;
	if (m_own_cb) delete m_callable;
	m_callable = cb;
	if (cb) {
	    m_id = getName();
	    m_long_id = getPrefix(cb->get_ret_type());
	    m_long_id += '(';
	    if (cb->get_args_count() == 0) {
		m_id += "__";
		m_id += typeid(void).name();
	    } else {
		const arglist& list = cb->get_args();
		arglist::const_iterator i = list.begin();
		bool notfirst = false;
		while (i != list.end()) {
		    const std::type_info* ti = *i++;
		    m_id += "__";
		    m_id += ti->name();
		    if (notfirst) m_long_id += ", ";
		    m_long_id += unmangle(ti->name());
		    notfirst = true;
		}
	    }
	    m_long_id += ')';
	}
	m_own_cb = true;
    }


inline
Method::~Method() {
}


//default constructor
inline
Method::Method(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name, const char *args, bool virt) :
MethodBase(pclass,access,type,name,args),
    m_virtual(virt)
{
}


//default constructor
inline
StaticMethod::StaticMethod(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name, const char *args) :
MethodBase(pclass,access,type,name,args)
{
}



//default constructor
inline
ConstructorMethod::ConstructorMethod(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name, const char *args, __callable__* placement_callable) :
StaticMethod(pclass,access,type,name,args), m_placement_callable(placement_callable)
{
}

///destructor
inline
Property::~Property() {
    if (m_handler) delete m_handler;
}

//default constructor
inline
Property::Property(const Class *pclass, const char *type, const char *name, const std::type_info & typeinfo) :
MemberBase(pclass, ACCESS_PUBLIC, type, name),
    m_handler(0),
    m_typeinfo(typeinfo)
{
}

template <typename T>
inline
void deleteAndClear(std::vector<const T*>& x)
{
  for (typename std::vector<const T*>::iterator i = x.begin();
       i != x.end();
       ++i)
  {
    delete (const_cast<T*>(*i));
  }
  x.clear();
}

#if 0
Class::ClassList Class::s_all_classes;
Class::ClassMap Class::s_class_maps[Class::NameMap_SIZE];
Class::TypeInfoMap Class::s_type_maps[Class::Type_NotFound];
#endif


//the default constructor
inline
Class::Class(const char *name,
             const Class *super,
             size_t s, _new_instance_fun fun,
             _super_cast_fun super_case,
             _super_cast_const_fun super_cast_const,
             const std::type_info & tid,
             const std::type_info &ctid,
             const std::type_info &ptid,
             const std::type_info &cptid,
	     const std::type_info& c1,
	     const std::type_info& c2,
	     const std::type_info& c3,
	     const std::type_info& c4
  ) : m_name(name),
      m_super(super),
      m_new(fun),
      m_size(s),
      m_class_typeinfo(tid),
      m_class_ptr_typeinfo(ptid),
      m_fullname(unmangle(tid.name())),
      m_supercast(super_case),
      m_supercastconst(super_cast_const),
      m_fields(),
      m_fieldMap(),
      m_staticFields(),
      m_staticFieldMap(),
      m_methods(),
      m_methodMap(),
      m_staticMethods(),
      m_staticMethodMap(),
      m_properties(),
      m_propertyMap(),
      m_constructorMethods()
{
    getClassList().push_back(this);
    registerClassMap(Name_CLASSFULLNAME, m_fullname);
    registerType(tid, Type_Class,  this );
    registerType(ctid, Type_ClassConst,  this );
    registerType(ptid, Type_ClassPointer,  this );
    registerType(cptid, Type_ClassPointerConst,  this );
    if (c1 != typeid(void)) registerCustomType(c1, Type_Custom1, this);
    if (c2 != typeid(void)) registerCustomType(c2, Type_Custom2, this);
    if (c3 != typeid(void)) registerCustomType(c3, Type_Custom3, this);
    if (c4 != typeid(void)) registerCustomType(c4, Type_Custom4, this);
}

//the Class_Holder constructor
inline
Class::Class(const char *name,
             const std::type_info & tid
  ) : m_name(name),
      m_super(0),
      m_new(0),
      m_size(0),
      m_class_typeinfo(tid),
      m_class_ptr_typeinfo(typeid(void)),
      m_fullname(),
      m_supercast(0),
      m_supercastconst(0),
      m_fields(),
      m_fieldMap(),
      m_staticFields(),
      m_staticFieldMap(),
      m_methods(),
      m_methodMap(),
      m_staticMethods(),
      m_staticMethodMap(),
      m_properties(),
      m_propertyMap(),
      m_constructorMethods()
{
}


inline
Class::~Class()
{
  deleteAndClear(m_fields);
  deleteAndClear(m_staticFields);
  deleteAndClear(m_methods);
  deleteAndClear(m_staticMethods);
  deleteAndClear(m_properties);
  deleteAndClear(m_constructorMethods);
  
}

inline
const Class * Class::super() const throw (SuperClassError) {
  if (m_super==0) return 0;
  if (const Class_Holder* ph = dynamic_cast<const Class_Holder *>(m_super)) {
    const Class *real_cls = ph->resolve();
    if (real_cls == 0) throw SuperClassError(std::string("Unable to resolve super class of ") + m_name);
    delete m_super;
    this->m_super = real_cls;
  }
  return m_super;
}


inline
bool Class::isBase(const Class &cl) const {
    for(const Class *c = cl.super(); c; c = c->super()) {
	if (c == this) return true;
    }
    return false;
}

inline
void * Class::newInstance() const throw (UnknownMethodError) {
    if (m_new != 0) 
	return (*m_new)();
	
    const ConstructorMethod & c = getDefaultConstructor();
    typedef const __static_callable0__<void*> CallableType;
    CallableType *cb = (CallableType *)(c.m_callable); /* WARN: Dangerous casting  to void *(*)() */
    return cb->invoke();
}

inline
bool Class::isRelative(const Class &cl) const {
        return isSame(cl) || isBase(cl) || cl.isBase(*this);
}

inline
const Field &Class::getField(const char *name, bool searchSuper) const throw (UnknownFieldError) {
    _FieldMap::const_iterator it = m_fieldMap.find(name);
    if (it == m_fieldMap.end()) {
	if (searchSuper && m_super) return super()->getField(name, true);
	throw UnknownFieldError(name);
    }
    return *it->second;
}

inline
const StaticField &Class::getStaticField(const char *name, bool searchSuper) const throw (UnknownFieldError) {
    _StaticFieldMap::const_iterator it = m_staticFieldMap.find(name);
    if (it == m_staticFieldMap.end()) {
	if (searchSuper && m_super) return super()->getStaticField(name, true);
	throw UnknownFieldError(name);
    }
    return *it->second;
}

inline
const Method &Class::getMethod(const char *name, bool searchSuper) const throw (UnknownMethodError) {
    _MethodMap::const_iterator it = m_methodMap.find(name);
    if (it == m_methodMap.end()) {
	if (searchSuper && m_super) return super()->getMethod(name, true);
	throw UnknownMethodError(name);
    }
    return *it->second;
}

inline
Class::MethodList Class::getAllMethods(const char *name, bool searchSuper) const throw (UnknownMethodError) {
    _MethodMap::const_iterator it = m_methodMap.find(name);
    const _MethodMap::const_iterator& end = m_methodMap.end();
    if (it == end) {
	if (searchSuper && m_super) return super()->getAllMethods(name, true);
	throw UnknownMethodError(name);
    } else {
	MethodList mlist;
	while (it != end && it->first == name) {
	    mlist.push_back(it->second);
	    ++it;
	}
	return mlist;
    }
}

inline
const StaticMethod &Class::getStaticMethod(const char *name, bool searchSuper) const throw (UnknownMethodError) {
    _StaticMethodMap::const_iterator it = m_staticMethodMap.find(name);
    if (it == m_staticMethodMap.end()) {
	if (searchSuper && m_super) return super()->getStaticMethod(name, true);
	throw UnknownMethodError(name);
    }
    return *it->second;
}

inline
Class::StaticMethodList Class::getAllStaticMethods(const char *name, bool searchSuper) const throw (UnknownMethodError) {
    _StaticMethodMap::const_iterator it = m_staticMethodMap.find(name);
    const _StaticMethodMap::const_iterator& end =m_staticMethodMap.end(); 
    if (it == end) {
	if (searchSuper && m_super) return super()->getAllStaticMethods(name, true);
	throw UnknownMethodError(name);
    } else {
	StaticMethodList mlist;
	while (it != end && it->first == name) {
	    mlist.push_back(it->second);
	    ++it;
	}
	return mlist;
    }
}

inline
const ConstructorMethod &Class::getDefaultConstructor() const throw (UnknownMethodError) {
    const ConstructorMethod * m = defaultConstructor();
    if (m != 0) return *m;
    throw UnknownMethodError("Default Constructor");
}

inline
const Property &Class::getProperty(const char *name, bool searchSuper) const throw (UnknownPropertyError) {
    _PropertyMap::const_iterator it = m_propertyMap.find(name);
    if (it == m_propertyMap.end()) {
	if (searchSuper && m_super) return super()->getProperty(name, true);
	throw UnknownPropertyError(name);
    }
    return *it->second;
}

inline
const Class * Class::forName(const std::string& name) {
    return findClassMap(Name_CLASSFULLNAME,name);
}

inline
const Class::ClassList &Class::getClasses() {
    return getClassList();
}

inline
bool Class::dynamicCastableFromSuper(void *super_ptr, const Class *sup) const {
    if (this == sup) return true;
    return (m_super != 0) && super()->dynamicCastableFromSuper(super_ptr, sup) && dynamicCastFromSuper(super_ptr) == super_ptr;
}

inline
bool Class::dynamicCastableFromSuper(const void *super_ptr, const Class *sup) const {
    if (this == sup) return true;
    return (m_super != 0) && super()->dynamicCastableFromSuper(super_ptr, sup) && dynamicCastFromSuperConst(super_ptr) == super_ptr;
}

inline
const ConstructorMethod *Class::defaultConstructor() const {
    ConstructorList::const_iterator i = m_constructorMethods.begin();
    while (i != m_constructorMethods.end()) {
	const ConstructorMethod& c = **i;

	if (c.m_callable != 0 && c.m_callable->get_args_count() == 0) {
	    return &c;
	}
	++i;
    }
    return 0;
}
    
inline
bool Class::registerClassMap(Class::NameMap t, const std::string& name) {
    std::pair<ClassMap::iterator, bool> result = getClassMap(t).insert(std::make_pair(name,this));
    if (! result.second) {
	std::cerr << "ERROR: Class " << m_name << " already registered";
	throw ClassRegistrationError(m_name);
    }
    return result.second;
}

inline
const Class * Class::findClassMap(Class::NameMap t, const std::string& name) {
    const ClassMap & cmap = getClassMap(t);
    ClassMap::const_iterator p=cmap.find(name);
    return (p == cmap.end())?0:p->second;
}

inline
Class::ClassMap& Class::getClassMap(NameMap t) {
  static ClassMap s_class_maps[NameMap_SIZE];
    return s_class_maps[t];
}

inline
Class::TypeInfoMap& Class::getTypeMap(TypeMap t) {
  static TypeInfoMap s_type_maps[Type_NotFound];
  return s_type_maps[t];
}

inline
Class::ClassList& Class::getClassList() {
  static ClassList s_all_classes;
  return s_all_classes;
}

inline
const Class * Class::forType(const std::type_info& tinfo)
{
  TypeInfoMap const & tmap = getTypeMap(Type_Class);
  TypeInfoMap::const_iterator i = tmap.find(TypeInfo(tinfo));
  if (i != tmap.end()) {
    return i->second;
  }
  return 0;
}

inline
std::pair<Class::TypeMap, const Class *> Class::findType(const std::type_info& tinfo)
{
  for (int j = Type_Class; j != Type_NotFound; ++j)
  {
    TypeMap t = static_cast<TypeMap>(j);
    TypeInfoMap const & tmap = getTypeMap(t);
    TypeInfoMap::const_iterator i = tmap.find(TypeInfo(tinfo));
    if (i != tmap.end()) {
      return std::make_pair(t, i->second);
    }
  }
  return std::make_pair<TypeMap, const Class*>(Type_NotFound, 0);
}
    
inline
bool Class::registerCustomType(const std::type_info&t, TypeMap tmap, const Class *cls)
{
  switch (tmap) {
    case Type_Custom1:
    case Type_Custom2:
    case Type_Custom3:
    case Type_Custom4:
      return registerType(t,tmap,cls);
    default:
      break;
  }
  return false;
}

inline
bool Class::registerType(const std::type_info&t, TypeMap tmap, const Class *cls)
{
  std::pair<TypeInfoMap::iterator, bool> result = getTypeMap(tmap).insert(std::make_pair(TypeInfo(t),cls));
    if (! result.second) {
	std::cerr << "ERROR: Class " << unmangle(t.name()) << " already registered";
	throw ClassRegistrationError(t.name());
    }
  return result.second;
}
    
inline
bool Class::registerAlias(const std::string& name, const Class *cls)
{
  std::pair<ClassMap::iterator, bool> result = getClassMap(Name_ALIAS).insert(std::make_pair(name,cls));
    if (! result.second) {
	std::cerr << "ERROR: Class " << name << " already registered";
	throw ClassRegistrationError(name);
    }
  return result.second;
}

inline
void Class::_addField(Field *field) {
    m_fields.push_back(field);
    m_fieldMap.insert(std::make_pair<std::string,const Field *>(field->getName(),field));
}

inline
void Class::_addStaticField(StaticField *field) {
    m_staticFields.push_back(field);
    m_staticFieldMap.insert(std::make_pair<std::string,const StaticField *>(field->getName(), field));
}

inline
void Class::_addMethod(Method *method, __callable__ *cb) {
    m_methods.push_back(method);
    method->set_callable( cb);
    m_methodMap.insert(std::make_pair<std::string, const Method*>(method->getName(),method));
}

inline
void Class::_addStaticMethod(StaticMethod *method, __callable__ *cb) {
    m_staticMethods.push_back(method);
    method->set_callable( cb );
    m_staticMethodMap.insert(std::make_pair<std::string, const StaticMethod*>(method->getName(),method));
}

inline
void Class::_addConstructorMethod(ConstructorMethod *method, __callable__ *cb) {
    m_constructorMethods.push_back(method);
    method->set_callable (cb);
}

inline
void Class::_addProperty(Property *prop, __property_base__ *handler) {
    m_properties.push_back(prop);
    prop->m_handler = handler;
    m_propertyMap.insert(std::make_pair<std::string, const Property *>(prop->getName(), prop));
}

inline
Class_Holder::Class_Holder(const char *name)
: Class(name, typeid(void))
{
}

inline
Class_Holder::Class_Holder(const std::type_info &tid)
: Class(0,tid)
{
}

inline
const Class* Class_Holder::resolve() const
{
  const Class * cls = 0;
  if (m_class_typeinfo != typeid(void)) {
    cls = forType(m_class_typeinfo);
  }
  if (cls == 0 && m_name) {
    cls = forName(m_name);
  }
  return cls;
}


inline
__register_field__::__register_field__(unsigned long offset, const std::type_info &typeinfo, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name) {
    Field* field = new Field(offset, typeinfo, pclass, access, type, name);
    (const_cast<Class *>(pclass))->_addField(field);
}

inline
__register_static_field__::__register_static_field__(void *address, const std::type_info &typeinfo, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name) {
        StaticField*  field= new StaticField(address, typeinfo, pclass, access, type, name);
        (const_cast<Class *>(pclass))->_addStaticField(field);
    }

inline
__register_method__::__register_method__(__callable__ *cb, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name, const char *args, const char *virt) {
        Method* method = new Method(pclass, access, type, name, args, virt[0] == 'v' ? true : false);
        (const_cast<Class *>(pclass))->_addMethod(method, cb);
    }

inline
__register_static_method__::__register_static_method__(__callable__ *cb, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name, const char *args) {
        StaticMethod* method = new StaticMethod(pclass, access, type, name, args);
        (const_cast<Class *>(pclass))->_addStaticMethod(method, cb);
    }

inline
__register_constructor_method__::__register_constructor_method__(__callable__ *cb, __callable__ *placement_cb, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name, const char *args) {
        ConstructorMethod* method = new ConstructorMethod(pclass, access, type, name, args, placement_cb);
        (const_cast<Class *>(pclass))->_addConstructorMethod(method, cb);
    }


inline
__register_property__::__register_property__(__property_base__ *handler, const Class *pclass, const char *type, const char *name, const std::type_info& typeinfo) {
        Property* property = new Property(pclass, type, name,typeinfo);
        (const_cast<Class *>(pclass))->_addProperty(property, handler);
    }

inline
std::string MethodBase::getPrefix(const std::type_info &ret_type) const {
	return unmangle(ret_type.name()) + ' ' + getClass().getFullName() + "::" + getName();
    }

inline
std::string StaticMethod::getPrefix(const std::type_info &ret_type) const {
    return std::string("static ") + this->MethodBase::getPrefix(ret_type);
    }

inline
std::string ConstructorMethod::getPrefix(const std::type_info & /*ret_type*/ ) const {
	return getClass().getFullName() + "::" + getName();
    }

inline
bool isCastable(const std::type_info &from_cls, const std::type_info &to_cls, void *objptr )
{

    if (from_cls == to_cls) return true;

    if (from_cls == typeid(void*)) {
	return to_cls == typeid(const void*);
    }


    std::pair<Class::TypeMap, const Class *> from_type = Class::findType(from_cls);
    std::pair<Class::TypeMap, const Class *> to_type = Class::findType(to_cls);

    if (from_type.first != Class::Type_NotFound && from_type.first == to_type.first)
    {
      /* from and to are the same type */
      if (to_type.second->isSameOrBase(*from_type.second))
      {
        return true;
      } else if ((from_type.first == Class::Type_ClassPointer || from_type.first == Class::Type_ClassPointerConst) && (objptr != 0)) {
          /* now we need to try doing dynamicCasting */
          /* since we know this is a pointer type, so we can cast the parameter to void ** */
          void ** objptrptr = static_cast<void **> (objptr);
          return to_type.second->dynamicCastableFromSuper(*objptrptr, from_type.second);
      }
      return false;
    }
    
    if (from_type.first == Class::Type_Class) {
    /* test case:
       from is a Class or referrence to Class,
       then
       to must be either a superClass or const superClass
    */
      return (to_type.first == Class::Type_ClassConst) && to_type.second->isSameOrBase(*from_type.second);
    } else if (from_type.first == Class::Type_ClassPointer) {
    /* test case:
       from is a Class *,
       then
       to must be either a void*, const void*, superClass *, or const superClass*
    */
	if (to_cls == typeid(void *) || to_cls == typeid(const void *)) return true;
	if (to_type.first == Class::Type_ClassPointerConst) {
          if (to_type.second->isSameOrBase(*from_type.second)) {
            return true;
            } else	    if (objptr != 0) {
		/* now we need to try doing dynamicCasting */
		/* since we know this is a pointer type, so we can cast the parameter to void ** */
		void ** objptrptr = static_cast<void **> (objptr);
		return to_type.second->dynamicCastableFromSuper(*objptrptr, from_type.second);
          }
	}
    } else if (from_type.first == Class::Type_ClassPointerConst) {
    /* test case:
       from is a const Class *,
       then
       to must be either a const void*, or const superClass*
    */
      return (to_cls == typeid(const void *));
    }
    return false;
}




/** operator that outputs the access type to the given stream
    @param str stream to output the access to
    @param access access type to output
    @return the given stream
    @exception std::range_error thrown if the access value is invalid
 */
inline
std::ostream &operator << (std::ostream &str, ACCESS_TYPE access)
{
    switch (access) {
        case ACCESS_PUBLIC:
            str << "public";
            break;

        case ACCESS_PROTECTED:
            str << "protected";
            break;

        case ACCESS_PRIVATE:
            str << "private";
            break;

        default:
            throw std::range_error("access");
    }

    return str;
}

/** operator to print out class information
    @param str stream to output the access to
    @param access access type to output
    @return the given stream
    @exception std::range_error thrown if the access value is invalid
 */
inline
std::ostream &operator << (std::ostream &out, const Class& pclass) {
	out << "class name = " << pclass.getName() << std::endl;
	out << "full name = " << pclass.getFullName() << std::endl;

	out << "superclass = ";
	if (pclass.hasSuper()) out << pclass.getSuper().getFullName();
	else out << "none";
	out << std::endl;
	
	out << "newInstace " << (pclass.hasNewInstance()?"true":"false") << std::endl;
	out << "object size: " << pclass.getSize() << std::endl;

	out << "constructor:\n";
	const Class::ConstructorList & clist = pclass.getConstructors();
	for (Class::ConstructorList::const_iterator i=clist.begin(); i != clist.end(); i++) {
	    const ConstructorMethod &method = **i;
	    out << "    " << method.getAccess();
	    out << " " << pclass.getName() << method.getArgs()
		<< "     ( " << method.getId() << " )" <<
		std::endl;
	    out << " { Signature: " << method.getSignature() << "}" << std::endl;
	}

	out << "fields:\n";
	for(Class::FieldList::const_iterator itField = pclass.getFields().begin();
	    itField != pclass.getFields().end();
	    ++itField)
	{
	    const Field &field = **itField;
	    out << "    " << field.getAccess() << " " << field.getType() << " " << field.getName() << std::endl;
	}

	out << "static fields:\n";
	for(Class::StaticFieldList::const_iterator itStaticField = pclass.getStaticFields().begin();
	    itStaticField != pclass.getStaticFields().end();
	    ++itStaticField)
	{
	    const StaticField &field = **itStaticField;
	    out << "    " << field.getAccess() << " " << field.getType() << " " << field.getName() << std::endl;
	}

	out << "static methods:\n";
	for(Class::StaticMethodList::const_iterator itMethod = pclass.getStaticMethods().begin();
	    itMethod != pclass.getStaticMethods().end();
	    ++itMethod)
	{
	    const StaticMethod &method = **itMethod;
	    out << "    " << method.getAccess();
	    out << " " << method.getType() << " " << method.getName() << method.getArgs()
		<< "     ( " << method.getId() << " )" <<
		std::endl;
	    out << " { Signature: " << method.getSignature() << "}" << std::endl;
	}

	out << "methods:\n";
	for(Class::MethodList::const_iterator itMethod = pclass.getMethods().begin();
	    itMethod != pclass.getMethods().end();
	    ++itMethod)
	{
	    const Method &method = **itMethod;
	    out << "    " << method.getAccess();
	    if (method.isVirtual()) out << " " << "virtual";
	    out << " " << method.getType() << " " << method.getName() << method.getArgs() 
		<< "     ( " << method.getId() << " )" <<
		std::endl;
	    out << " { Signature: " << method.getSignature() << "}" << std::endl;
	}

	out << "properties:\n";
	for(Class::PropertyList::const_iterator itProperty = pclass.getProperties().begin();
	    itProperty != pclass.getProperties().end();
	    ++itProperty)
	{
	    const Property &property = **itProperty;
	    out << "    " << property.getType() << " " << property.getName() << std::endl;
	}

	out << "\n-----------------------------------------------------------\n";
	return out;
    }



}}

// $Id: reflection.tpp 1766 2008-01-10 13:38:39Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
