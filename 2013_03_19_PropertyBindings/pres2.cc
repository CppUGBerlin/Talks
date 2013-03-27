#include <string>
#include <iostream>

template <char... S> struct RegExp;


void f1() {
#if 0 //
    RegExp<"(..)-(..)"> rx;
    rx.match("03-19");
    std::cout << rx.cap<0>() <<"/" << rx.cap<1>() << std::endl;
#endif
}



#if 0//

template <char ...S> struct RegExp {
    bool match(const std::string&);
    template <int N> std::string cap();
};

#endif




#include <regex>
template <char ... S> struct RegExp {

    bool match(const std::string &s) {
        return std::regex_match(s, result, impl);
    }

    template<int N> std::string cap() {
        return result[N+1];
    }

private:
    static std::string str() {
        char s[] = {S..., '\0'};
        return s;
    }
    std::regex impl { str() };
    std::smatch result;
};







void f2() {
    RegExp<'(', '.', '.', ')', '-' , '(' , '.', '.', ')'> rx;
    rx.match("03-19");
    std::cout << rx.cap<0>() <<"/" << rx.cap<1>() << std::endl;
}











template<char...S>
RegExp<S...> operator "" _rx() {
    return {};
}


void f3() {
#if 0 //
    auto rx = "(..)-(..)"_rx;
    rx.match("03-19");
    std::cout << rx.cap<0>() <<"/" << rx.cap<1>() << std::endl;
#endif
}







template<wchar_t ...> struct wstr {};

void f4() {
    typedef wstr<'(..)', '-(..', ')'> m;
}











#include <boost/mpl/string.hpp>

void f5() {
    typedef boost::mpl::string<'(..)', '-(..', ')'> m;
}













void f6() {

    enum { Foo = "hello"[2] };

#define Str9(S) \
    S[0], S[1], S[2], S[3], S[4], S[5] , S[6], S[7], S[8], S[9]

    RegExp<Str9("(..)-(..)")> rx;
    rx.match("03-19");
    std::cout << rx.cap<0>() <<"/" << rx.cap<1>() << std::endl;
}


























template <char...S> struct string {
    static void print() {
        char foo[] = { S... , '\0' };
        std::cout << foo << std::endl;
    }
};

template <int...> struct idx {};
template <typename I, int N> struct push_back;
template <int N,  int...I> struct push_back<idx<I...>, N> {
    typedef idx<I..., N> result;
};
template <int N> struct make_idx {
    typedef typename push_back<typename make_idx<N-1>::result, N-1>::result result;
};
template <> struct make_idx<0> { typedef idx<> result; };


#define STRING(NAME, STR) \
struct build_string_##__LINE__ { \
template <typename idx> struct build;\
template <int ...I> struct build<idx<I...>> { \
typedef  string<STR[I]...> result; }; }; \
typedef build_string_##__LINE__::build<make_idx<sizeof(STR)>::result>::result NAME


void f7() {
    STRING (foo, "Hello World!");
    foo::print();
}












template<int N>
constexpr int atoi_helper(const char (&val)[N+1], int pos) {
    return pos >= N ? 0 :
        10 * atoi_helper<N>(val, pos+1) + (val[N-pos-1] - '0');
}



template<int N>
constexpr int cst_atoi(const char (&val)[N]) {
    return atoi_helper<N-1>(val, 0);
}


int f8() {
    enum { Val = cst_atoi("315") };

    std::cout << Val << std::endl;
    return Val;
}















int main() {
    f8();
}
#if 0
#endif

