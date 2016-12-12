#include <iostream>
#include "btree.hpp"

int main( )
{
    BinaryTree<int> pTree;
    pTree.createRoot( 5 );
    pTree.getRoot( )->createLeftNode( 7 );
    pTree.getRoot( )->createRightNode( 3 );

    int nSum = 0;
    auto pSumFunction = [&nSum]( BinaryTreeNode<int>* pNode )
    {
        nSum += pNode->getData( );
    };
    
    pTree.inorderTraversal( pSumFunction );

    std::cout << "Sum of the tree is: " << nSum << std::endl;
    
    return 0;
}
