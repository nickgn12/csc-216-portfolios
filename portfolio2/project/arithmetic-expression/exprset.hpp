#ifndef EXPRESSION_SET_HPP
#define EXPRESSION_SET_HPP

#include <vector>
#include "exprtree.hpp"

class ExpressionSet
{
private:
    std::vector<ExpressionTree> vExpressions;
    std::vector<std::string> vExpressionsInput;
    std::vector<double> vExpressionsValues;
    VariableStore pVariables;

public:
    ExpressionSet( ): vExpressions( ), vExpressionsInput( ), vExpressionsValues( ), pVariables( ) { }
    
    void addExpression( std::string sInput );
    void addMultiExpressions( std::string sInput );
    
    size_t length( ) const
    {
        return vExpressions.size( );
    }
    
    size_t size( ) const
    {
        return vExpressions.size( );
    }

    VariableStore* getVariables( ){ return &pVariables; }
    
    std::string getValue( size_t nIndex ) const;
    double getNumericalValue( size_t nIndex ) const;

    friend std::istream& operator>>( std::istream& pInput, ExpressionSet& pExprSet );
};

#endif
