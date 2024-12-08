#ifndef __VM_H__
#define __VM_H__

#define FAKE_POS 1
#define FAKE_NEG -1
#define OK 0
#define UNKNOWN -2
#define KNOWN -1
#define EQUAL 0
#define LIGHT 1
#define HEAVY 2

const char *sep =
	"--------------------------------------------------------------";

typedef struct {
	int n_unknown;
	int n_light;
	int n_heavy;
	int n_left;
	int n_right;
	int n_ref; // prev 0, pprev -2
} choice_bowl_t;

typedef struct {
	choice_bowl_t left;
	choice_bowl_t right;
} choice_t;

typedef enum {
	ONLY_UNKNOWN,
	NOT_EQ_FROM_PREV_WEIGH,
	EQ_FROM_PREV_WEIGH,
	LIGHT_OR_HEAVY_OF_PREV_WEIGH,
	ON_LIGHT_OF_PREV_PREV_WEIGH,
	ON_LIGHT_OF_PREV_WEIGH,
	ON_HEAVY,
} fake_t;

typedef struct {
	fake_t fake;
} pick_fake_t;

typedef struct {
	union {
		pick_fake_t pick_fake;
		choice_t choice;
	} action;
} instr_t;

// Тяжелая чаша -
typedef enum {
	RESULT_NONE,
	RESULT_EQ,
	RESULT_RIGHT,
	RESULT_LEFT,
} coin13_weigh_result_t;

typedef struct {
	int n_coins;
	int coins_l[6];
	int coins_r[6];
	coin13_weigh_result_t result;
	int first_available_light;
	int first_available_heavy;
	int first_available_left;
	int first_available_right;
} coin13_weigh_state_t;

static inline int __my_assert(int cond, const char *file, int line,
			      const char *format, ...)
{
	if (cond)
		return 0;

	va_list vargs;
	va_start(vargs, format);
	//fprintf(stderr, "Assertion failed. %s: %d: ", file, line);
	//vfprintf(stderr, format, vargs);
	va_end(vargs);
	return -1;
}

#define my_assert(c, fmt, ...) \
	__my_assert(c, __FILE__, __LINE__, fmt __VA_OPT__(, ) __VA_ARGS__)

#endif
