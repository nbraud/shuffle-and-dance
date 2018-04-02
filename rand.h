#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

struct rand_t;

struct rand_t *rand_new(void);
struct rand_t *rand_dup(struct rand_t *r);

uint64_t rand_int(struct rand_t *r, uint64_t n);

#ifdef __cplusplus
}
#endif
