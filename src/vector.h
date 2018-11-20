#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stdlib.h>

#include "mem.h"
#include "vbool.h"

struct VECTOR;

typedef enum _VECTOR_SORT_ORDER {
   VECTOR_SORT_A_LIGHTER = -1,
   VECTOR_SORT_A_B_EQUAL = 0,
   VECTOR_SORT_A_HEAVIER = 1
} VECTOR_SORT_ORDER;

enum VECTOR_SENTINAL {
   VECTOR_SENTINAL_NONE = 0,
   VECTOR_SENTINAL_V1 = 1212
};

#define VECTOR_SIZE_MAX 10000

#define VECTOR_ERR_FULL -1

#define vector_push( v, item ) \
   vector_insert( v, 0, item )

#define vector_enqueue( v, item ) \
   vector_add( v, item )

#define vector_dequeue( v ) \
   vector_pop( v )

typedef void* (*vector_iter_cb)( size_t idx, void* iter, void* arg );
typedef void* (*vector_iter_x_cb)( size_t idx, const void* iter, void* arg );
typedef VBOOL (*vector_rem_cb)( size_t idx, void* iter, void* arg );
typedef VECTOR_SORT_ORDER (*vector_sorter_cb)( void* a, void* b );

struct VECTOR* vector_new();
void vector_init( struct VECTOR* v );
void vector_cleanup_force( struct VECTOR* v );
void vector_cleanup( struct VECTOR* v );
void vector_free_force( struct VECTOR** v );
void vector_free( struct VECTOR** v );
void* vector_pop( struct VECTOR* v );
size_t vector_insert( struct VECTOR* v, size_t index, void* data )
#ifdef USE_GNUC_EXTENSIONS
__attribute__ ((warn_unused_result))
#endif /* USE_GNUC_EXTENSIONS */
;
size_t vector_add( struct VECTOR* v, void* data )
#ifdef USE_GNUC_EXTENSIONS
__attribute__ ((warn_unused_result))
#endif /* USE_GNUC_EXTENSIONS */
;

#ifdef USE_VECTOR_SCALAR
void vector_add_scalar( struct VECTOR* v, int32_t value, VBOOL allow_dupe );
void vector_set_scalar( struct VECTOR* v, size_t index, int32_t value );
int32_t vector_get_scalar( const struct VECTOR* v, size_t index );
int32_t vector_get_scalar_value( const struct VECTOR* v, int32_t value );
void vector_remove_scalar( struct VECTOR* v, size_t index );
size_t vector_remove_scalar_value( struct VECTOR* v, int32_t value );
#endif /* USE_VECTOR_SCALAR */

size_t vector_set( struct VECTOR* v, size_t index, void* data, VBOOL force );
void* vector_get( const struct VECTOR* v, size_t index );
size_t vector_remove_cb( struct VECTOR* v, vector_rem_cb callback, void* arg );
void vector_remove( struct VECTOR* v, size_t index );
size_t vector_remove_all( struct VECTOR* v );
size_t vector_count( const struct VECTOR* v );
void vector_lock( struct VECTOR* v, VBOOL lock );
void* vector_iterate( struct VECTOR* v, vector_iter_cb callback, void* arg );
void* vector_iterate_x(
   const struct VECTOR* v, vector_iter_x_cb callback, void* arg );
void* vector_iterate_i(
   struct VECTOR* v, vector_iter_cb callback, void* arg, size_t i
);
void* vector_iterate_r( struct VECTOR* v, vector_iter_cb callback, void* arg );
struct VECTOR* vector_iterate_v(
   struct VECTOR* v, vector_iter_cb callback, void* arg
);
void vector_sort_cb( struct VECTOR* v, vector_sorter_cb );
VBOOL vector_is_valid( const struct VECTOR* v );
VBOOL vector_is_locked( const struct VECTOR* v );

#endif /* VECTOR_H */
