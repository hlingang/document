#include <ctime>
#include <cstdlib>
#include <iterator>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

template < typename Iter > Iter __search( Iter __first1, Iter __last1, Iter __first2, Iter __last2 )
{
    if ( __first1 == __last1 || __first2 == __last2 )
    {
        return __last1;
    }
    Iter p = __first2;
    if ( ++p == __last2 )
    {
        return __find( __first1, __last1, *__first1 );
    }
    for ( ;; )
    {
        __first1 = __find( __first1, __last1, *__first2 );
        if ( __first1 == __last1 )
        {
            return __last1;
        }
        Iter current = __first1;
        Iter p       = __first2;
        while ( current != __last1 && p != __last2 && *current == *p )
        {
            ++current;
            ++p;
        }
        if ( p == __last2 )
        {
            return __first1;
        }
        if ( current == __last1 )
        {
            return __last1;
        }
        ++__first1;  // 序列匹配
    }
}
// 序列1 和序列2 均已排序
template < typename Iter > Iter __include( Iter __first1, Iter __last1, Iter __first2, Iter __last2 )
{
    if ( __first1 == __last1 || __first2 == __last2 )
    {
        return __last1;
    }
    for ( ; __first1 != __last1 && __first2 != __last2; )
    {
        if ( *__first1 > *__first2 )
        {
            return false;
        }
        if ( *__first1 < *__first2 )
        {
            ++__first1;
        }
        ++__first1;
        ++__first2;
    }
    return static_cast< bool >( __first2 == __last2 );
}

template < typename Iter, typename T > Iter __find( Iter __first1, Iter __last1, T __val )
{
    while ( __first1 != __last1 && *__first1 != __val )
    {
        ++__first1;
    }
    return __first1;
};

template < typename Iter, typename T > Iter __find_not( Iter __first1, Iter __last1, T __val )
{
    while ( __first1 != __last1 && *__first1 == __val )
    {
        ++__first1;
    }
    return __first1;
};

template < typename Iter, typename T > Iter __find_n_aux( Iter __first1, Iter __last1, int n, T __val )
{

    if ( __first1 == __last1 )
    {
        return __last1;
    }
    for ( ;; )
    {
        __first1 = __find( __first1, __last1, __val );
        if ( __first1 == __last1 )
        {
            return __last1;
        }
        Iter p = __first1;
        for ( ; p != __last1 && *p == __val && n; )
        {
            ++p;
            --n;
        }
        if ( n == 0 )
        {
            return __first1;
        }
        if ( p == __last1 )
        {
            return __last1;
        }
        __first1 = p;
    }
    return __first1;
};

template < typename Iter, typename T > Iter __remove( Iter __first1, Iter __last1, T __val )
{
    if ( __first1 == __last1 )
    {
        return __last1;
    }
    __first1 = __find( __first1, __last1, __val );
    if ( __first1 == __last1 )
    {
        return __last1;
    }
    Iter p = __first1;
    while ( ++p != __last1 )
    {
        if ( *p != __val )
        {
            *__first1 = *p;
            ++__first1;
        }
    }
};

template < typename Iter > Iter __shuffle( Iter __first1, Iter __last1 )
{
    Iter p = __first1;
    if ( p == __last1 || ++p == __last1 )
    {
        return __last1;
    }
    int __i = 1;
    srand( time( nullptr ) );  // 随机化种子
    for ( ; p != __last1; ++p, ++__i )
    {
        int __j = rand() % ( __i + 1 );
        if ( __i != __j )
        {
            Iter __s = __first1 + __j;
            iter_swap( __s, p );
        }
    }
    return __first1;
};

template < typename Iter > bool equal( Iter __first1, Iter __last1, Iter __first2 )
{
    for ( ; __first1 != __last1 && *__first1 == *__first2; )
    {
        ++__first1;
        ++__first2;
    }
    return static_cast< bool >( __first1 == __last1 );
};

template < typename Iter > bool equal( Iter __first1, Iter __last1, Iter __first2, Iter __last2 )
{
    for ( ; __first1 != __last1 && __first2 != __last2 && *__first1 == *__first2; )
    {
        ++__first1;
        ++__first2;
    }
    return static_cast< bool >( __first1 == __last1 && __first2 == __last2 );
};

template < typename Iter > Iter find_adjacent( Iter __first1, Iter __last1 )
{
    Iter p = __first1;
    // 解引用前必须检查
    if ( p == __last1 || ++p == __last1 )
    {
        return __last1;
    }
    for ( ; p != __last1; ++p, ++__first1 )
    {
        if ( *p == *__first1 )
        {
            return __first1;  // 返回首位置
        }
    }
    return __last1;
};

template < typename Iter > Iter __unique( Iter __first1, Iter __last1 )
{

    __first1 = find_adjacent( __first1, __last1 );
    if ( __first1 == __last1 )
    {
        return __last1;
    }
    Iter p = __first1;
    ++p;
    if ( p == __last1 || ++p == __last1 )
    {
        return __last1;
    }
    for ( ; p != __last1; ++p )
    {
        if ( *p != *__first1 )
        {
            *++__first1 = *p;
        }
    }
    return ++__first1;  // 返回结尾的后一个指针
};

int main()
{
    vector< int > px   = { 1, 2, 3, 4, 5, 6 };
    auto          __it = __find_n_aux( px.begin(), px.end(), 3, 5 );
    // remove( px.begin(), px.end(), 5 );
    __shuffle( px.begin(), px.end() );
    for_each( px.begin(), px.end(), []( int val ) { cout << "val = " << val << endl; } );
    cout << distance( px.begin(), __it ) << endl;
    int arr[] = { 1, 2, 3, 4 };
    cout << arr[ 0 ] << endl;
    cout << arr[ 1 ] << endl;
    vector< int > qx1 = { 1, 2, 3, 4 };
    vector< int > qx2 = { 1, 2, 3, 4, 5 };
    cout << "equal = " << equal( qx1.begin(), qx1.end(), qx2.begin() ) << endl;
    cout << "equal = " << equal( qx1.begin(), qx1.end(), qx2.begin(), qx2.end() ) << endl;
    vector< int > ax = { 20, 1, 2, 10, 10, 5, 1, 5, 5, 5, 5, 30 };
    auto          it = __unique( ax.begin(), ax.end() );
    cout << "distance = " << distance( ax.begin(), it ) << endl;
    cout << "value = " << *it << endl;
    for_each( ax.begin(), ax.end(), []( int val ) { cout << "val = " << val << endl; } );
    ax = { 1, 2 };
    ax.reserve( 10 );
    for_each( ax.begin(), ax.end(), []( int val ) { cout << "val = " << val << endl; } );
}