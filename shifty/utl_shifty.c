#include "shiftbrite.h"

#define R_CORR ( 120 )
#define G_CORR ( 100 )
#define B_CORR ( 100 )

void UTL_multifade( shiftbrite *    sb,
                    sint16_t        d_r,
                    sint16_t        d_g,
                    sint16_t        d_b,
                    sint16_t        r0,
                    sint16_t        g0,
                    sint16_t        b0,
                    sint16_t        steps )
{

}

void UTL_set_many( shiftbrite *     sb,
                   sint16_t         r0,
                   sint16_t         g0,
                   sint16_t         b0,
                   sint16_t         num_to_set
                 )
{
    int i;

    //shiftbrite_configure( sb, R_CORR, B_CORR, G_CORR, CLOCK_800MHZ );
    //shiftbrite_latch( sb );

    for( i=0; i<num_to_set; i++ )
    {
        shiftbrite_rgb( sb, r0, g0, b0 );
        shiftbrite_latch( sb );
    }

}