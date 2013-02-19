#include <boost/type_erasure/any.hpp>

#include <boost/type_erasure/any_cast.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/tuple.hpp>

#include <iostream>

namespace bte = boost::type_erasure;
namespace mpl = boost::mpl;

//--------------------------------------------------------------------------------------------------

int main() {
   typedef bte::any< mpl::vector<
         bte::copy_constructible<>,
         bte::typeid_<>,
         bte::addable<>,
         bte::ostreamable<>
      > > any_type;

   any_type x(10);
   any_type y(7);
   any_type z(x + y);
   std::cout << z << std::endl;
}


