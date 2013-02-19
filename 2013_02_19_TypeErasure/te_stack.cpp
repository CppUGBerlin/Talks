#include <boost/type_erasure/any.hpp>

#include "te_stack.hpp"

#include <vector>
#include <list>

//--------------------------------------------------------------------------------------------------

using any_stack_ref = bte::any<stack<bte::_self, int>, bte::_self&>;
using any_stack = bte::any< mpl::vector<
                          bte::copy_constructible<>,
                          stack<bte::_self, int>
                       >, bte::_self>;

//--------------------------------------------------------------------------------------------------

any_stack create_stack() {
   return std::list<int>();
   //return std::vector<int>();
}

void fill(any_stack_ref stack) {
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
   show(stack);
}

