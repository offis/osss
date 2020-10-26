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


#ifndef AGM_REFLECTION_HPP
#define AGM_REFLECTION_HPP

#include "o3s/utils/osss_common.h"

#ifdef OSSS_MSC_
#pragma warning (disable: 4786)
#pragma warning (disable: 4003)
#endif

#include <map>
#include <list>
#include <vector>
#include <string>
#include <typeinfo>
#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <sstream>

//joins
#define REF_JOIN( X, Y ) REF_DO_JOIN( X, Y )
#define REF_DO_JOIN( X, Y ) REF_DO_JOIN2(X,Y)
#define REF_DO_JOIN2( X, Y ) X##Y

#define __IF0(E)
#define __IF1(E)      _##E
#define __IF2(E)      _##E
#define __IF3(E)      _##E
#define __IF4(E)      _##E
#define __IF5(E)      _##E
#define __IF6(E)      _##E
#define __IF7(E)      _##E
#define __IF8(E)      _##E
#define __IF9(E)      _##E
#define __IF10(E)      _##E
#define __IF11(E)      _##E
#define __IF12(E)      _##E
#define __IF13(E)      _##E
#define __IF14(E)      _##E
#define __IF15(E)      _##E
#define __IF16(E)      _##E
#define __IF17(E)      _##E
#define __IF18(E)      _##E
#define __IF19(E)      _##E
#define __IF20(E)      _##E
#define __IF(N,E)      __IF##N(E)



//repeaters
#define __REPEAT0(M, C, S)         
#define __REPEAT1(M, C, S)      _##S M(1)
#define __REPEAT2(M, C, S)         __REPEAT1(M, C, S)  _##C M(2)
#define __REPEAT3(M, C, S)         __REPEAT2(M, C, S)  _##C M(3)
#define __REPEAT4(M, C, S)         __REPEAT3(M, C, S)  _##C M(4)
#define __REPEAT5(M, C, S)         __REPEAT4(M, C, S)  _##C M(5)
#define __REPEAT6(M, C, S)         __REPEAT5(M, C, S)  _##C M(6)
#define __REPEAT7(M, C, S)         __REPEAT6(M, C, S)  _##C M(7)
#define __REPEAT8(M, C, S)         __REPEAT7(M, C, S)  _##C M(8)
#define __REPEAT9(M, C, S)         __REPEAT8(M, C, S)  _##C M(9)
#define __REPEAT10(M, C, S)        __REPEAT9(M, C, S)  _##C M(10)
#define __REPEAT11(M, C, S)        __REPEAT10(M, C, S) _##C M(11)
#define __REPEAT12(M, C, S)        __REPEAT11(M, C, S) _##C M(12)
#define __REPEAT13(M, C, S)        __REPEAT12(M, C, S) _##C M(13)
#define __REPEAT14(M, C, S)        __REPEAT13(M, C, S) _##C M(14)
#define __REPEAT15(M, C, S)        __REPEAT14(M, C, S) _##C M(15)
#define __REPEAT16(M, C, S)        __REPEAT15(M, C, S) _##C M(16)
#define __REPEAT17(M, C, S)        __REPEAT16(M, C, S) _##C M(17)
#define __REPEAT18(M, C, S)        __REPEAT17(M, C, S) _##C M(18)
#define __REPEAT19(M, C, S)        __REPEAT18(M, C, S) _##C M(19)
#define __REPEAT20(M, C, S)        __REPEAT19(M, C, S) _##C M(20)
#define __REPEAT(N, M, C, S, E)       __REPEAT##N(M, C, S) __IF##N(E)


//various defs needed for parameters
#define __MAX_PARAMS__       20
#define ___NOTHING__          
#define ___COMMA__            ,
#define ___SEMICOLON__        ;
#define ___TEMPLATE_DECLARE_BEGIN__   template <
#define ___TEMPLATE_BEGIN__   <
#define ___TEMPLATE_END__     >
#define __TEMPLATE_ARG__(N)  class T##N
#define __TYPE_ARG__(N)      T##N 
#define __ARG__(N)           T##N t##N
#define __ARG_PTR__(N)       T##N* t##N
#define __PARAM__(N)         t##N
#define __PARAM_PTR__(N)     & __PARAM__(N)
#define __NOT_VIRTUAL__



//calculates the offset of a field

//#ifdef offsetof
//#define __OFFSET__(C, M) offsetof(C, M)
#if 1
#define __OFFSET__(C, M)\
    ((unsigned long)(&((const C *)1024)->M)-1024)
#endif

namespace agm { namespace reflection {


std::string unmangle(const char * mangledName);

template<typename T>
std::string demangle()
{
    return unmangle(typeid(T).name());
}




//root of all callables
struct __callable__ {
    typedef std::vector<const std::type_info *> arg_list_type;
    virtual ~__callable__() {}
    virtual int get_args_count() const = 0;
    virtual const arg_list_type& get_args() const = 0;
    virtual const std::type_info& get_ret_type() const = 0;
};

// parameter for generating 
#define __TYPE_INFO_ARG__(N)  this->args_list.push_back(& typeid(__TYPE_ARG__(N)))

//callable that implements the argument capturing and reporting
#define __CALLABLE__ARGS(N) \
    template <int args_size __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> struct __callable_args##N##__ : public __callable__ {\
        const std::type_info& ret_type;\
	arg_list_type args_list;\
        __callable_args##N##__(const std::type_info & ret_id) : ret_type(ret_id), args_list() {\
	    __REPEAT(N, __TYPE_INFO_ARG__, __SEMICOLON__, __NOTHING__, __NOTHING__);\
	};\
	int get_args_count () const { return args_size; }\
	const arg_list_type& get_args() const { return args_list; }\
        const std::type_info& get_ret_type() const { return ret_type; }\
    }


//callable class macro with return type
#define __CALLABLE__(N)\
template <class R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> struct __callable##N##__ : public __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) > {\
    typedef R (C::*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__));\
    MethodType method;\
    __callable##N##__(MethodType m) : __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) >(typeid(R)), method(m) {\
    }\
    R invoke(C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__, __NOTHING__)) const {\
        return (object->*method)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
    }\
};


//callable class macro with return type and const type
#define __CALLABLE_CONST__(N)\
template <class R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> struct __callable_const##N##__ : public __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) >  {\
    typedef R (C::*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__)) const;\
    MethodType method;\
    __callable_const##N##__(MethodType m) :  __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) >(typeid(R)), method(m) {\
    }\
    R invoke(C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__, __NOTHING__)) const {\
        return (object->*method)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
    }\
};


//callable class macro with void return type
#define __CALLABLE_VOID__(N)\
template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> struct __callable_void##N##__ : public __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) > {\
    typedef void (C::*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__));\
    MethodType method;\
    __callable_void##N##__(MethodType m) : __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) >(typeid(void)), method(m) {\
    }\
    void invoke(C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__, __NOTHING__)) const {\
        (object->*method)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
    }\
};


//callable class macro with void return type and const type
#define __CALLABLE_CONST_VOID__(N)\
template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> struct __callable_const_void##N##__ : public __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) > {\
    typedef void (C::*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__)) const;\
    MethodType method;\
    __callable_const_void##N##__(MethodType m) : __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) >(typeid(void)), method(m) {\
    }\
    void invoke(C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__, __NOTHING__)) const {\
        (object->*method)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
    }\
};


//static callable class macro with return type
#define __STATIC_CALLABLE__(N)\
template <class R __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> struct __static_callable##N##__ : public __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) > {\
    typedef R (*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__));\
    MethodType method;\
    __static_callable##N##__(MethodType m) : __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) >(typeid(R)), method(m) {\
    }\
    R invoke(__REPEAT(N, __ARG__, __COMMA__, __NOTHING__, __NOTHING__)) const {\
        return (*method)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
    }\
};



//define callables
__CALLABLE__ARGS(0);
__CALLABLE__ARGS(1);
__CALLABLE__ARGS(2);
__CALLABLE__ARGS(3);
__CALLABLE__ARGS(4);
__CALLABLE__ARGS(5);
__CALLABLE__ARGS(6);
__CALLABLE__ARGS(7);
__CALLABLE__ARGS(8);
__CALLABLE__ARGS(9);
__CALLABLE__ARGS(10);
__CALLABLE__ARGS(11);
__CALLABLE__ARGS(12);
__CALLABLE__ARGS(13);
__CALLABLE__ARGS(14);
__CALLABLE__ARGS(15);
__CALLABLE__ARGS(16);
__CALLABLE__ARGS(17);
__CALLABLE__ARGS(18);
__CALLABLE__ARGS(19);
__CALLABLE__ARGS(20);

#if 0
//void version for 0 params
struct __static_callable_void0__ : public __callable_args0__<0> {
    typedef void (*MethodType)();
    MethodType method;
    __static_callable_void0__(MethodType m) : __callable_args0__<0>(typeid(void)), method(m) {
    }
    void invoke() const {
        (*method)();
    }
};
#endif

//void version
#define __STATIC_CALLABLE_VOID__(N)\
__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __TEMPLATE_DECLARE_BEGIN__, __TEMPLATE_END__) \
struct __static_callable_void##N##__ : public __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) > {\
    typedef void (*MethodType)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__));\
    MethodType method;\
    __static_callable_void##N##__(MethodType m) : __callable_args##N##__<N __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__) >(typeid(void)), method(m) {\
    }\
    void invoke(__REPEAT(N, __ARG__, __COMMA__, __NOTHING__, __NOTHING__)) const {\
        (*method)(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
    }\
};


//define callables
__CALLABLE__(0);
__CALLABLE__(1);
__CALLABLE__(2);
__CALLABLE__(3);
__CALLABLE__(4);
__CALLABLE__(5);
__CALLABLE__(6);
__CALLABLE__(7);
__CALLABLE__(8);
__CALLABLE__(9);
__CALLABLE__(10);
__CALLABLE__(11);
__CALLABLE__(12);
__CALLABLE__(13);
__CALLABLE__(14);
__CALLABLE__(15);
__CALLABLE__(16);
__CALLABLE__(17);
__CALLABLE__(18);
__CALLABLE__(19);
__CALLABLE__(20);




//define const callables
__CALLABLE_CONST__(0);
__CALLABLE_CONST__(1);
__CALLABLE_CONST__(2);
__CALLABLE_CONST__(3);
__CALLABLE_CONST__(4);
__CALLABLE_CONST__(5);
__CALLABLE_CONST__(6);
__CALLABLE_CONST__(7);
__CALLABLE_CONST__(8);
__CALLABLE_CONST__(9);
__CALLABLE_CONST__(10);
__CALLABLE_CONST__(11);
__CALLABLE_CONST__(12);
__CALLABLE_CONST__(13);
__CALLABLE_CONST__(14);
__CALLABLE_CONST__(15);
__CALLABLE_CONST__(16);
__CALLABLE_CONST__(17);
__CALLABLE_CONST__(18);
__CALLABLE_CONST__(19);
__CALLABLE_CONST__(20);


//define void callables
__CALLABLE_VOID__(0);
__CALLABLE_VOID__(1);
__CALLABLE_VOID__(2);
__CALLABLE_VOID__(3);
__CALLABLE_VOID__(4);
__CALLABLE_VOID__(5);
__CALLABLE_VOID__(6);
__CALLABLE_VOID__(7);
__CALLABLE_VOID__(8);
__CALLABLE_VOID__(9);
__CALLABLE_VOID__(10);
__CALLABLE_VOID__(11);
__CALLABLE_VOID__(12);
__CALLABLE_VOID__(13);
__CALLABLE_VOID__(14);
__CALLABLE_VOID__(15);
__CALLABLE_VOID__(16);
__CALLABLE_VOID__(17);
__CALLABLE_VOID__(18);
__CALLABLE_VOID__(19);
__CALLABLE_VOID__(20);


