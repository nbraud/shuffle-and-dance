/* _GNU_SOURCE required (under glibc) to get reallocarray... */
#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>

#include "rand.h"


// Bog-standard Fisher-Yates shuffle
void shuffle_std(struct rand_t *r, int t[], size_t n) {
  for(int i = 0; i < n; ++i) {
    size_t j = rand_int(r, i+1);
    int x = t[i];
    t[i] = t[j];
    t[j] = x;
  }
}

// Fisher-Yates shuffle with pre-generated indexes
void shuffle_ind(int t[], size_t u[], size_t n) {
  for(int i = 0; i < n; ++i) {
    size_t j = u[i];
    int x = t[i];
    t[i] = t[j];
    t[j] = x;
  }
}

// Index table generation
size_t *shuffle_gen(struct rand_t *r, size_t n) {
  size_t* t = reallocarray(NULL, n, sizeof(size_t));
  for(int i = 0; i < n; ++i) {
    t[i] = rand_int(r, i+1);
  }

  return t;
}


// Fisher-Yates shuffle with larger interleaved batches
#define UNROLL 32
#include "duff.h"
void shuffle_batch(struct rand_t *r, int t[], size_t n) {
  size_t i = 0;
  size_t u[UNROLL];
  size_t j;
  DUFF32(n, {
      j = 0;
      for(size_t k = 0; k<UNROLL; ++k) {
        u[k] = rand_int(r, UNROLL*i + k);
      }
    }, {
      size_t k = u[j];
      int x = t[UNROLL*i + j];
      t[UNROLL*i + j] = t[k];
      t[k] = x;
      j++;
    }, { i++; }
  );
}

