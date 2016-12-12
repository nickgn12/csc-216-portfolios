#ifndef EXPR_TREE_HPP
#define EXPR_TREE_HPP

#include <string>
#include <vector>
#include "btree.hpp"

enum ExpressionOperator
{
    OP_POWER,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MODULUS,
    OP_EQUALS,
};

enum ExpressionNodeType
{
    TYPE_VARIABLE,
    TYPE_NUMBER,
    TYPE_OPERATOR
};

struct ExpressionVariable
{
    char cVariable;
    unsigned short nSubscript;
    double dValue;
};

struct ExpressionNode
{
    ExpressionNodeType nType;

    union
    {
        double dValue;
        ExpressionOperator nOperator;
        ExpressionVariable* pVariable;
    };
};

class VariableStore
{
private:
    std::vector<ExpressionVariable> vVariables;

public:
    VariableStore( ): vVariables( ) { }
    
    ExpressionVariable* getVariable( char cVariable,
                                     unsigned short nSubscript );
    ExpressionVariable* createVariable( char cVariable,
                                        unsigned short nSubscript );
    ExpressionVariable* findVariable( char cVariable,
                                      unsigned short nSubscript );

    size_t length( ) const
    {
        return vVariables.size( );
    }
    
    size_t size( ) const
    {
        return vVariables.size( );
    }

    ExpressionVariable* getVarIndex( size_t nIndex )
    {        
        return &( vVariables[nIndex] );
    }
};

class ExpressionTree
{
private:
    BinaryTree<ExpressionNode> pTree;
    VariableStore* pVariables;
    bool bCreatedVarStore;
    
    bool isOperator( char cParse ) const;
    size_t getTopLevelOpLocation( std::string sParse ) const;
    std::string getTopLevelOp( std::string sParse ) const;
    std::string removeParen( std::string sInput ) const;
    ExpressionOperator getOpFromChar( char cOperator ) const;
    int getOpPriority( ExpressionOperator nOperator ) const;
    
    void parseInputTree( std::string sInput,
                         BinaryTreeNode<ExpressionNode>& bTreeNode );
    double calculateTree( BinaryTreeNode<ExpressionNode>& bTreeNode );

    std::string parenthesizeOp( std::string sInputm, int nOperators ) const;
    
public:
    std::string parenthesize( std::string sInput ) const;
    
    ExpressionTree( );
    ExpressionTree( const ExpressionTree& pExprTree );
    ExpressionTree( std::string sInput );
    ExpressionTree& operator=( const ExpressionTree& pExprTree );
    ~ExpressionTree( )
    {
        if ( pVariables && bCreatedVarStore )
        {
            delete pVariables;
        }
    }
    
    void readline( std::string sInput, bool bParenthesized=false );
    double calculate( );
    void setVariableStore( VariableStore* pVar )
    {
        if ( pVariables && bCreatedVarStore )
        {
            delete pVariables;
            bCreatedVarStore = false;
        }
        
        pVariables = pVar;
    }
};

#endif
