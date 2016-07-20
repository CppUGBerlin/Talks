## Fabio Fracassi

http://fabio.fracassi.de

## C++17

**What it is not:** Flashy signature featurs
 * No Modules
 * No Concepts
 * No default comparisons

 Those are great, but will have to wait a while longer.

----

**What it is:**
 * More library than language.
 * A few big (though not flashy) features
 * A lot of smaller Features that make stuff much easier
 * Quite a few fixes

----

if you want to know more follow the paper trail, http://wg21.link/[PN]xxxx 
will take you directly to the latest revision of the paper.

----

## Extending our vocabulary
----

##### Optional (from Lib Fundamentals)

	if (std::optional<int> value = get_thing()) {
		use(*value);
	} else {
		log("meh!");
	}

or more concisely (using exceptions for error handling):

	int value = get_thing().value()

or if you do not need errors but defaults:

	int value = get_thing().value_or(-1);

you should be using optional a lot. 
Basically every time you want to return a value that might not be there. 
Optional is purely stack based and has only a small overhead (1 byte + alignment)

---

**The future:** there is a very similar class in the works:

	expected<Type, ErrorType> 

to be used when you want to also give a reason on why there is no value.

---

##### Variant (Woooooohooooooo!)

a typesafe union, and an efficient way to do dynamic dispatch:

	//------------------- library: ----
	using shape = std::variant<std::monostate, point, line, rectangle>;

	auto intersect(shape const& lhs, shape const& rhs) -> shape {
		return std::visit( 
					[](auto const& lhs, auto const& rhs ) { return intersect(lhs, rhs); }
			   		, lhs, rhs
			   );
	}

	void show(shape const & s) {
		std::visit( not_yet_std::overload(
					[](point const& p)     { draw(p); }
					[](line const& l)      { draw(l); }
					[](rectangle const& r) { draw(r); }
				  ), s);
	}
	//------------------- usage: ----
	{
		Shape r1 = rectangle{ {5, 5}, {10, 10} };
		Shape r2 = rectangle{ {9, 5}, {15, 10} };
		Shape l1 = line{ {7, 4}, {8, 11} };

		auto i1 = intersect(r1, r2);
		auto i2 = intersect(r1, l1);
		auto i3 = intersect(r2, l1);

		show(i1);
		show(i2);
		show(i3);
	}

**Good performance:**

 * No Heap needed
 * amendable to compiler optimizations (const propagation, inlineing) 
 * no aliasing

**Easily understandable:**

 * value semantics
 * bound(controlled) set of types

----

##### Any

the generic value type to use when anything else fails.
or: "a safer void pointer"

A note of caution: Whenever you reach for this type, 
check if you can use variant instead, because more often than not 
variant is the better option


----

##### string_view

the simplest type you never knew you needed. 

consider:

	void process(std::string const& s);

	auto hw = std::string("Hello World");
	process("Hello World"); // meh! performance bug, temporary string on each call
	process(hw); 			// yay!

or: 

	auto hw = std::string("Hello World");
	process("Hello World"); // yay!
	process(hw); 		    // does not even compile, need ugly hw.c_str();

solution pre- string_view: 

	void process(std::string const& s) { process(s.c_str()); }
	void process(char const* sz);  // meh! no size, and can't use string stuff in here. 

C++17:

	void process(std::string_view sv) // by value! safe interface

	process("Hello World"); // yay, no temporaries
	process(hw); 		    // works!

Whenever you do not need to take ownership of a string you should use 
`std::string_view` from now on. This will make your code conveniently and
efficiently useable with any contigious char range.

A nice little example I thought of recently:

	template<typename ... Strs> auto string_cat(Strs ... strs) -> std::string {  
	   std::array<std::string_view, sizeof...(Strs) av {std::string_view(strs)...};
	   std::string r(accumulate(av, [](auto v){ return v.size(); }), '\0');
	   size_t cpos = 0;
	   for (auto const& v : av) { 
	      std::copy(v.begin(), v.end(), r.data() + cpos);  // std::string::data() can be non-const in 17
	      cpos += v.size(); 
	   }
	}

One caveat:

`string_view` is not guaranteed to be zero terminated! 
So be aware: 

	void read_config(std::string_view sv) {
		// std::fstream(sv.data()); // BUGBUG!!!
		std::fstream(std::to_string(sv)); // Works, because it creates a temporary string
	}

**solution:** *???* I would suggest a zstring_view, that deals with the details

