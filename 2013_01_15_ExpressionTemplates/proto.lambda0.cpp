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



int main() {

    lambda_context  lctx;
    lctx.args[0] = 45;
    lctx.args[1] = 50;

    double d = proto::eval( _1 + _2, lctx );

    cout << d << endl;

//    proto::display_expr( (_2 - _1) / _2 * 100 );
}

