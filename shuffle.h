#ifdef __cplusplus
extern "C"
{
#endif

// Bog-standard Fisher-Yates shuffle
void shuffle_std(struct rand_t *r, int t[], size_t n);

// Fisher-Yates shuffle with pre-generated indexes
void shuffle_ind(int t[], size_t u[], size_t n);
// Index table generation
size_t *shuffle_gen(struct rand_t *r, size_t n);

// Fisher-Yates shuffle with larger interleaved batches
void shuffle_batch(struct rand_t *r, int t[], size_t n);

#ifdef __cplusplus
}
#endif
