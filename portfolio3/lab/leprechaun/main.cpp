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
    int j = 0;
    
    for ( LepMapKey* i=pMap.first( ); i != nullptr; i = pMap.getNext( i ) )
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
            j = 0;
        }

        j++;
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
        std::cout << i << "   " << nIter++ << " @ " << i->nKey << " w/ " << i->nValue.nGold << " gold" << std::endl;
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
    //iterateLeprechauns( pLeprechauns );
    pLeprechauns.remove( pLeprechauns.first( ) );
    printLeprechauns( pLeprechauns );
    pLeprechauns.remove( pLeprechauns.last( ) );
    printLeprechauns( pLeprechauns );
    pLeprechauns.remove( pLeprechauns.getNext( pLeprechauns.first( ) ) );
    printLeprechauns( pLeprechauns );

    std::cerr << "finished" << std::endl;
}
