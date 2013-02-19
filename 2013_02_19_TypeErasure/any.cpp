// based on boost any (Kevlin Henney, http://www.boost.org/libs/any )
// shortend to highlight key implementation details.

//----------------------------------------------------------------------------------------

struct any {
   ~any() { delete content; }

   any(const any & other) : content(other.content ? other.content->clone() : 0) {}

   template<typename ValueType>
   any(const ValueType & value) : content(new holder<ValueType>(value)) {}

private: // types

   struct placeholder {

      virtual ~placeholder() {}
      virtual placeholder * clone() const = 0;
   };

   template<typename ValueType>
   struct holder : placeholder {

      holder(const ValueType & value) : held(value) {}
      virtual placeholder * clone() const { return new holder(held); }

      private:
         ValueType held;
   };

   placeholder * content;
};

//----------------------------------------------------------------------------------------

void black_box(any param) {
   any copy = param;
}

int main() {
   any value = 5;
   black_box(value);
}