//define const void callables
__CALLABLE_CONST_VOID__(0);
__CALLABLE_CONST_VOID__(1);
__CALLABLE_CONST_VOID__(2);
__CALLABLE_CONST_VOID__(3);
__CALLABLE_CONST_VOID__(4);
__CALLABLE_CONST_VOID__(5);
__CALLABLE_CONST_VOID__(6);
__CALLABLE_CONST_VOID__(7);
__CALLABLE_CONST_VOID__(8);
__CALLABLE_CONST_VOID__(9);
__CALLABLE_CONST_VOID__(10);
__CALLABLE_CONST_VOID__(11);
__CALLABLE_CONST_VOID__(12);
__CALLABLE_CONST_VOID__(13);
__CALLABLE_CONST_VOID__(14);
__CALLABLE_CONST_VOID__(15);
__CALLABLE_CONST_VOID__(16);
__CALLABLE_CONST_VOID__(17);
__CALLABLE_CONST_VOID__(18);
__CALLABLE_CONST_VOID__(19);
__CALLABLE_CONST_VOID__(20);


//static callable classes
__STATIC_CALLABLE__(0)
__STATIC_CALLABLE__(1)
__STATIC_CALLABLE__(2)
__STATIC_CALLABLE__(3)
__STATIC_CALLABLE__(4)
__STATIC_CALLABLE__(5)
__STATIC_CALLABLE__(6)
__STATIC_CALLABLE__(7)
__STATIC_CALLABLE__(8)
__STATIC_CALLABLE__(9)
__STATIC_CALLABLE__(10)
__STATIC_CALLABLE__(11)
__STATIC_CALLABLE__(12)
__STATIC_CALLABLE__(13)
__STATIC_CALLABLE__(14)
__STATIC_CALLABLE__(15)
__STATIC_CALLABLE__(16)
__STATIC_CALLABLE__(17)
__STATIC_CALLABLE__(18)
__STATIC_CALLABLE__(19)
__STATIC_CALLABLE__(20)
__STATIC_CALLABLE_VOID__(0)
__STATIC_CALLABLE_VOID__(1)
__STATIC_CALLABLE_VOID__(2)
__STATIC_CALLABLE_VOID__(3)
__STATIC_CALLABLE_VOID__(4)
__STATIC_CALLABLE_VOID__(5)
__STATIC_CALLABLE_VOID__(6)
__STATIC_CALLABLE_VOID__(7)
__STATIC_CALLABLE_VOID__(8)
__STATIC_CALLABLE_VOID__(9)
__STATIC_CALLABLE_VOID__(10)
__STATIC_CALLABLE_VOID__(11)
__STATIC_CALLABLE_VOID__(12)
__STATIC_CALLABLE_VOID__(13)
__STATIC_CALLABLE_VOID__(14)
__STATIC_CALLABLE_VOID__(15)
__STATIC_CALLABLE_VOID__(16)
__STATIC_CALLABLE_VOID__(17)
__STATIC_CALLABLE_VOID__(18)
__STATIC_CALLABLE_VOID__(19)
__STATIC_CALLABLE_VOID__(20)


//macro of a inline method that accepts a method pointer and creates a callable for it
#define __CREATE_CALLABLE__(N)\
    template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> static inline __callable##N##__<R, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> *create(R (C::*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__))) {\
        return new __callable##N##__<R, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)>(method);\
    }


//macro of a inline method that accepts a method pointer and creates a const callable for it
#define __CREATE_CALLABLE_CONST__(N)\
    template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> static inline __callable_const##N##__<R, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> *create(R (C::*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__)) const) {\
        return new __callable_const##N##__<R, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)>(method);\
    }


//void version
#define __CREATE_CALLABLE_VOID__(N)\
    template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> static inline __callable_void##N##__<C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> *create(void (C::*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__))) {\
        return new __callable_void##N##__<C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)>(method);\
    }


//const void version
#define __CREATE_CALLABLE_CONST_VOID__(N)\
    template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> static inline __callable_const_void##N##__<C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> *create(void (C::*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__)) const) {\
        return new __callable_const_void##N##__<C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)>(method);\
    }


//macro to create a static callable
#define __CREATE_STATIC_CALLABLE__(N)\
    template <class R1 __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> static inline __static_callable##N##__<R1 __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> *create(R1 (*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__))) {\
        return new __static_callable##N##__<R1 __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)>(method);\
    }


#if 0
//void version for 0 params
#define __CREATE_STATIC_CALLABLE_VOID__0\
    static inline __static_callable_void0__ *create(void (*method)()) {\
        return new __static_callable_void0__(method);\
    }
#endif

//void version
#define __CREATE_STATIC_CALLABLE_VOID__(N)\
    __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __TEMPLATE_DECLARE_BEGIN__, __TEMPLATE_END__) \
static inline __static_callable_void##N##__ __REPEAT(N, __TYPE_ARG__, __COMMA__, __TEMPLATE_BEGIN__, __TEMPLATE_END__) *create(void (*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__))) {\
        return new __static_callable_void##N##__ __REPEAT(N, __TYPE_ARG__, __COMMA__, __TEMPLATE_BEGIN__, __TEMPLATE_END__) (method);\
    }


//factory method for non-voids
template <class R> struct __callable_factory__ {
    __CREATE_CALLABLE__(0)
    __CREATE_CALLABLE__(1)
    __CREATE_CALLABLE__(2)
    __CREATE_CALLABLE__(3)
    __CREATE_CALLABLE__(4)
    __CREATE_CALLABLE__(5)
    __CREATE_CALLABLE__(6)
    __CREATE_CALLABLE__(7)
    __CREATE_CALLABLE__(8)
    __CREATE_CALLABLE__(9)
    __CREATE_CALLABLE__(10)
    __CREATE_CALLABLE__(11)
    __CREATE_CALLABLE__(12)
    __CREATE_CALLABLE__(13)
    __CREATE_CALLABLE__(14)
    __CREATE_CALLABLE__(15)
    __CREATE_CALLABLE__(16)
    __CREATE_CALLABLE__(17)
    __CREATE_CALLABLE__(18)
    __CREATE_CALLABLE__(19)
    __CREATE_CALLABLE__(20)
    __CREATE_CALLABLE_CONST__(0)
    __CREATE_CALLABLE_CONST__(1)
    __CREATE_CALLABLE_CONST__(2)
    __CREATE_CALLABLE_CONST__(3)
    __CREATE_CALLABLE_CONST__(4)
    __CREATE_CALLABLE_CONST__(5)
    __CREATE_CALLABLE_CONST__(6)
    __CREATE_CALLABLE_CONST__(7)
    __CREATE_CALLABLE_CONST__(8)
    __CREATE_CALLABLE_CONST__(9)
    __CREATE_CALLABLE_CONST__(10)
    __CREATE_CALLABLE_CONST__(11)
    __CREATE_CALLABLE_CONST__(12)
    __CREATE_CALLABLE_CONST__(13)
    __CREATE_CALLABLE_CONST__(14)
    __CREATE_CALLABLE_CONST__(15)
    __CREATE_CALLABLE_CONST__(16)
    __CREATE_CALLABLE_CONST__(17)
    __CREATE_CALLABLE_CONST__(18)
    __CREATE_CALLABLE_CONST__(19)
    __CREATE_CALLABLE_CONST__(20)
};


//factory method for non-voids
template <> struct __callable_factory__<void> {
    __CREATE_CALLABLE_VOID__(0)
    __CREATE_CALLABLE_VOID__(1)
    __CREATE_CALLABLE_VOID__(2)
    __CREATE_CALLABLE_VOID__(3)
    __CREATE_CALLABLE_VOID__(4)
    __CREATE_CALLABLE_VOID__(5)
    __CREATE_CALLABLE_VOID__(6)
    __CREATE_CALLABLE_VOID__(7)
    __CREATE_CALLABLE_VOID__(8)
    __CREATE_CALLABLE_VOID__(9)
    __CREATE_CALLABLE_VOID__(10)
    __CREATE_CALLABLE_VOID__(11)
    __CREATE_CALLABLE_VOID__(12)
    __CREATE_CALLABLE_VOID__(13)
    __CREATE_CALLABLE_VOID__(14)
    __CREATE_CALLABLE_VOID__(15)
    __CREATE_CALLABLE_VOID__(16)
    __CREATE_CALLABLE_VOID__(17)
    __CREATE_CALLABLE_VOID__(18)
    __CREATE_CALLABLE_VOID__(19)
    __CREATE_CALLABLE_VOID__(20)
    __CREATE_CALLABLE_CONST_VOID__(0)
    __CREATE_CALLABLE_CONST_VOID__(1)
    __CREATE_CALLABLE_CONST_VOID__(2)
    __CREATE_CALLABLE_CONST_VOID__(3)
    __CREATE_CALLABLE_CONST_VOID__(4)
    __CREATE_CALLABLE_CONST_VOID__(5)
    __CREATE_CALLABLE_CONST_VOID__(6)
    __CREATE_CALLABLE_CONST_VOID__(7)
    __CREATE_CALLABLE_CONST_VOID__(8)
    __CREATE_CALLABLE_CONST_VOID__(9)
    __CREATE_CALLABLE_CONST_VOID__(10)
    __CREATE_CALLABLE_CONST_VOID__(11)
    __CREATE_CALLABLE_CONST_VOID__(12)
    __CREATE_CALLABLE_CONST_VOID__(13)
    __CREATE_CALLABLE_CONST_VOID__(14)
    __CREATE_CALLABLE_CONST_VOID__(15)
    __CREATE_CALLABLE_CONST_VOID__(16)
    __CREATE_CALLABLE_CONST_VOID__(17)
    __CREATE_CALLABLE_CONST_VOID__(18)
    __CREATE_CALLABLE_CONST_VOID__(19)
    __CREATE_CALLABLE_CONST_VOID__(20)
};


//factory method for static non-voids
template <class R> struct __static_callable_factory__ {
    __CREATE_STATIC_CALLABLE__(0)
    __CREATE_STATIC_CALLABLE__(1)
    __CREATE_STATIC_CALLABLE__(2)
    __CREATE_STATIC_CALLABLE__(3)
    __CREATE_STATIC_CALLABLE__(4)
    __CREATE_STATIC_CALLABLE__(5)
    __CREATE_STATIC_CALLABLE__(6)
    __CREATE_STATIC_CALLABLE__(7)
    __CREATE_STATIC_CALLABLE__(8)
    __CREATE_STATIC_CALLABLE__(9)
    __CREATE_STATIC_CALLABLE__(10)
    __CREATE_STATIC_CALLABLE__(11)
    __CREATE_STATIC_CALLABLE__(12)
    __CREATE_STATIC_CALLABLE__(13)
    __CREATE_STATIC_CALLABLE__(14)
    __CREATE_STATIC_CALLABLE__(15)
    __CREATE_STATIC_CALLABLE__(16)
    __CREATE_STATIC_CALLABLE__(17)
    __CREATE_STATIC_CALLABLE__(18)
    __CREATE_STATIC_CALLABLE__(19)
    __CREATE_STATIC_CALLABLE__(20)
};


//factory method for static voids
template <> struct __static_callable_factory__<void> {
    __CREATE_STATIC_CALLABLE_VOID__(0)
    __CREATE_STATIC_CALLABLE_VOID__(1)
    __CREATE_STATIC_CALLABLE_VOID__(2)
    __CREATE_STATIC_CALLABLE_VOID__(3)
    __CREATE_STATIC_CALLABLE_VOID__(4)
    __CREATE_STATIC_CALLABLE_VOID__(5)
    __CREATE_STATIC_CALLABLE_VOID__(6)
    __CREATE_STATIC_CALLABLE_VOID__(7)
    __CREATE_STATIC_CALLABLE_VOID__(8)
    __CREATE_STATIC_CALLABLE_VOID__(9)
    __CREATE_STATIC_CALLABLE_VOID__(10)
    __CREATE_STATIC_CALLABLE_VOID__(11)
    __CREATE_STATIC_CALLABLE_VOID__(12)
    __CREATE_STATIC_CALLABLE_VOID__(13)
    __CREATE_STATIC_CALLABLE_VOID__(14)
    __CREATE_STATIC_CALLABLE_VOID__(15)
    __CREATE_STATIC_CALLABLE_VOID__(16)
    __CREATE_STATIC_CALLABLE_VOID__(17)
    __CREATE_STATIC_CALLABLE_VOID__(18)
    __CREATE_STATIC_CALLABLE_VOID__(19)
    __CREATE_STATIC_CALLABLE_VOID__(20)
};


//callable generator
#define __CALLABLE_GENERATOR__(N)\
    template <class R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> inline __callable__ *__create_callable__(R (C::*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__))) {\
        return __callable_factory__<R>::create(method);\
    }


