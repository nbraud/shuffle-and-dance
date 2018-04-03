//#define USE_PAPI
#include "geiger/geiger.h"

#include "rand.h"
#include "shuffle.h"

#ifndef SIZE
# define SIZE 100000
#endif

int main(void) {
  geiger::init();
  //  geiger::suite<geiger::instr_profiler> bench;
  geiger::suite<> bench;

  int t[SIZE];
  struct rand_t *r = rand_new();

  bench.add("standard", [&r, &t]() {
      struct rand_t *s = rand_dup(r);
      shuffle_std(s, t, SIZE);
      free(s);
    });

  bench.add("precalc", [&r, &t]() {
      struct rand_t *s = rand_dup(r);
      size_t *u = shuffle_gen(s, SIZE);
      shuffle_ind(t, u, SIZE);
      free(s);
    });

  bench.add("batched", [&r, &t]() {
      struct rand_t *s = rand_dup(r);
      shuffle_batch(s, t, SIZE);
      free(s);
    });

  bench.set_printer<geiger::printer::console<>>();

  // Run each test during one second
  bench.run();

  return 0;
}
