#include "vm.h"

typedef unsigned long uint64_t;

#define BOWL_FMT "%d, %d, %d, %d, %d, %d"
#define BOWL_ARG(bowl)                                                   \
	(bowl).n_unknown, (bowl).n_light, (bowl).n_heavy, (bowl).n_left, \
		(bowl).n_right, (bowl).n_ref

static void print_choice(choice_t ch)
{
	/*printf("    { .action = { .choice = { .left  = {" BOWL_FMT "},\n"
	       "                    .right = {" BOWL_FMT "}},\n",
	       BOWL_ARG(ch.left), BOWL_ARG(ch.right));*/
}

const char *pick_fake_names[] = {
	[ONLY_UNKNOWN] = "ONLY_UNKNOWN",
	[NOT_EQ_FROM_PREV_WEIGH] = "NOT_EQ_FROM_PREV_WEIGH",
	[EQ_FROM_PREV_WEIGH] = "EQ_FROM_PREV_WEIGH",
	[LIGHT_OR_HEAVY_OF_PREV_WEIGH] = "LIGHT_OR_HEAVY_OF_PREV_WEIGH",
	[ON_LIGHT_OF_PREV_PREV_WEIGH] = "ON_LIGHT_OF_PREV_PREV_WEIGH",
	[ON_LIGHT_OF_PREV_WEIGH] = "ON_LIGHT_OF_PREV_WEIGH",
	[ON_HEAVY] = "ON_HEAVY",
};

static void print_pick_fake(pick_fake_t pf)
{
	/*printf("    { .action = { .pick_fake = {%s} } },\n",
	       pick_fake_names[pf.fake]);*/
}

static void print_prog(const instr_t prog[static 15])
{
	//printf("static const instr_t prog[15] = {\n");

	ssize_t i;

	for (i = 0; i < 7; i++) {
		print_choice(prog[i].action.choice);
	}

	for (; i < 15; i++) {
		print_pick_fake(prog[i].action.pick_fake);
	}
	//printf("};\n\n");
}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static const choice_bowl_t bowl_lut_0[125] = {
	{ 4, 0, 0, 0, 0 }, { 0, 0, 0, 0, 4 }, { 0, 4, 0, 0, 0 },
	{ 0, 0, 4, 0, 0 }, { 0, 0, 0, 4, 0 }, { 0, 3, 1, 0, 0 },
	{ 0, 3, 0, 1, 0 }, { 0, 3, 0, 0, 1 }, { 0, 1, 3, 0, 0 },
	{ 0, 1, 0, 3, 0 }, { 0, 1, 0, 0, 3 }, { 0, 0, 3, 1, 0 },
	{ 0, 0, 3, 0, 1 }, { 0, 0, 1, 3, 0 }, { 0, 0, 1, 0, 3 },
	{ 0, 0, 0, 3, 1 }, { 0, 0, 0, 1, 3 }, { 3, 1, 0, 0, 0 },
	{ 3, 0, 1, 0, 0 }, { 3, 0, 0, 1, 0 }, { 3, 0, 0, 0, 1 },
	{ 1, 3, 0, 0, 0 }, { 1, 0, 3, 0, 0 }, { 1, 0, 0, 3, 0 },
	{ 1, 0, 0, 0, 3 }, { 0, 3, 0, 0, 0 }, { 0, 0, 3, 0, 0 },
	{ 0, 0, 0, 3, 0 }, { 0, 0, 0, 0, 3 }, { 3, 0, 0, 0, 0 },
	{ 0, 2, 2, 0, 0 }, { 0, 2, 0, 2, 0 }, { 0, 2, 0, 0, 2 },
	{ 0, 0, 2, 2, 0 }, { 0, 0, 2, 0, 2 }, { 0, 0, 0, 2, 2 },
	{ 2, 2, 0, 0, 0 }, { 2, 0, 2, 0, 0 }, { 2, 0, 0, 2, 0 },
	{ 2, 0, 0, 0, 2 }, { 0, 2, 1, 1, 0 }, { 0, 2, 1, 0, 1 },
	{ 0, 2, 0, 1, 1 }, { 0, 1, 2, 1, 0 }, { 0, 1, 2, 0, 1 },
	{ 0, 1, 1, 2, 0 }, { 0, 1, 1, 0, 2 }, { 0, 1, 0, 2, 1 },
	{ 0, 1, 0, 1, 2 }, { 0, 0, 2, 1, 1 }, { 0, 0, 1, 2, 1 },
	{ 0, 0, 1, 1, 2 }, { 2, 1, 1, 0, 0 }, { 2, 1, 0, 1, 0 },
	{ 2, 1, 0, 0, 1 }, { 2, 0, 1, 1, 0 }, { 2, 0, 1, 0, 1 },
	{ 2, 0, 0, 1, 1 }, { 1, 2, 1, 0, 0 }, { 1, 2, 0, 1, 0 },
	{ 1, 2, 0, 0, 1 }, { 1, 1, 2, 0, 0 }, { 1, 1, 0, 2, 0 },
	{ 1, 1, 0, 0, 2 }, { 1, 0, 2, 1, 0 }, { 1, 0, 2, 0, 1 },
	{ 1, 0, 1, 2, 0 }, { 1, 0, 1, 0, 2 }, { 1, 0, 0, 2, 1 },
	{ 1, 0, 0, 1, 2 }, { 0, 2, 1, 0, 0 }, { 0, 2, 0, 1, 0 },
	{ 0, 2, 0, 0, 1 }, { 0, 1, 2, 0, 0 }, { 0, 1, 0, 2, 0 },
	{ 0, 1, 0, 0, 2 }, { 0, 0, 2, 1, 0 }, { 0, 0, 2, 0, 1 },
	{ 0, 0, 1, 2, 0 }, { 0, 0, 1, 0, 2 }, { 0, 0, 0, 2, 1 },
	{ 0, 0, 0, 1, 2 }, { 2, 1, 0, 0, 0 }, { 2, 0, 1, 0, 0 },
	{ 2, 0, 0, 1, 0 }, { 2, 0, 0, 0, 1 }, { 1, 2, 0, 0, 0 },
	{ 1, 0, 2, 0, 0 }, { 1, 0, 0, 2, 0 }, { 1, 0, 0, 0, 2 },
	{ 0, 2, 0, 0, 0 }, { 0, 0, 2, 0, 0 }, { 0, 0, 0, 2, 0 },
	{ 0, 0, 0, 0, 2 }, { 2, 0, 0, 0, 0 }, { 0, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 0 }, { 1, 1, 1, 0, 1 }, { 1, 1, 0, 1, 1 },
	{ 1, 0, 1, 1, 1 }, { 0, 1, 1, 1, 0 }, { 0, 1, 1, 0, 1 },
	{ 0, 1, 0, 1, 1 }, { 0, 0, 1, 1, 1 }, { 1, 1, 1, 0, 0 },
	{ 1, 1, 0, 1, 0 }, { 1, 1, 0, 0, 1 }, { 1, 0, 1, 1, 0 },
	{ 1, 0, 1, 0, 1 }, { 1, 0, 0, 1, 1 }, { 0, 1, 1, 0, 0 },
	{ 0, 1, 0, 1, 0 }, { 0, 1, 0, 0, 1 }, { 0, 0, 1, 1, 0 },
	{ 0, 0, 1, 0, 1 }, { 0, 0, 0, 1, 1 }, { 1, 1, 0, 0, 0 },
	{ 1, 0, 1, 0, 0 }, { 1, 0, 0, 1, 0 }, { 1, 0, 0, 0, 1 },
	{ 0, 1, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 1 }, { 1, 0, 0, 0, 0 },
};

