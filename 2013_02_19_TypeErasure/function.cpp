#include <vector>
#include <functional>
#include <iostream>

using functions = std::vector<std::function<void (int)>>;

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

