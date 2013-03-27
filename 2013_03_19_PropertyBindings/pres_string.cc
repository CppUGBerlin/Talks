#include <vector>
#include <string>
#include <iostream>



namespace RE {

typedef std::vector<std::string> vec;

template <typename ...> struct list {};

template <typename e, typename l> struct cons;
template <typename e, typename ... l> struct cons<e, list<l...>>
{ typedef list<e, l...> result; };

template <char ...> struct match;


template <> struct match<> {
    static bool m(vec &, const char *&s) { return true; }
};
template <char h, char...t> struct match<h, t...> : match<t...>
{

    static bool m(vec &v, const char *&s) {
        char c = *s;
        if (!c)
            return false;
        if (c != h && h != '.')
            return false;
        s++;
        return match<t...>::m(v, s);
    }
};


template <char...t> struct capture
{
    static bool m(vec &v, const char *&s) {
        const char *begin = s;
        if (!match<t...>::m(v, s))
            return false;
        v.foo(t...);
        v.push_back(std::string(begin, s));
        return true;
    }
};



template <char, typename R> struct add_char;
template <char c, typename ... t, template <char...> class e, char ...x>
struct add_char<c, list<e<x...>, t...>> { typedef list<e<c, x...>, t...> result; };


template <char... > struct parse_rx;

template<> struct parse_rx<>
{ typedef list<match<>> result; };

template <char... t> struct parse_rx<'\0', t...> : parse_rx<>  { };


template <char h, char... t>
struct parse_rx<h, t...>  {
    typedef typename add_char<h, typename parse_rx<t...>::result>::result result;
};

template <char... t>
struct parse_rx<')', t...>  {
    typedef typename cons<capture<>, typename parse_rx<t...>::result>::result result;
};

template <char... t>
struct parse_rx<'(', t...>  {
    typedef typename cons<match<>, typename parse_rx<t...>::result>::result result;
};


template<typename W> struct match_rx;
template<> struct match_rx<list<>> {
    static bool m(vec &, const char *s) { return true; }
};
template<typename H, typename... T> struct match_rx<list<H, T...>> {
    static bool m(vec &v, const char *s) {
        if (!H::m(v, s))
            return false;
        return match_rx<list<T...>>::m(v, s);
    }
};

}

template <char ... S> struct RegExp {

    bool match(const char *s) {
        result.clear();

        typedef typename RE::parse_rx<S...>::result p;

        return RE::match_rx<p>::m(result, s);

    }

    template<int N> std::string cap() { return result[N]; }
private:
    RE::vec result;
};



template <char...S> struct string {};

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



template<typename S> struct build_rx;
template<char... S> struct build_rx<string<S...>> { typedef RegExp<S...> result; };


#include <iostream>
int main() {
    STRING(r, "(....)-(..)-(..)");
    build_rx<r>::result rx;
    rx.match("2013-03-19");
    std::cout << rx.cap<0>() <<"/" << rx.cap<1>() << "/" << rx.cap<2>() << std::endl;
}

