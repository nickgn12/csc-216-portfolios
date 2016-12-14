inorderDFS( Tree T )
{
    Stack S;
    TreeNode* current = T.getRoot( );
    bool finished = false;

    while ( !finished )
    {
        if ( current != NULL )
        {
            S.push( current );
            current = current->getLeft( );
        }
        else
        {
            if ( !S.empty( ) )
            {
                current = S.pop( );

                /* Do something with current's data */

                current = current.getRight( );
            }
            else
            {
                finished = true;
            }
        }
    }
}     
 
