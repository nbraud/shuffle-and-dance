#include "rand.h"

#include <assert.h>
#include <string.h>
#include <sodium.h>

/* I needed some quick (CS)PRNG that is deterministic (after
 * initialization), copyable, and has a simple API.
 *
 * This is NOT what you want out of a PRNG used in a security-sensitive
 * context, so don't use it for that (or anything else, for that matter).
 */

struct rand_t {
  unsigned char k[randombytes_SEEDBYTES];
};

struct rand_t *rand_new(void) {
  struct rand_t *r = malloc(sizeof(struct rand_t));
  randombytes_buf(r->k, randombytes_SEEDBYTES);
  return r;
}

struct rand_t *rand_rand(struct rand_t *r) {
  struct rand_t *s = malloc(sizeof(struct rand_t));

  char k[2 * randombytes_SEEDBYTES];
  randombytes_buf_deterministic((unsigned char*)&k, sizeof(k), r->k);

  memcpy(r->k, k, randombytes_SEEDBYTES);
  memcpy(s->k, k + randombytes_SEEDBYTES, randombytes_SEEDBYTES);
  return s;
}

struct rand_t *rand_dup(struct rand_t *r) {
  struct rand_t *s = malloc(sizeof(struct rand_t));
  memcpy(s->k, r->k, randombytes_SEEDBYTES);
  return s;
}

/* Return a uniformly-random 64bit integer */
uint64_t rand_int64(struct rand_t *r) {
  uint64_t k[randombytes_SEEDBYTES/sizeof(uint64_t) +1];
  randombytes_buf_deterministic((unsigned char*)&k, sizeof(k), r->k);
  memcpy(r->k, k+1, randombytes_SEEDBYTES);
  return k[0];
}


/* Compute the smallest 2^n -1 larger than v */
#if defined(__clang__)
# if __has_builtin(__builtin_clz)
#  define HAS_CLZ
# endif
#elif defined(__GNUC__)
# define HAS_CLZ
#endif

uint64_t bitmask(uint64_t v) {
#ifndef HAS_CLZ
  /* Adapted from Sean Anderson's Bit Twiddling Hacks collection:
   * https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
   */
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;

  return v;
#else
  return (1 << (64 - __builtin_clz(v))) -1;
#endif
}


/* Return a uniformly random k s.t. 0 <= k < n */
uint64_t rand_int(struct rand_t *r, uint64_t n) {
  assert(n > 0);
  uint64_t k, mask = bitmask(n);

  do {
    k = rand_int64(r) & mask;
  } while(k >= n);

  return k;
}
