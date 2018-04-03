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

  struct rand_t *r_s = rand_dup(r);
  bench.add("standard", [&r_s, &t]() {
      struct rand_t *s = rand_rand(r_s);
      shuffle_std(s, t, SIZE);
      free(s);
    });

  struct rand_t *r_pc = rand_dup(r);
  bench.add("precalc", [&r_pc, &t]() {
      struct rand_t *s = rand_rand(r_pc);
      size_t *u = shuffle_gen(s, SIZE);
      shuffle_ind(t, u, SIZE);
      free(s);
    });

  struct rand_t *r_b = rand_dup(r);
  bench.add("batched", [&r_b, &t]() {
      struct rand_t *s = rand_rand(r_b);
      shuffle_batch(s, t, SIZE);
      free(s);
    });

  struct rand_t *r_pf = rand_dup(r);
  bench.add("prefetched", [&r_pf, &t]() {
      struct rand_t *s = rand_rand(r_pf);
      shuffle_prefetch(s, t, SIZE);
      free(s);
    });

  bench.set_printer<geiger::printer::console<>>();

  // Run each test during one second
  bench.run();

  return 0;
}