//const callable generator
#define __CALLABLE_GENERATOR_CONST__(N)\
    template <class R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> inline __callable__ *__create_callable__(R (C::*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__)) const) {\
        return __callable_factory__<R>::create(method);\
    }


//static callable generator
#define __STATIC_CALLABLE_GENERATOR__(N)\
    template <class R __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> inline __callable__ *__create_static_callable__(R (*method)(__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__))) {\
        return __static_callable_factory__<R>::create(method);\
    }


//generators
__CALLABLE_GENERATOR__(0)
__CALLABLE_GENERATOR__(1)
__CALLABLE_GENERATOR__(2)
__CALLABLE_GENERATOR__(3)
__CALLABLE_GENERATOR__(4)
__CALLABLE_GENERATOR__(5)
__CALLABLE_GENERATOR__(6)
__CALLABLE_GENERATOR__(7)
__CALLABLE_GENERATOR__(8)
__CALLABLE_GENERATOR__(9)
__CALLABLE_GENERATOR__(10)
__CALLABLE_GENERATOR__(11)
__CALLABLE_GENERATOR__(12)
__CALLABLE_GENERATOR__(13)
__CALLABLE_GENERATOR__(14)
__CALLABLE_GENERATOR__(15)
__CALLABLE_GENERATOR__(16)
__CALLABLE_GENERATOR__(17)
__CALLABLE_GENERATOR__(18)
__CALLABLE_GENERATOR__(19)
__CALLABLE_GENERATOR__(20)
__CALLABLE_GENERATOR_CONST__(0)
__CALLABLE_GENERATOR_CONST__(1)
__CALLABLE_GENERATOR_CONST__(2)
__CALLABLE_GENERATOR_CONST__(3)
__CALLABLE_GENERATOR_CONST__(4)
__CALLABLE_GENERATOR_CONST__(5)
__CALLABLE_GENERATOR_CONST__(6)
__CALLABLE_GENERATOR_CONST__(7)
__CALLABLE_GENERATOR_CONST__(8)
__CALLABLE_GENERATOR_CONST__(9)
__CALLABLE_GENERATOR_CONST__(10)
__CALLABLE_GENERATOR_CONST__(11)
__CALLABLE_GENERATOR_CONST__(12)
__CALLABLE_GENERATOR_CONST__(13)
__CALLABLE_GENERATOR_CONST__(14)
__CALLABLE_GENERATOR_CONST__(15)
__CALLABLE_GENERATOR_CONST__(16)
__CALLABLE_GENERATOR_CONST__(17)
__CALLABLE_GENERATOR_CONST__(18)
__CALLABLE_GENERATOR_CONST__(19)
__CALLABLE_GENERATOR_CONST__(20)
__STATIC_CALLABLE_GENERATOR__(0)
__STATIC_CALLABLE_GENERATOR__(1)
__STATIC_CALLABLE_GENERATOR__(2)
__STATIC_CALLABLE_GENERATOR__(3)
__STATIC_CALLABLE_GENERATOR__(4)
__STATIC_CALLABLE_GENERATOR__(5)
__STATIC_CALLABLE_GENERATOR__(6)
__STATIC_CALLABLE_GENERATOR__(7)
__STATIC_CALLABLE_GENERATOR__(8)
__STATIC_CALLABLE_GENERATOR__(9)
__STATIC_CALLABLE_GENERATOR__(10)
__STATIC_CALLABLE_GENERATOR__(11)
__STATIC_CALLABLE_GENERATOR__(12)
__STATIC_CALLABLE_GENERATOR__(13)
__STATIC_CALLABLE_GENERATOR__(14)
__STATIC_CALLABLE_GENERATOR__(15)
__STATIC_CALLABLE_GENERATOR__(16)
__STATIC_CALLABLE_GENERATOR__(17)
__STATIC_CALLABLE_GENERATOR__(18)
__STATIC_CALLABLE_GENERATOR__(19)
__STATIC_CALLABLE_GENERATOR__(20)


//static new instance generator
#define __NEWINSTANCE_CALLER__(CNAME, N)\
    template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__) > static C *__new_instance__(__REPEAT(N, __ARG__, __COMMA__, __NOTHING__, __NOTHING__)) {\
        return new CNAME (__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
    }

#define __NEWINSTANCE_PTR_CALLER__(CNAME, N)\
    template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> \
    static C *__new_instance_ptr__(void* ptr __REPEAT(N, __ARG__, __COMMA__, __COMMA__, __NOTHING__)) {\
        return new (ptr) CNAME (__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
    } \
    template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> \
    static C * (* \
__create_new_instance_ptr__(C* (*) (__REPEAT(N, __TYPE_ARG__, __COMMA__, __NOTHING__, __NOTHING__)))) (void * __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)) {\
        return  & __new_instance_ptr__<C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)>;\
    }


//property handler base
struct __property_base__ {
    virtual ~__property_base__() {
    }
};


//property handler
template <class T> struct __property_handler__ : public __property_base__ {
    //get
    virtual T get(const void *object) const = 0;

    //set
    virtual void set(void *object, T value) const = 0;
};


//property handler class
template <class C, class T> struct __property__ : public __property_handler__<T> {
    //type of getters/setters
    typedef T (C::*Getter)() const;
    typedef void (C::*Setter)(T);

    //pointer to member getters/setters
    Getter getter;
    Setter setter;

    //default constructor
    __property__(Getter g, Setter s) : getter(g), setter(s) {
    }

    //get
    virtual T get(const void *object) const {
        const C *o = (const C *)(object);
        return (o->*getter)();
    }

    //set
    virtual void set(void *object, T value) const {
        C *o = (C *)(object);
        (o->*setter)(value);
    }
};

  template <class C, class T>
  __property_base__* __create_property__( T (C::*Getter)() const, void (C::*Setter)(T)) {
    return new __property__<C,T>(Getter, Setter);
  }


//declares a method
#define __METHOD__(ACCESS_ATTR, VIRTUAL, RETURN_TYPE, METHOD_NAME, METHOD_ARGS, LINE)\
private:\
    struct REF_JOIN(__method_##METHOD_NAME##__,LINE) {\
        REF_JOIN(__method_##METHOD_NAME##__,LINE) () {\
            typedef RETURN_TYPE (ClassType::*method_ptr_type) METHOD_ARGS;\
 method_ptr_type method_ptr = & ClassType::METHOD_NAME; \
            static agm::reflection::__register_method__ reg(agm::reflection:: __create_callable__(method_ptr), getClassStaticPtr(), agm::reflection:: ACCESS_##ACCESS_ATTR, #RETURN_TYPE, #METHOD_NAME, #METHOD_ARGS, #VIRTUAL);\
        }\
    } REF_JOIN(__method_##METHOD_NAME##__,LINE);\
    friend struct REF_JOIN(__method_##METHOD_NAME##__,LINE) ;\
ACCESS_ATTR :\
    VIRTUAL RETURN_TYPE METHOD_NAME METHOD_ARGS

#define __MISMATCHED_STRING__(S,M,E,P) S << M  << " mismatched: expected (" << E << ") passed (" << P << ");\n"

#define __TYPE_MISMATCH_TEST__(N) if ( (*(*i)) !=  typeid(__TYPE_ARG__(N)) ) { if ( isCastable(typeid(__TYPE_ARG__(N)), *(*i)) ) res << "(WARN only: type castable)"; __MISMATCHED_STRING__(res, "Parameter " << count , unmangle((*i)->name()), unmangle(typeid(__TYPE_ARG__(N)).name())); } i++; count++;

// detail message of the type mismatch
#define __TYPE_MISMATCH_ERROR_(N) \
__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __TEMPLATE_DECLARE_BEGIN__, __TEMPLATE_END__) \
static std::string findMismatchedType##N(__callable__ *cb, const std::type_info &cb_classtype, const std::type_info & rettype, const std::type_info &classtype) {\
    std::ostringstream res;\
    if (cb_classtype != classtype) {\
        if (isCastable(classtype, cb_classtype)) res << "(WARN only: type castable)";\
	__MISMATCHED_STRING__(res, "object class type", unmangle(cb_classtype.name()), unmangle(classtype.name()));\
    }\
    std::string strcount;\
    if (cb->get_args_count() != N) {\
	__MISMATCHED_STRING__(res, "number of parameter", cb->get_args_count(), N);\
	return std::string(res.str());\
	/* can't continue if number of arg is mismatched */\
    }\
    if (cb->get_ret_type() != rettype) {\
        if (isCastable(cb->get_ret_type(), rettype)) res << "(WARN only: type castable)";\
	__MISMATCHED_STRING__(res, "return type", unmangle(cb->get_ret_type().name()), unmangle(rettype.name()));\
    }\
    int count; count = 1;\
    __callable__::arg_list_type::const_iterator i = cb->get_args().begin(); \
    __REPEAT(N, __TYPE_MISMATCH_TEST__, __NOTHING__, __NOTHING__, __NOTHING__);\
    return std::string(res.str());\
}


#define __TYPE_COMPATIBLE_LOOP__(N) && isCastable( typeid(__TYPE_ARG__(N)), (*(*i++)), __PARAM__(N) )

// dynamic casting & down casting
#define __TYPE_COMPATIBLE_TEST__(N) \
__REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __TEMPLATE_DECLARE_BEGIN__, __TEMPLATE_END__) \
static bool testCompatible##N(__callable__ *cb, const std::type_info &cb_classtype, const std::type_info & rettype, const std::type_info &classtype, void *optr \
__REPEAT(N, __ARG_PTR__, __COMMA__, __COMMA__, __NOTHING__) \
) \
{\
    __callable__::arg_list_type::const_iterator i = cb->get_args().begin(); \
    return isCastable(classtype, cb_classtype, optr) && isCastable(cb->get_ret_type(), rettype)\
	    __REPEAT(N, __TYPE_COMPATIBLE_LOOP__, __NOTHING__, __NOTHING__, __NOTHING__);\
}





