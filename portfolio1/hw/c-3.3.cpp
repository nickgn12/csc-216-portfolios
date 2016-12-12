int getRepeatNumber( int* pArray, size_t nLength )
{
    bool bFound = false;
    int nRepeat = -1;
    
    for ( size_t i=0; i < nLength && !bFound; i++ )
    {
        for ( size_t j=i+1; j < nLength && !bFound; j++ )
        {
            if ( pArray[i] == pArray[j] )
            {
                nRepeat = pArray[i];
                bFound = true;
            }
        }
    }

    return nRepeat;
}
