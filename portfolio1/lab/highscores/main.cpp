#include <iostream>

/* P-3.4
 * Write a class that maintains the top 10 scores for a game application, 
 * implementing the add and remove functions of Section 3.1.1, but use a
 * doubly linked list. Your implementation of remove(i) should make the
 * fewest number of pointer hops to get to the game entry at index i.
 */

/**
 * High score entry containing name and score.
 */

class GameEntry
{
private:
    std::string name;
    int score;

public:
    GameEntry( const std::string& setName="", int setScore=0 ): name( setName ), score( setScore ) { }
    GameEntry( const GameEntry& pOther ): name( pOther.name ), score( pOther.score ) { }

    GameEntry& operator=( const GameEntry& pOther )
    {
        name = pOther.name;
        score = pOther.score;

        return *this;
    }
    
    std::string getName( ) const
    {
        return name;
    }
    
    int getScore( ) const
    {
        return score;
    }

    void setName( const std::string& sNewName )
    {
        name = sNewName;
    }

    void setScore( int nNewScore )
    {
        score = nNewScore;
    }
};

/**
 * Linked-list node.
 */

class ListNode
{
private:
    GameEntry pData;
    class ListNode* pPrev, *pNext;
    
public:
    ListNode( ): pPrev( NULL ), pNext( NULL ) { }
    
    ListNode( const GameEntry& pSetData ): pData( pSetData ),
                                           pPrev( NULL ),
                                           pNext( NULL ) { }
    
    ListNode( const ListNode& pOther ): pData( pOther.pData ),
                                        pPrev( pOther.pPrev ),
                                        pNext( pOther.pNext ) { }
    
    ListNode& operator=( const ListNode& pOther )
    {
        pData = pOther.pData;
        pPrev = pOther.pPrev;
        pNext = pOther.pNext;
        
        return *this;
    }

    GameEntry& getData( )
    {
        return pData;
    }

    GameEntry getData( ) const
    {
        return pData;
    }
    
    friend class List;
};

/**
 * Linked-list implementation class.
 */

class List
{
private:
    ListNode* pFirst, *pLast;
    size_t nLength;

    ListNode* getNode( size_t nIndex )
    {
        if ( nLength == 0 )
            return NULL;
        
        ListNode* pReturn;
        
        if ( nIndex < nLength/2 )
        {
            pReturn = pFirst;

            for ( size_t i=0; i < nIndex; i++ )
            {
                pReturn = pReturn->pNext;
            }
        }
        else
        {
            pReturn = pLast;

            for ( size_t i=nLength-1; i > nIndex; i-- )
            {
                pReturn = pReturn->pPrev;
            }
        }

        return pReturn;
    }
    
public:
    List( ): pFirst( NULL ), pLast( NULL ), nLength( 0 )
    {
        pFirst = new ListNode;
        pLast = pFirst;
    }

    List( const List& pOther ): pFirst( pOther.pFirst ),
                                pLast( pOther.pLast ),
                                nLength( pOther.nLength ) { }

    List& operator=( const List& pOther )
    {
        pFirst = pOther.pFirst;
        pLast = pOther.pLast;
        nLength = pOther.nLength;
        
        return *this;
    }

    ~List( )
    {
        if ( nLength != 0 )
        {
            for ( ListNode* pCurrent = pFirst;
                  pCurrent != NULL;
                  pCurrent = pCurrent->pNext )
            {
                delete pCurrent;
            }
        }
    }
    
    size_t getLength( ) { return nLength; }
    
    GameEntry& get( size_t nIndex )
    {
        return getNode( nIndex )->getData( );
    }

    GameEntry& operator[]( const size_t nIndex )
    {
        return getNode( nIndex )->getData( );
    }

    void push_back( const GameEntry& pData )
    {
        if ( nLength == 0 )
        {
            pFirst = new ListNode( pData );
            pLast = pFirst;
        }
        else
        {
            ListNode* pTempNode = new ListNode( pData );
            pLast->pNext = pTempNode;
            pTempNode->pPrev = pLast;
            pLast = pTempNode;
        }

        nLength++;
    }

    bool insert( size_t nIndex, const GameEntry& pData )
    {
        if ( nLength == 0 )
        {
            if ( nIndex != 0 )
                return false;

            /* If theres no other nodes, call our push_back function */
            
            push_back( pData );

            return true;
        }
        else
        {
            if ( nIndex > nLength )
                return false; /* Fail if last node slot + 1 */

            if ( nIndex == nLength )
            {
                /* If we're trying to place at the last slot then call push_back */
                
                push_back( pData );
                return true;
            }
            else if ( nIndex == 0 )
            {
                /* If we're trying to place at the beginning */
                
                ListNode* pSecond = pFirst;
                ListNode* pNewTemp = new ListNode( pData );

                pNewTemp->pNext = pSecond;
                pFirst = pNewTemp;

                nLength++;
                
                return true;
            }
            else
            {
                /* Placing at an arbitrary point in the list */
                
                ListNode* pAt = getNode( nIndex );
                ListNode* pPrev = pAt->pPrev;
                ListNode* pNewTemp = new ListNode( pData );

                pNewTemp->pPrev = pPrev;
                pNewTemp->pNext = pAt;
                pAt->pPrev = pNewTemp;
                pPrev->pNext = pNewTemp;

                nLength++;
                
                return true;
            }
        }

        return false;
    }
    
    bool remove( size_t nIndex )
    {
        if ( nLength == 0 )
        {
            /* Can't remove when there's already nothing */
            
            return true;
        }
        else if ( nLength == 1 )
        {
            delete pFirst;
            pFirst = NULL;
            pLast = NULL;

            return true;
        }
        else
        {
            if ( nIndex >= nLength )
                return false;

            /* If last node, remove and update the new last node */
            
            if ( nIndex == nLength - 1 )
            {
                ListNode* pNewEnd = pLast->pPrev;
                delete pLast;

                pLast = pNewEnd;
                pNewEnd->pNext = NULL;

                return true;
            }
            else
            {
                /* Else, remove node and update next and previous nodes to point
                   to each other */

                ListNode* pPrev, *pNext, *pCurrent;
                pCurrent = getNode( nIndex );
                pPrev = pCurrent->pPrev;
                pNext = pCurrent->pNext;

                delete pCurrent;
                pPrev->pNext = pNext;
                pNext->pPrev = pPrev;

                return true;
            }
        }
    }
};

int main( )
{
    List scores;
    
    scores.push_back( GameEntry( "BOB", 50000 ) );
    scores.push_back( GameEntry( "NIK", 42000 ) );
    scores.push_back( GameEntry( "ASK", 36900 ) );
    scores.push_back( GameEntry( "TUT", 31000 ) );
    scores.push_back( GameEntry( "DAN", 20000 ) );

    scores.insert( 0, GameEntry( "LOL", 65000 ) );
    scores.insert( 6, GameEntry( "SUX", 100 ) );
    scores.insert( 6, GameEntry( "BWA", 500 ) );
    scores.insert( 6, GameEntry( "ASD", 1000 ) );
    scores.insert( 6, GameEntry( "DSF", 2000 ) );
    
    for ( size_t i=0; i < scores.getLength( ); i++ )
    {
        std::cout << i << ": " << scores[i].getName( ) << ": " << scores[i].getScore( ) << std::endl;
    }

    return 0;
}
