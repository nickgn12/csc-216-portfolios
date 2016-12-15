#ifndef ORDERED_MAP_HPP
#define ORDERED_MAP_HPP

#include "btree.hpp"

template<typename Key, typename Value>
struct MapKey
{
    Key nKey;
    Value nValue;

    BinaryTreeNode< struct MapKey<Key,Value> >* pNode;
};

template<typename Key, typename Value>
class OrderedMap
{
private:
    typedef MapKey<Key,Value> MapNode;
    BinaryTree<MapNode> pSearchTree;

    MapNode* getMaxUnderValue( const Key& nKey, BinaryTreeNode<MapNode>* pNode )
    {
        if ( nKey <= pNode->getData( ).nKey &&
             pNode->hasLeft( ) )
        {
            return getMaxUnderValue( nKey, pNode->getLeftNode( ) );
        }
        if ( nKey > pNode->getData( ).nKey &&
             pNode->hasRight( ) )
        {
            MapNode* pNodeTemp =
                getMaxUnderValue( nKey, pNode->getRightNode( ) );

            if ( pNodeTemp != nullptr )
            {
                Key nTemp = pNodeTemp->nKey;

                return ( nTemp > pNode->getDataPtr( )->nKey ) ? pNodeTemp : pNode->getDataPtr( );
            }
            else
            {
                return pNode->getDataPtr( );
            }
        }
        else if ( nKey > pNode->getDataPtr( )->nKey )
        {
            return pNode->getDataPtr( );
        }
        
        return nullptr;
    }

    MapNode* getMinOverValue( const Key& nKey, BinaryTreeNode<MapNode>* pNode )
    {
        if ( nKey >= pNode->getData( ).nKey &&
             pNode->hasRight( ) )
        {
            return getMinOverValue( nKey, pNode->getRightNode( ) );
        }
        if ( nKey < pNode->getData( ).nKey &&
             pNode->hasLeft( ) )
        {
            MapNode* pNodeTemp =
                getMinOverValue( nKey, pNode->getLeftNode( ) );

            if ( pNodeTemp != nullptr )
            {
                Key nTemp = pNodeTemp->nKey;

                return ( nTemp < pNode->getDataPtr( )->nKey ) ? pNodeTemp : pNode->getDataPtr( );
            }
            else
            {
                return pNode->getDataPtr( );
            }
        }
        else if ( nKey < pNode->getDataPtr( )->nKey )
        {
            return pNode->getDataPtr( );
        }
        
        return nullptr;
    }

public:
    OrderedMap( ): pSearchTree( ) { }
    OrderedMap( const OrderedMap& pMap ): pSearchTree( pMap.pSearchTree ) { }
    OrderedMap& operator=( const OrderedMap& pMap )
    {
        pSearchTree = pMap.pSearchTree;
    }
    
    /* Returns a reference to the smallest key value */
    
    MapNode* first( )
    {
        if ( pSearchTree.getRoot( ) == nullptr )
        {
            throw std::runtime_error( "Calling first() when map is empty." );
        }

        BinaryTreeNode<MapNode>* pLeft = pSearchTree.getRoot( );

        while ( pLeft->getLeftNode( ) != nullptr )
        {
            pLeft = pLeft->getLeftNode( );
        }

        return pLeft->getDataPtr( );
    }

    /* Returns a reference to the largest key value */
    
    MapNode* last( )
    {
        if ( pSearchTree.getRoot( ) == nullptr )
        {
            throw std::runtime_error( "Calling last() when map is empty." );
            return nullptr;
        }

        BinaryTreeNode<MapNode>* pRight = pSearchTree.getRoot( );

        while ( pRight->getRightNode( ) != nullptr )
        {
            pRight = pRight->getRightNode( );
        }

        return pRight->getDataPtr( );        
    }

    MapNode* find( const Key& nKey )
    {
        if ( pSearchTree.getRoot( ) == nullptr )
        {
            throw std::runtime_error( "Calling find() when map is empty." );
            return nullptr;
        }

        BinaryTreeNode<MapNode>* pCurrent = pSearchTree.getRoot( );
        bool bLooping = true;
        
        while ( bLooping && pCurrent->getDataPtr( )->nKey != nKey )
        {
            if ( pCurrent->hasRight( ) )
            {
                if ( nKey > pCurrent->getRightNode( )->getDataPtr( )->nKey )
                {
                    pCurrent = pCurrent->getRightNode( );
                }
                else
                {
                    bLooping = false;
                }
            }
            else if ( pCurrent->hasLeft( ) )
            {
                if ( nKey < pCurrent->getLeftNode( )->getDataPtr( )->nKEy )
                {
                    pCurrent = pCurrent->getLeftNode( );
                }
                else
                {
                    bLooping = false;
                }
            }
        }

        if ( pCurrent->getDataPtr( )->nKey == nKey )
        {
            return pCurrent->getDataPtr( );
        }
        else
        {
            return nullptr;
        }
    }
    
    /* Returns the key that has the greatest value less than pKey */
    
    MapNode* getLower( const Key& nKey )
    {
        return getMaxUnderValue( nKey, pSearchTree.getRoot( ) );
    }

    /* Returns the key that has the smallest value greater than pKey */

