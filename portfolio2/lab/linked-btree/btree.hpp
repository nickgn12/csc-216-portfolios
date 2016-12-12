#ifndef BTREE_HPP
#define BTREE_HPP

template<typename Data>
class BinaryTreeNode
{
private:
    BinaryTreeNode<Data>* pLeft,* pRight,* pParent;
    Data pData;
    
public:
    /* big three */
    
    BinaryTreeNode( ): pLeft( nullptr ), pRight( nullptr ), pParent( nullptr ), pData( ) { }

    BinaryTreeNode( const Data& pDataNew ): pLeft( nullptr ), pRight( nullptr ), pParent( nullptr ), pData( pDataNew ) { }
    
    BinaryTreeNode( const BinaryTreeNode& pNode ): pLeft( nullptr ), pRight( nullptr ), pParent( nullptr ), pData( )
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

        pParent = pNode.pParent;
        
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

    BinaryTreeNode* getParent( ) const
    {
        return pParent;
    }
    
    Data getData( ) const
    {
        return pData;
    }

    Data setData( const Data& pDataNew )
    {
        pData = pDataNew;
    }
    
    /* create children */

    BinaryTreeNode* createLeftNode( )
    {
        if ( pLeft )
        {
            delete pLeft;
            pLeft = nullptr;
        }

        pLeft = new BinaryTreeNode;
        pLeft->pParent = this;

        return pLeft;
    }

    BinaryTreeNode* createLeftNode( const Data& pDataNew )
    {
        if ( pLeft )
        {
            delete pLeft;
            pLeft = nullptr;
        }

        pLeft = new BinaryTreeNode( pDataNew );
        pLeft->pParent = this;

        return pLeft;
    }

    BinaryTreeNode* createRightNode( )
    {
        if ( pRight )
        {
            delete pRight;
            pRight = nullptr;
        }

        pRight = new BinaryTreeNode;
        pRight->pParent = this;

        return pRight;
    }

    BinaryTreeNode* createRightNode( const Data& pDataNew )
    {
        if ( pRight )
        {
            delete pRight;
            pRight = nullptr;
        }

        pRight = new BinaryTreeNode( pDataNew );
        pRight->pParent = this;

        return pRight;
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

    /* traversals */

    void preorderTraversal( const std::function<void(BinaryTreeNode<Data>*)>& pTraverseFunction )
    {
        pTraverseFunction( this );

        if ( pLeft != nullptr )
        {
            pLeft->preorderTraversal( pTraverseFunction );
        }

        if ( pRight != nullptr )
        {
            pRight->preorderTraversal( pTraverseFunction );
        }
    }

    void postorderTraversal( const std::function<void(BinaryTreeNode<Data>*)>& pTraverseFunction )
    {
        if ( pLeft != nullptr )
        {
            pLeft->postorderTraversal( pTraverseFunction );
        }

        if ( pRight != nullptr )
        {
            pRight->postorderTraversal( pTraverseFunction );
        }

        pTraverseFunction( this );
    }

    void inorderTraversal( const std::function<void(BinaryTreeNode<Data>*)>& pTraverseFunction )
    {
        if ( pLeft != nullptr )
        {
            pLeft->inorderTraversal( pTraverseFunction );
        }

        pTraverseFunction( this );
        
        if ( pRight != nullptr )
        {
            pRight->inorderTraversal( pTraverseFunction );
        }
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

    BinaryTreeNode<Data>* createRoot( )
    {
        if ( pRoot )
        {
            delete pRoot;
            pRoot = nullptr;
        }

        pRoot = new BinaryTreeNode<Data>;
        
        return pRoot;
    }

    BinaryTreeNode<Data>* createRoot( const Data& pData )
    {
        if ( pRoot )
        {
            delete pRoot;
            pRoot = nullptr;
        }

        pRoot = new BinaryTreeNode<Data>( pData );
        
        return pRoot;
    }
    
    void preorderTraversal( const std::function<void(BinaryTreeNode<Data>*)>& pTraverseFunction )
    {
        if ( pRoot != nullptr )
        {
            pRoot->preorderTraversal( pTraverseFunction );
        }
    }

    void postorderTraversal( const std::function<void(BinaryTreeNode<Data>*)>& pTraverseFunction )
    {
        if ( pRoot != nullptr )
        {
            pRoot->postorderTraversal( pTraverseFunction );
        }
    }

    void inorderTraversal( const std::function<void(BinaryTreeNode<Data>*)>& pTraverseFunction )
    {
        if ( pRoot != nullptr )
        {
            pRoot->inorderTraversal( pTraverseFunction );
        }
    }
};

#endif
