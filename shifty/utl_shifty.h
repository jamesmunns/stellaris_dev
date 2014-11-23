#pragma once

void UTL_set_many( shiftbrite *     sb,
                   sint16_t         r0,
                   sint16_t         g0,
                   sint16_t         b0,
                   sint16_t         num_to_set
                 );

void UTL_multifade( shiftbrite *    sb,
                    sint16_t        d_r,
                    sint16_t        d_g,
                    sint16_t        d_b,
                    sint16_t        r0,
                    sint16_t        g0,
                    sint16_t        b0,
                    sint16_t        steps );