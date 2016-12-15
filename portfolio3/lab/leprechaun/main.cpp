#include <iostream>
#include <cstdlib>
#include "orderedmap.hpp"

typedef double Position;
struct LeprechaunData
{
    double nGold;
    bool bIterated;
};
typedef OrderedMap<Position,LeprechaunData> LepMap;
typedef MapKey<Position,LeprechaunData> LepMapKey;

void iterateLeprechauns( LepMap& pMap )
{
    LepMapKey* i = pMap.first( );
    
    //for ( LepMapKey* i=pMap.first( ); i != nullptr; i = pMap.getNext( i ) )
    while ( i != nullptr )
    {
        if ( i->nValue.bIterated == false )
        {
            double r = static_cast<double>( rand( ) % 2000 - 1000 ) / 1000.0;
            i->nKey += r * i->nValue.nGold;

            Position p = i->nKey;
            LeprechaunData d = i->nValue;
            d.bIterated = true;

            pMap.remove( i );

            auto* pHigher = pMap.getHigher( p );
            auto* pLower = pMap.getLower( p );

            if ( pHigher != nullptr )
            {
                d.nGold += pHigher->nValue.nGold / 2;
                pHigher->nValue.nGold /= 2;
            }
            
            if ( pLower != nullptr )
            {    
                d.nGold += pLower->nValue.nGold / 2;
                pLower->nValue.nGold /= 2;
            }
                     
            pMap.insert( p, d );
            i = pMap.first( );
        }
        else
        {
            i = pMap.getNext( i );
        }
    }

    for ( LepMapKey* i=pMap.first( ); i != nullptr; i = pMap.getNext( i ) )
    {
        i->nValue.bIterated = false;
    }
}

void printLeprechauns( LepMap& pMap )
{
    std::cout << "Contents of leprechaun map: " << std::endl;

    int nIter = 1;
    for ( LepMapKey* i=pMap.first( ); i != nullptr; i = pMap.getNext( i ) )
    {
        std::cout << nIter++ << " @ " << i->nKey << " w/ " << i->nValue.nGold << " gold" << std::endl;
    }
}

int main( )
{
    LepMap pLeprechauns;

    std::srand( static_cast<unsigned int>( time( nullptr ) ) );
    size_t nLeprechauns = std::rand( ) % 4 + 3;

    std::cout << "There are " << nLeprechauns << " leprechauns." << std::endl;
    
    for ( size_t i=0; i < nLeprechauns; i++ )
    {
        pLeprechauns.insert( std::rand( ) % 100, { 1000000, false } );
    }

    printLeprechauns( pLeprechauns );

    for ( int i=0; i < 5; i++ )
    {
        iterateLeprechauns( pLeprechauns );
    }

    std::cout << "Iterated leprechauns 5 times" << std::endl;
    
    printLeprechauns( pLeprechauns );
}
