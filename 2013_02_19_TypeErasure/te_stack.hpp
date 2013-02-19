#ifndef te_stack_hpp
#define te_stack_hpp

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/concept_interface.hpp>
#include <boost/type_erasure/rebind_any.hpp>

namespace bte = boost::type_erasure;
namespace mpl = boost::mpl;

//--------------------------------------------------------------------------------------------------

template<class C, class T>
struct push_back {
   static void apply(C& cont, const T& arg) { cont.push_back(arg); }
};

namespace boost { namespace type_erasure {
   template<class C, class T, class Base>
   struct concept_interface< ::push_back<C, T>, Base, C> : Base {
      void push_back(typename rebind_any<Base, const T&>::type arg) {
         call(::push_back<C, T>(), *this, arg);
      }
   };
} }

//--------------------------------------------------------------------------------------------------

template<class C, class T>
struct back {
   static T apply(C& cont) { return cont.back(); }
};

namespace boost { namespace type_erasure {
   template<class C, class T, class Base>
   struct concept_interface< ::back<C, T>, Base, C> : Base {
      typename rebind_any<Base, T>::type back() {
         return call(::back<C, T>(), *this);
      }
   };
} }

//--------------------------------------------------------------------------------------------------

template<class C>
struct pop_back {
   static void apply(C& cont) { cont.pop_back(); }
};

namespace boost { namespace type_erasure {
   template<class C, class Base>
   struct concept_interface< ::pop_back<C>, Base, C> : Base {
      void pop_back() { call(::pop_back<C>(), *this); }
   };
} }

//--------------------------------------------------------------------------------------------------

template<class C>
struct empty {
   static bool apply(C& cont) { return cont.empty(); }
};

namespace boost { namespace type_erasure {
   template<class C, class Base>
   struct concept_interface< ::empty<C>, Base, C> : Base {
      bool empty() { return call(::empty<C>(), *this); }
   };
} }

//--------------------------------------------------------------------------------------------------

template<class S = bte::_self, class T = int>
struct stack : mpl::vector<
   push_back<S, T>,
   back<S, T>,
   pop_back<S>,
   empty<S>
> {};

//--------------------------------------------------------------------------------------------------

#endif
