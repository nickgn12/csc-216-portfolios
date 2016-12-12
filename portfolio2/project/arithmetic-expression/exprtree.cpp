#include <iostream>
#include <cmath>
#include <cstddef>

#include "exprtree.hpp"

#define CERR_DEBUG_PRINT 0

/**
 * The big three.
 */

ExpressionTree::ExpressionTree( ):
    pTree( ),
    pVariables( nullptr ),
    bCreatedVarStore( false ) { }

ExpressionTree::ExpressionTree( const ExpressionTree& pExprTree ):
    pTree( pExprTree.pTree ),
    pVariables( pExprTree.pVariables ),
    bCreatedVarStore( pExprTree.bCreatedVarStore ) { }

ExpressionTree::ExpressionTree( std::string sInput ):
    pTree( ),
    pVariables( nullptr ),
    bCreatedVarStore( false )
{
    readline( sInput );
}

ExpressionTree& ExpressionTree::operator=( const ExpressionTree& pExprTree )
{
    pTree = pExprTree.pTree;
    pVariables = pExprTree.pVariables;
    bCreatedVarStore = pExprTree.bCreatedVarStore;

    return *this;
}

/**
 * Gets the relative priority of an operator.
 */

int ExpressionTree::getOpPriority( ExpressionOperator nOperator ) const
{
    int nPriority;
    
    switch ( nOperator )
    {
    case OP_POWER:
        nPriority = 5;
        break;
    case OP_MULTIPLICATION:
        nPriority = 2;
        break;
    case OP_DIVISION:
        nPriority = 2;
        break;
    case OP_ADDITION:
        nPriority = 1;
        break;
    case OP_SUBTRACTION:
        nPriority = 1;
        break;
    case OP_MODULUS:
        nPriority = 2;
        break;
    case OP_EQUALS:
        nPriority = 0;
        break;
    default:
        nPriority = -1;
        break;
    }

    return nPriority;
}

/**
 * Returns the operator enum from a character.
 */

ExpressionOperator ExpressionTree::getOpFromChar( char cOperator ) const
{
    ExpressionOperator nReturn;

    switch ( cOperator )
    {
    default:
    case '+':
        nReturn = OP_ADDITION;
        break;
    case '-':
        nReturn = OP_SUBTRACTION;
        break;
    case '^':
        nReturn = OP_POWER;
        break;
    case '*':
        nReturn = OP_MULTIPLICATION;
        break;
    case '/':
        nReturn = OP_DIVISION;
        break;
    case '%':
        nReturn = OP_MODULUS;
        break;
    case '=':
        nReturn = OP_EQUALS;
        break;
    }

    return nReturn;
}

/**
 * Returns true if the given character is an operator.
 */

bool ExpressionTree::isOperator( char cParse ) const
{
    bool bReturn;

    switch ( cParse )
    {
    case '+':
    case '-':
    case '^':
    case '/':
    case '%':
    case '=':
    case '*':
        bReturn = true;
        break;
    default:
        bReturn = false;
        break;
    }

    return bReturn;
}

/**
 * Get the top level operator in an expression.  If the expression is fully parenthesized,
 * then there will be only one top level operator.
 */

std::string ExpressionTree::getTopLevelOp( std::string sParse ) const
{
    int nParen = 0;
    bool bParsing = true;
    std::string sReturn;
    
    for ( size_t i=0; i < sParse.length( ) && bParsing; i++ )
    {
        if ( sParse[i] == '(' )
        {
            nParen++;
        }
        if ( sParse[i] == ')' )
        {
            nParen--;
        }        
        if ( nParen == 0 && isOperator( sParse[i] ) )
        {            
            sReturn += sParse[i];

            /* In case we have multi-character operators such as +=, -=, etc. */
            
            if ( isOperator( sParse[i+1] ) )
            {
                sReturn += sParse[i+1];
            }

            bParsing = false;
        }
    }

    return sReturn;
}

