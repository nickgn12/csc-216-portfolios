template<typename T>
class ArrayDeque
{
private:
    Array<T> array;

public:
    void insertFront( T )
    {
        /* O(n) because it has to shift everything down */
        
        array.shiftRight( );
        array[0] = T;
    }

    void insertBack( T )
    {
        /* O(1) because it just places it at the end */

        array[array.size( ) - 1] = T;
    }

    void eraseFront( )
    {
        /* O(n) because it must shift everything to the left */
        
        array.shiftLeft( );
    }

    void eraseBack( )
    {
        /* O(1) because it just has to decrease the size variable */
        
        array.remove( array.size( ) - 1 );
    }
    
    T& getFront( )
    {
        return array[0];
    }

    T& getBack( )
    {
        return array[array.size( ) - 1];
    }
};
