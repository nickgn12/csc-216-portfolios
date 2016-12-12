#ifndef BTREE_HPP
#define BTREE_HPP

template<typename Data>
class BinaryTreeNode
{
private:
    BinaryTreeNode<Data>* pLeft,* pRight;
    Data pData;
    
public:
    /* big three */
    
    BinaryTreeNode( ): pLeft( nullptr ), pRight( nullptr ), pData( ) { }

    BinaryTreeNode( const Data& pDataNew ): pLeft( nullptr ), pRight( nullptr ), pData( pDataNew ) { }
    
    BinaryTreeNode( const BinaryTreeNode& pNode ): pLeft( nullptr ), pRight( nullptr ), pData( )
    {
        operator=( pNode );
    }
    
    BinaryTreeNode& operator=( const BinaryTreeNode& pNode )
    {
        if ( pNode.pLeft != nullptr )
        {
            pLeft = new BinaryTreeNode( *pNode.pLeft );
        }

        if ( pNode.pRight != nullptr )
        {
            pRight = new BinaryTreeNode( *pNode.pRight );
        }

        return *this;
    }
    
    ~BinaryTreeNode( )
    {
        if ( pLeft != nullptr )
        {
            delete pLeft;
        }

        if ( pRight != nullptr )
        {
            delete pRight;
        }
    }

    /* reference pointers so that we can modify the children */
    
    BinaryTreeNode*& getLeftNode( )
    {
        return pLeft;
    }
    
    BinaryTreeNode*& getRightNode( )
    {
        return pRight;
    }

    Data& getData( )
    {
        return pData;
    }
    
    /* const non-reference versions */
    
    BinaryTreeNode* getLeftNode( ) const
    {
        return pLeft;
    }

    BinaryTreeNode* getRightNode( ) const
    {
        return pRight;
    }

    Data getData( ) const
    {
        return pData;
    }
    
    /* checks to see if children exist */
    
    bool hasLeft( ) const
    {
        return ( pLeft ? true : false );
    }
    
    bool hasRight( ) const
    {
        return ( pRight ? true : false );
    }

    bool hasChildren( ) const
    {
        return ( ( pRight && pLeft ) ? true : false );
    }
};

template<typename Data>
class BinaryTree
{
protected:
    BinaryTreeNode<Data>* pRoot;
    
public:
    /* big three */
    
    BinaryTree( ): pRoot( nullptr ) { }
    
    BinaryTree( const BinaryTree& pTree ): pRoot( nullptr )
    {
        if ( pTree.pRoot != nullptr )
        {
            pRoot = new BinaryTreeNode<Data>( *pTree.pRoot );
        }
    }
    
    BinaryTree& operator=( const BinaryTree& pTree )
    {
        if ( pRoot != nullptr )
        {
            delete pRoot;
            pRoot = nullptr;
        }
        
        if ( pTree.pRoot != nullptr )
        {
            pRoot = new BinaryTreeNode<Data>( *pTree.pRoot );
        }

        return *this;
    }
    
    ~BinaryTree( )
    {
        if ( pRoot != nullptr )
        {
            delete pRoot;
        }
    }
    
    BinaryTreeNode<Data>*& getRoot( )
    {
        return pRoot;
    }

    BinaryTreeNode<Data>* getRoot( ) const
    {
        return pRoot;
    }
};

#endif
