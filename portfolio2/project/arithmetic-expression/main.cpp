#include <iostream>
#include "exprset.hpp"

int testSet( )
{
    ExpressionSet pExprSet;

    std::cout << "Enter equation(s) separated by semicolons:" << std::endl;
    std::cin >> pExprSet;

    std::cout << std::endl << "Values: " << std::endl;
    
    for ( size_t i=0; i < pExprSet.size( ); i++ )
    {
        std::cout << pExprSet.getValue( i ) << std::endl;
    }

    VariableStore* pVariables = pExprSet.getVariables( );

    if ( pVariables->size( ) > 0 )
    {
        std::cout << std::endl << "Variables: " << std::endl;
        
        for ( size_t i=0; i < pVariables->size( ); i++ )
        {
            ExpressionVariable* pVar = pVariables->getVarIndex( i );
        
            std::cout << pVar->cVariable;

            if ( pVar->nSubscript != 0 )
            {
                std::cout << pVar->nSubscript;
            }

            std::cout << " = " << pVar->dValue << std::endl;
        }
    }
    
    return 0;
}

int testTree( )
{
    ExpressionTree pTree;
    std::cout << pTree.parenthesize( "5+(5*2)-(2*6/2)" ) << std::endl;

    return 0;
}

int main( )
{
    return testSet( );
}
