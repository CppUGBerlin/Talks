#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/any_cast.hpp>

#include "te_stack.hpp"

#include <vector>
#include <list>


//--------------------------------------------------------------------------------------------------

using any_stack_ref = bte::any<stack<bte::_self, int>, bte::_self&>;
using any_push_back_ref = bte::any<push_back<bte::_self, int>, bte::_self&>;
using any_stack = bte::any< mpl::vector<
                          bte::copy_constructible<>,
                          bte::typeid_<>,
                          stack<bte::_self, int>
                       >, bte::_self>;



//--------------------------------------------------------------------------------------------------

any_stack create_stack() {
   return std::list<int>();
}

void fill(any_push_back_ref stack) {
   stack.push_back(10);
   stack.push_back(8);
   stack.push_back(6);
   stack.push_back(4);
   stack.push_back(2);
   stack.push_back(0);
}

void show(any_stack_ref stack) {

   std::cout << "[";
   while (!stack.empty()) {
      std::cout << stack.back();
      stack.pop_back();
      if (!stack.empty()) {std::cout << ", "; }
   }
   std::cout << "]" << std::endl;

   return;
}

//--------------------------------------------------------------------------------------------------

int main() {
   any_stack stack = create_stack();
      fill(stack);
   any_push_back_ref pb = stack;
      fill(pb);

   // feature doesn't exist!
   //any_stack_ref sr = dynamic_concept_cast<???>(pb);
   //show(sr);

   std::list<int> l = bte::any_cast<std::list<int>>(stack);
   show(l);
   show(l);
   show(stack);
   show(stack);

   try {
      std::vector<int> v = bte::any_cast<std::vector<int>>(stack);
      assert(false);
   } catch (const bte::bad_any_cast&) {
      std::cout << "stack does not contain a vector!" << std::endl;
   }

   // casting

   struct Base {};
   struct Derived : Base {};

   using any = bte::any<mpl::vector<bte::copy_constructible<>, bte::typeid_<>>>;

   Base* base_ptr = new Derived();
   any ap = base_ptr;
   try {
      Derived* derived_ptr = bte::any_cast<Derived*>(ap);
      assert(false);
   } catch (const bte::bad_any_cast&) {
      std::cout << "WRONG! WRONG! cannot tell that ap does contain a Derived*!" << std::endl;
   }

   delete base_ptr;
}

