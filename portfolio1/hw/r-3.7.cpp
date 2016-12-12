node* getPenultimateNode( node* pStart )
{
    // Check to see if our starting node is NULL or not

    if ( pStart != NULL )
    {
        // Make sure we have more than 1 node

        if ( pStart->next == NULL )
            return pStart;

        node* pCurrent = pStart;

        while ( pCurrent->next->next != NULL ||
              ( pCurrent->next->next == NULL && pCurrent->next != NULL ) )
        {
            pCurrent = pCurrent->next;
        }

        return pCurrent;
    }
    else
    {
        return NULL;
    }
}
