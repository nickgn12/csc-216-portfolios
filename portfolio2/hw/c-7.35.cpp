void pathLengthRec( TreeNode& T, int depth, int& length )
{
    length += depth;

    for ( size_t i=0; i < T.numChildren( ); i++ )
    {
        pathLengthRec( T.getChild(i), depth+1, length );
    }
}

int pathLength( Tree& T )
{
    int length = 0;    
    calculateDepthRec( T.root( ), 1, length );

    return length;
}
