
#define GOKIBURI_C
#include "goki.h"

#ifdef DEBUG
#include <assert.h>
#endif /* DEBUG */

#define LG_BUFFER_ALLOC 255
#define LG_CAT_MAX 32
#define LG_CB_MAX 8
#define LG_PRINT_BUFFER_ALLOC 255

static struct tagbstring lg_ansi_color_strs[7] = {
   /* GRAPHICS_COLOR_BLUE        =  9, */ bsStatic( "\x1b[34m" ),
   /* GRAPHICS_COLOR_GREEN       = 10, */ bsStatic( "\x1b[32m" ),
   /* GRAPHICS_COLOR_CYAN        = 11, */ bsStatic( "\x1b[36m" ),
   /* GRAPHICS_COLOR_RED         = 12, */ bsStatic( "\x1b[31m" ),
   /* GRAPHICS_COLOR_MAGENTA     = 13, */ bsStatic( "\x1b[35m" ),
   /* GRAPHICS_COLOR_YELLOW      = 14, */ bsStatic( "\x1b[33m" ),
   /* GRAPHICS_COLOR_WHITE       = 15  */ bsStatic( "\x1b[0m" )
};

struct LG_TRACE_CAT {
   enum LG_COLOR color;
   const char* name;
   int index;
};

static bstring lg_buffer = NULL;
static struct LG_TRACE_CAT lg_categories[LG_CAT_MAX] = { 0 };
static lg_info_callback lg_info_cb = NULL;
static int lg_trace_cat_index = -1;

void lg_set_info_cb( lg_info_callback cb ) {
   lg_info_cb = cb;
}

int lg_add_trace_cat( const char* name, enum LG_COLOR color ) {
   int i = 0;

   for( i = 0 ; LG_CAT_MAX > i ; i++ ) {
      if( NULL == lg_categories[i].name ) {
         lg_categories[i].color = color;
         lg_categories[i].name = name;
         lg_categories[i].index = i;
         break;
      }
   }

   /* Return -1 if we're out of slots. */
   if( LG_CAT_MAX <= i ) {
      i = -1;
   }

   return i;
}

const char* lg_get_trace_cat_name() {
   if( 0 > lg_trace_cat_index ) {
      return NULL;
   }
   return lg_categories[lg_trace_cat_index].name;
}

int lg_set_trace_cat( const char* name ) {
   int i = 0;

   for( i = 0 ; LG_CAT_MAX > i ; i++ ) {
      if( NULL == lg_categories[i].name ) {
         break;
      }
      if( 0 == strcmp( name, lg_categories[i].name ) ) {
         break;
      }
   }

   /* Return -1 if we're out of slots. */
   if( LG_CAT_MAX <= i || NULL == lg_categories[i].name ) {
      i = -1;
   } else {
      lg_trace_cat_index = i;
   }

   return i;
}

bstring lg_trim_filename( bstring path ) {
   bstring mod_file_b = NULL;
   int bstr_ret = 0,
      mod_b_len = 0;

   mod_b_len = blength( path );
   bstr_ret = bstrrchrp( path, '/', mod_b_len - 1 );
   mod_file_b = bstrcpy( path );
   if( 0 < bstr_ret ) {
      bassignmidstr( mod_file_b, path, bstr_ret + 1, mod_b_len );
   } else {
      bstr_ret = bstrrchrp( path, '\\', mod_b_len - 1 );
      if( 0 < bstr_ret ) {
         bassignmidstr( mod_file_b, path, bstr_ret + 1, mod_b_len );
      }
   }

   return mod_file_b;
}

static void lg_log(
   void* log, const char* mod_in, enum LG_COLOR color,
   const char* message, va_list varg
) {
   int bstr_ret = 0;
   bstring mod_b = NULL;
   bstring mod_file_b = NULL;

   lgc_silence();

   if( NULL == lg_buffer ) {
      lg_buffer = bfromcstralloc( LG_BUFFER_ALLOC, "" );
   }

   mod_b = bfromcstr( mod_in );
   mod_file_b = lg_trim_filename( mod_b );

#ifdef DEBUG
   assert( NULL != lg_buffer );
#endif /* DEBUG */

   /* TODO: Silence first! */
   bstr_ret = btrunc( lg_buffer, 0 );
   lgc_nonzero( bstr_ret );

#ifdef DEBUG
   if( 0 <= lg_trace_cat_index ) {
      lg_snprintf(
         lg_buffer, "%b: %s: ",
         mod_file_b, lg_categories[lg_trace_cat_index].name
      );
   } else {
#endif /* DEBUG */
      lg_snprintf( lg_buffer, "%b: ", mod_file_b );
#ifdef DEBUG
   }
#endif /* DEBUG */
   lg_vsnprintf( lg_buffer, message, varg );

   lg_colorize( lg_buffer, color );

   lgc_unsilence();

   fprintf( log, "%s", bdata( lg_buffer ) );

cleanup:
   bdestroy( mod_b );
   bdestroy( mod_file_b );
   return;
}

void lg_debug( const char* mod_in, const char* message, ... ) {
#ifdef DEBUG
   va_list varg;

#ifndef HEATSHRINK_DEBUGGING_LOGS
   /* How's this for an innovative solution to keeping it C89? */
   if( 0 == strncmp( "hs", mod_in, 2 ) ) {
      goto cleanup;
   } if( 0 == strncmp( "syncbuff", mod_in, 8 ) ) {
      goto cleanup;
   }
#endif /* !HEATSHRINK_DEBUGGING_LOGS */

   va_start( varg, message );
   lg_log(
      lg_handle_err, mod_in, lg_categories[lg_trace_cat_index].color,
      message, varg
   );
   va_end( varg );

cleanup:
   return;
#endif /* DEBUG */
}

