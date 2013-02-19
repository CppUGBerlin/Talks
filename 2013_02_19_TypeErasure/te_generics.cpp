#include <boost/type_erasure/any.hpp>

#include "te_stack.hpp"

#include <vector>
#include <list>

//--------------------------------------------------------------------------------------------------

using any_stack_ref = bte::any<stack<bte::_self, int>, bte::_self&>;

//--------------------------------------------------------------------------------------------------

void fill(any_stack_ref stack) {
   stack.push_back(10);
   stack.push_back(8);
   stack.push_back(6);
   stack.push_back(4);
   stack.push_back(2);
   stack.push_back(0);
}

template <class STACK>
void show_impl(STACK stack) {

   std::cout << "[";
   while (!stack.empty()) {
      std::cout << stack.back();
      stack.pop_back();
      if (!stack.empty()) {std::cout << ", "; }
   }
   std::cout << "]" << std::endl;

   return;
}


void show(const std::vector<int>& stack) { std::cout << "std::vec "; show_impl(stack); }
void show(any_stack_ref stack) { std::cout << "generic "; show_impl(stack); }

//--------------------------------------------------------------------------------------------------

int main() {
   std::list<int> list;
   fill(list);
   show(list);

   std::vector<int> vec;
   fill(vec);
   show(vec);
}

