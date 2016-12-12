#include <iostream>
#include <utility>

/* Recursively rearrange an array so that all even numbers
 * will appear before odd ones.  Rearranged array will not
 * be sorted and is not stable */

void sort( int* pArray, size_t nLength )
{   
    long nEven = -1, nOdd = -1;
    
    for ( long i=0; i < static_cast<long>( nLength ) && ( nEven == -1 || nOdd == -1 ); i++ )
    {
        if ( nEven == -1 && pArray[i] % 2 == 0 )
            nEven = i;

        if ( nOdd == -1 && pArray[i] % 2 == 1 )
            nOdd = i;
    }

    if ( nEven != -1 && nOdd != -1 )
    {
        std::swap( pArray[nEven], pArray[nOdd] );
    }

    if ( nLength > 1 )
    {
        sort( pArray + 1, nLength - 1 );
    }
}

template<typename T>
void printArray( T* pArray, size_t nLength )
{
    for ( size_t i=0; i < nLength; i++ )
    {
        std::cout << pArray[i] << ' ';
    }

    std::cout << std::endl;
}

int main( )
{
    const int LENGTH = 20;
    int example[LENGTH] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                            11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    std::cout << "Array pre sorting:" << std::endl;
    printArray( example, LENGTH );
    
    sort( example, LENGTH );

    std::cout << "Array post sorting:" << std::endl;
    printArray( example, LENGTH );
    
    return 0;
}
