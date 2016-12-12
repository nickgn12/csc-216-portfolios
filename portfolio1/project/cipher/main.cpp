#include <iostream>
#include <cmath>
#include <fstream>
#include <cstring>
#include <ctime>

class CipherKey
{
private:
    static const unsigned long KEY_LENGTH = 26;
    char pKey[KEY_LENGTH + 1]; // Plus one for the null terminator

    unsigned long getKeyIndex( char c ) const
    {
        unsigned long nReturn = 0;
        
        if ( !( c >= 'A' && c <= 'Z' ) )
            return 0;

        for ( unsigned long i=0; i < KEY_LENGTH && nReturn == 0; i++ )
        {
            if ( c == pKey[i] )
                nReturn = i;
        }
        
        return nReturn;
    }
    
public:
    CipherKey( )
    {
        std::memset( pKey, 0, KEY_LENGTH + 1 );
    }

    CipherKey( const CipherKey& pOther )
    {
        std::memcpy( pKey, pOther.pKey, KEY_LENGTH );
    }

    CipherKey& operator=( const CipherKey& pOther )
    {
        std::memcpy( pKey, pOther.pKey, KEY_LENGTH );

        return *this;
    }

    void generate( unsigned int nSeed )
    {
        std::srand( nSeed );
        
        for ( unsigned long i=0; i < KEY_LENGTH; i++ )
        {
            pKey[i] = 'A' + static_cast<char>( i );
        }

        for ( unsigned long i=0; i < KEY_LENGTH; i++ )
        {
            std::swap( pKey[i], pKey[ static_cast<unsigned int>( std::rand( ) ) % KEY_LENGTH ] );
        }
    }

    void setKey( const char* pNewKey )
    {
        if ( strlen( pNewKey ) != KEY_LENGTH )
        {
            std::cerr << "CipherKey::setKey(" << pNewKey << "): new key is not "
                      << KEY_LENGTH << "characters long." << std::endl;
            return;
        }

        std::strncpy( pKey, pNewKey, KEY_LENGTH + 1 );
    }
    
    std::string encrypt( const std::string& sEncrypt ) const
    {
        std::string sReturn;

        for ( size_t i=0; i < sEncrypt.length( ); i++ )
        {
            char c = sEncrypt[i];
            
            if ( std::islower( c ) )
                c = static_cast<char>( std::toupper( c ) );

            if ( std::isupper( c ) )
                sReturn += pKey[ c - 'A' ];
        }

        return sReturn;
    }

    std::string decrypt( const std::string& sDecrypt ) const
    {
        std::string sReturn;

        for ( size_t i=0; i < sDecrypt.length( ); i++ )
        {
            unsigned long nIndex = getKeyIndex( sDecrypt[i] );

            sReturn += 'A' + static_cast<char>( nIndex );
        }

        return sReturn;
    }
    
    unsigned int getKeyLength( ) const
    {
        return KEY_LENGTH;
    }
    
    const char* getKey( ) const
    {
        return pKey;
    }
};

class FileCipher
{
private:
    CipherKey pCipherKey;
    std::string sMessage;
    
public:
    FileCipher( ): pCipherKey( ), sMessage( "" )
    {
        pCipherKey.generate( static_cast<unsigned int>( std::time( NULL ) ) );
    }

    FileCipher( const std::string sSetMessage ): pCipherKey( ), sMessage( sSetMessage )
    {
        pCipherKey.generate( static_cast<unsigned int>( std::time( NULL ) ) );
    }
    
    FileCipher( const FileCipher& pOther ): pCipherKey( pOther.pCipherKey ),
                                            sMessage( pOther.sMessage ) { }

    FileCipher& operator=( const FileCipher& pOther )
    {
        pCipherKey = pOther.pCipherKey;
        sMessage = pOther.sMessage;
        
        return *this;
    }

    std::string getMessage( ) const
    {
        return sMessage;
    }

    void setMessage( std::string sNewMessage )
    {
        sMessage = sNewMessage;
    }

    std::string encrypt( ) const
    {
        return pCipherKey.encrypt( sMessage );
    }

    std::string decrypt( std::string sToDecrypt )
    {
        return pCipherKey.decrypt( sToDecrypt );
    }

    std::string encryptWithKey( ) const
    {
        std::string sEncrypted = encrypt( );
        std::string sReturn = "";
        const char* szKey = pCipherKey.getKey( );

        double nCharsPerSlot = (double) pCipherKey.getKeyLength( ) /
            ( sMessage.length( ) + 1 );
        unsigned long nCurrentKey = 0;
        
        /* Do first key pass */
            
        for ( unsigned long i=nCurrentKey; i < nCharsPerSlot; i++, nCurrentKey++ )
        {
            sReturn += szKey[ nCurrentKey ];
        }

        /* Write all characters */
            
        for ( unsigned long nChar=0; nChar < sMessage.length( ); nChar++ )
        {
            sReturn += sEncrypted[nChar];
            
            /* Write keys after all message characters */
                
            for ( ; nCurrentKey < static_cast<unsigned long>( nCharsPerSlot * ( nChar + 2 ) ); nCurrentKey++ )
            {
                sReturn += szKey[ nCurrentKey ];
            }
        }

        /* Write any leftover keys */
            
        for ( ; nCurrentKey < pCipherKey.getKeyLength( ); nCurrentKey++ )
        {
            sReturn += szKey[ nCurrentKey ];
        }
 
        return sReturn;
    }

