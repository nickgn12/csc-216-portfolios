void calculateDepthRec( TreeNode& T, int depth )
{
    /* The current node is depth, do whatever you want with that */

    for ( size_t i=0; i < T.numChildren( ); i++ )
    {
        calculateDepthRec( T.getChild(i), depth+1 );
    }
}

void calculateDepth( Tree& T )
{
    calculateDepthRec( T.root( ), 1 );
}
