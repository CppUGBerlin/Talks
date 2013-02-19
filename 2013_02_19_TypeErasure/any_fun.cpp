// based on boost any (Kevlin Henney, http://www.boost.org/libs/any )
// shortend to highlight key implementation details.

#include <vector>
#include <iostream>
#include <cassert>

//----------------------------------------------------------------------------------------

struct any_fun {
   ~any_fun() { delete content; }

   any_fun(const any_fun & other) : content(other.content ? other.content->clone() : 0) {}

   template<typename ValueType>
   any_fun(const ValueType & value) : content(new holder<ValueType>(value)) {}

   void operator()(int param) { assert(content); content->call(param); }

private: // types

   struct placeholder {

      virtual ~placeholder() {}
      virtual placeholder * clone() const = 0;

      virtual void call(int param) const = 0;
   };

   template<typename ValueType>
   struct holder : placeholder {

      holder(const ValueType & value) : held(value) {}
      virtual placeholder * clone() const { return new holder(held); }

      virtual void call(int param) const { held(param); }

      private:
         ValueType held;
   };

   placeholder * content;
};

//----------------------------------------------------------------------------------------

using functions = std::vector<any_fun>;

void call_all(functions funs, int value) {
   for(auto& fun : funs) fun(value);
}

void one(int p) { std::cout << "1: " << p << std::endl; }
void two(int p) { std::cout << "2: " << p * 2 << std::endl; }

int main() {
   functions f;
      f.emplace_back(&one);
      f.emplace_back(&two);
   call_all(f, 21);
}

