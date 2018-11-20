
#ifndef GOKIBURI_H
#define GOKIBURI_H

#include <stdio.h>
#include "bstrlib/bstrlib.h"
#include "vbool.h"

typedef void (*lg_info_callback)(bstring);

enum LG_COLOR {
   LG_COLOR_DARK_BLUE   =  1,
   LG_COLOR_DARK_GREEN  =  2,
   LG_COLOR_DARK_CYAN   =  3,
   LG_COLOR_DARK_RED    =  4,
   LG_COLOR_PURPLE      =  5,
   LG_COLOR_BROWN       =  6,
   LG_COLOR_GRAY        =  7,
   LG_COLOR_CHARCOAL    =  8,
   LG_COLOR_BLUE        =  9,
   LG_COLOR_GREEN       = 10,
   LG_COLOR_CYAN        = 11,
   LG_COLOR_RED         = 12,
   LG_COLOR_MAGENTA     = 13,
   LG_COLOR_YELLOW      = 14,
   LG_COLOR_WHITE       = 15
};

enum LGC_ERROR {
   LGC_ERROR_NONE,
   LGC_ERROR_NULLPO,
   LGC_ERROR_NOT_NULLPO,
   LGC_ERROR_NONZERO,
   LGC_ERROR_OUTOFBOUNDS,
   LGC_ERROR_ZERO,
   LGC_ERROR_NEGATIVE,
   LGC_ERROR_UNEQUAL
};

#define lgc_silence() lgc_error_silent = VTRUE;
#define lgc_unsilence() lgc_error_silent = VFALSE;

#define lgc_false( condition, msg ) \
    if( !condition ) { \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, "False on line: %d: %s\n", __LINE__, msg ); \
        } \
        goto cleanup; \
    }

#define lgc_null_msg( pointer, message ) \
    if( NULL == pointer ) { \
        lgc_error = LGC_ERROR_NULLPO; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, \
               "Null pointer on line: %d: %s\n", \
               __LINE__, message ); \
        } \
        goto cleanup; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }

#define lgc_null_warning( pointer ) \
    if( NULL == pointer ) { \
        lgc_error = LGC_ERROR_NULLPO; \
        if( VTRUE != scaffold_warning_silent ) { \
            lg_warning( \
               __FILE__, "Null pointer on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }

#define lgc_null( pointer ) \
    if( NULL == pointer ) { \
        lgc_error = LGC_ERROR_NULLPO; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, "Null pointer on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }

#define lgc_null_continue( pointer ) \
    if( NULL == pointer ) { \
        lgc_error = LGC_ERROR_NULLPO; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, "Null pointer on line: %d\n", __LINE__ ); \
            lg_debug( __FILE__, "Continuing loop..." ); \
        } \
        continue; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }

#define lgc_not_null( pointer ) \
    if( NULL != pointer ) { \
        lgc_error = LGC_ERROR_NOT_NULLPO; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, \
               "Non-null pointer on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }

#define lgc_bounds( index, bound ) \
    if( index >= bound ) { \
        lgc_error = LGC_ERROR_OUTOFBOUNDS; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, "Out of bounds on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }

#define lgc_negative( value ) \
    if( 0 > value ) { \
        lgc_error = LGC_ERROR_NEGATIVE; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, "Bad negative on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }

#define lgc_nonzero( value ) \
    if( 0 != value ) { \
        lgc_error = LGC_ERROR_NONZERO; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, "Nonzero error on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }

#define lgc_zero_msg( value, message ) \
    if( 0 == value ) { \
        lgc_error = LGC_ERROR_ZERO; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, \
               "Zero error on line: %d: %s\n", __LINE__, message ); \
        } \
        goto cleanup; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }


#define lgc_zero_against_warning( last, value, msg ) \
    if( 0 == value && LGC_ERROR_ZERO != last ) { \
        last = LGC_ERROR_ZERO; \
        if( VTRUE != scaffold_warning_silent ) { \
            lg_warning( \
               __FILE__, \
               "Zero warning on line: %d: %s\n", __LINE__, msg ); \
        } \
        goto cleanup; \
    } else if( LGC_ERROR_ZERO != last ) { \
        last = LGC_ERROR_NONE; \
    }

#define lgc_zero_against( last, value, msg ) \
    if( 0 == value && LGC_ERROR_ZERO != last ) { \
        last = LGC_ERROR_ZERO; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__, \
               "Zero error on line: %d: %s\n", __LINE__, msg ); \
        } \
        goto cleanup; \
    } else if( LGC_ERROR_ZERO != last ) { \
        last = LGC_ERROR_NONE; \
    }

#define lgc_equal( value1, value2 ) \
    if( value1 != value2 ) { \
        lgc_error = LGC_ERROR_UNEQUAL; \
        if( VTRUE != lgc_error_silent ) { \
            lg_error( \
               __FILE__,\
               "Values not equal: %d and %d: error on line: %d\n", \
               value1, value2, __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        lgc_error = LGC_ERROR_NONE; \
    }

#define lgc_zero( value, msg ) \
   lgc_zero_against( lgc_error, value, msg )

void lg_set_info_cb( lg_info_callback cb );
int lg_add_trace_cat( const char* name, enum LG_COLOR color );
int lg_set_trace_cat( const char* name );
const char* lg_get_trace_cat_name();
bstring lg_trim_filename( bstring path );
void lg_debug( const char* mod_in, const char* message, ... );
void lg_color(
   const char* mod_in, enum LG_COLOR color, const char* message, ...
);
void lg_info( const char* mod_in, const char* message, ... );
void lg_error( const char* mod_in, const char* message, ... );
void lg_warning( const char* mod_in, const char* message, ... );
void lg_snprintf( bstring buffer, const char* message, ... );
void lg_vsnprintf( bstring buffer, const char* message, va_list varg );
void lg_colorize( bstring str, enum LG_COLOR color );

#ifdef GOKIBURI_C
enum LGC_ERROR lgc_error = LGC_ERROR_NONE;
enum VBOOL lgc_error_silent = VFALSE;
#else
extern enum LGC_ERROR lgc_error;
extern enum VBOOL lgc_error_silent;
#endif /* GOKIBURI_C */

#ifdef USE_LOG_FILE
#ifndef GOKIBURI_C
extern FILE* lg_handle_out;
extern FILE* lg_handle_err;
#endif /* !GOKIBURI_C */
#else
#define lg_handle_out stdout
#define lg_handle_err stderr
#endif /* USE_LOG_FILE */

#endif /* GOKIBURI_H */
