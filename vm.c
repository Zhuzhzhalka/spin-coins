#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "vm_impl.c"

int main()
{
	/* for testing */
	const instr_t prog[15] = {
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
		// { .action = { .choice = { .left = { 1, 0, 0 },
		// 			  .right = { 0, 0, 0 } } } },
		// { .action = { .choice = { .left = { 0, 0, 0 },
		// 			  .right = { 0, 0, 0, 0, 0 } } } },
		// { .action = { .choice = { .left = { 0, 0, 0 },
		// 			  .right = { 0, 0, 0 } } } },
		// { .action = { .choice = { .left = { 0, 0, 0 },
		// 			  .right = { 0, 0, 0 } } } },
		// { .action = { .choice = { .left = { 0, 0, 0, 0, 0 },
		// 			  .right = { 0, 0, 0, 0, 0 } } } },
		// { .action = { .choice = { .left = { 0, 0, 0, 0, 0, 0 },
		// 			  .right = { 0, 0, 0, 0, 0, 0 } } } },
		// { .action = { .choice = { .left = { 0, 0, 0, 0, 0, 0 },
		// 			  .right = { 0, 0, 0, 0, 0, 0 } } } },
		{ .action = { .pick_fake = ONLY_UNKNOWN } },
		{ .action = { .pick_fake = NOT_EQ_FROM_PREV_WEIGH } },
		{ .action = { .pick_fake = EQ_FROM_PREV_WEIGH } },
		{ .action = { .pick_fake = LIGHT_OR_HEAVY_OF_PREV_WEIGH } },
		{ .action = { .pick_fake = ON_LIGHT_OF_PREV_PREV_WEIGH } },
		{ .action = { .pick_fake = ON_LIGHT_OF_PREV_PREV_WEIGH } },
		{ .action = { .pick_fake = ON_LIGHT_OF_PREV_WEIGH } },
		{ .action = { .pick_fake = ON_HEAVY } },
	};
	int res;
	int ok = 0;

	for (int i = 0; i < 26; i++) {
		int fake_idx = i % 13;
		int fake_weight = (i % 2) * 2 - 1;

		for (int j = 0; j < sizeof(unknowns) / sizeof(unknowns[0]); j++)
			unknowns[j] = UNKNOWN;
		for (int j = 0; j < sizeof(coins) / sizeof(coins[0]); j++)
			coins[j] = OK;

		coins[fake_idx] = fake_weight;

		printf("%s\n", sep);
		printf("Fake index: %d, fake weight %d\n\n", fake_idx,
		       fake_weight);

		res = c13_run(prog);
		printf("FAKE is %d\n\n", res);
		if (res == fake_idx)
			ok++;
	}
	printf("\nOK: %d, F: %d, OK/ALL %lf\n", ok, 26 - ok, (double)ok / 26);

	return 0;
}
