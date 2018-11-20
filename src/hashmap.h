/*
 * Originally by Elliot C Back - http://elliottback.com/wp/hashmap-implementation-in-c/
 *
 * Modified by Pete Warden to fix a serious performance problem, support strings as keys
 * and removed thread synchronization - http://petewarden.typepad.com
 */

/**
 * @file
 * @brief Generic hashmap manipulation functions
 */

#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include "bstrlib/bstrlib.h"
#include "vector.h"

struct HASHMAP;

typedef void* (*hashmap_iter_cb)( const bstring idx, void* iter, void* arg );
typedef VBOOL (*hashmap_rem_cb)( const bstring idx, void* iter, void* arg );

enum HASHMAP_ERROR {
   HASHMAP_ERROR_NONE,
   HASHMAP_ERROR_FULL = -1,
   HASHMAP_ERROR_ITEM_EXISTS = -2
};

struct HASHMAP* hashmap_new();
void hashmap_init( struct HASHMAP* m );
void* hashmap_iterate( struct HASHMAP* m, hashmap_iter_cb callback, void* arg );
struct VECTOR* hashmap_iterate_v( struct HASHMAP* m, hashmap_iter_cb callback, void* arg );
size_t hashmap_remove_cb(
   struct HASHMAP* m, hashmap_rem_cb callback, void* arg );
enum HASHMAP_ERROR hashmap_put(
   struct HASHMAP* m, const bstring key, void* value, VBOOL overwrite )
#ifdef USE_GNUC_EXTENSIONS
__attribute__ ((warn_unused_result))
#endif /* USE_GNUC_EXTENSIONS */
;
void* hashmap_get( struct HASHMAP* m, const bstring key );
VBOOL hashmap_contains_key( struct HASHMAP* m, const bstring key );
VBOOL hashmap_remove( struct HASHMAP* m, const bstring key );
size_t hashmap_remove_all( struct HASHMAP* m );
void hashmap_cleanup( struct HASHMAP* m );
void hashmap_free( struct HASHMAP** m );
size_t hashmap_count( const struct HASHMAP* m );
void hashmap_lock( struct HASHMAP* m, VBOOL lock );
VBOOL hashmap_is_valid( const struct HASHMAP* m );
void* hashmap_get_first( struct HASHMAP* m );
VBOOL hashmap_is_locked( const struct HASHMAP* m );

#endif /* __HASHMAP_H__ */
