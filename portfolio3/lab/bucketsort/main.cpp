#include <iostream>
#include <list>
#include <stack>
#include <cstdio>

std::list<int> bucketSort( const std::list<int>& pList, int nMin, int nMax )
{
    std::stack<int>* pBuckets = new std::stack<int>[static_cast<unsigned int>(nMax - nMin) + 1];
    std::list<int> pReturnList;

    for ( auto i = pList.begin( ); i != pList.end( ); i++ )
    {
        pBuckets[*i - nMin].push( *i );
    }

    for ( int i=nMin; i <= nMax; i++ )
    {
        while ( !pBuckets[i-nMin].empty( ) )
        {
            pReturnList.push_back( pBuckets[i-nMin].top( ) );
            pBuckets[i-nMin].pop( );
        }
    }
    
    delete[] pBuckets;

    return pReturnList;
}

std::list<int> bucketSort( std::list<int>& pList )
{
    int nMin = pList.front( ), nMax = pList.front( );

    for ( auto i = ++pList.begin( ); i != pList.end( ); i++ )
    {
        if ( *i < nMin )
        {
            nMin = *i;
        }

        if ( *i > nMax )
        {
            nMax = *i;
        }
    }

    return bucketSort( pList, nMin, nMax );
}

template<typename T>
void printList( const std::list<T>& pList )
{
    std::cout << "Printing list of size " << pList.size( ) << std::endl;
    
    for ( auto i = pList.begin( ); i != pList.end( ); i++ )
    {
        std::cout << *i << " ";
    }

    std::cout << std::endl;
}

int main( )
{
    srand( static_cast<unsigned int>( time( nullptr ) ) );

    std::list<int> pList;

    for ( size_t i=0; i < 15; i++ )
    {
        pList.push_back( rand( ) % 50 );
    }

    std::cout << "Pre sorted list:" << std::endl;

    printList( pList );
    pList = bucketSort( pList );

    std::cout << "After sorting: " << std::endl;
    printList( pList );
    
    return 0;
}
