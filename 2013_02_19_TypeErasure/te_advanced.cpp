#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/concept_interface.hpp>
#include <boost/type_erasure/rebind_any.hpp>
#include <vector>

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

using container = bte::any<push_back<bte::_self, int>, bte::_self&>;

void fill(container c) {
   c.push_back(10);
   c.push_back(8);
   c.push_back(6);
   c.push_back(4);
   c.push_back(2);
   c.push_back(0);
}

//--------------------------------------------------------------------------------------------------

int main() {
   std::vector<int> vec;

   fill(vec);

   std::cout << "[";
   for (auto& i : vec) {
      std::cout << i << ",";
   }
   std::cout << "]" << std::endl;
}

