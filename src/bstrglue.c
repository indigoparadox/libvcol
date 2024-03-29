
#define BSTRGLUE_C
#include "bstrglue.h"

#include <stdlib.h>

struct VECTOR* bgsplit( const_bstring str, char split ) {
   int i = 0;
   unsigned char* chr;
   struct VECTOR* v = NULL;
   bstring current_str = NULL;

   v = vector_new();

   current_str = bfromcstr( "" );
   lgc_null( current_str );

   while( str->slen > i ) {
      chr = &(str->data[i]);

      if( *chr == split ) {
         if( 0 == blength( current_str ) ) {
            /* Skip all whitespace until the next chunk starts. */
            i++;
            continue;
         }
         vector_add( v, current_str );
         current_str = bfromcstr( "" );
         lgc_null( current_str );
      } else {
         bconchar( current_str, *chr );
      }

      i++;
   }

   vector_add( v, current_str );

cleanup:
   return v;
}

int bgtoi( bstring i_b ) {
   const char* i_c = NULL;
   int i_out = 0;

   i_c = bdata( i_b );
   lgc_null( i_c );

   i_out = atoi( i_c );

cleanup:
   return i_out;
}
