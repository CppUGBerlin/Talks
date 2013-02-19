#include <boost/type_erasure/any.hpp>

//#include <boost/type_erasure/concept_interface.hpp>
//#include <boost/type_erasure/rebind_any.hpp>

#include <boost/type_erasure/any_cast.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/tuple.hpp>

#include <iostream>

namespace bte = boost::type_erasure;
namespace mpl = boost::mpl;

//--------------------------------------------------------------------------------------------------

void one() {
   using any_type = bte::any< mpl::vector<
                        bte::copy_constructible<>,
                        bte::typeid_<>,
                        bte::addable<>,
                        bte::ostreamable<>
                    > >;

   any_type x(10);
   any_type y(7);
   any_type z(x + y);
   std::cout << z << std::endl;
}

//--------------------------------------------------------------------------------------------------

void one_alt() {
   using any_type = bte::any< mpl::vector<
                        bte::copy_constructible<bte::_a>,
                        bte::copy_constructible<bte::_b>,
                        bte::typeid_<bte::_a>,
                        bte::addable<bte::_a, bte::_b, bte::_a>
                    > >;

   //any_type x(10);
   //any_type y(7);
   //any_type z(x + y);
   //std::cout << z << std::endl;
}

//--------------------------------------------------------------------------------------------------



void two() {
   using requirements = mpl::vector<
                           bte::copy_constructible<bte::_a>,
                           bte::copy_constructible<bte::_b>,
                           bte::typeid_<bte::_a>,
                           bte::addable<bte::_a, bte::_b, bte::_a>,
                           bte::ostreamable<std::ostream, bte::_a>,
                           bte::ostreamable<std::ostream, bte::_b>
                        >;

   bte::tuple<requirements, bte::_a, bte::_b> t(10.2, 7);
   bte::any<requirements, bte::_a> x(bte::get<0>(t) + bte::get<1>(t));

   std::cout << x << std::endl;
}

//--------------------------------------------------------------------------------------------------

int main() {
   one();
   one_alt();
   two();
}