//macro that defines an 'invoke' method with a return type
#define __INVOKE__(N)\
    template <class R, class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> void invoke(R &result, C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__, __NOTHING__)) const {\
        if (getAccess() != ACCESS_PUBLIC) throw IllegalAccessError(getName());\
        typedef const __callable##N##__<R, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> CallableType1;\
        typedef const __callable_const##N##__<R, C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> CallableType2;\
        CallableType1 *cb1 = dynamic_cast<CallableType1 *>(m_callable);\
        if (cb1) {\
            result = cb1->invoke(object __REPEAT(N, __PARAM__, __COMMA__, __COMMA__, __NOTHING__));\
            return;\
        }\
        CallableType2 *cb2 = dynamic_cast<CallableType2 *>(m_callable);\
        if (cb2) {\
            result = cb2->invoke(object __REPEAT(N, __PARAM__, __COMMA__, __COMMA__, __NOTHING__));\
            return;\
        }\
        if (testCompatible##N (m_callable, getClass().getPtrTypeInfo(), typeid(R), typeid(C*), &object __REPEAT(N, __PARAM_PTR__, __COMMA__, __COMMA__, __NOTHING__) ))\
        {\
	    /* force casting it */ \
	    cb1 = (CallableType1 *) (m_callable);\
            result = cb1->invoke(object __REPEAT(N, __PARAM__, __COMMA__, __COMMA__, __NOTHING__));\
            return;\
        }\
        std::string errmsg ( findMismatchedType##N __REPEAT(N, __TYPE_ARG__, __COMMA__, __TEMPLATE_BEGIN__, __TEMPLATE_END__) (m_callable, getClass().getTypeInfo(), typeid(R), typeid(C)));\
        throw TypeMismatchError(getSignature() + ":\n" + errmsg);\
    }


//macro that defines an 'invoke' method without a return type
#define __INVOKE_VOID__(N)\
    template <class C __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> void invokeVoid(C *object __REPEAT(N, __ARG__, __COMMA__, __COMMA__, __NOTHING__)) const {\
        if (getAccess() != ACCESS_PUBLIC) throw IllegalAccessError(getName());\
        typedef const __callable_void##N##__<C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> CallableType1;\
        typedef const __callable_const_void##N##__<C __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> CallableType2;\
        CallableType1 *cb1 = dynamic_cast<CallableType1 *>(m_callable);\
        if (cb1) {\
            cb1->invoke(object __REPEAT(N, __PARAM__, __COMMA__, __COMMA__, __NOTHING__));\
            return;\
        }\
        CallableType2 *cb2 = dynamic_cast<CallableType2 *>(m_callable);\
        if (cb2) {\
            cb2->invoke(object __REPEAT(N, __PARAM__, __COMMA__, __COMMA__, __NOTHING__));\
            return;\
        }\
        if (testCompatible##N (m_callable, getClass().getPtrTypeInfo(), typeid(void), typeid(C*), &object __REPEAT(N, __PARAM_PTR__, __COMMA__, __COMMA__, __NOTHING__)))\
        {\
	    /* force casting it */ \
	    cb1 = (CallableType1 *) (m_callable);\
            cb1->invoke(object __REPEAT(N, __PARAM__, __COMMA__, __COMMA__, __NOTHING__));\
            return;\
        }\
        std::string errmsg ( findMismatchedType##N __REPEAT(N, __TYPE_ARG__, __COMMA__, __TEMPLATE_BEGIN__, __TEMPLATE_END__) (m_callable, getClass().getTypeInfo(), typeid(void), typeid(C)));\
        throw TypeMismatchError(getSignature() + ":\n" + errmsg);\
    }



//static invoke non-void method
#define __STATIC_INVOKE__(N, METHOD_NAME, CALLABLE)\
    template <class R __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> \
void METHOD_NAME(R &result __REPEAT(N, __ARG__, __COMMA__, __COMMA__, __NOTHING__)) const {\
        if (getAccess() != ACCESS_PUBLIC) throw IllegalAccessError(getName());\
        typedef const __static_callable##N##__<R __REPEAT(N, __TYPE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> CallableType;\
        CallableType *cb = dynamic_cast<CallableType *>(CALLABLE);\
        if (cb) {\
            result = cb->invoke(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
            return;\
        }\
        if (testCompatible##N (CALLABLE, getClass().getTypeInfo(), typeid(R), getClass().getTypeInfo(), 0 __REPEAT(N, __PARAM_PTR__, __COMMA__, __COMMA__, __NOTHING__)))\
        {\
	    /* force casting it */ \
	    cb = (CallableType *) (CALLABLE);\
            result = cb->invoke(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
            return;\
        }\
        std::string errmsg ( findMismatchedType##N __REPEAT(N, __TYPE_ARG__, __COMMA__, __TEMPLATE_BEGIN__, __TEMPLATE_END__) (CALLABLE, getClass().getTypeInfo(), typeid(R), getClass().getTypeInfo()));\
        throw TypeMismatchError(getSignature() + ":\n" + errmsg);\
    }

//static invoke non-void method
#define __CONSTRUCTOR_PLACEMENT_INVOKE__(N)\
    template <class R __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __COMMA__, __NOTHING__)> \
void invokePlacement(R &result, void* ptr __REPEAT(N, __ARG__, __COMMA__, __COMMA__, __NOTHING__)) const {\
    if (m_placement_callable == 0) throw UnknownMethodError("placement constructor"); \
    this->_invoke_placement(result, ptr __REPEAT(N, __PARAM__, __COMMA__, __COMMA__, __NOTHING__));\
    }

//static invoke void method with 0 params
#define __STATIC_INVOKE_VOID__0 \
__STATIC_INVOKE_VOID__(0)

#if 0
    inline void invokeVoid() const {\
        if (getAccess() != ACCESS_PUBLIC) throw IllegalAccessError(getName());\
        typedef const __static_callable_void0__ CallableType;\
        CallableType *cb = dynamic_cast<CallableType *>(m_callable);\
        if (cb) {\
            cb->invoke();\
            return;\
        }\
        if (testCompatible0 (m_callable, typeid(void), typeid(void), typeid(void), 0))\
        {\
	    /* force casting it */ \
	    cb = (CallableType *) (m_callable);\
            cb->invoke();\
            return;\
        }\
        std::string errmsg ( findMismatchedType0 (m_callable, typeid(void), typeid(void), typeid(void)));\
        throw TypeMismatchError(getSignature() + ":\n" + errmsg);\
    }
#endif

//static invoke void method
#define __STATIC_INVOKE_VOID__(N)\
    __REPEAT(N, __TEMPLATE_ARG__, __COMMA__, __TEMPLATE_DECLARE_BEGIN__, __TEMPLATE_END__) \
void invokeVoid(__REPEAT(N, __ARG__, __COMMA__, __NOTHING__, __NOTHING__)) const {\
        if (getAccess() != ACCESS_PUBLIC) throw IllegalAccessError(getName());\
        typedef const __static_callable_void##N##__ __REPEAT(N, __TYPE_ARG__, __COMMA__, __TEMPLATE_BEGIN__, __TEMPLATE_END__) CallableType;\
        CallableType *cb = dynamic_cast<CallableType *>(m_callable);\
        if (cb) {\
            cb->invoke(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
            return;\
        }\
        if (testCompatible##N (m_callable, typeid(void), typeid(void), typeid(void), 0 __REPEAT(N, __PARAM_PTR__, __COMMA__, __COMMA__, __NOTHING__)))\
        {\
	    /* force casting it */ \
	    cb = (CallableType *) (m_callable);\
            cb->invoke(__REPEAT(N, __PARAM__, __COMMA__, __NOTHING__, __NOTHING__));\
            return;\
        }\
        std::string errmsg ( findMismatchedType##N __REPEAT(N, __TYPE_ARG__, __COMMA__, __TEMPLATE_BEGIN__, __TEMPLATE_END__) (m_callable, typeid(void), typeid(void), typeid(void)) );\
        throw TypeMismatchError(getSignature() + ":\n" + errmsg);\
    }

#ifndef REFLECTION_TYPE_CUSTOM1
#define REFLECTION_TYPE_CUSTOM1(CLASS_NAME) typeid(void)
#endif

#ifndef REFLECTION_TYPE_CUSTOM2
#define REFLECTION_TYPE_CUSTOM2(CLASS_NAME) typeid(void)
#endif

#ifndef REFLECTION_TYPE_CUSTOM3
#define REFLECTION_TYPE_CUSTOM3(CLASS_NAME) typeid(void)
#endif

#ifndef REFLECTION_TYPE_CUSTOM4
#define REFLECTION_TYPE_CUSTOM4(CLASS_NAME) typeid(void)
#endif


/** the CLASS_NEW macro is used to declare reflection support in a class. It must
    be placed on the public part of the class.  This MACRO will allow creation
    of new object using newInstance() method on the Class
    @param CLASS_NAME name of this class.
    @param SUPER_CLASS_NAME name of the super class; if there is no base class,
           pass NullClass.
 */
#define CLASS_NEW(CLASS_NAME, SUPER_CLASS_NAME) \
private:\
    static void * __new_instance0() { return new CLASS_NAME (); } \
    CLASS_NEWINSTANCE(CLASS_NAME, SUPER_CLASS_NAME, &CLASS_NAME::__new_instance0)


/** the CLASS macro is used to declare reflection support in a class. It must
    be placed on the public part of the class.
    @param CLASS_NAME name of this class.
    @param SUPER_CLASS_NAME name of the super class; if there is no base class,
           pass NullClass.
 */
#define CLASS(CLASS_NAME, SUPER_CLASS_NAME) \
private:\
CLASS_NEWINSTANCE(CLASS_NAME, SUPER_CLASS_NAME, 0)


#define CLASS_NEWINSTANCE(CLASS_NAME, SUPER_CLASS_NAME, NEWINSTANCE_FUN) \
__NEWINSTANCE_CALLER__(CLASS_NAME,0);\
__NEWINSTANCE_CALLER__(CLASS_NAME,1);\
__NEWINSTANCE_CALLER__(CLASS_NAME,2);\
__NEWINSTANCE_CALLER__(CLASS_NAME,3);\
__NEWINSTANCE_CALLER__(CLASS_NAME,4);\
__NEWINSTANCE_CALLER__(CLASS_NAME,5);\
__NEWINSTANCE_CALLER__(CLASS_NAME,6);\
__NEWINSTANCE_CALLER__(CLASS_NAME,7);\
__NEWINSTANCE_CALLER__(CLASS_NAME,8);\
__NEWINSTANCE_CALLER__(CLASS_NAME,9);\
__NEWINSTANCE_CALLER__(CLASS_NAME,10);\
__NEWINSTANCE_CALLER__(CLASS_NAME,11);\
__NEWINSTANCE_CALLER__(CLASS_NAME,12);\
__NEWINSTANCE_CALLER__(CLASS_NAME,13);\
__NEWINSTANCE_CALLER__(CLASS_NAME,14);\
__NEWINSTANCE_CALLER__(CLASS_NAME,15);\
__NEWINSTANCE_CALLER__(CLASS_NAME,16);\
__NEWINSTANCE_CALLER__(CLASS_NAME,17);\
__NEWINSTANCE_CALLER__(CLASS_NAME,18);\
__NEWINSTANCE_CALLER__(CLASS_NAME,19);\
__NEWINSTANCE_CALLER__(CLASS_NAME,20);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,0);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,1);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,2);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,3);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,4);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,5);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,6);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,7);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,8);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,9);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,10);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,11);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,12);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,13);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,14);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,15);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,16);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,17);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,18);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,19);\
__NEWINSTANCE_PTR_CALLER__(CLASS_NAME,20);\
static void * __super_cast(void *p) { \
    return dynamic_cast<CLASS_NAME *> ( static_cast<SUPER_CLASS_NAME *>(p)); \
}\
static const void * __super_cast_const(const void *p) { \
    return dynamic_cast<const CLASS_NAME *> ( static_cast<const SUPER_CLASS_NAME *>(p)); \
}\
protected:\
    typedef CLASS_NAME ClassType;\
    static const agm::reflection::Class *getClassStaticPtr() {\
    static const agm::reflection::Class _class(#CLASS_NAME, SUPER_CLASS_NAME::getClassStaticPtr(), sizeof(CLASS_NAME), NEWINSTANCE_FUN, &ClassType::__super_cast, &ClassType::__super_cast_const, typeid(CLASS_NAME), typeid(const CLASS_NAME), typeid(CLASS_NAME *), typeid(const CLASS_NAME *)\
        ,(REFLECTION_TYPE_CUSTOM1(CLASS_NAME))\
        ,(REFLECTION_TYPE_CUSTOM2(CLASS_NAME))\
        ,(REFLECTION_TYPE_CUSTOM3(CLASS_NAME))\
        ,(REFLECTION_TYPE_CUSTOM4(CLASS_NAME)));\
        return &_class;\
    }\
public:\
    static const agm::reflection::Class &getClassStatic() {\
        return *CLASS_NAME::getClassStaticPtr();\
    }\
    virtual const agm::reflection::Class &getClass() const {\
        return *CLASS_NAME::getClassStaticPtr();\
    }



/** The FIELD macro is used to declare a reflected field.
    @param ACCESS_ATTR access attribute (public, protected or private)
    @param FIELD_TYPE type of the field
    @param FIELD_NAME name of the field
 */
#define FIELD(ACCESS_ATTR, FIELD_TYPE, FIELD_NAME)\
_FIELD_LINE(ACCESS_ATTR, FIELD_TYPE, FIELD_NAME, __LINE__)

