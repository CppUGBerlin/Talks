#include <iostream>
#include <array>
#include <boost/proto/proto.hpp>

using namespace std;
using namespace boost;

template< int I >  struct placeholder {};


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


template<typename Expr>  struct lambda_expr;

struct lambda_domain : proto::domain< proto::generator<lambda_expr> > {};

template<typename Expr>
struct lambda_expr : proto::extends<Expr, lambda_expr<Expr>, lambda_domain>
{
    typedef
        proto::extends<Expr, lambda_expr<Expr>, lambda_domain>
        base_type;

    lambda_expr( Expr const& expr = Expr() ) : base_type( expr ) {}

    typedef double result_type;

    double operator() ( double a1 = 0, double a2 = 0 ) const
    {
        lambda_context ctx;
        ctx.args[0] = a1;
        ctx.args[1] = a2;

        return proto::eval( *this, ctx );
    }
};


lambda_expr< proto::terminal< placeholder<0> >::type >  const _1;
lambda_expr< proto::terminal< placeholder<1> >::type >  const _2;


int main() {  cout << ( (_2 - _1) / _2 * 100 )( 45 , 50 ) << endl;  }

