#include <iostream>
#include <fstream>
#include "fakecurses.hpp"

class StringList
{
private:    
    struct StringListNode
    {
        StringListNode( ): cChar( 0 ), pNext( nullptr ), pPrev( nullptr ) { }
        StringListNode( const StringListNode& pOther ):
            cChar( pOther.cChar ), pNext( pOther.pNext ), pPrev( nullptr ) { }
        StringListNode& operator=( const StringListNode& pOther )
        {
            cChar = pOther.cChar;
            pNext = pOther.pNext;
            pPrev = pOther.pPrev;

            return *this;
        }
        ~StringListNode( )
        {
            if ( pNext != nullptr )
            {
                delete pNext;
            }
        }
        
        char cChar;
        StringListNode* pNext, *pPrev;
    };

    StringListNode* pHead;
    StringListNode* pCursor;
    size_t nCursorPos;
    
public:
    StringList( ): pHead( nullptr ), pCursor( nullptr ), nCursorPos( 0 )
    {
        pHead = new StringListNode( );
        pCursor = pHead;
    }
    StringList( const StringList& pOther ): pHead( pOther.pHead ), pCursor( nullptr ), nCursorPos( pOther.nCursorPos ) { }
    StringList& operator=( const StringList& pOther )
    {
        pHead = pOther.pHead;
        pCursor = pOther.pCursor;
        nCursorPos = pOther.nCursorPos;

        return *this;
    }
    ~StringList( )
    {
        if ( pHead != nullptr )
        {
            delete pHead;
        }
    }

    void clear( )
    {
        if ( pHead != nullptr )
        {
            delete pHead;
        }

        pHead = new StringListNode( );
        pCursor = pHead;
        nCursorPos = 0;
    }
    
    void left( )
    {
        if ( pCursor->pPrev != nullptr )
        {
            pCursor = pCursor->pPrev;
            nCursorPos--;
        }
    }

    void right( )
    {
        if ( pCursor->pNext != nullptr )
        {
            pCursor = pCursor->pNext;
            nCursorPos++;
        }
    }

    void erase( )
    {
        if ( nCursorPos != 0 )
        {
            StringListNode* pPrev, *pNext;
            pCursor = pCursor->pPrev;
            pPrev = pCursor->pPrev;
            pNext = pCursor->pNext;
            
            pCursor->pPrev = nullptr;
            pCursor->pNext = nullptr;

            if ( pCursor == pHead )
            {
                pHead = pNext;
            }
            
            delete pCursor;
            pCursor = pNext;
            
            if ( pPrev != nullptr )
            {
                pPrev->pNext = pNext;
            }

            if ( pNext != nullptr )
            {
                pNext->pPrev = pPrev;
            }
            
            nCursorPos--;
        }
    }

    void insert( char c )
    {
        if ( nCursorPos == 0 )
        {
            StringListNode* pPrevHead = pHead;
            pHead = new StringListNode;
            pHead->pNext = pPrevHead;
            pHead->cChar = c;
            pPrevHead->pPrev = pHead;
        }
        else
        {
            StringListNode* pPrev, *pNext, *pInsert;
            pPrev = pCursor->pPrev;
            pNext = pCursor;

            pInsert = new StringListNode;
            pInsert->cChar = c;
            pInsert->pPrev = pPrev;
            pInsert->pNext = pNext;

            pCursor = pInsert->pNext;
            
            if ( pPrev != nullptr )
            {
                pPrev->pNext = pInsert;
            }
            
            if ( pNext != nullptr )
            {    
                pNext->pPrev = pInsert;
            }
        }

        nCursorPos++;
    }
    
    size_t getCursorPos( ) const
    {
        return nCursorPos;
    }

    std::string toString( )
    {
        std::string sReturn;
        StringListNode* pNode = pHead;

        while ( pNode->cChar != 0 )
        {
            sReturn += pNode->cChar;
            pNode = pNode->pNext;
        }

        return sReturn;
    }
    
    friend std::ostream& operator<<( std::ostream& oInput, const StringList& pList )
    {
        StringListNode* pNode = pList.pHead;

        while ( pNode != nullptr )
        {
            if ( pNode->cChar != 0 )
            {
                oInput << pNode->cChar;
            }
            
            pNode = pNode->pNext;
        }
        
        return oInput;
    }

    friend std::istream& operator>>( std::istream& oInput, StringList& pList )
    {
        pList.clear( );

        while ( !oInput.eof( ) )
        {
            char cTemp = static_cast<char>( oInput.get( ) );

            if ( cTemp >= ' ' && cTemp <= '~' )
            {
                pList.insert( cTemp );
            }
        }
        
        return oInput;
    }
};

enum ProgramMode
{
    MODE_NORMAL,
    MODE_SAVING,
    MODE_LOADING
};

int main( )
{    
    StringList pList, pFileBuffer;
    char cInput = -1;
    ProgramMode nMode = MODE_NORMAL;
    
    fakecurses::init( );
    fakecurses::clearScreen( );
    
    while ( cInput != 3 )
    {
        cInput = fakecurses::getKey( );

        if ( cInput != -1 )
        {
            StringList& pCurrentList = ( nMode == MODE_NORMAL ? pList : pFileBuffer );
            
            if ( cInput >= ' ' && cInput <= '~' )
            {
                /* See if we did a control sequence using the arrow keys */
                
                if ( cInput == '[' )
                {
                    char cNext = fakecurses::getKey( );
                    
                    if ( cNext == -1 )
                    {
                        pCurrentList.insert( cInput );
                    }
                    else
                    {
                        if ( cNext == 'D' )
                        {
                            pCurrentList.left( );
                        }
                        else if ( cNext == 'C' )
                        {
                            pCurrentList.right( );
                        }
                    }
                }
                else
                {
                    pCurrentList.insert( cInput );
                }
            }
            else if ( cInput == 127 )
            {
                pCurrentList.erase( );
            }
            else if ( cInput == 19 && nMode == MODE_NORMAL) /* Control+S */
            {
                nMode = MODE_SAVING;
            }
            else if ( cInput == 6 && nMode == MODE_NORMAL ) /* Control+F */
            {
                nMode = MODE_LOADING;
            }
            else if ( cInput == 13 && nMode != MODE_NORMAL ) /* Enter button */
            {
                std::string sFilename = pFileBuffer.toString( );
                std::cerr << sFilename << std::endl;
                
                if ( nMode == MODE_SAVING )
                {
                    std::ofstream sStream( sFilename );
                    sStream << pList;
                }
                else if ( nMode == MODE_LOADING )
                {
                    std::ifstream sStream( sFilename );
                    sStream >> pList;
                }
                
                nMode = MODE_NORMAL;
                pFileBuffer.clear( );
            }
            
            fakecurses::clearScreen( );
            fakecurses::setCursor( 1, 1 );
            std::cout << pList;
            fakecurses::setCursor( 1 + static_cast<short>( pList.getCursorPos( ) ), 1 );

            if ( nMode != MODE_NORMAL )
            {
                fakecurses::setCursor( 1, fakecurses::getScrHeight( ) );

                if ( nMode == MODE_SAVING )
                {
                    std::cout << "Save to: ";
                }
                else
                {
                    std::cout << "Load from: ";
                }
                
                std::cout << pFileBuffer;
            }
        }
    }
    
    fakecurses::cleanup( );
    
    return 0;
}
