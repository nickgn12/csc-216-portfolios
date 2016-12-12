#include <iostream>
#include <sstream>
#include <cstring>
#include "exprset.hpp"

void ExpressionSet::addExpression( std::string sInput )
{
    ExpressionTree pTemp;

    std::string sParen = pTemp.parenthesize( sInput );
    
    pTemp.setVariableStore( &pVariables );
    pTemp.readline( sParen, true );
    
    vExpressions.push_back( pTemp );
    vExpressionsInput.push_back( sParen );
    vExpressionsValues.push_back( pTemp.calculate( ) );
}

void ExpressionSet::addMultiExpressions( std::string sInput )
{
    char* szInput = new char[sInput.length( ) + 1];
    strcpy( szInput, sInput.c_str( ) );
    
    char* szToken = strtok( szInput, ";" );
    
    while ( szToken != nullptr )
    {
        addExpression( szToken );
        
        szToken = strtok( nullptr, ";" );
    }

    delete[] szInput;
}

std::string ExpressionSet::getValue( size_t nIndex ) const
{
    if ( nIndex >= length( ) )
    {
        throw std::out_of_range( "Trying to access expression out of bounds." );
    }

    std::stringstream sstream;
    sstream << vExpressionsInput[nIndex] << " = " << vExpressionsValues[nIndex];

    return sstream.str( );
}

double ExpressionSet::getNumericalValue( size_t nIndex ) const
{
    if ( nIndex >= length( ) )
    {
        throw std::out_of_range( "Trying to access expression out of bounds." );
    }

    return vExpressionsValues[nIndex];
}

std::istream& operator>>( std::istream& pInput, ExpressionSet& pExprSet )
{
    std::string sInput;
    getline( pInput, sInput );
    
    pExprSet.addMultiExpressions( sInput );

    return pInput;
}