**future:**
 we will get a very similar class `std::span<T>` soon after C++17
 `std::span<char const>` is nearly identical to string_view but with a lighter interface.
 `std::span<char>` can be used to change the underlying buffer. 

----

 On that note: we have **range based for with sentinels** ([P0184](http://wg21.link/P0184)) now, so: 

	struct zstring_view {
		zstring_view(char const* p) : ptr_(p) {}
		zstring_view(std::string_view sv) : ptr_(sv.data()) { assert(*sv.back() == '\0'); }
		// more ctors ...

		struct iter {
			iter(char const* p) : ptr_(p) {}
			auto operator++() -> iter& { ++ptr_; return *this; }
			auto operator*() const -> char { return *ptr_; }
			private: 
				char const* ptr_;
		};
		struct sentinel {};
		auto begin() -> iter { return iter(ptr_); }
		auto end() -> sentinel { return sentinel{}; }
		
		private: 
			char const* ptr_;
	};
	bool operator!=(zstring_view::iter const& it, zstring_view::sentinel) { return *it != '\0'; }

	zstring_view hello("Hello World");
	for (auto c : hello) { do_something_with(c); } // look ma! no strlen!

----

## Language features


Ok, I lied, there are a few cool language features

and they integrate quite nicely, too.

----

##### [Selection statements with initializer](http://wg21.link/P0305)
##### [Structured bindings](http://wg21.link/P0217)

	std::map<int, std::string> m;
	if (auto [pos, inserted] = m.insert({1701, "Live long and prosper!"}); !inserted) {
		std::cout << "Of course you know Star Trek, welcome back!\n";
		use(pos);
	} else {
		std::cout << "Took you long enough! Welcome on board.\n";
	}

Sweet sweet syntactic sugar!

it is not quite lambda expressions, but I expect seeing more multiple 
return value designs in the future.

----

##### [Template argument deduction for class templates](http://wg21.link/P0091)
##### `[[nodiscard]]`
##### `std::uncaught_exceptions` [N4259](http://wg21.link/N4259)

getting rid of gratious factory functions

	template<typename OnSuccessFn, typename OnFailureFn> 
	struct [[nodiscard]] sg_base {
		sg_base(OnSuccessFn osf, OnFailureFn off) 
			: exceptions_on_entry_{std::uncaught_exceptions()}
			, success_(osf)
			, failure_(off) 
		{}
		~sg_base() {
			if(std::uncaught_exceptions() > exceptions_on_entry_) { failure_(); }
															 else { success_(); }
		}
		private:
			int exceptions_on_entry_;
			OnSuccessFn success_;
			OnFailureFn failure_;
	};
	template<typename Fn> struct on_success : sg_base { on_success(Fn fn) : sg_base(fn, []{}) {}};
	template<typename Fn> struct on_failure : sg_base { on_failure(Fn fn) : sg_base([]{}, fn) {}};
	template<typename Fn> struct [[nodiscard]] on_exit { 
		on_exit(Fn fn) : exit_(fn) {} ~on_exit() { exit_(); } private: Fn exit_; 
	};

	{
		// on_exit (log("done!")); // that would be wrong, and the compiler will tell us!
		on_exit eg (log("done!"));  // look, no factory!
		Transaction t;
		on_success os([&]{t.commit();});
		on_failure of([&]{t.cleanup();});

		fill(t);	
	}

**The future:** 

we will get some form of scope guards like those in a future standard. [P0052](http://wg21.link/P0052)

----

## Lots more ...

language and library

##### searchers

	std::string haystack = load_text("TheCompleteWorksOfShakespeare.txt");
	std::string needle = "Julia";

	if  ( auto here = std::search( std::begin(haystack), std::end(haystack)
								 , std::make_boyer_moore_searcher( std::begin(needle)
								 								 , std::end(needle)))
		; here != std::end(haystack)
		)
	{
		romeo.julia_is(here);
	} else {
		romeo.set_status(mood::sad);
	}

----

##### [Elementary string conversions](http://wg21.link/p0067) 

`to_chars` / `from_chars` writing out/parsing numbers, 
without locales, format strings, etc.

very low-level, efficient interface.
 
----

##### [Refining Expression Evaluation Order for Idiomatic C++](http://wg21.link/P0145)

make `future.then().then()` work!
or `str.replace().replace().replace()`

guaranteed order for
	assignment: `x = y`   `x += y`    `x -= y`    ...  (y before x)
everything else, left to right:

	x << y 
	x >> y
	x.y
	x->y
	... 

still indetermined order:
	`f(a,b,c)` but no interleaving any longer between a,b,c

---

##### [void_t](http://wg21.link/N3911)

	namespace std { template <class...> using void_t = void; }

that's it! The whole feature! 

It is simple but fairly useful:

	template< class, class = void > 
	struct has_render_member                               : std::false_type {};
	template< class T > 
	struct has_render_member <T, std::void_t<T::render()>> : std::true_type  {};

---

##### [if constexpr](http://wg21.link/P0292)

	template<typename App> struct app {
		void run() {
			if constexpr (has_render_with_time_member_v<App>) {
				auto start = clock::now();
				while (!close_requested()) {
					auto now = clock::now();
					render(now - start);
				}
			} else if constexpr (has_render_member_v<App>) {
				while (!close_requested()) { render(); }
			} else {
				static_assert( std::is_same_v<App, App>
							 , "you need to implement render() or render(duration)"
							 );
			}
		}
	};

---

##### [Constexpr Lambda](http://wg21.link/P0170)

	auto addOne = [](int n) { return n + 1; };
	static_assert( addOne(3) == 4);

oh, the last one is: **[improved static_assert](http://wg21.link/N3928)** btw:

	static_assert(sizeof(T) > 4); // compiler should now give a good message like:
	                              //    sizeof(T) > 4 [with T = myInt] failed.

---

##### Lambda Capture of `*this` by value [P0018](http://wg21.link/P0018) 

you couldn't do `[=,*this]` before, which lead to strange
javascripty workarounds like `[=, self = *this] { ... self->meh(); }`

---

##### [Ordered by Default](http://wg21.link/p0181)

	my_complex x{10.3, -2.3}, y{1.3, 2.7};
	bool smaller = x < y; // doesn't make sense, should not compile
	template <> struct default_order<my_complex> { using type = my_complex_map_order; };
	std::map<my_complex, some_data> cmap = /* ... */;

--- 

##### [Extending memory management tools](http://wg21.link/p0040)   

`uninitialized_*`

---

##### [make_from_tuple](http://wg21.link/p0209) 

	struct X { X(int, float); X(int, int, float)};
	tuple<int, int, float> t{1, 2, 3.0};
	auto x = std::make_form_tuple<X>(t); // constructs X using the second ctor

---

##### [Splicing Maps and Sets](http://wg21.link/p0083)

---

##### [Contiguous Iterators](http://wg21.link/N4284)

   new iterator category which allows to exploit more optimizations
   (e.g. SIMD operations)

---

##### [Variadic lock_guard](http://wg21.link/P0156)

	{
		auto lk = lock_guard(mx1, mx2, mx3);

	}

--- 

##### `clamp` a value between a pair of boundary values [P0025](http://wg21.link/P0025)
 
	auto red = std::clamp(input.red, 0, 255);

---

##### `not_fn` [P0005](http://wg21.link/P0005) 

	std::remove_if(v.begin(), v.end(), std::not_fn(my_type::is_valid));
 
---

##### Declaring non-type template parameters with auto [P0127](http://wg21.link/P0127)

	template < auto T t > struct constant {	constexpr T value = t };

---

##### [shared_mutex](http://wg21.link/N4508)

This one was missing, you need it to implent efficient single producer/multiple consumer scenarios

---

## Invisible? goodies

those will help you even if you do not know they are there, 
or you have been using them for a while even though they were not stantdard

---

##### [guaranteed copy elision](http://wg21.link/P0135)

Every major compiler has been doing this for several years anyway. 
the guarantee allows non-copyable, non-moveable types to be 
returned from (e.g. factory) functions

----

##### non-const `.data()` for `std::string` [P0272](http://wg21.link/P0272)

----

##### [allow for inline variables](http://wg21.link/P0386)

	namespace mine {
		inline int global = 10; // look ma, I already got my value!
	}
	// no need to define int mine::global = 10 in a separete and unique TU.

 But global state is evil! 
 Why is this great? 

 One Word: **Header only libraries!** 

---

## Small goodies

	switch(i) {
		case 1: unique_action(); break;
		case 2: prepare(); [[fallthrough]] // trust me I think I know what I am doing!
		case 3: action(); break
		// ... 
	}

---

	template<template<typename...> typename Container> auto accumulate(Container const& c);
	//                             ^^^^ you had to write class here before (meh!)

---

**u8 character literals** [N4267](http://wg21.link/N4267) `auto utf8 = "Hällo Wórld: "u8;`

---

##### Constant View: `std::as_const` [P0007](http://wg21.link/P0007) 

	void my_func(std::vector<int>& v) {
		modify(v);
		do_something(std::as_const(v)); // make sure that the non modifing do_something gets called
	}

---

##### Improved insertion interface for unique-key maps [N4279](http://wg21.link/N4279)

	std::map<int, Heavy> m;
	// insert stuff
	m.try_emplace(42, ??? )
	
---

##### [Emplace Return Type](http://wg21.link/p0084)

  `m.emplace(Heavy(45, 34)).set_heavy_param(5);`
  Note: doesn't work for push_back (abi issues)

---

##### Non-member `size()`

free functions work better in generic code

---

##### [Improved pair and tuple](http://wg21.link/N4387)

	// std::tuple<int, int> foo_tuple() { return {1, -1};  /* tuple ctor is always explicit in C++14 */ }
	std::tuple<int, int> foo_tuple()    { return std::tuple<int, int>{1, -1};  /* OK in C++14 */ }
	std::tuple<int, int> foo_tuple()    { return {1, -1};     /* Now works in C++17 */ }

---

##### Minimal incomplete type support for standard containers [N4510](http://wg21.link/N4510)

	struct entry { std::vector<entry> sub_entries_; };

---

##### [bool_constant](http://wg21.link/N4389)

	template <bool B> using bool_constant = integral_constant<bool, B>;
 	
	using true_type  = integral_constant<bool, true>;
	using false_type = integral_constant<bool, false>;

---

##### Dynamic memory allocation for over-aligned data [P0035](http://wg21.link/P0035)

	auto ap = std::unique_ptr<int[]>(new(sizeof(int) * count, std::align_val_t(alignof(simd::vec128_t))));

---

## Removed stuff

we are finally getting ready to remove cruft.

**Removing trigraphs??!** [N4086](http://wg21.link/N4086) no need to wonder why `std::cout << "WTF ??!\n";` behaves so strange. 

**Removing `auto_ptr`, `random_shuffle()`, And Old `<functional>` Stuff	[N4190](http://wg21.link/N4190)**
	if you are still using those: *go! fix! your! @#$@Q#$^R! code!*


---

## Even more new stuff ...

##### [Parallel Algorithms](http://wg21.link/P0024)

Parallel versions of STL algorithms, plus a few extras

	std::transform_reduce( std::execution::unseq
						 , std::begin(data), std::end(data)
						 , [](auto const& de){ return process_element(de); }
						 , result
						 , [](auto const& rhs, auto const& lhs) {
								return merge(rhs, lhs);
						   }
						 );

----

##### [File System](http://wg21.link/P0218)

	for ( auto const& entry : std::filesystem::recursive_directory_iterator(
		     						std::filesystem::path("/opt")
		  					  )
		) 
	{
    	process(it->path());
	}

----

##### [Folding expressions](http://wg21.link/N4295)

	template<typename... Args> bool all(Args... args) { 
		return (args && ...); 
	}

	bool b = all(true, true, true, false);

----

## And on and on and on

##### C++17 rebased on C11 instead of C99 [P0063](http://wg21.link/p0063) 

##### polymorphic allocators

##### Allow constant evaluation for all non-type template arguments [N4268](http://wg21.link/N4268)

##### [N4089](http://wg21.link/N4089) Safe conversions in unique_ptr<T[]>
##### [P0074](http://wg21.link/P0074) Making std::owner_less more flexible
##### [P0092](http://wg21.link/P0092) Polishing <chrono>
##### [N4258](http://wg21.link/N4258) Cleaning-up noexcept in the Library
##### [P0004](http://wg21.link/P0004) Remove Deprecated iostreams aliases
##### [P0137](http://wg21.link/P0137) Replacement of class objects containing reference members
##### [P0283](http://wg21.link/P0283) Standard and non-standard attributes
##### [P0295](http://wg21.link/p0295) Adopt Selected Library Fundamentals V2 Components for C++17

##### [P0017](http://wg21.link/P0017) Extension to aggregate initialization
##### [P0036](http://wg21.link/P0036) Unary Folds and Empty Parameter Packs (revision 1)
##### [P0138](http://wg21.link/P0138) Construction Rules for enum class Values
##### [P0245](http://wg21.link/P0245) Hexadecimal floating literals for C++
##### [P0226](http://wg21.link/P0226) Mathematical Special Functions for C++17
##### [P0033](http://wg21.link/P0033) Re-enabling shared_from_this (revision 1).
##### [P0152](http://wg21.link/P0152) constexpr atomic::is_always_lock_free.
##### [P0185](http://wg21.link/P0185) Adding [nothrow-]swappable traits, revision 3.
##### [P0253](http://wg21.link/P0253) Fixing a design mistake in the searchers interface.
##### [P0154](http://wg21.link/P0154) constexpr std::hardware_{constructive,destructive}_interference_size.
##### [P0030](http://wg21.link/P0030) Proposal to Introduce a 3-Argument Overload to std::hypot.
##### [P0031](http://wg21.link/P0031) A Proposal to Add Constexpr Modifiers to reverse_iterator, move_iterator, array and Range Access and apply the resolution of LWG Issue 2296 - std::addressof should be constexpr
##### [P0077](http://wg21.link/P0077) is_callable, the missing INVOKE related trait.
##### [p0175](http://wg21.link/p0175) Synopses for the C library
##### [p0307](http://wg21.link/p0307) Making Optional Greater Equal Again
##### [p0393](http://wg21.link/p0393) Making Variant Greater Equal
##### [p0032](http://wg21.link/p0032) Homogeneous interface for variant, any and optional
##### [p0254](http://wg21.link/p0254) Integrating std::string_view and std::string
##### [p0258](http://wg21.link/p0258) has_unique_object_representations
##### [p0302](http://wg21.link/p0302) Removing Allocator Support in std::function
##### [p0163](http://wg21.link/p0163) shared_ptr::weak_type
##### [p0174](http://wg21.link/p0174) Deprecating Vestigial Library Parts in C++17
##### [p0337](http://wg21.link/p0337) Delete operator= for polymorphic_allocator
##### [p0358](http://wg21.link/p0358) Fixes for not_fn
##### [p0219](http://wg21.link/p0219) Relative Paths for Filesystem
##### [p0392](http://wg21.link/p0392) Adapting string_view by filesystem paths
##### [P0394](http://wg21.link/P0394) Hotel Parallelifornia: terminate() for Parallel Algorithms Exception Handling
##### [p0336](http://wg21.link/p0336) Better Names for Parallel Execution Policies in C++17
##### [p0371](http://wg21.link/p0371) Temporarily discourage memory_order_consume
##### [p0346](http://wg21.link/p0346) A <random> Nomenclature Tweak
##### [p0180](http://wg21.link/p0180) Reserve a New Library Namespace for Future Standardization
##### [P0296](http://wg21.link/P0296) Forward progress guarantees: Base definitions
##### [P0299](http://wg21.link/P0299) Forward progress guarantees for the Parallelism TS features
##### [N4261](http://wg21.link/N4261) Proposed resolution for Core Issue 330: Qualification conversions and pointers to arrays of pointers
##### [P0012](http://wg21.link/P0012) Make exception specifications be part of the type system
##### [P0002](http://wg21.link/P0002) Remove Deprecated operator++(bool)
##### [N3922](http://wg21.link/N3922) New Rules for auto deduction from braced-init-list
##### [P0001](http://wg21.link/P0001) Remove Deprecated Use of the register Keyword
##### [N4266](http://wg21.link/N4266) Attributes for namespaces and enumerators
##### [N4230](http://wg21.link/N4230) Nested namespace definition
##### [P0136](http://wg21.link/P0136) Rewording inheriting constructors (core issue 1941 et al)
##### [P0134](http://wg21.link/P0134) Introducing a name for brace-or-equal-initializers for non-static data members
##### [N4262](http://wg21.link/N4262) Wording for Forwarding References
##### [N4285](http://wg21.link/N4285) Cleanup for exception-specification and throw-expression
##### [P0061](http://wg21.link/P0061) __has_include for C++17
##### [N4366](http://wg21.link/N4366) LWG 2228: Missing SFINAE rule in unique_ptr templated assignment
##### [N4169](http://wg21.link/N4169) A proposal to add invoke function template
##### [N4277](http://wg21.link/N4277) TriviallyCopyable reference_wrapper
##### [P0006](http://wg21.link/P0006) Adopt Type Traits Variable Templates from Library Fundamentals TS for C++17
##### [P0013](http://wg21.link/P0013) Logical Operator Type Traits
##### [P0391](http://wg21.link/P0391) P0391R0: Introducing the term 'templated entity'
##### [P0398](http://wg21.link/P0398) P0398R0: Core issue 1518: Explicit default constructors and copy-list-initialization

