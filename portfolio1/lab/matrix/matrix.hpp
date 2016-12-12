#ifndef LIB_MATRIX_HPP
#define LIB_MATRIX_HPP

#include <fstream>

/**
 * Template class for an nRows by nCols matrix.
 */

template <short nRows, short nCols>
class Matrix
{
private:
    double dData[nRows*nCols];

public:
    Matrix( )
    {
        for ( size_t i=0; i < nRows*nCols; i++ )
        {
            dData[i] = 0.0;
        }
    }

    Matrix( std::initializer_list<double> dList )
    {
        for ( size_t i=0; i < dList.size( ) && i < nRows*nCols; i++ )
        {
            dData[i] = *( dList.begin( ) + i );
        }
    }
    
    constexpr unsigned long getArea( ) const { return nRows*nCols; }
    constexpr short getRows( ) const { return nRows; }
    constexpr short getCols( ) const { return nCols; }
    constexpr short getHeight( ) const { return nRows; }
    constexpr short getWidth( ) const { return nCols; }
    
    double& operator[]( const size_t nIndex )
    {
        if ( nIndex >= getArea( ) )
            return dData[ getArea( ) - 1 ];

        return dData[ nIndex ];
    }

    double& get( const size_t x, const size_t y )
    {
        return dData[ ( y * nCols ) + x ];
    }

    double get( const size_t x, const size_t y ) const
    {
        return dData[ ( y * nCols ) + x ];
    }
    
    Matrix<nRows,nCols> add( const Matrix<nRows,nCols>& mOther )
    {
        Matrix<nRows,nCols> mReturn;

        for ( size_t i=0; i < getArea( ); i++ )
        {
            mReturn.dData[i] = dData[i] + mOther.dData[i];
        }

        return mReturn;
    }

    Matrix<nRows,nCols> operator+( const Matrix<nRows,nCols> & mOther )
    {
        return add( mOther );
    }

    template<short nCols2>
    Matrix<nRows,nCols2> multiply( const Matrix<nCols,nCols2>& mOther )
    {
        /* Adapted from pseudocode on https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm */
        
        Matrix<nRows,nCols2> mReturn;

        for ( size_t i=0; i < nRows; i++ )
        {
            for ( size_t j=0; j < nCols2; j++ )
            {
                double dSum = 0;

                for ( size_t k=0; k < nCols; k++ )
                {
                    dSum += get( k, i ) * mOther.get( j, k );
                }

                mReturn.get( j, i ) = dSum;
            }
        }
        
        return mReturn;
    }

    template<short nCols2>
    Matrix<nRows,nCols2> operator*( const Matrix<nCols,nCols2>& mOther )
    {
        return multiply( mOther );
    }

    friend std::ostream& operator<<( std::ostream& stream, const Matrix& mMatrix )
    {
        stream << "(" << nRows << "x" << nCols << ")[";

        for ( size_t i=0; i < mMatrix.getArea( ); i++ )
        {
            stream << ' ' << mMatrix.dData[i];

            if ( ( i + 1 ) % nCols == 0 && i != mMatrix.getArea( )-1 )
                stream << ',';
            else
                stream << ' ';
        }

        stream << "]";

        return stream;
    }
};

#endif
