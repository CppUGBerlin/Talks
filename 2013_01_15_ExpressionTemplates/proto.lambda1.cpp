#include <iostream>
#include <array>
#include <string>
#include <boost/proto/proto.hpp>

using namespace std;
using namespace boost;

template< int I >  struct placeholder {};

proto::terminal< placeholder<0> >::type  const _1 = {{}};
proto::terminal< placeholder<1> >::type  const _2 = {{}};


struct lambda_context : proto::callable_context< const lambda_context >
{
    std::array<double,2> args;

    typedef double result_type;

    template<int I>
    double operator() ( proto::tag::terminal , placeholder<I> ) const
    {
        return args[I];
    }
};

struct lambda_context2 : proto::callable_context< const lambda_context2 >
{
    std::array<string,2> args;

    typedef string result_type;

    template<int I>
    string operator() ( proto::tag::terminal , placeholder<I> ) const
    {
        return args[I];
    }
};



int main() {

    lambda_context  ctx1;
    ctx1.args[0] = 45;
    ctx1.args[1] = 50;

    lambda_context2  ctx2;
    ctx2.args[0] = "hello";
    ctx2.args[1] = " world";

    cout <<  proto::eval( _1 + _2 , ctx1 ) << endl;
    cout <<  proto::eval( _1 + _2 , ctx2 ) << endl;
}