/**
 * Get the location of the top-level operator.
 */

size_t ExpressionTree::getTopLevelOpLocation( std::string sParse ) const
{
    int nParen = 0;
    bool bParsing = true;
    size_t nLocation = 0;
    
    for ( size_t i=0; i < sParse.length( ) && bParsing; i++ )
    {
        if ( sParse[i] == '(' )
        {
            nParen++;
        }
        if ( sParse[i] == ')' )
        {
            nParen--;
        }        
        if ( nParen == 0 && isOperator( sParse[i] ) )
        {            
            nLocation = i;
            bParsing = false;
        }
    }

    return nLocation;
}

/**
 * Removes outer parenthesis in an expression if it contains them.
 */

std::string ExpressionTree::removeParen( std::string sInput ) const
{
    if ( sInput[0] == '(' && sInput[sInput.length( ) - 1 ] == ')' )
    {
        return sInput.substr( 1, sInput.length( ) - 2 );
    }
    else
    {
        return sInput;
    }
}

/**
 * Recursively subdivide a string into an expression tree.
 */

void ExpressionTree::parseInputTree( std::string sInput,
                                     BinaryTreeNode<ExpressionNode>& bTreeNode )
{
    std::string sOperator = getTopLevelOp( sInput );

    /* If there is no operator then are at the end of the branch. */
    
    if ( sOperator.length( ) != 0 )
    {
        std::string sExprLeft, sExprRight;
        sExprLeft = removeParen( sInput.substr( 0, getTopLevelOpLocation( sInput ) ) );
        sExprRight = removeParen( sInput.substr( getTopLevelOpLocation( sInput ) + sOperator.length( ),
                                                sInput.length( ) - 1 ) );

        /* Handle special case for +=, -=, etc. */
        
        if ( sOperator.length( ) == 2 && sOperator[1] == '=' )
        {
            /* If we have x+=5, convert it to x=x+5 */
            
            sExprRight = sExprLeft + sOperator[0] + "(" + sExprRight + ")";
            sOperator = "=";
        }

#if CERR_DEBUG_PRINT
        std::cerr << "Input expression: " << sInput << std::endl;
        std::cerr << "Operator: " << sOperator << std::endl;
        std::cerr << "Left expression: " << sExprLeft << std::endl;
        std::cerr << "Right expression: " << sExprRight << std::endl;
#endif

        bTreeNode.getData( ).nType = TYPE_OPERATOR;
        bTreeNode.getData( ).nOperator = getOpFromChar( sOperator[0] );
        
        bTreeNode.getLeftNode( ) = new BinaryTreeNode<ExpressionNode>;
        bTreeNode.getRightNode( ) = new BinaryTreeNode<ExpressionNode>;
        
        parseInputTree( sExprLeft, *bTreeNode.getLeftNode( ) );
        parseInputTree( sExprRight, *bTreeNode.getRightNode( ) );
    }
    else
    {
        /* If there is no operator then this is either a variable or number. */

        if ( std::isalpha( sInput[0] ) )
        {
            /* If the first character is a letter then it's a variable. */

            char cVar = sInput[0];
            unsigned short nSubscript = 0;
            
            if ( sInput.length( ) > 1 )
            {
                /* Subscripts are optional. */
                
                nSubscript = static_cast<unsigned short>
                    ( atoi( sInput.c_str( ) + 1 ) );
            }
            
            bTreeNode.getData( ).nType = TYPE_VARIABLE;
            bTreeNode.getData( ).pVariable =
                pVariables->findVariable( cVar, nSubscript );
        }
        else
        {
            /* Else, it's a number. */

            bTreeNode.getData( ).nType = TYPE_NUMBER;
            bTreeNode.getData( ).dValue = strtod( sInput.c_str( ), nullptr );
        }

    }
}

/**
 * Recursively calculate the numerical value of a node and it's children.
 */

