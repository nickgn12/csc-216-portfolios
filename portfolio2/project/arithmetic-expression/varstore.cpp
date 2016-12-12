#include <cstddef>
#include "exprtree.hpp"

ExpressionVariable* VariableStore::getVariable( char cVariable, unsigned short nSubscript )
{
    if ( vVariables.size( ) == 0 )
    {
        return nullptr;
    }
    else
    {
        for ( size_t i=0; i < vVariables.size( ); i++ )
        {
            if ( cVariable == vVariables[i].cVariable &&
                 nSubscript == vVariables[i].nSubscript )
            {
                return &( vVariables[i] );
            }
        }

        return nullptr;
    }
}
    
ExpressionVariable* VariableStore::createVariable( char cVariable, unsigned short nSubscript )
{
    ExpressionVariable pVariable;
    pVariable.cVariable = cVariable;
    pVariable.nSubscript = nSubscript;
    pVariable.dValue = 0;

    vVariables.push_back( pVariable );
    return &( vVariables.back( ) );
}
    
ExpressionVariable* VariableStore::findVariable( char cVariable, unsigned short nSubscript )
{
    ExpressionVariable* pVariable = getVariable( cVariable, nSubscript );

    if ( pVariable == nullptr )
    {
        pVariable = createVariable( cVariable, nSubscript );
    }

    return pVariable;
}