    std::string decryptWithKey( std::string sEncrypted )
    {
        std::string sMsg, sKey;
        
        unsigned long nChars = sEncrypted.length( ) - static_cast<unsigned long>( pCipherKey.getKeyLength( ) );
        double nCharsPerSlot = (double) pCipherKey.getKeyLength( ) /
            ( nChars + 1 );
        
        /* nCurrent is position in overall string, nCurrentKey is
         * current part of the key */
        
        unsigned long nCurrent=0, nCurrentKey = 0;
        
        /* Do first key pass */

        for ( ; nCurrentKey < nCharsPerSlot; nCurrentKey++ )
        {
            sKey += sEncrypted[nCurrent++];
        }

        /* Look at all characters */

        for ( size_t nMsg = 0; nMsg < nChars; nMsg++ )
        {
            sMsg += sEncrypted[nCurrent++];
            
            for ( ; nCurrentKey < static_cast<unsigned long>( nCharsPerSlot * ( nMsg + 2 ) ); nCurrentKey++ )
            {
                sKey += sEncrypted[nCurrent++];
            }
        }

        /* Check any leftover keys */

        for ( ; nCurrent < sEncrypted.length( ); nCurrent++ )
        {
            sKey += sEncrypted[nCurrent];
        }

        pCipherKey.setKey( sKey.c_str( ) );
        sMessage = decrypt( sMsg );
        
        return sMessage;
    }

    friend std::ostream& operator<<( std::ostream& pOutput, const FileCipher& pCipher )
    {
        pOutput << pCipher.encryptWithKey( ) << std::endl;

        return pOutput;
    }

    friend std::istream& operator>>( std::istream& pInput, FileCipher& pCipher )
    {
        std::string sLine;
        std::getline( pInput, sLine );

        pCipher.decryptWithKey( sLine );

        return pInput;
    }
};

void printUsage( char* arg0 )
{
    std::cout << "USAGE: " << arg0 << " -f <filename> [options]" << std::endl << std::endl;
    std::cout << "OPTIONS: " << std::endl;
    std::cout << "    -f, --file\t\t\tFile to do operations on." << std::endl;
    std::cout << "    -e, --encrypt <msg>\t\tEncrypts <msg> and saves it in file." << std::endl;
    std::cout << "    -d, --decrypt\t\tDecrypts the message loaded from file." << std::endl << std::endl;
}

enum ProgramStatus
{
    STATUS_UNKNOWN,
    STATUS_ENCRYPT,
    STATUS_DECRYPT
};

int main( int argc, char** argv )
{
    std::string sFile;
    std::string sInputMsg;
    ProgramStatus nStatus = STATUS_UNKNOWN;
    
    if ( argc < 2 )
    {
        printUsage( argv[0] );
        return 1;
    }
    else
    {
        bool bArgsGood = true;
        int i = 1;
        
        while ( i < argc && bArgsGood )
        {
            char* szArg = argv[i];

            if ( szArg[0] != '-' )
            {
                bArgsGood = false;
            }
            else
            {
                if ( szArg[1] == 'f' || std::strcmp( szArg, "--file" ) == 0 )
                {
                    sFile = argv[i+1];
                    i += 2;
                }
                if ( szArg[1] == 'e' || std::strcmp( szArg, "--encrypt" ) == 0 )
                {
                    nStatus = STATUS_ENCRYPT;
                    sInputMsg = argv[i+1];
                        
                    i += 2;
                }
                if ( szArg[1] == 'd' || std::strcmp( szArg, "--decrypt" ) == 0 )
                {
                    nStatus = STATUS_DECRYPT;
                    i++;
                }
            }
        }
        
        if ( !bArgsGood )
        {
            printUsage( argv[0] );
            return 1;
        }
    }
    
    FileCipher pCipher;

    if ( nStatus == STATUS_ENCRYPT )
    {
        std::ofstream fOutput;
        
        pCipher.setMessage( sInputMsg );
        fOutput.open( sFile, std::ofstream::out );

        fOutput << pCipher;

        fOutput.close( );
    }
    else if ( nStatus == STATUS_DECRYPT )
    {
        std::ifstream fInput;

        fInput.open( sFile, std::ifstream::in );
        fInput >> pCipher;

        std::cout << pCipher.getMessage( ) << std::endl;
    }
    
    return 0;
}
