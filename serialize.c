#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "serialize_impl.c"

static const instr_t prog[15] = {
	{ .action = { .choice = { .left = { 4, 0, 0 },
				  .right = { 4, 0, 0 } } } },
	{ .action = { .choice = { .left = { 3, 0, 0 },
				  .right = { 0, 0, 0, 0, 3 } } } },
	{ .action = { .choice = { .left = { 0, 1, 2 },
				  .right = { 0, 1, 2 } } } },
	{ .action = { .choice = { .left = { 1, 0, 0 },
				  .right = { 0, 1, 0 } } } },
	{ .action = { .choice = { .left = { 0, 0, 0, 1, 0 },
				  .right = { 0, 0, 0, 1, 0 } } } },
	{ .action = { .choice = { .left = { 0, 1, 0, 0, 0, -2 },
				  .right = { 0, 1, 0, 0, 0, -2 } } } },
	{ .action = { .choice = { .left = { 0, 0, 1, 0, 0, -1 },
				  .right = { 0, 0, 1, 0, 0, -1 } } } },
	{ .action = { .pick_fake = { ONLY_UNKNOWN } } },
	{ .action = { .pick_fake = { NOT_EQ_FROM_PREV_WEIGH } } },
	{ .action = { .pick_fake = { EQ_FROM_PREV_WEIGH } } },
	{ .action = { .pick_fake = { LIGHT_OR_HEAVY_OF_PREV_WEIGH } } },
	{ .action = { .pick_fake = { ON_LIGHT_OF_PREV_PREV_WEIGH } } },
	{ .action = { .pick_fake = { ON_LIGHT_OF_PREV_PREV_WEIGH } } },
	{ .action = { .pick_fake = { ON_LIGHT_OF_PREV_WEIGH } } },
	{ .action = { .pick_fake = { ON_HEAVY } } },
};

int main(void)
{
	print_prog(prog);

	__uint128_t out = serialize(prog);

	const uint64_t p[2] = { out >> 64, out };
	const int p2[4] = { out >> 96, out >> 64, out >> 32, out };

	printf("Out: %016lx|%016lx\n", p[0], p[1]);
	printf("Out: %u, %u, %u, %u\n", p2[0], p2[1], p2[2], p2[3]);
	instr_t new_prog[15] = {};

	deserialize(new_prog, out);
	printf("Deserialized:\n");
	print_prog(new_prog);
}