double ExpressionTree::calculateTree( BinaryTreeNode<ExpressionNode>& bTreeNode )
{    
    if ( bTreeNode.getData( ).nType == TYPE_OPERATOR )
    {
        double dReturn = 0;
        double dLeft = calculateTree( *bTreeNode.getLeftNode( ) );
        double dRight = calculateTree( *bTreeNode.getRightNode( ) );
        
        switch ( bTreeNode.getData( ).nOperator )
        {
        case OP_POWER:
            dReturn = pow( dLeft, dRight );
            break;
        case OP_MULTIPLICATION:
            dReturn = dLeft * dRight;
            break;
        case OP_DIVISION:
            dReturn = dLeft / dRight;
            break;
        case OP_ADDITION:
            dReturn = dLeft + dRight;
            break;
        case OP_SUBTRACTION:
            dReturn = dLeft - dRight;
            break;
        case OP_MODULUS:
            dReturn = fmod( dLeft, dRight );
            break;
        case OP_EQUALS:
            if ( bTreeNode.getLeftNode( )->getData( ).nType == TYPE_VARIABLE )
            {
                /* Set the variable and return its new value. */
                
                bTreeNode.getLeftNode( )->getData( ).pVariable->dValue = dRight;
                dReturn = bTreeNode.getLeftNode( )->getData( ).pVariable->dValue;
            }
            else
            {
                /* Return whether or not the two sides are equal. */
                
                dReturn = ( fabs( dLeft - dRight ) < 0.001 );
            }
            break;
        }
        
        return dReturn;
    }
    else if ( bTreeNode.getData( ).nType == TYPE_VARIABLE )
    {
        return bTreeNode.getData( ).pVariable->dValue;
    }
    else if ( bTreeNode.getData( ).nType == TYPE_NUMBER )
    {
        return bTreeNode.getData( ).dValue;
    }

    return 0;
}

/**
 * Second stage recursive function in the parenthesizer.
 */

std::string ExpressionTree::parenthesizeOp( std::string sInput, int nOperators ) const
{
    if ( nOperators == 1 )
    {
        /* If there is only one operator then we don't have to
           parenthesize anything. */
        
        return sInput;
    }
    else
    {
        /* Find the highest priority operator. */
        
        int nParen = 0, nPriority = -1;
        size_t nHighestPos = 0;

        for ( size_t i=0; i < sInput.length( ); i++ )
        {
            if ( sInput[i] == '(' )
            {
                nParen++;
            }
            else if ( sInput[i] == ')' )
            {
                nParen--;
            }
            else if ( nParen == 0 && isOperator( sInput[i] ) )
            {
                int nCurPriority = getOpPriority( getOpFromChar( sInput[i] ) );

                if ( nCurPriority > nPriority )
                {
                    nPriority = nCurPriority;
                    nHighestPos = i;
                }
            }
        }

        /* Find where to put the parentheses */
        
        size_t nLeftParen = nHighestPos-1;
        size_t nRightParen = nHighestPos+1;
        bool bLooping = true;
        
        nParen = 0;

        /* Go left from the operator for the left parenthesis */
        
        while ( nLeftParen > 0 && bLooping )
        {
            if ( sInput[nLeftParen] == '(' )
            {
                nParen++;
            }
            else if ( sInput[nLeftParen] == ')' )
            {
                nParen--;
            }
            else if ( nParen == 0 && isOperator( sInput[nLeftParen] ) )
            {
                bLooping = false;
                nLeftParen++;
            }

            if ( bLooping )
            {
                nLeftParen--;
            }
        }

        nParen = 0;
        bLooping = true;

        /* Go right from the operator for the right parenthesis */
       
        while ( nRightParen < sInput.length( ) && bLooping )
        {
            if ( sInput[nRightParen] == '(' )
            {
                nParen++;
            }
            else if ( sInput[nRightParen] == ')' )
            {
                nParen--;
            }
            else if ( nParen == 0 && isOperator( sInput[nRightParen] ) )
            {
                bLooping = false;
            }

            if ( bLooping )
            {
                nRightParen++;
            }
        }

        sInput.insert( nLeftParen, 1, '(' );
        sInput.insert( nRightParen + 1, 1, ')' );

        return parenthesizeOp( sInput, nOperators - 1 );
    }

    return sInput;
}

