#include <iostream>
#include <stack>
#include <cmath>
#include <cstring>

double calculateRPN( std::string sInput )
{
    std::stack<double> pOperands;

    /* Check if there are spaces around every operator */

    for ( size_t i=0; i < sInput.length( ); i++ )
    {
        if ( sInput[i] == '+' ||
             sInput[i] == '-' ||
             sInput[i] == '*' ||
             sInput[i] == '/' ||
             sInput[i] == '%' ||
             sInput[i] == '^' )
        {
            if ( i != 0 && sInput[i-1] != ' ' )
            {
                sInput.insert( i, 1, ' ' );
                i++;
            }
            if ( i != sInput.length( ) -1 && sInput[i+1] != ' ' )
            {
                sInput.insert( i+1, 1, ' ' );
                i++;
            }
        }
    }

    /* Make a copy of the string so we can blow it up with strtok( ) */
    
    char* szInput = new char[sInput.length( ) + 1];
    strcpy( szInput, sInput.c_str( ) );

    char* szToken = strtok( szInput, " " );

    while ( szToken != nullptr )
    {
        if ( ( szToken[0] >= '0' && szToken[0] <= '9' ) || szToken[0] == '.' )
        {
            /* We have a number if the first character is a digit or begins with . (i.e. .5) */
            
            pOperands.push( strtod( szToken, nullptr ) );
        }
        else
        {
            /* Else, we have an operator */

            double dRight, dLeft, dResult;
            
            dRight = pOperands.top( );
            pOperands.pop( );
            
            dLeft = pOperands.top( );
            pOperands.pop( );
            
            switch ( szToken[0] )
            {
            case '+':
                dResult = dLeft + dRight;
                break;
            case '-':
                dResult = dLeft - dRight;
                break;
            case '/':
                dResult = dLeft / dRight;
                break;
            case '*':
                dResult = dLeft * dRight;
                break;
            case '^':
                dResult = pow( dLeft, dRight );
                break;
            case '%':
                dResult = fmod( dLeft, dRight );
                break;
            }

            pOperands.push( dResult );
        }
        
        szToken = strtok( nullptr, " " );
    }
    
    delete[] szInput;

    /* The last operand on the stack is the value of the expression */
    
    return pOperands.top( );
}

int main( )
{
    std::string sExpression;
    std::cout << "Please enter an expression in postfix (reverse polish) notation:" << std::endl;
    std::getline( std::cin, sExpression );

    std::cout << std::endl << "The value is: " << calculateRPN( sExpression ) << std::endl;

    return 0;
}
