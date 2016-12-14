void recursiveRemove( int size, Stack& S )
{
    if ( size > 0 )
    {
        recursiveRemove( size-1, S );
    }

    S.pop( );
}

void popall( Stack& S )
{
    recursiveRemove( S.size( ), S );
}
