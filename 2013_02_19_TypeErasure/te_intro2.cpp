#include <boost/type_erasure/any.hpp>

#include <boost/type_erasure/concept_interface.hpp>
#include <boost/type_erasure/rebind_any.hpp>

namespace bte = boost::type_erasure;
namespace mpl = boost::mpl;

//--------------------------------------------------------------------------------------------------

template <class CLASS>
struct one_who_gives_orders {
   static void apply(CLASS& c) { c.give_order(); }
};

namespace boost { namespace type_erasure {
   template<class CLASS, class Base>
   struct concept_interface< ::one_who_gives_orders<CLASS>, Base, CLASS> : Base {
      void give_order() { call(::one_who_gives_orders<CLASS>(), *this); }
   };
}}

using leader = bte::any<mpl::vector<bte::copy_constructible<>,one_who_gives_orders<bte::_self>>>;

void emergency_promote(leader lead) {
   lead.give_order();
}

//--------------------------------------------------------------------------------------------------

struct young_kirk {
   void misbehave() { std::cout << "Driving uncles car seems like a good idea!" << std::endl; }
   void give_order() { std::cout << "Beam me up!" << std::endl; }
};

//--------------------------------------------------------------------------------------------------

struct young_spock {
   void be_logical() { std::cout << "<no emotions>" << std::endl; }
   void live_long_and_prosper() { std::cout << "Live long and prosper!" << std::endl; }
};

leader select_someone_suitable() {
   return young_kirk();
   //return young_spock();
}

//--------------------------------------------------------------------------------------------------

int main() {

   young_kirk james;
   james.misbehave();

   emergency_promote(james);

   emergency_promote(select_someone_suitable());
}

//--------------------------------------------------------------------------------------------------