static const choice_bowl_t bowl_lut_1[55] = {
	{ 3, 0, 0, 0, 0 }, { 0, 0, 0, 0, 3 }, { 0, 1, 2, 0, 0 },
	{ 0, 3, 0, 0, 0 }, { 0, 0, 3, 0, 0 }, { 0, 0, 0, 3, 0 },
	{ 0, 2, 1, 0, 0 }, { 0, 2, 0, 1, 0 }, { 0, 2, 0, 0, 1 },
	{ 0, 1, 0, 2, 0 }, { 0, 1, 0, 0, 2 }, { 0, 0, 2, 1, 0 },
	{ 0, 0, 2, 0, 1 }, { 0, 0, 1, 2, 0 }, { 0, 0, 1, 0, 2 },
	{ 0, 0, 0, 2, 1 }, { 0, 0, 0, 1, 2 }, { 2, 1, 0, 0, 0 },
	{ 2, 0, 1, 0, 0 }, { 2, 0, 0, 1, 0 }, { 2, 0, 0, 0, 1 },
	{ 1, 2, 0, 0, 0 }, { 1, 0, 2, 0, 0 }, { 1, 0, 0, 2, 0 },
	{ 1, 0, 0, 0, 2 }, { 0, 2, 0, 0, 0 }, { 0, 0, 2, 0, 0 },
	{ 0, 0, 0, 2, 0 }, { 0, 0, 0, 0, 2 }, { 2, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 0 }, { 0, 1, 1, 0, 1 }, { 0, 1, 0, 1, 1 },
	{ 0, 0, 1, 1, 1 }, { 1, 1, 1, 0, 0 }, { 1, 1, 0, 1, 0 },
	{ 1, 1, 0, 0, 1 }, { 1, 0, 1, 1, 0 }, { 1, 0, 1, 0, 1 },
	{ 1, 0, 0, 1, 1 }, { 0, 1, 1, 0, 0 }, { 0, 1, 0, 1, 0 },
	{ 0, 1, 0, 0, 1 }, { 0, 0, 1, 1, 0 }, { 0, 0, 1, 0, 1 },
	{ 0, 0, 0, 1, 1 }, { 1, 1, 0, 0, 0 }, { 1, 0, 1, 0, 0 },
	{ 1, 0, 0, 1, 0 }, { 1, 0, 0, 0, 1 }, { 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0 },
};