    MapNode* getHigher( const Key& nKey )
    {
        return getMinOverValue( nKey, pSearchTree.getRoot( ) );
    }
    
    MapNode* getNext( const MapNode* pKey )
    {
        if ( pKey == last( ) )
        {
            return nullptr;
        }
        
        BinaryTreeNode<MapNode>* pCurrent = pKey->pNode;

        if ( pCurrent->getParent( ) != nullptr )
        {
            if ( pCurrent->hasRight( ) )
            {
                /* If we are a middle node, go to the right. */
                
                pCurrent = pCurrent->getRightNode( );

                while ( pCurrent->hasLeft( ) )
                {
                    pCurrent = pCurrent->getLeftNode( );
                }
            }
            else if ( pCurrent->getParent( )->getLeftNode( ) == pCurrent )
            {
                /* If we are a left node, go up to the middle node. */

                pCurrent = pCurrent->getParent( );
            }
            else
            {
                /* If we are a right node, we have to traverse the tree
                   until we get to a suitable middle node */
        
                while ( pKey->nKey >
                        pCurrent->getParent( )->getData( ).nKey )
                {
                    pCurrent = pCurrent->getParent( );
                }

                pCurrent = pCurrent->getParent( );
            }
        }
        else
        {
            if ( pCurrent->hasRight( ) )
            {
                pCurrent = pCurrent->getRightNode( );
                
                while ( pCurrent->hasLeft( ) )
                {
                    pCurrent = pCurrent->getLeftNode( );
                }
            }
            else
            {
                return nullptr;
            }
        }

        return pCurrent->getDataPtr( );
    }

    MapNode* insert( const Key& nKey, const Value& nValue )
    {
        BinaryTreeNode<MapNode>* pNode;
        
        if ( pSearchTree.getRoot( ) == nullptr )
        {
            /* If our search tree is empty just stick it at the top */
            
            pNode = pSearchTree.createRoot( );
        }
        else
        {
            /* Otherwise loop through and find a good place to put it. */
            
            BinaryTreeNode<MapNode>* pCurrent = pSearchTree.getRoot( );
            bool bLooping = true;

            while ( bLooping )
            {
                if ( nKey < pCurrent->getDataPtr( )->nKey )
                {
                    /* Go left if smaller */
                    
                    if ( pCurrent->hasLeft( ) )
                    {
                        pCurrent = pCurrent->getLeftNode( );
                    }
                    else
                    {
                        pNode = pCurrent->createLeftNode( );
                        bLooping = false;
                    }
                }
                else if ( nKey >= pCurrent->getDataPtr( )->nKey )
                {
                    /* Go right if larger */
                    
                    if ( pCurrent->hasRight( ) )
                    {
                        pCurrent = pCurrent->getRightNode( );
                    }
                    else
                    {
                        pNode = pCurrent->createRightNode( );
                        bLooping = false;
                    }
                }
            }
        }

        MapNode* pData = pNode->getDataPtr( );
        pData->pNode = pNode;
        pData->nKey = nKey;
        pData->nValue = nValue;

        return pData;
    }

    void remove( MapNode* pNode )
    {
        if ( pNode == nullptr )
        {
            return;
        }
        
        BinaryTreeNode<MapNode>* pTreeNode = pNode->pNode;
        
        if ( !pNode->pNode->hasAnyChildren( ) )
        {
            /* No children */
            
            if ( pTreeNode->getParent( ) != nullptr )
            {
                if ( pTreeNode->isRightChild( ) )
                {
                    pTreeNode->getParent( )->removeRightNode( );
                }
                else
                {
                    pTreeNode->getParent( )->removeLeftNode( );
                }
            }
            else if ( pSearchTree.getRoot( ) == pTreeNode )
            {
                pSearchTree.deleteRoot( );
            }
        }
        else if ( !pTreeNode->hasChildren( ) && pTreeNode->hasAnyChildren( ) )
        {
            /* Only one child */
            
            BinaryTreeNode<MapNode>* pChild;

            if ( pTreeNode->hasLeft( ) )
            {
                pChild = pTreeNode->getLeft( );
            }
            else
            {
                pChild = pTreeNode->getRight( );
            }
            
            if ( pTreeNode->getParent( ) != nullptr )
            {
                if ( pTreeNode->isRightChild( ) )
                {
                    pTreeNode->getParent( )->getRight( ) = pChild;
                }
                else
                {
                    pTreeNode->getParent( )->getLeft( ) = pChild;
                }

                pChild->setParent( pTreeNode->getParent( ) );
            }
            else
            {
                pSearchTree.getRoot( ) = pChild;
                pChild->setParent( nullptr );
            }

            pTreeNode->getLeft( ) = nullptr;
            pTreeNode->getRight( ) = nullptr;
            delete pTreeNode;
        }
        else
        {
            /* Has both children 
               Find the smallest node on the right side */
            
            BinaryTreeNode<MapNode>* pSmallest = pTreeNode->getRight( );

            while ( pSmallest->hasLeft( ) )
            {
                pSmallest = pSmallest->getLeft( );
            }

            pTreeNode->getData( ) = pSmallest->getData( );
            pTreeNode->getData( ).pNode = pTreeNode;
            
            remove( pSmallest->getDataPtr( ) );
        }
    }
};

#endif
