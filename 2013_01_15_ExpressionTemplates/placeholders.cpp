#include <iostream>
using namespace std;


template < bool >
struct eval_trait {
    template < class T , class V >
    static  V apply ( const T& t , const V& v )   { return  t(v); }
};

template <>
struct eval_trait<true> {
    template < class T , class V >
    static  T apply ( const T& t , const V& )  { return  t; }
};


struct Plus  { template < class T >  static T apply( const T& lhs , const T& rhs ) { return   lhs + rhs; }  };
struct Minus { template < class T >  static T apply( const T& lhs , const T& rhs ) { return   lhs - rhs; }  };
struct Times { template < class T >  static T apply( const T& lhs , const T& rhs ) { return   lhs * rhs; }  };
struct DivBy { template < class T >  static T apply( const T& lhs , const T& rhs ) { return   lhs / rhs; }  };


template < class Lhs , class Op , class Rhs >
struct Expression
{
    Expression( const Lhs& lhs , const Rhs& rhs ) : _lhs(lhs) , _rhs(rhs) {}

    template < class T >
    T operator() ( const T& t ) const
    {
        return  Op::apply(
                    eval_trait< is_fundamental<Lhs>::value >::apply( _lhs , t ),
                    eval_trait< is_fundamental<Rhs>::value >::apply( _rhs , t )
                );
    }

private:
    const Lhs& _lhs;
    const Rhs& _rhs;
};


// - - - - little helper - - - - - -

template < class Lhs , class Op , class Rhs >
Expression<Lhs,Op,Rhs>  make_Expression( const Lhs& lhs , const Op& op , const Rhs& rhs )
{
    return  Expression<Lhs,Op,Rhs>( lhs , rhs );
}


// - - - - operators - - - - - -

template < class Lhs , class Op , class Rhs , class OtherType >
Expression< Expression<Lhs,Op,Rhs> , Plus , OtherType >
operator+ ( const Expression<Lhs,Op,Rhs>& lhs , const OtherType& rhs )
{
    return  make_Expression( lhs , Plus() , rhs );
}

template < class Lhs , class Op , class Rhs , class OtherType >
Expression< Expression<Lhs,Op,Rhs> , Minus , OtherType >
operator- ( const Expression<Lhs,Op,Rhs>& lhs , const OtherType& rhs )
{
    return  make_Expression( lhs , Minus() , rhs );
}

template < class OtherType , class Lhs , class Op , class Rhs >
Expression< OtherType , Minus , Expression<Lhs,Op,Rhs> >
operator- ( const OtherType& lhs , const Expression<Lhs,Op,Rhs>& rhs )
{
    return  make_Expression( lhs , Minus() , rhs );
}

// ...


struct Placeholder
{
    template < class Rhs >
    Expression< Placeholder , Plus , Rhs >   operator+ ( const Rhs& rhs )
    {
        return  make_Expression( *this , Plus() , rhs );
    }
    
    template < class T >  T  operator()( const T& t ) const { return  t; }
};


template < class Lhs >
Expression< Lhs , Minus , Placeholder >
operator- ( const Lhs& lhs , const Placeholder& rhs )
{
    return  make_Expression( lhs , Minus() , rhs );
}


Placeholder _1;




int main()
{
    auto  exp  =  1000 - _1 + 42;
    Expression< Expression<int,Minus,Placeholder> , Plus , int >  exp1  =  1000 -   _1 + 42  ;
    Expression< int , Minus , Expression<Placeholder,Plus,int> >  exp2  =  1000 - ( _1 + 42 ) ;
    cout << exp(8) << endl;
}



