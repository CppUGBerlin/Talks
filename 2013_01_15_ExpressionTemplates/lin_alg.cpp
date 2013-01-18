#include <iostream>
#include <array>
#include <type_traits>


namespace std {


   template < bool >
   struct index_trait {
      template < class T , class I >
      static  double index ( const T & t , const I & i )   { return  t[i]; }
   };

   template <>
   struct index_trait<true> {
      template < class T , class I >
      static  T index ( const T & t , const I & )  { return  t; }
   };


   struct Plus  { template < class T >  static T apply( const T& lhs , const T& rhs ) { return   lhs + rhs; }  };
   struct Minus { template < class T >  static T apply( const T& lhs , const T& rhs ) { return   lhs - rhs; }  };
   struct Times { template < class T >  static T apply( const T& lhs , const T& rhs ) { return   lhs * rhs; }  };
   struct DivBy { template < class T >  static T apply( const T& lhs , const T& rhs ) { return   lhs / rhs; }  };


    template < class Lhs , class Op , class Rhs >
    struct  Expression {

        Expression( const Lhs& lhs , const Rhs& rhs ) : _lhs(lhs) , _rhs(rhs) {}

        double  operator[] ( size_t n ) const
        {
            return  Op::apply(
                        index_trait< is_fundamental<Lhs>::value >::index( _lhs , n ),
                        index_trait< is_fundamental<Rhs>::value >::index( _rhs , n )
                    );
        }

    private:
        const Lhs& _lhs;
        const Rhs& _rhs;
    };


    template < class Lhs , class Op , class Rhs >
    Expression<Lhs,Op,Rhs>  make_Expression( const Lhs& lhs , const Op& op , const Rhs& rhs )
    {
        return  Expression<Lhs,Op,Rhs>( lhs , rhs );
    }


    template < class Lhs , class Op , class Rhs , class OtherExp >
    Expression< Expression<Lhs,Op,Rhs> , Plus , OtherExp >
    operator+ ( const Expression<Lhs,Op,Rhs>& lhs , const OtherExp& rhs )
    {
        return  make_Expression( lhs , Plus() , rhs );
    }

 


    template < class T , size_t N >
    Expression< array<T,N> , Plus , array<T,N> >  operator+ ( const array<T,N>& lhs , const array<T,N>& rhs )
    {
        return  make_Expression( lhs , Plus() , rhs );
    }

/*
    template < class T , size_t N >  array<T,N>  operator+ ( const array<T,N>& lhs , const array<T,N> rhs )
    {
        array<T,N>  out;
        for ( size_t n = 0 ; n < N ; ++n )  out[n] = lhs[n] + rhs[n];
        return  out;
    }
*/
}



int main()
{
    std::array<int,5>
        a1 = { 3,1,4,1,5 },
        a2 = { 2,7,1,8,2 },
        a3 = { 4,6,6,9,2 };

    std::cout << (a1 + a2 + a3 )[0] << std::endl;
}
