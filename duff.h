#if !defined(UNROLL) || UNROLL != 32
# error "duff.h was generated for an unrolling factor of 32"
#endif

#define DUFF32(N, init, code, end)       \
  do {                                   \
    size_t __duff_n = ((N) + 31u) / 32u; \
    init;                                \
    switch ((N) % 32u) {                 \
      case 0: do { code;   \
      case 31:     code;   \
      case 30:     code;   \
      case 29:     code;   \
      case 28:     code;   \
      case 27:     code;   \
      case 26:     code;   \
      case 25:     code;   \
      case 24:     code;   \
      case 23:     code;   \
      case 22:     code;   \
      case 21:     code;   \
      case 20:     code;   \
      case 19:     code;   \
      case 18:     code;   \
      case 17:     code;   \
      case 16:     code;   \
      case 15:     code;   \
      case 14:     code;   \
      case 13:     code;   \
      case 12:     code;   \
      case 11:     code;   \
      case 10:     code;   \
      case 9:      code;   \
      case 8:      code;   \
      case 7:      code;   \
      case 6:      code;   \
      case 5:      code;   \
      case 4:      code;   \
      case 3:      code;   \
      case 2:      code;   \
      case 1:      code;   \
      } while (--__duff_n); \
      end;                  \
    }} while(0)
