node* getMiddleNode( list* pList )
{
    if ( pList == NULL )
        return NULL;

    // Have two nodes, one that starts at the beginning of the list and one that starts at the end

    node* pStart, pEnd;
    pStart = list->start;
    pEnd = list->end;

    // Loop until the nodes "intersect" and point to the same data

    while ( !( pStart == pEnd || ( pStart->next == pEnd && pEnd->prev == pStart ) ) )
    {
        pStart = pStart->next;
        pEnd = pEnd->prev;
    }
    
    return pStart;
}
