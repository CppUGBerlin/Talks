#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/concept_interface.hpp>
#include <boost/type_erasure/rebind_any.hpp>
#include <vector>


namespace bte = boost::type_erasure;
namespace mpl = boost::mpl;

//--------------------------------------------------------------------------------------------------

template<class OBJ, class T>
struct drawable {
   static void apply(const OBJ& obj, T& os, size_t pos) { obj.draw(os, pos); }
   // LIMITATION: draw(obj, os, pos) doesn't work
};

namespace boost { namespace type_erasure {
   template<class OBJ, class T, class Base>
   struct concept_interface< ::drawable<OBJ, T>, Base, OBJ> : Base {
      void draw(typename rebind_any<Base, T&>::type os, size_t pos) const {
         call(::drawable<OBJ, T>(), *this, os, pos);
      }
   };
} }

using object_t = bte::any< mpl::vector<
                     bte::copy_constructible<>,
                     drawable<bte::_self, std::ostream>
                 >>;

//--------------------------------------------------------------------------------------------------

void draw(const int& value, std::ostream& out, size_t position) {
   out << std::string(position, ' ') << "int = " << value << std::endl;
}
template<> struct drawable<int, std::ostream> {
   static void apply(const int& i, std::ostream& os, size_t pos) { draw(i, os, pos); }
};


void draw(const std::string& value, std::ostream& out, size_t position) {
   out << std::string(position, ' ') << "string = " << value << std::endl;
}
template<> struct drawable<std::string, std::ostream> {
   static void apply(const std::string& s, std::ostream& os, size_t pos) { draw(s, os, pos); }
};

struct my_class_t {
   void draw(std::ostream& out, size_t position) const {
      out << std::string(position, ' ') << "my_class_t" << std::endl;
   }
};

//--------------------------------------------------------------------------------------------------


using document_t = std::vector<object_t>;  // or std::vector<copy_on_write<object_t>>;

void draw(const document_t& x, std::ostream& out, size_t position) {
   out << std::string(position, ' ') << "<document>" << std::endl;
   for (auto& e : x) {
      e.draw(out, position + 2);
   }
   out << std::string(position, ' ') << "</document>" << std::endl;
}

template<> struct drawable<document_t, std::ostream> {
   static void apply(const document_t& d, std::ostream& os, size_t pos) { draw(d, os, pos); }
};


//--------------------------------------------------------------------------------------------------

using history_t = std::vector<document_t>;

void commit(history_t& x) { assert(x.size()); x.push_back(x.back()); }
void undo(history_t& x) { assert(x.size()); x.pop_back(); }
document_t& current(history_t& x) { assert(x.size()); return x.back(); }

//--------------------------------------------------------------------------------------------------

int main () {

   history_t h(1);
   current(h).emplace_back(0);
   current(h).emplace_back(std::string("Hello!"));

   draw(current(h), std::cout, 0);
   std::cout << "--------------------------" << std::endl;

   commit(h);

   current(h).emplace_back(current(h));
   current(h).emplace_back(my_class_t());

   draw(current(h), std::cout, 0);
   std::cout << "--------------------------" << std::endl;

   undo(h);
   draw(current(h), std::cout, 0);
}

