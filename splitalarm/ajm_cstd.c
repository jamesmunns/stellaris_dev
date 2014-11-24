#include "ajm_types.h"

#include "ajm_cstd.h"

void memset( uint8_t* src, uint8_t val, uint32_t ct )
{
    uint32_t i;

    for( i = 0; i<ct; i++ )
    {
        src[i] = val;
    }
}

void memcpy( uint8_t* src, uint8_t* dst, uint32_t ct )
{
    uint32_t i;

    for( i = 0; i<ct; i++ )
    {
        dst[i] = src[i];
    }
}

uint8_t strncmp( uint8_t* src, uint8_t* dst, uint32_t ct )
{
    uint32_t i;

    for( i = 0; i<ct; i++ )
    {
        if( dst[i] != src[i] )
        {
            return src[i]-dst[i];
        }
    }
    return 0;
}