/**
 * Converts a given expression to be fully parenthesized.
 */

std::string ExpressionTree::parenthesize( std::string sInput ) const
{
    size_t nStartParen = 0;
    int nParen = 0, nOper = 0;
    std::vector<std::string> vSubParen;

    /* Check if there are any parentheses and if so split those off
       and recursively parenthesize them. */
    
    for ( size_t i=0; i < sInput.length( ); i++ )
    {
        if ( sInput[i] == '(' )
        {
            if ( nParen == 0 )
            {
                nStartParen = i;
            }
            
            nParen++;
        }
        if ( sInput[i] == ')' )
        {
            if ( nParen == 1 )
            {
                std::string sAdd = sInput.substr( nStartParen + 1, i - ( nStartParen + 1) );
                
                vSubParen.push_back( parenthesize( sAdd ) );
                sInput.erase( nStartParen + 1, sAdd.length( ) + 1 );
                sInput[nStartParen] = '!';

                i -= sAdd.length( ) + 1;
            }

            nParen--;
        }
        if ( nParen == 0 && isOperator( sInput[i] ) )
        {
            nOper++;
        }
    }

    /* Parenthesize order of operations */
    
    if ( nOper > 1 )
    {
        sInput = parenthesizeOp( sInput, nOper );
    }

    /* Replace parentheses expressions back into our expression */
    
    if ( vSubParen.size( ) != 0 )
    {
        for ( size_t i=0; i < vSubParen.size( ); i++ )
        {
            size_t nSpot = sInput.find_first_of( "!" );
            
            sInput.erase( nSpot, 1 );
            sInput.insert( nSpot, vSubParen[i] );
        }
    }
    
    return "(" + sInput + ")";
}

/**
 * Reads and parses one expression.
 */

void ExpressionTree::readline( std::string sInput, bool bParenthesized )
{
    /* Kill all whitespace */
    
    std::string sInputNoWs;

    for ( size_t i=0; i < sInput.length( ); i++ )
    {
        if ( !std::isspace( sInput[i] ) )
        {
            sInputNoWs += sInput[i];
        }
    }

    /* Check if the parenthesis are correct */

    int nParen = 0;
    
    for ( size_t i=0; i < sInputNoWs.length( ); i++ )
    {
        if ( sInputNoWs[i] == '(' )
        {
            nParen++;
        }
        else if ( sInputNoWs[i] == ')' )
        {
            if ( nParen > 0 )
            {
                nParen--;
            }
            else
            {
                throw std::runtime_error( "Ending parenthesis before closing parenthesis." );
            }
        }
    }
    
    if ( nParen != 0 )
    {
        throw std::runtime_error( "Parenthesis in expression are unbalanced." );
    }

    /* Check if we have a variables store */
    
    if ( pVariables == nullptr )
    {
        pVariables = new VariableStore;
        bCreatedVarStore = true;
    }

    /* Parenthesize our function if it isn't already. */
    
    if ( !bParenthesized )
    {
        sInputNoWs = parenthesize( removeParen( sInputNoWs ) );
    }
    
    pTree.getRoot( ) = new BinaryTreeNode<ExpressionNode>;
    parseInputTree( removeParen( sInputNoWs ), *pTree.getRoot( ) );
}

/**
 * Calculates the numerical value of the expression.
 */

double ExpressionTree::calculate( )
{
    if ( pTree.getRoot( ) == nullptr )
    {
        return 0;
    }
    else
    {        
        return calculateTree( *pTree.getRoot( ) );
    }
}
