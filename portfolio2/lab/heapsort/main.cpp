#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <typeinfo>

template <typename T>
class VectorHeap
{
private:
    std::vector<T>& pHeap;

public:
    VectorHeap( std::vector<T>& pSetHeap ): pHeap( pSetHeap ) { }

    constexpr size_t getRoot( ) const
    {
        return 0;
    }

    constexpr size_t getLeft( size_t n ) const
    {
        return ( n * 2 ) + 1;
    }

    constexpr size_t getRight( size_t n ) const
    {
        return ( n * 2 ) + 2;
    }

    constexpr size_t getParent( size_t n ) const
    {
        if ( n > 0 )
        {        
            size_t nParent = n - 1;
        
            if ( nParent % 2 == 1 )
            {
                nParent--;
            }

            return nParent / 2;
        }
        else
        {
            return 0;
        }
    }
    
    T& getRootValue( )
    {
        return pHeap[0];
    }

    T& getLeftValue( size_t n )
    {
        return pHeap[getLeft(n)];
    }

    T& getRightValue( size_t n )
    {
        return pHeap[getRight(n)];
    }

    T& getParentValue( size_t n )
    {
        return pHeap[getParent(n)];
    }

    /* Filter the heap so that the largest value is at the top
       of the heap. */
    
    void filterDown( size_t nStart, size_t nEnd )
    {
        size_t nRoot = nStart;
        bool bLooping = true;

        while ( getLeft( nRoot ) <= nEnd && bLooping )
        {
            /* While the root has at least one child left */
            
            size_t nChild, nSwap;
            
            nChild = getLeft( nRoot );
            nSwap = nRoot;

            if ( pHeap[nSwap] < pHeap[nChild] )
            {
                /* If the left child is greater, then swap with it */
                
                nSwap = nChild;
            }

            if ( nChild + 1 <= nEnd && pHeap[nSwap] < pHeap[nChild+1] )
            {
                /* If the right child exists, and is greater than what we have
                   currently, then swap with it. */
                
                nSwap = nChild + 1;
            }

            if ( nSwap == nRoot )
            {
                /* If the root is the largest value, then we're done */
                
                bLooping = false;
            }
            else
            {
                /* Else, swap the value with the root */
                
                std::swap( pHeap[nRoot], pHeap[nSwap] );
                nRoot = nSwap;
            }
        }
    }

    void filterDown( )
    {
        filterDown( 0, pHeap.size( ) );
    }

    void filterUp( size_t nStart, size_t nEnd )
    {
        size_t nChild = nEnd;
        bool bLooping = true;
        
        while ( nChild > nStart && bLooping )
        {
            size_t nParent = getParent( nChild );

            if ( pHeap[nParent] < pHeap[nChild] )
            {
                /* If the heap property is not satisfied */

                std::swap( pHeap[nParent], pHeap[nChild] );
                nChild = nParent;
            }
            else
            {
                /* We're done and the heap is fully sorted */
                
                bLooping = false;
            }
        }
    }

    void filterUp( )
    {
        filterUp( 0, pHeap.size( ) );
    }

    void heapify( )
    {
        for ( size_t i=1; i < pHeap.size( ); i++ )
        {
            filterUp( 0, i );
        }
    }
};

template<typename T>
void printVector( const std::vector<T>& pVector )
{
    std::cout << "Contents of: " << typeid( T ).name( ) << " vector: " << std::endl;

    for ( size_t i=0; i < pVector.size( ); i++ )
    {
        std::cout << pVector[i] << " ";
    }

    std::cout << std::endl;
}

template<typename T>
void heapSort( std::vector<T>& pSort )
{
    VectorHeap<T> pHeap( pSort );
    pHeap.heapify( );

    for ( size_t i=pSort.size( ) - 1; i > 0; i-- )
    {
        std::swap( pSort[i], pSort[0] );
        pHeap.filterDown( 0, i - 1 );
    }
}

int main( )
{
    std::vector<int> pSortVec;

    srand( static_cast<unsigned int>( time( nullptr ) ) );

    for ( int i=0; i < 20; i++ )
    {
        pSortVec.push_back( rand( ) % 100 );
    }

    printVector( pSortVec );

    std::cout << std::endl << "Sorting vector..." << std::endl << std::endl;

    heapSort( pSortVec );
    printVector( pSortVec );
    
    return 0;
}