void lg_color(
   const char* mod_in, enum LG_COLOR color, const char* message, ...
) {
#ifdef DEBUG
   va_list varg;
   va_start( varg, message );
   lg_log( lg_handle_err, mod_in, color, message, varg );
   va_end( varg );
   return;
#endif /* DEBUG */
}

void lg_info( const char* mod_in, const char* message, ... ) {
   va_list varg;
   int bstr_ret;
   bstring mod_b = NULL;
   bstring mod_file_b = NULL;

   lgc_silence();

   mod_b = bfromcstr( mod_in );
   mod_file_b = lg_trim_filename( mod_b );

   if( NULL == lg_buffer ) {
      lg_buffer = bfromcstralloc( LG_PRINT_BUFFER_ALLOC, "" );
   }
#ifdef DEBUG
   assert( NULL != lg_buffer );
#endif /* DEBUG */

   bstr_ret = btrunc( lg_buffer, 0 );
   lgc_nonzero( bstr_ret );

   va_start( varg, message );
   lg_vsnprintf( lg_buffer, message, varg );
   va_end( varg );

#ifdef DEBUG
   fprintf( lg_handle_out, "%s: %s", bdata( mod_file_b ), bdata( lg_buffer ) );
#endif /* DEBUG */

   if( NULL != lg_info_cb ) {
      lg_info_cb( lg_buffer );
   }

cleanup:
   bdestroy( mod_b );
   bdestroy( mod_file_b );
   lgc_unsilence();
   return;
}

void lg_error( const char* mod_in, const char* message, ... ) {
   va_list varg;
#ifdef DEBUG
   va_start( varg, message );
   lg_log(
      lg_handle_err, mod_in, LG_COLOR_RED, message, varg
   );
   va_end( varg );
#endif /* DEBUG */
}

void lg_warning( const char* mod_in, const char* message, ... ) {
   va_list varg;
#ifdef DEBUG
   va_start( varg, message );
   lg_log(
      lg_handle_err, mod_in, LG_COLOR_YELLOW, message, varg
   );
   va_end( varg );
#endif /* DEBUG */
}

/*
void scaffold_snprintf( bstring buffer, const char* message, ... ) {
   va_list varg;
   va_start( varg, message );
   scaffold_vsnprintf( buffer, message, varg );
   va_end( varg );
}
*/

void lg_snprintf( bstring buffer, const char* message, ... ) {
   va_list varg;
   va_start( varg, message );
   lg_vsnprintf( buffer, message, varg );
   va_end( varg );
}

void lg_vsnprintf( bstring buffer, const char* message, va_list varg ) {
   const char* chariter;
   bstring insert = NULL;
   int bstr_res;
   int i;
   void* p;

   for( chariter = message ; '\0' != *chariter ; chariter++ ) {
      if( '%' != *chariter ) {
         bstr_res = bconchar( buffer, *chariter );
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         continue;
      }

      switch( *++chariter ) {
      case 'c':
         i = va_arg( varg, int );
         bstr_res = bconchar( buffer, i );
         if( 0 != bstr_res ) {
            bstr_res = bcatcstr( buffer, "%c" );
         }
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;

      case 'd':
         i = va_arg( varg, int );
         insert = bformat( "%d", i );
#ifdef DEBUG
         assert( NULL != insert );
#endif /* DEBUG */
         bstr_res = bconcat( buffer, insert );
         bdestroy( insert );
         insert = NULL;
         if( 0 != bstr_res ) {
            bstr_res = bcatcstr( buffer, "%d" );
         }
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;

      case 's':
         insert = bfromcstr( va_arg( varg, char*) );
#ifdef DEBUG
         //assert( NULL != insert );
#endif /* DEBUG */
         bstr_res = bconcat( buffer, insert );
         bdestroy( insert );
         insert = NULL;
         if( 0 != bstr_res ) {
            bstr_res = bcatcstr( buffer, "%s" );
         }
#ifdef DEBUG
         assert( 0 == bstr_res );
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
#endif /* DEBUG */
         break;

      case 'b':
         insert = va_arg( varg, bstring );
         /* TODO: (NULL) if null. */
         bstr_res = bconcat( buffer, insert );
         insert = NULL;
         if( 0 != bstr_res ) {
            bstr_res = bcatcstr( buffer, "%b" );
         }
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;

      case 'p':
         p = va_arg( varg, void* );
         insert = bformat( "%p", p );
#ifdef DEBUG
         assert( NULL != insert );
#endif /* DEBUG */
         bstr_res = bconcat( buffer, insert );
         bdestroy( insert );
         insert = NULL;
         if( 0 != bstr_res ) {
            bstr_res = bcatcstr( buffer, "%p" );
         }
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;

      case '%':
         bstr_res = bconchar( buffer, '%' );
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;
      }
   }

/* cleanup: */
   bdestroy( insert );
   return;
}

void lg_colorize( bstring str, enum LG_COLOR color ) {
   bstring str_color = NULL;
   int color_i = (int)color;
   int bstr_ret;

   if( color_i > 6 ) {
      color_i -= 9;
   }
   if( 0 > color_i || 7 <= color_i ) {
      goto cleanup;
   }
   str_color = &(lg_ansi_color_strs[color_i]);

   bstr_ret = binsert( str, 0, str_color, '\0' );
   if( 0 != bstr_ret ) {
      goto cleanup;
   }
   bstr_ret = bconcat( str, &(lg_ansi_color_strs[LG_COLOR_WHITE - 9]) );
   if( 0 != bstr_ret ) {
      goto cleanup;
   }

cleanup:
   return;
}
