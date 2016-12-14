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
    for ( LepMapKey* i=pMap.first( ); i != nullptr; i = pMap.getNext( i ) )
    {
        if ( i->nData.bIterated == false )
        {
            double r = static_cast<double>( rand( ) % 2000 - 1000 ) / 1000.0;
            i->nKey += r * i->nData.nGold;

            Position p = i->nKey;
            LeprechaunData d = i->nData;
            
            i = pMap.first( );
        }
    }

    for ( LepMapKey* i=pMap.first( ); i != nullptr; i = pMap.getNext( i ) )
    {
        i->nData.bIterated = false;
    }
}

void printLeprechauns( LepMap& pMap )
{
    std::cout << "Contents of leprechaun map: " << std::endl;

    int nIter = 1;
    for ( LepMapKey* i=pMap.first( ); i != nullptr; i = pMap.getNext( i ) )
    {
        std::cout << nIter++ << " @ " << i->nKey << " w/ " << i->nData.nGold << " gold" << std::endl;
    }
}

int main( )
{
    LepMap pLeprechauns;

    std::srand( static_cast<unsigned int>( time( nullptr ) ) );
    size_t nLeprechauns = std::rand( ) % 4 + 3;

    for ( size_t i=0; i < nLeprechauns; i++ )
    {
        pLeprechauns.insert( std::rand( ) % 100, { 1000000, false } );
    }

    for ( LepMapKey* i=pLeprechauns.first( );
          i != nullptr;
          i = pLeprechauns.getNext( i ) )
    {
        std::cout << i->nKey;

        auto* pLower = pLeprechauns.getLower( i->nKey );
        auto* pHigher = pLeprechauns.getHigher( i->nKey );

        if ( pLower != nullptr )
        {
            std::cout << " left is " << pLower->nKey;
        }

        if ( pHigher != nullptr )
        {
            std::cout << " right is " << pHigher->nKey;
        }

        std::cout << std::endl;
    }
}
