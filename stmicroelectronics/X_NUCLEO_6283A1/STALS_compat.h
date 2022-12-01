#ifndef __STALS_COMPAT__
#define __STALS_COMPAT__ 1

/* compiler is suppose to support c99 */
/*Kernel / user code compatibility macro */
#ifdef __KERNEL__
#define POPCOUNT(a)				hweight_long(a)
#define CTZ(a)					__builtin_ctz(a)
#define assert(c)				BUG_ON(!(c))
#define UNUSED_P(x)				x __attribute__((unused))
#else /* !__KERNEL__ */
#ifdef __GNUC__
#define POPCOUNT(a)				__builtin_popcount(a)
#define CTZ(a)					__builtin_ctz(a)
#define UNUSED_P(x)				x __attribute__((unused))
#else /* !__GNUC__ */
#define POPCOUNT(a)				popcount(a)
#define CTZ(a)					ctz(a)
#define UNUSED_P(x)				(x)
#endif /* __GNUC__ */
#define div64_u64(a, b)				((a) / (b))
#endif /* __KERNEL__ */

#ifndef __GNUC__
static inline int popcount(unsigned int x)
{
	int res = 0;

	while (x > 0) {
		res += x & 1;
		x >>= 1;
	}

	return res;
}

static inline int ctz(unsigned int x)
{
	int res = 0;

	if (!x)
		return 0;

	while (!(x & 1)) {
		res++;
		x >>= 1;
	}

	return res;
}
#endif

#endif