#define _FIELD_LINE(ACCESS_ATTR, FIELD_TYPE, FIELD_NAME, LINE)\
private:\
    struct REF_JOIN(__field_##FIELD_NAME##__,LINE) {\
        REF_JOIN(__field_##FIELD_NAME##__,LINE)() {\
            static agm::reflection::__register_field__ reg(__OFFSET__(ClassType, FIELD_NAME), typeid(FIELD_TYPE), getClassStaticPtr(), agm::reflection:: ACCESS_##ACCESS_ATTR, #FIELD_TYPE, #FIELD_NAME);\
        }\
    } REF_JOIN(__field_##FIELD_NAME##__,LINE);\
    friend struct REF_JOIN(__field_##FIELD_NAME##__,LINE);\
ACCESS_ATTR :\
    FIELD_TYPE FIELD_NAME


/** The STATIC_FIELD macro is used to declare a reflected static field.
    @param ACCESS_ATTR access attribute (public, protected or private)
    @param FIELD_TYPE type of the field
    @param FIELD_NAME name of the field
 */
#define STATIC_FIELD(ACCESS_ATTR, FIELD_TYPE, FIELD_NAME)\
_STATIC_FIELD_LINE(ACCESS_ATTR, FIELD_TYPE, FIELD_NAME, __LINE__)
    
#define _STATIC_FIELD_LINE(ACCESS_ATTR, FIELD_TYPE, FIELD_NAME, LINE)\
private:\
    struct REF_JOIN(__static_field_##FIELD_NAME##__,LINE) {\
        REF_JOIN(__static_field_##FIELD_NAME##__,LINE)() {\
            static agm::reflection::__register_static_field__ reg((void *)&FIELD_NAME, typeid(FIELD_TYPE), getClassStaticPtr(), agm::reflection:: ACCESS_##ACCESS_ATTR, #FIELD_TYPE, #FIELD_NAME);\
        }\
    } REF_JOIN(__static_field_##FIELD_NAME##__,LINE);\
    friend struct REF_JOIN(__static_field_##FIELD_NAME##__,LINE);\
ACCESS_ATTR :\
    static FIELD_TYPE FIELD_NAME

/** The METHOD macro is used to declare a reflected method.
    @param ACCESS_ATTR access attribute (public, protected or private)
    @param RETURN_TYPE the return type of the method
    @param METHOD_NAME name of the method
    @param METHOD_ARGS the method's arguments enclosed in parentheses
 */
#define METHOD(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)\
    __METHOD__(ACCESS_ATTR, __NOT_VIRTUAL__, RETURN_TYPE, METHOD_NAME, METHOD_ARGS, __LINE__)
#define METHOD_N(NUM, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)\
    __METHOD__(ACCESS_ATTR, __NOT_VIRTUAL__, RETURN_TYPE, METHOD_NAME, METHOD_ARGS, NUM)
#define METHOD0(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(0, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD1(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(1, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD2(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(2, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD3(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(3, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD4(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(4, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD5(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(5, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD6(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(6, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD7(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(7, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD8(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(8, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD9(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(9, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD10(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(10, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD11(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(11, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD12(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(12, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD13(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(13, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD14(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(14, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD15(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(15, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD16(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(16, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD17(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(17, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD18(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(18, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD19(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(19, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define METHOD20(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) METHOD_N(20, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)



/** The VIRTUAL_METHOD macro is used to declare a reflected method that is
    virtual, i.e. can be overloaded by subclasses.
    @param ACCESS_ATTR access attribute (public, protected or private)
    @param VIRTUAL pass virtual or no_virtual
    @param RETURN_TYPE the return type of the method
    @param METHOD_NAME name of the method
    @param METHOD_ARGS the method's arguments enclosed in parentheses
 */
#define VIRTUAL_METHOD(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)\
    __METHOD__(ACCESS_ATTR, virtual, RETURN_TYPE, METHOD_NAME, METHOD_ARGS, __LINE__)

#define VIRTUAL_METHOD_N(NUM, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)\
    __METHOD__(ACCESS_ATTR, virtual, RETURN_TYPE, METHOD_NAME, METHOD_ARGS, NUM)
#define VIRTUAL_METHOD0(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(0, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD1(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(1, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD2(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(2, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD3(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(3, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD4(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(4, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD5(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(5, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD6(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(6, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD7(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(7, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD8(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(8, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD9(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(9, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD10(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(10, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD11(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(11, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD12(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(12, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD13(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(13, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD14(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(14, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD15(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(15, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD16(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(16, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD17(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(17, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD18(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(18, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD19(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(19, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)
#define VIRTUAL_METHOD20(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS) VIRTUAL_METHOD_N(20, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)


/** The STATIC_METHOD macro is used to declare a reflected static method.
    When overloading static method, different STATIC_METHODx MACRO should be used instead.
    @param ACCESS_ATTR access attribute (public, protected or private)
    @param RETURN_TYPE the return type of the method
    @param METHOD_NAME name of the method
    @param METHOD_ARGS the method's arguments enclosed in parentheses
 */
#define STATIC_METHOD(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)\
_STATIC_METHOD_LINE(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS, __LINE__)
#define STATIC_METHOD_N(NUM, ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS)\
_STATIC_METHOD_LINE(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS, NUM)
#define STATIC_METHOD0(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(0, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD1(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(1, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD2(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(2, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD3(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(3, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD4(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(4, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD5(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(5, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD6(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(6, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD7(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(7, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD8(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(8, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD9(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(9, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD10(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(10, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD11(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(11, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD12(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(12, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD13(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(13, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD14(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(14, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD15(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(15, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD16(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(16, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD17(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(17, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD18(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(18, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD19(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(19, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)
#define STATIC_METHOD20(ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS) STATIC_METHOD_N(20, ACCESS_ATTR, RETURN_TYPE, STATIC_METHOD_NAME, METHOD_ARGS)

#define _STATIC_METHOD_LINE(ACCESS_ATTR, RETURN_TYPE, METHOD_NAME, METHOD_ARGS, LINE)\
private:\
    struct REF_JOIN(__static_method_##METHOD_NAME##__,LINE) {\
        REF_JOIN(__static_method_##METHOD_NAME##__,LINE)() {\
            typedef RETURN_TYPE (* method_fun_ptr) METHOD_ARGS; \
            method_fun_ptr fun_ptr = & ClassType::METHOD_NAME ; \
            static agm::reflection::__register_static_method__ reg(agm::reflection::__create_static_callable__(fun_ptr),\
            getClassStaticPtr(),agm::reflection:: ACCESS_##ACCESS_ATTR, #RETURN_TYPE, #METHOD_NAME, #METHOD_ARGS);\
        }\
    } REF_JOIN(__static_method_##METHOD_NAME##__,LINE);\
    friend struct REF_JOIN(__static_method_##METHOD_NAME##__,LINE);\
ACCESS_ATTR :\
    static RETURN_TYPE METHOD_NAME METHOD_ARGS


/** The CONSTRUCTOR macro is used to declare a reflected static method.
    When overloading constructor, different CONSTRUCTORx MACRO should be used.
    @param ACCESS_ATTR access attribute (public, protected or private)
    @param CLASS_NAME name of the method
    @param METHOD_ARGS the method's arguments enclosed in parentheses
 */
#define CONSTRUCTOR(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)\
_CONSTRUCTOR_LINE(ACCESS_ATTR, CLASS_NAME *, CLASS_NAME, METHOD_ARGS, __LINE__)

#define CONSTRUCTOR_N(NUM, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)\
_CONSTRUCTOR_LINE(ACCESS_ATTR, CLASS_NAME *, CLASS_NAME, METHOD_ARGS, NUM)

#define CONSTRUCTOR0(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(0, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR1(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(1, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR2(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(2, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR3(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(3, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR4(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(4, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR5(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(5, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR6(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(6, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR7(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(7, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR8(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(8, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR9(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(9, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR10(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(10, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR11(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(11, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR12(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(12, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR13(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(13, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR14(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(14, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR15(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(15, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR16(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(16, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR17(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(17, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR18(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(18, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR19(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(19, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)
#define CONSTRUCTOR20(ACCESS_ATTR, CLASS_NAME, METHOD_ARGS) CONSTRUCTOR_N(20, ACCESS_ATTR, CLASS_NAME, METHOD_ARGS)

		    
#define _CONSTRUCTOR_LINE(ACCESS_ATTR, RETURN_TYPE, CLASS_NAME, METHOD_ARGS, LINE)\
private:\
    struct REF_JOIN(__constructor_##CLASS_NAME##__,LINE) {\
        REF_JOIN(__constructor_##CLASS_NAME##__,LINE) () {\
            typedef RETURN_TYPE (*new_instance_fun_ptr) METHOD_ARGS;\
            new_instance_fun_ptr fun_ptr = & ClassType :: __new_instance__;\
            static agm::reflection::__register_constructor_method__ reg(agm::reflection::__create_static_callable__(fun_ptr), \
                                                                                agm::reflection::__create_static_callable__(__create_new_instance_ptr__(fun_ptr)), getClassStaticPtr(),agm::reflection:: ACCESS_##ACCESS_ATTR, #RETURN_TYPE, #CLASS_NAME, #METHOD_ARGS);\
        }\
    } REF_JOIN(__constructor_##CLASS_NAME##__,LINE);\
    friend struct REF_JOIN(__constructor_##CLASS_NAME##__,LINE);\
ACCESS_ATTR :\
    CLASS_NAME METHOD_ARGS


/** The PROPERTY macro is used to declare an object's property. When used,
    the class must have two private methods in the form of TYPE get()/void set(TYPE)
    for setting and getting the property. By using this macro, the object 
    gets a property interface for managing the value is if it was a field.
    Arithmetic and other operators work as expected. The defined property
    is reflected: it can be examined and set in run-time. Properties do
    not define an access attribute: there are always public. The property
    does not add any extra bytes to the class, besides those needed for reflection.
    @param TYPE type of the property
    @param NAME name of the property
 */
#define PROPERTY(TYPE, NAME)\
private:\
    template <class T__> class REF_JOIN(__property__,NAME) {\
    public:\
        typedef REF_JOIN(__property__,NAME) <T__> Type;\
        REF_JOIN(__property__,NAME)<T__>() {\
            static agm::reflection::__register_property__ reg(agm::reflection::__create_property__<ClassType, T__>(&ClassType::get_##NAME, &ClassType::set_##NAME), getClassStaticPtr(), #TYPE, #NAME, typeid(TYPE));\
        }\
        inline TYPE get() const {\
            return owner()->get_##NAME();\
        }\
        inline void set(TYPE value) {\
            owner()->set_##NAME(value);\
        }\
        inline operator TYPE () const {\
            return get();\
        }\
        inline bool operator == (TYPE value) const {\
            return get() == value;\
        }\
        inline bool operator != (TYPE value) const {\
            return get() != value;\
        }\
        inline bool operator < (TYPE value) const {\
            return get() < value;\
        }\
        inline bool operator > (TYPE value) const {\
            return get() > value;\
        }\
        inline bool operator <= (TYPE value) const {\
            return get() <= value;\
        }\
        inline bool operator >= (TYPE value) const {\
            return get() >= value;\
        }\
        inline Type &operator = (TYPE value) {\
            set(value);\
            return *this;\
        }\
        inline Type &operator = (const Type &prop) {\
            if (&prop != this) set(prop.get());\
            return *this;\
        }\
        inline Type &operator += (TYPE value) {\
            set(get() + value);\
            return *this;\
        }\
        inline Type &operator -= (TYPE value) {\
            set(get() - value);\
            return *this;\
        }\
        inline Type &operator *= (TYPE value) {\
            set(get() * value);\
            return *this;\
        }\
        inline Type &operator /= (TYPE value) {\
            set(get() / value);\
            return *this;\
        }\
        inline Type &operator %= (TYPE value) {\
            set(get() % value);\
            return *this;\
        }\
        inline Type &operator <<= (int bits) {\
            set(get() << bits);\
            return *this;\
        }\
        inline Type &operator >>= (int bits) {\
            set(get() >> bits);\
            return *this;\
        }\
        inline Type &operator &= (TYPE value) {\
            set(get() & value);\
            return *this;\
        }\
        inline Type &operator |= (TYPE value) {\
            set(get() | value);\
            return *this;\
        }\
        inline Type &operator ^= (TYPE value) {\
            set(get() ^ value);\
            return *this;\
        }\
        inline TYPE operator ~() {\
            return ~get();\
        }\
        inline TYPE operator ++() {\
            TYPE val = get();\
            set(val + 1);\
            return val;\
        }\
        inline TYPE operator ++(int i) {\
            TYPE val = get();\
            set(val + 1);\
            return get();\
        }\
        inline TYPE operator --() {\
            TYPE val = get();\
            set(val - 1);\
            return val;\
        }\
        inline TYPE operator --(int i) {\
            TYPE val = get();\
            set(val - 1);\
            return get();\
        }\
    private:\
        ClassType *owner() {\
            return (ClassType *)(((const char *)this) - __OFFSET__(ClassType, NAME));\
        }\
        const ClassType *owner() const {\
            return (const ClassType *)(((const char *)this) - __OFFSET__(ClassType, NAME));\
        }\
    };\
    friend class REF_JOIN(__property__,NAME)< TYPE >;\
public:\
    REF_JOIN(__property__,NAME)< TYPE > NAME


/** access type enumeration
 */
enum ACCESS_TYPE {
    ///public access
    ACCESS_PUBLIC,

    ///protected access
    ACCESS_PROTECTED,

    ///private access
    ACCESS_PRIVATE
};


//access synonyms used in reflection
#define ACCESS_public        ACCESS_PUBLIC
#define ACCESS_protected     ACCESS_PROTECTED
#define ACCESS_private       ACCESS_PRIVATE


//class Class;

//##############################################################################

/** Exception thrown when there is a type mismatch.
 */
class TypeMismatchError : public std::runtime_error {
public:
    /** the default constructor
        @param what the item that caused the error
     */
    TypeMismatchError(const std::string &what) throw() : runtime_error(what) {
    }
};


/** Exception thrown when there is an illegal access error.
 */
class IllegalAccessError : public std::runtime_error {
public:
    /** the default constructor
        @param what the item that caused the error
     */
    IllegalAccessError(const std::string &what) throw() : runtime_error(what) {
    }
};


/** Exception thrown when an unknown field has been requested.
 */
class UnknownFieldError : public std::runtime_error {
public:
    /** the default constructor
        @param what the item that caused the error
     */
    UnknownFieldError(const std::string &what) throw () : runtime_error(what) {
    }
};


/** Exception thrown when an unknown method has been requested.
 */
class UnknownMethodError : public std::runtime_error {
public:
    /** the default constructor
        @param what the item that caused the error
     */
    UnknownMethodError(const std::string &what) throw() : runtime_error(what) {
    }
};


/** Exception thrown when an unknown property has been requested.
 */
class UnknownPropertyError : public std::runtime_error {
public:
    /** the default constructor
        @param what the item that caused the error
     */
    UnknownPropertyError(const std::string &what) throw() : runtime_error(what) {
    }
};


/** Exception thrown when there the superclass of a class was asked to be
    returned but there was no superclass.
 */
class SuperClassError : public std::runtime_error {
public:
    /** the default constructor
        @param what the item that caused the error
     */
    SuperClassError(const std::string &what) throw() : runtime_error(what) {
    }
};

/** Exception thrown when there the class registration was 
    already registered
 */
class ClassRegistrationError : public std::runtime_error {
public:
    /** the default constructor
        @param what the item that caused the error
     */
    ClassRegistrationError(const std::string &what) throw (): runtime_error(what) {
    }
};

/** Exception thrown when there the Class object is not
    initialized fully.  Should never happen.
 */
class InvalidStateError : public std::runtime_error {
public:
    /** the default constructor
        @param what the item that caused the error
     */
    InvalidStateError(const std::string &what) throw () : runtime_error(what) {
    }
};

/**
   determine whether the from_cls can be casted to to_cls based on inhertiance structure
   If obj_ptr is supplied, it must be of object of from_cls type.  dynamic casting will
   be used to determine whether the obj_ptr is castable to the to_cls
*/
bool isCastable(const std::type_info &from_cls, const std::type_info &to_cls, void *obj_ptr = 0);


//##############################################################################

class Field;
class StaticField;
class Method;
class StaticMethod;
class ConstructorMethod;
class Property;
class TypeInfo;

/** The Class class is used as a placeholder for run-time information of an
    an object's class. A class can be used to query an object's fields, 
    methods, properties, events and super class.
 */
class Class {
public:
    ///type of list of fields
    typedef std::vector<const Field *> FieldList;

    ///type of list of static fields
    typedef std::vector<const StaticField *> StaticFieldList;

    ///type of list of methods
    typedef std::vector<const Method *> MethodList;

    ///type of list of static methods
    typedef std::vector<const StaticMethod *> StaticMethodList;

    ///type of list of constructor methods
    typedef std::vector<const ConstructorMethod *> ConstructorList;

    ///type of list of properties
    typedef std::vector<const Property *> PropertyList;

    ///type of list of properties
    typedef std::vector<const Class *> ClassList;

    ///type of class registry name
    typedef std::map<std::string,const Class *> ClassMap;
    
    ///type of typeid registry map
    typedef std::map<TypeInfo,const Class *> TypeInfoMap;

    ///type of iterator
    typedef ClassList::const_iterator class_iterator;

    ///function pointer type to newInstance invocation
    typedef void * (*_new_instance_fun)();

    ///function pointer type to dynamic cast
    typedef void * (*_super_cast_fun)(void *);
    typedef const void * (*_super_cast_const_fun)(const void *);

    //the default constructor
  Class(const char *name, const Class *super, size_t s, _new_instance_fun fun, _super_cast_fun super_case, _super_cast_const_fun super_cast_const, const std::type_info & tid, const std::type_info &ctid, const std::type_info &ptid, const std::type_info &cptid,
        const std::type_info& c1,
        const std::type_info& c2,
        const std::type_info& c3,
        const std::type_info& c4
        );
    
    virtual ~Class();

    /** returns the class name
        @return the class name
     */
    inline const char *getName() const {
        return m_name;
    }

    /** returns the full class name including namespace, if any.
        @return the class name
     */
    inline const std::string& getFullName() const {
        return m_fullname;
    }

    /** checks if the class has a superclass
        @return true if the class has a superclass
     */
    inline const bool hasSuper() const {
        return m_super != 0;
    }

    /** returns the superclass of the class
        @return the superclass of the class; the superclass must not be null
        @exception SuperClassError thrown if there is no super class
     */
    inline const Class &getSuper() const throw (SuperClassError) {
        if (!m_super) throw SuperClassError(m_name);
        return *super();
    }

  /** return the superclass pointer of the class
      @return the superclass or null if no super class
      @exception SuperClassError thrown if unable to resolve super class
  */
  const Class * super() const throw (SuperClassError);

    /** returns the collection of fields
        @return the collection of fields
     */
    inline const FieldList &getFields() const {
        return m_fields;
    }

    /** returns the collection of static fields
        @return the collection of static fields
     */
    inline const StaticFieldList &getStaticFields() const {
        return m_staticFields;
    }

    /** returns the list of methods
        @return the list of methods
     */
    inline const MethodList &getMethods() const {
        return m_methods;
    }

    /** returns the collection of static methods
        @return the collection of static methods
     */
    inline const StaticMethodList &getStaticMethods() const {
        return m_staticMethods;
    }

    /** returns the collection of properties
        @return the collection of properties
     */
    inline const PropertyList &getProperties() const {
        return m_properties;
    }

    /** checks if this class is a base class of the given class.
        @param cl class to check against
        @return true if this class is a base class of the given class.
     */
    bool isBase(const Class &cl) const;
    

    /** return the sizeof the class instance
	@return class object size
     */
    inline size_t getSize() const {
	return m_size;
    }

    /** checks if this class is the super class of the given class.
        @param cl class to check against
        @return true if this class is the super class of the given class.
     */
    inline bool isSuper(const Class &cl) const {
        return cl.super() == this;
    }

    /** checks if this class is the same as given class
        @param cl class to check against
     */
    inline bool isSame(const Class &cl) const {
        return &cl == this;
    }

  inline bool isSameOrBase(const Class &cl) const {
    return this->isSame(cl) || this->isBase(cl);
  }

    /** checks if the class has a newInstance method defined
        @return true if the class newInstance method
     */
    inline const bool hasNewInstance() const {
        return m_new != 0 || hasDefaultConstructor();
    }

    /** create a instance
        @param cl class to check against
     */
    void * newInstance() const throw (UnknownMethodError);
    

    /** checks if the given class is relative to this class. In other words,
        it checks if this class is a base class of the given class, or if they
        are the same class or the given class is a base class of this class.
        @param cl class to check against
        @return true if this class is a base class of the given class or the
                same class.
     */
    bool isRelative(const Class &cl) const;
    
    /** returns field from name
        @param name name of the field to search for
        @param searchSuper if true, super classes are searched if this class does not have the requested member
        @return the field of given name
        @exception UnknownFieldError
     */
    const Field &getField(const char *name, bool searchSuper = true) const throw (UnknownFieldError);
    

    /** returns static field from name
        @param name name of the field to search for
        @param searchSuper if true, super classes are searched if this class does not have the requested member
        @return the field of given name
        @exception UnknownFieldError
     */
    const StaticField &getStaticField(const char *name, bool searchSuper = true) const throw (UnknownFieldError);
    

    /** returns method from name
        @param name name of the method to search for
        @param searchSuper if true, super classes are searched if this class does not have the requested member
        @return the method of given id
        @exception UnknownMethodError
     */
    const Method &getMethod(const char *name, bool searchSuper = true) const throw (UnknownMethodError);
    


    /** returns list of method that matches the name
        @param name name of the method to search for
        @param searchSuper if true, super classes are searched if this class does not have the requested member
        @return the method list of given id
        @exception UnknownMethodError
     */
    MethodList getAllMethods(const char *name, bool searchSuper = true) const throw (UnknownMethodError);
    

    /** returns static method from name
        @param name name of the method to search for
        @param searchSuper if true, super classes are searched if this class does not have the requested member
        @return the method of given name
        @exception UnknownMethodError
     */
    const StaticMethod &getStaticMethod(const char *name, bool searchSuper = true) const throw (UnknownMethodError);
    

    /** returns list of static method that matches the name
        @param name name of the method to search for
        @param searchSuper if true, super classes are searched if this class does not have the requested member
        @return the static method list of given id
        @exception UnknownMethodError
     */
    StaticMethodList getAllStaticMethods(const char *name, bool searchSuper = true) const throw (UnknownMethodError);
    
    /** returns constructor method from name
        @param name name of the method to search for
        @return the constructor method
        @exception UnknownMethodError
     */
    const ConstructorList &getConstructors() const {
        return m_constructorMethods;
    }

    /** determine if default constructor and newInstance() can be called
	@return true if default constructor is defined
    */
    bool hasDefaultConstructor() const {
	return defaultConstructor() != 0;
    }

    /** return the default constructor
	@return constructor if default constructor is defined
        @exception UnknownMethodError
    */
    const ConstructorMethod &getDefaultConstructor() const throw (UnknownMethodError);
    

    /** returns property from name
        @param name name of the property to search for
        @param searchSuper if true, super classes are searched if this class does not have the requested member
        @return the property of given name
        @exception UnknownPropertyError
     */
    const Property &getProperty(const char *name, bool searchSuper = true) const throw (UnknownPropertyError);
    

    /** return the RTTI std::type_info object for this class
	@return type_info object
    */
    inline const std::type_info& getTypeInfo() const {
	return m_class_typeinfo;
    }

    /** return the RTTI std::type_info object representing a ptr to this class
	@return type_info object
    */
    inline const std::type_info& getPtrTypeInfo() const {
	return m_class_ptr_typeinfo;
    }

    /** search and found the class based on name.  name must be the
	full name
	@return Class if found or 0 if not found
    */
    static const Class * forName(const std::string& name);
    
    /** search and found the class based on type_info.
	@return Class if found or 0 if not found
    */
    static const Class * forType(const std::type_info& tinfo);
    
    /** return the entire class registry list
	@return class registry
     */
    static const ClassList &getClasses();
    
    /** attempt to dynamically cast a pointer which is the super class object
	@return 0 if dynamic cast is unsuccessful; non-zero otherwise
    */
    inline void * dynamicCastFromSuper(void *p) const {
	return ((*m_supercast)(p));
    }

    /** attempt to dynamically cast a const pointer which is the super class object
        @return 0 if dynamic cast is unsuccessful; non-zero otherwise
    */
    inline const void * dynamicCastFromSuperConst(const void *p) const {
	return ((*m_supercastconst)(p));
    }

    /** determine whether dynamically cast a pointer which is the super class object
	@return false if dynamic cast is unsuccessful; true otherwise
    */
    bool dynamicCastableFromSuper(void *super_ptr, const Class *super) const;
    

    /** determine whether dynamically cast a const pointer which is the super class object
	@return false if dynamic cast is unsuccessful; true otherwise
    */
    bool dynamicCastableFromSuper(const void *super_ptr, const Class *super) const;
    
    enum TypeMap {
      Type_Class,
      Type_ClassConst,
      Type_ClassPointer,
      Type_ClassPointerConst,
      Type_Custom1,
      Type_Custom2,
      Type_Custom3,
      Type_Custom4,
      Type_NotFound
    };
    
    static std::pair<TypeMap, const Class *> findType(const std::type_info& t);
    
    static bool registerCustomType(const std::type_info&t, TypeMap tmap, const Class *cls);
    
    static bool registerAlias(const std::string& name, const Class *cls);
    
protected:

  Class(const char *name, const std::type_info& tid);

    /** find default constructor */
    const ConstructorMethod *defaultConstructor() const;
    


    enum NameMap {
	Name_CLASSFULLNAME = 0,
	Name_ALIAS,
	NameMap_SIZE
    };

    typedef std::map<std::string, const Field *> _FieldMap;
    typedef std::map<std::string, const StaticField *> _StaticFieldMap;
    typedef std::multimap<std::string, const Method *> _MethodMap;
    typedef std::multimap<std::string, const StaticMethod *> _StaticMethodMap;
    typedef std::map<std::string, const Property *> _PropertyMap;

    bool registerClassMap(NameMap t, const std::string& name);
    static bool registerType(const std::type_info&t, TypeMap tmap, const Class *cls);

    static const Class * findClassMap(NameMap t, const std::string& name);
    


  static ClassMap& getClassMap(NameMap t);
  static TypeInfoMap& getTypeMap(TypeMap t);
  static ClassList& getClassList();


    
    

    const char *m_name;
    mutable const Class *m_super;
    const _new_instance_fun m_new;
    const size_t m_size;
    const std::type_info & m_class_typeinfo;
    const std::type_info & m_class_ptr_typeinfo;
    const std::string m_fullname;
    const _super_cast_fun m_supercast;
    const _super_cast_const_fun m_supercastconst;
    FieldList m_fields;
    _FieldMap m_fieldMap;
    StaticFieldList m_staticFields;
    _StaticFieldMap m_staticFieldMap;
    MethodList m_methods;
    _MethodMap m_methodMap;
    StaticMethodList m_staticMethods;
    _StaticMethodMap m_staticMethodMap;
    PropertyList m_properties;
    _PropertyMap m_propertyMap;
    ConstructorList m_constructorMethods;

    //adds a field
    void _addField(Field *field);
    
    //adds a static field
    void _addStaticField(StaticField *field);
    

    //adds a method
    void _addMethod(Method *method, __callable__ *cb);
    

    //adds a static method
    void _addStaticMethod(StaticMethod *method, __callable__ *cb);
    

    //adds a constructor
    void _addConstructorMethod(ConstructorMethod *method, __callable__ *cb);
    

    //adds a property
    void _addProperty(Property *prop, __property_base__ *handler);
    

    friend struct __register_field__;
    friend struct __register_static_field__;
    friend struct __register_method__;
    friend struct __register_static_method__;
    friend struct __register_constructor_method__;
    friend struct __register_property__;
    friend bool isCastable(const std::type_info &from_cls, const std::type_info &to_cls, void *objptr );

};

//##############################################################################
//##############################################################################


/** Base class for all parts
 */
class MemberBase {
public:
    //    virtual ~MemberBase() {}
    /** returns the class that the member is declared into.
        @return the class that the member is declared into.
     */
    inline const Class &getClass() const {
        return *m_class;
    }

    /** returns the member's access.
        @return the member's access.
     */
    inline const ACCESS_TYPE getAccess() const {
        return m_access;
    }

    /** returns the member's type.
        @return the member's type.
     */
    inline const char *getType() const {
        return m_type.c_str();
    }

    /** returns the member's name.
        @return the member's name.
     */
    inline const char *getName() const {
        return m_name.c_str();
    }

	    
protected:
    /**
       constructor protected for inhertiance only
    */
    MemberBase(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name);

    /**assignment operator
     */
    MemberBase& operator = (const MemberBase& other);

    /** copy constructor
     */
    MemberBase(const MemberBase & other);

private:
    const Class *m_class;
    enum ACCESS_TYPE m_access;
    std::string m_type;
    std::string m_name;
};


/** The Field class represents an object's field.
 */
class Field : public MemberBase {
public:

    /** return the C++ std::typeinfo RTTI info object
        @return the field's datatype typeinfo.
     */
    inline const std::type_info& getTypeInfo() const {
	return m_typeinfo;
    }

    /** retrieves the field of the given object.
        @param result variable to store the result of the field
        @param object object to set the field of
        @exception TypeMismatchError thrown if the object is of invalid class or 
                   the result is of the wrong type.
     */
    template <class Object, class Value> void get(Value &result, Object *object) const;

    /** sets the field of the given object.
        @param object object to set the field of
        @param value new value of the field
        @exception TypeMismatchError thrown if the object is of invalid class or 
                   the value is of the wrong type.
        @exception IllegalAccessError thrown if the field's access is not public.
     */
    template <class Object, class Value> void set(Object *object, const Value &value) const;

protected:

private:
    const std::type_info &m_typeinfo;
    unsigned long m_offset;

    //default constructor
    Field(unsigned long offset, const std::type_info &typeinfo, const Class *pclass, ACCESS_TYPE access, const char *type, const char *name);
    

    friend class Class;
    friend struct __register_field__;
};


/** The StaticField class represents an object's static field.
 */
class StaticField : public MemberBase {
public:
    
    /** return the C++ std::typeinfo RTTI info object
        @return the static field's datatype typeinfo.
     */
    inline const std::type_info& getTypeInfo() const {
	return m_typeinfo;
    }

    /** retrieves the static field's value.
        @param value variable to store the value of the field
        @exception TypeMismatchError thrown if the object is of invalid class or 
                   the value is of the wrong type.
     */
    template <class Value> void get(Value &value) const {
        if (getAccess() != ACCESS_PUBLIC) throw IllegalAccessError(getName());
        if (typeid(Value) != m_typeinfo) throw TypeMismatchError("value");
        value = *(const Value *)(m_address);
    }

    /** sets the static field's value.
        @param value new value of the field
        @exception TypeMismatchError thrown if the object is of invalid class or 
                   the value is of the wrong type.
        @exception IllegalAccessError thrown if the field's access is not public.
     */
    template <class Value> void set(const Value &value) const {
        if (m_access != ACCESS_PUBLIC) throw IllegalAccessError(m_name);
        if (typeid(Value) != m_typeinfo) throw TypeMismatchError("value");
        *(Value *)(m_address) = value;
    }

private:
    const std::type_info &m_typeinfo;
    void *m_address;

    //default constructor
    StaticField(void *address, const std::type_info &typeinfo, const Class *pclass, ACCESS_TYPE access, const char *type, const char *name);
    

    friend class Class;
    friend struct __register_static_field__;
};

class MethodBase : public MemberBase {
public:
    /** return argument type for parameter information
     */
    typedef __callable__::arg_list_type ArgumentTypeList;

    /** destructor
	
     */
    virtual ~MethodBase() {
        if (m_own_cb) delete m_callable;
    }
    /** returns the method's id (name + arguments).
        @return the method's name.
     */
    const char *getId() const {
        return m_id.c_str();
    }

    /** returns the method's id (name + arguments).
        @return the method's name.
     */
    const std::string &getSignature() const {
        return m_long_id;
    }

    /** returns the method's arguments.
        @return the method's arguments.
     */
    const char *getArgs() const {
        return m_args.c_str();
    }

    /** retrieve argument parameter information
	@return argument parameters
    */
    const ArgumentTypeList & getArgsTypes() const;
    
    /** number of argument defined
	@return argument count
     */
    int getArgsCount() const;
    
    /** retrieve return type information.  For if method does not return
	typeid(void) will be returned
	@return return type
    */
    const std::type_info& getReturnType() const;

    const bool hasReturnType() const {
      return (getReturnType() != typeid(void));
    }

    /** assignment operator
     */
    MethodBase& operator = (const MethodBase& other);
    

protected:
    MethodBase(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name, const char *args);
    
    __callable__ *m_callable;
    virtual std::string getPrefix(const std::type_info &ret_type) const;

    MethodBase(const MethodBase& other);
    

__TYPE_MISMATCH_ERROR_(0);
__TYPE_MISMATCH_ERROR_(1);
__TYPE_MISMATCH_ERROR_(2);
__TYPE_MISMATCH_ERROR_(3);
__TYPE_MISMATCH_ERROR_(4);
__TYPE_MISMATCH_ERROR_(5);
__TYPE_MISMATCH_ERROR_(6);
__TYPE_MISMATCH_ERROR_(7);
__TYPE_MISMATCH_ERROR_(8);
__TYPE_MISMATCH_ERROR_(9);
__TYPE_MISMATCH_ERROR_(10);
__TYPE_MISMATCH_ERROR_(11);
__TYPE_MISMATCH_ERROR_(12);
__TYPE_MISMATCH_ERROR_(13);
__TYPE_MISMATCH_ERROR_(14);
__TYPE_MISMATCH_ERROR_(15);
__TYPE_MISMATCH_ERROR_(16);
__TYPE_MISMATCH_ERROR_(17);
__TYPE_MISMATCH_ERROR_(18);
__TYPE_MISMATCH_ERROR_(19);
__TYPE_MISMATCH_ERROR_(20);

__TYPE_COMPATIBLE_TEST__(0);
__TYPE_COMPATIBLE_TEST__(1);
__TYPE_COMPATIBLE_TEST__(2);
__TYPE_COMPATIBLE_TEST__(3);
__TYPE_COMPATIBLE_TEST__(4);
__TYPE_COMPATIBLE_TEST__(5);
__TYPE_COMPATIBLE_TEST__(6);
__TYPE_COMPATIBLE_TEST__(7);
__TYPE_COMPATIBLE_TEST__(8);
__TYPE_COMPATIBLE_TEST__(9);
__TYPE_COMPATIBLE_TEST__(10);
__TYPE_COMPATIBLE_TEST__(11);
__TYPE_COMPATIBLE_TEST__(12);
__TYPE_COMPATIBLE_TEST__(13);
__TYPE_COMPATIBLE_TEST__(14);
__TYPE_COMPATIBLE_TEST__(15);
__TYPE_COMPATIBLE_TEST__(16);
__TYPE_COMPATIBLE_TEST__(17);
__TYPE_COMPATIBLE_TEST__(18);
__TYPE_COMPATIBLE_TEST__(19);
__TYPE_COMPATIBLE_TEST__(20);

public:

private:
    std::string m_id;
    std::string m_args;
    std::string m_long_id;
    bool m_own_cb;
    void set_callable(__callable__ *cb);
    
    friend class Class;
    
};

/** The Method class represents an object's method.
 */
class Method : public MethodBase {
public:
    ///destructor
    ~Method();
    

    /** returns true if the method is virtual.
        @return true if the method is virtual.
     */
    bool isVirtual() const {
        return m_virtual;
    }

    /** invokes a method that has a result
        @param result optional variable to store the result (if the method is non-void)
        @param object object to execute the method of
        @param t1...tn parameters of the invocation
        @exception IllegalAccessError if the method is not public
        @exception TypeMismatchError if the method has different arguments than
                   the ones passed to it
     */
    __INVOKE__(0)
    __INVOKE__(1)
    __INVOKE__(2)
    __INVOKE__(3)
    __INVOKE__(4)
    __INVOKE__(5)
    __INVOKE__(6)
    __INVOKE__(7)
    __INVOKE__(8)
    __INVOKE__(9)
    __INVOKE__(10)
    __INVOKE__(11)
    __INVOKE__(12)
    __INVOKE__(13)
    __INVOKE__(14)
    __INVOKE__(15)
    __INVOKE__(16)
    __INVOKE__(17)
    __INVOKE__(18)
    __INVOKE__(19)
    __INVOKE__(20)

    /** invokes a method that 'returns' void
        @param object object to execute the method of
        @param t1...tn parameters of the invocation
        @exception IllegalAccessError if the method is not public
        @exception TypeMismatchError if the method has different arguments than
                   the ones passed to it
     */
    __INVOKE_VOID__(0)
    __INVOKE_VOID__(1)
    __INVOKE_VOID__(2)
    __INVOKE_VOID__(3)
    __INVOKE_VOID__(4)
    __INVOKE_VOID__(5)
    __INVOKE_VOID__(6)
    __INVOKE_VOID__(7)
    __INVOKE_VOID__(8)
    __INVOKE_VOID__(9)
    __INVOKE_VOID__(10)
    __INVOKE_VOID__(11)
    __INVOKE_VOID__(12)
    __INVOKE_VOID__(13)
    __INVOKE_VOID__(14)
    __INVOKE_VOID__(15)
    __INVOKE_VOID__(16)
    __INVOKE_VOID__(17)
    __INVOKE_VOID__(18)
    __INVOKE_VOID__(19)
    __INVOKE_VOID__(20)

protected:

private:
    bool m_virtual;

    //default constructor
    Method(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name, const char *args, bool virt);
    

    friend class Class;
    friend struct __register_method__;
};


/** The StaticMethod class represents an object's static method.
 */
class StaticMethod : public MethodBase {
public:

    /** invokes the non-void method
        @param result optional variable to store the result (if the method is non-void)
        @param t1...tn parameters of the invocation
        @exception IllegalAccessError if the method is not public
        @exception TypeMismatchError if the method has different arguments than
                   the ones passed to it
     */
    __STATIC_INVOKE__(0, invoke, m_callable)
    __STATIC_INVOKE__(1, invoke, m_callable)
    __STATIC_INVOKE__(2, invoke, m_callable)
    __STATIC_INVOKE__(3, invoke, m_callable)
    __STATIC_INVOKE__(4, invoke, m_callable)
    __STATIC_INVOKE__(5, invoke, m_callable)
    __STATIC_INVOKE__(6, invoke, m_callable)
    __STATIC_INVOKE__(7, invoke, m_callable)
    __STATIC_INVOKE__(8, invoke, m_callable)
    __STATIC_INVOKE__(9, invoke, m_callable)
    __STATIC_INVOKE__(10, invoke, m_callable)
    __STATIC_INVOKE__(11, invoke, m_callable)
    __STATIC_INVOKE__(12, invoke, m_callable)
    __STATIC_INVOKE__(13, invoke, m_callable)
    __STATIC_INVOKE__(14, invoke, m_callable)
    __STATIC_INVOKE__(15, invoke, m_callable)
    __STATIC_INVOKE__(16, invoke, m_callable)
    __STATIC_INVOKE__(17, invoke, m_callable)
    __STATIC_INVOKE__(18, invoke, m_callable)
    __STATIC_INVOKE__(19, invoke, m_callable)
    __STATIC_INVOKE__(20, invoke, m_callable)

    /** invokes the void method
        @param t1...tn parameters of the invocation
        @exception IllegalAccessError if the method is not public
        @exception TypeMismatchError if the method has different arguments than
                   the ones passed to it
     */
    __STATIC_INVOKE_VOID__(0)
    __STATIC_INVOKE_VOID__(1)
    __STATIC_INVOKE_VOID__(2)
    __STATIC_INVOKE_VOID__(3)
    __STATIC_INVOKE_VOID__(4)
    __STATIC_INVOKE_VOID__(5)
    __STATIC_INVOKE_VOID__(6)
    __STATIC_INVOKE_VOID__(7)
    __STATIC_INVOKE_VOID__(8)
    __STATIC_INVOKE_VOID__(9)
    __STATIC_INVOKE_VOID__(10)
    __STATIC_INVOKE_VOID__(11)
    __STATIC_INVOKE_VOID__(12)
    __STATIC_INVOKE_VOID__(13)
    __STATIC_INVOKE_VOID__(14)
    __STATIC_INVOKE_VOID__(15)
    __STATIC_INVOKE_VOID__(16)
    __STATIC_INVOKE_VOID__(17)
    __STATIC_INVOKE_VOID__(18)
    __STATIC_INVOKE_VOID__(19)
    __STATIC_INVOKE_VOID__(20)

protected:
    //default constructor
    StaticMethod(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name, const char *args);
    

    std::string getPrefix(const std::type_info &ret_type) const;
private:

    friend class Class;
    friend struct __register_static_method__;
};


/** The StaticMethod class represents an object's static method.
 */
class ConstructorMethod : public StaticMethod {
public:
__CONSTRUCTOR_PLACEMENT_INVOKE__(0)
__CONSTRUCTOR_PLACEMENT_INVOKE__(1)
__CONSTRUCTOR_PLACEMENT_INVOKE__(2)
__CONSTRUCTOR_PLACEMENT_INVOKE__(3)
__CONSTRUCTOR_PLACEMENT_INVOKE__(4)
__CONSTRUCTOR_PLACEMENT_INVOKE__(5)
__CONSTRUCTOR_PLACEMENT_INVOKE__(6)
__CONSTRUCTOR_PLACEMENT_INVOKE__(7)
__CONSTRUCTOR_PLACEMENT_INVOKE__(8)
__CONSTRUCTOR_PLACEMENT_INVOKE__(9)
__CONSTRUCTOR_PLACEMENT_INVOKE__(10)
__CONSTRUCTOR_PLACEMENT_INVOKE__(11)
__CONSTRUCTOR_PLACEMENT_INVOKE__(12)
__CONSTRUCTOR_PLACEMENT_INVOKE__(13)
__CONSTRUCTOR_PLACEMENT_INVOKE__(14)
__CONSTRUCTOR_PLACEMENT_INVOKE__(15)
__CONSTRUCTOR_PLACEMENT_INVOKE__(16)
__CONSTRUCTOR_PLACEMENT_INVOKE__(17)
__CONSTRUCTOR_PLACEMENT_INVOKE__(18)
__CONSTRUCTOR_PLACEMENT_INVOKE__(19)

protected:
    std::string getPrefix(const std::type_info &ret_type) const;

private:
    __STATIC_INVOKE__(0, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(1, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(2, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(3, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(4, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(5, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(6, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(7, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(8, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(9, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(10, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(11, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(12, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(13, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(14, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(15, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(16, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(17, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(18, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(19, _invoke_placement, m_placement_callable)
    __STATIC_INVOKE__(20, _invoke_placement, m_placement_callable)

    //default constructor
    ConstructorMethod(const Class *pclass, ACCESS_TYPE access, const char *type, const char *name, const char *args, __callable__*);
    
    __callable__* m_placement_callable;
    
    friend struct __register_constructor_method__;
};

/** The Property class holds information about an object's property.
 */
class Property : public MemberBase{
public:
    ///destructor
    ~Property();
    
    /** retrieve the std::type_info RTTI object
	@return type_info
    */
    inline const std::type_info& getTypeInfo() const {
	return m_typeinfo;
    }

    /** returns the value of the property
        @param result result to store the value to
        @param object object to get the property of
        @exception TypeMismatchError thrown if there is a type mismatch
     */
    template <class Object, class Value> void get(Value &result, const Object *object) const;

    /** sets the property's value
        @param object object to set the property of
        @param value value of the object
        @exception TypeMismatchError thrown if there is a type mismatch
     */
    template <class Object, class Value> void set(Object *object, const Value &value) const;

private:
    __property_base__ *m_handler;
    const std::type_info &m_typeinfo;

    //default constructor
    Property(const Class *pclass, const char *type, const char *name, const std::type_info & typeinfo);
    

    friend class Class;
    friend struct __register_property__;
};

/**
  A simple std::type_info wrapper for use in std container
*/
class TypeInfo {
public:
  inline TypeInfo()
  : pInfo(0)
  {
  }
  
  inline TypeInfo(const std::type_info& tinfo)
  : pInfo(&tinfo)
  {
  }
  
  inline TypeInfo(const TypeInfo& that)
  : pInfo(that.pInfo)
  {
  }
  
  inline TypeInfo& operator=(const TypeInfo& that)
  {
    /* pointer assignment.  so we can safely discard comparing this == that */
    this->pInfo = that.pInfo;
    return *this;
  }
  
  inline const std::type_info& get() const
  {
    if (this->pInfo == 0) throw InvalidStateError("TypeInfo is not initialized");
    return *this->pInfo;
  }
  
  inline const std::type_info& operator *() const
  {
    return this->get();
  }
  
  inline bool before(const TypeInfo& that) const
  {
    return this->get().before(that.get());
  }
  
  inline const char *name() const
  {
    return this->get().name();
  }
  
  private:
  
  const std::type_info* pInfo;
  
};

inline bool operator == (const TypeInfo& a, const TypeInfo& b)
{
  return a.get() == b.get();
}

inline bool operator != (const TypeInfo& a, const TypeInfo& b)
{
  return a.get() != b.get();
}

inline bool operator < (const TypeInfo& a, const TypeInfo& b)
{
  return a.before(b);
}

inline bool operator <= (const TypeInfo& a, const TypeInfo& b)
{
  return a==b || a.before(b);
}

inline bool operator > (const TypeInfo& a, const TypeInfo& b)
{
  return ! (a<=b);
}

inline bool operator >= (const TypeInfo& a, const TypeInfo& b)
{
  return ! (a<b);
}


class Class_Holder : public Class
{
public:
  Class_Holder(const char *name);
  
  Class_Holder(const std::type_info & tid);

  const Class* resolve() const;
};


/** Pseudo-class used when a class does not inherit from another class. It
    can not be instantiated.
 */
class NullClass {
public:
    static const Class *getClassStaticPtr() { return 0; }
private:
    NullClass() {}  /* can't create */
    virtual ~NullClass() {}   /* can't delete or define on local */
};


//internal class for registering a field
struct __register_field__ {
    __register_field__(unsigned long offset, const std::type_info &typeinfo, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name);
    
};


//internal class for registering a static field
struct __register_static_field__ {
    __register_static_field__(void *address, const std::type_info &typeinfo, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name);
    
};


//internal class for registering a method
struct __register_method__ {
    __register_method__(__callable__ *cb, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name, const char *args, const char *virt);
    
};


//internal class for registering a static method
struct __register_static_method__ {
    __register_static_method__(__callable__ *cb, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name, const char *args);
    
};

//internal class for registering a constructor method
struct __register_constructor_method__ {
    __register_constructor_method__(__callable__ *cb, __callable__ *placement_cb, const Class *pclass, enum ACCESS_TYPE access, const char *type, const char *name, const char *args);
    
};


//internal class for registering a property
struct __register_property__ {
    __register_property__(__property_base__ *handler, const Class *pclass, const char *type, const char *name, const std::type_info& typeinfo);
    
};


template <class Object, class Value> void Field::get(Value &result, Object *object) const {
        if (getAccess() != ACCESS_PUBLIC) throw IllegalAccessError(getName());
        const Class * cls = Class::forType(typeid(Object));
        if (cls==0 || !getClass().isRelative(*cls)) throw TypeMismatchError("object");
        if (typeid(Value) != m_typeinfo) throw TypeMismatchError("result");
        result = *(const Value *)(((const char *)object) + m_offset);
    }


template <class Object, class Value> void Field::set(Object *object, const Value &value) const {
        if (getAccess() != ACCESS_PUBLIC) throw IllegalAccessError(getName());
        const Class * cls = Class::forType(typeid(Object));
        if (cls==0 || !getClass().isRelative(*cls)) throw TypeMismatchError("object");
//        if (!getClass().isRelative(object->getClass())) throw TypeMismatchError("object");
        if (typeid(Value) != m_typeinfo) throw TypeMismatchError("value");
        *(Value *)(((char *)object) + m_offset) = value;
    }

template <class Object, class Value> void Property::get(Value &result, const Object *object) const {
        const Class * cls = Class::forType(typeid(Object));
        if (cls==0 || !getClass().isRelative(*cls)) throw TypeMismatchError("object");
//        if (!getClass().isRelative(object->getClass())) throw TypeMismatchError("object");
        typedef const __property_handler__<Value> PropertyType;
        PropertyType *prop = dynamic_cast<PropertyType *>(m_handler);
        if (!prop) throw TypeMismatchError(getName());
        result = prop->get((const void *)object);
    }

template <class Object, class Value> void Property::set(Object *object, const Value &value) const {
        const Class * cls = Class::forType(typeid(Object));
        if (cls==0 || !getClass().isRelative(*cls)) throw TypeMismatchError("object");
//        if (!getClass().isRelative(object->getClass())) throw TypeMismatchError("object");
        typedef const __property_handler__<Value> PropertyType;
        PropertyType *prop = dynamic_cast<PropertyType *>(m_handler);
        if (!prop) throw TypeMismatchError(getName());
        prop->set((void *)object, value);
    }

std::ostream &operator << (std::ostream &str, agm::reflection::ACCESS_TYPE access);
std::ostream &operator << (std::ostream &out, const agm::reflection::Class& pclass);


}}


///alias to Null class for reflection
// typedef doesn't seen to work in gcc
using agm::reflection::NullClass;

#include "o3s/extern/reflection/reflection.tpp"

#endif //AGM_REFLECTION_HPP
// $Id: reflection.h 1766 2008-01-10 13:38:39Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