static const choice_bowl_t bowl_lut_2[5] = {
	{ 1, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0 }, { 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 1 }, { 0, 0, 1, 0, 0 },
};

typedef struct {
	size_t lut_size;
	const choice_bowl_t *lut;
} bowl_lookup_t;

static bowl_lookup_t lookups[] = {
	{ ARRAY_SIZE(bowl_lut_0), bowl_lut_0 },
	{ ARRAY_SIZE(bowl_lut_1), bowl_lut_1 },
	{ ARRAY_SIZE(bowl_lut_2), bowl_lut_2 },
};

#define get_lookup(i) lookups[((i) > 0) + ((i) > 2)]

static uint64_t pack_choice_bowl(choice_bowl_t b, bowl_lookup_t lookup)
{
	for (size_t i = 0; i < lookup.lut_size; i++) {
		choice_bowl_t l = lookup.lut[i];
		if (b.n_unknown == l.n_unknown && b.n_light == l.n_light &&
		    b.n_heavy == l.n_heavy && b.n_left == l.n_left &&
		    b.n_right == l.n_right) {
			uint64_t result = i;
			my_assert((result & 0x7FLLU) == result, "F");
			result *= 4;
			switch (b.n_ref) {
			case -2:
				result |= 2;
				break;
			case -1:
				result |= 1;
				break;
			case 0:
				result |= 0;
				break;
			default:
				my_assert(0, "Unreachable");
				break;
			}

			/*printf("BOWL: Packing %zd {" BOWL_FMT "} -> %lu\n", i,
			       BOWL_ARG(b), result);*/

			return result;
		}
	}
	my_assert(0, "Unreachable");
	return 0;
}

static uint64_t pack_choice(choice_t ch, ssize_t i)
{
	uint64_t l = pack_choice_bowl(ch.left, get_lookup(i));
	uint64_t r = pack_choice_bowl(ch.right, get_lookup(i));
	return l * (get_lookup(i).lut_size * 4) + r;
}

static uint64_t pack_pick_fake(pick_fake_t pf)
{
	return pf.fake;
}

static choice_bowl_t unpack_choice_bowl(uint64_t serialized,
					bowl_lookup_t lookup)
{
	my_assert(serialized / 4 < lookup.lut_size, "F");
	choice_bowl_t result = lookup.lut[serialized / 4];

	switch (serialized % 4) {
	case 0:
		result.n_ref = 0;
		break;
	case 1:
		result.n_ref = -1;
		break;
	case 2:
		result.n_ref = -2;
		break;
	default:
		my_assert(0, "Deserialization failed: wrong n_ref");
		break;
	}

	return result;
}

static choice_t unpack_choice(uint64_t serialized, ssize_t i)
{
	uint64_t num = get_lookup(i).lut_size * 4;
	choice_bowl_t r = unpack_choice_bowl(serialized % num, get_lookup(i));
	serialized /= num;
	choice_bowl_t l = unpack_choice_bowl(serialized % num, get_lookup(i));
	return (choice_t){ .left = l, .right = r };
}

static pick_fake_t unpack_pick_fake(uint64_t serialized)
{
	return (pick_fake_t){ .fake = serialized % 8 };
}

static __uint128_t serialize(const instr_t prog[static 15])
{
	ssize_t i;

	__uint128_t out = 0;

	for (i = 0; i < 7; i++) {
		uint64_t num = get_lookup(i).lut_size * 4;
		uint64_t s = pack_choice(prog[i].action.choice, i);
		//printf("%zd: Packing 2x%lu: %lu\n", i, num, s);
		out *= num * num;
		out += s;
	}

	for (; i < 15; i++) {
		uint64_t s = pack_pick_fake(prog[i].action.pick_fake);
		//printf("%zd: Packing 8: %lu\n", i, s);
		out *= 8;
		out += s;
	}

	return out;
}

static void deserialize(instr_t prog[static 15], __uint128_t in)
{
	ssize_t i;

	for (i = 14; i >= 7; i--) {
		uint64_t s = in % 8;
		//printf("%zd: Unpacking 8: %lu\n", i, s);
		prog[i].action.pick_fake = unpack_pick_fake(s);
		in /= 8;
	}

	for (; i >= 0; i--) {
		uint64_t num = get_lookup(i).lut_size * 4;
		uint64_t s = in % (num * num);
		//printf("%zd: Unpacking 2x%lu: %lu\n", i, num, s);
		prog[i].action.choice = unpack_choice(s, i);
		in /= num * num;
	}
}
