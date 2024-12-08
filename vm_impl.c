#include "vm.h"

static int coins[13];
static int unknowns[13];

static int weigh(coin13_weigh_state_t *state)
{
	int left_sum = 0, right_sum = 0;
	int n = state->n_coins;

	if (!n)
		return -1;

	for (int i = 0; i < n; i++) {
		left_sum += coins[state->coins_l[i]];
		right_sum += coins[state->coins_r[i]];
	}

	if (left_sum == right_sum)
		state->result = RESULT_EQ;
	else if (left_sum > right_sum)
		state->result = RESULT_LEFT;
	else
		state->result = RESULT_RIGHT;

	/*printf(" left  side: ");
	for (int i = 0; i < n; i++)
		printf("%d ", state->coins_l[i]);

	printf("\n right side: ");
	for (int i = 0; i < n; i++)
		printf("%d ", state->coins_r[i]);
	printf("\n");

	printf("Weighing coins: %s\n\n",
	       state->result == RESULT_EQ   ? "Equal" :
	       state->result == RESULT_LEFT ? "Left heavier" :
					      "Right heavier");*/

	return 0;
}

static int __choose_coins(int n_type, int base, int chosen[6],
			  coin13_weigh_result_t res,
			  coin13_weigh_state_t prev_state,
			  int *first_available_idx, int uncond)
{
	int j = 0;
	int *coins_p;
	int ret = 0;

	if (!n_type)
		return 0;

	if (uncond)
		coins_p = (res == RESULT_RIGHT) ? prev_state.coins_r :
						  prev_state.coins_l;
	else
		coins_p = (prev_state.result == res) ? prev_state.coins_r :
						       prev_state.coins_l;
	for (int i = 0; i < 6; i++) {
		if (j >= *first_available_idx)
			chosen[base + j - *first_available_idx] = coins_p[i];
		j++;
		if (j == *first_available_idx + n_type)
			break;
	}

	ret = my_assert(j == *first_available_idx + n_type,
			"Can't choose %d coins from LIGHT\n", (n_type));

	*first_available_idx += n_type;

	return ret;
}

static int choose_coins(choice_bowl_t bowl, int chosen[6],
			coin13_weigh_state_t *prev_state)
{
	int *coins_light;
	int *coins_heavy;
	int ret = 0;

	if (bowl.n_unknown) {
		int j = 0;
		for (int i = 0; i < 13; i++) {
			if (unknowns[i] == UNKNOWN) {
				unknowns[i] = KNOWN;
				chosen[j] = i;
				j++;
				if (j == bowl.n_unknown)
					break;
			}
		}
		ret = my_assert(j == bowl.n_unknown,
				"Can't choose %d coins from UNKNOWN\n",
				bowl.n_unknown);
		if (ret)
			return ret;
	}

	ret = __choose_coins(bowl.n_light, bowl.n_unknown, chosen, RESULT_LEFT,
			     *prev_state, &prev_state->first_available_light,
			     0);
	if (ret)
		return ret;
	ret = __choose_coins(bowl.n_heavy, bowl.n_unknown + bowl.n_light,
			     chosen, RESULT_RIGHT, *prev_state,
			     &prev_state->first_available_heavy, 0);
	if (ret)
		return ret;

	ret = __choose_coins(bowl.n_left, bowl.n_unknown, chosen, RESULT_LEFT,
			     *prev_state, &prev_state->first_available_left, 1);
	if (ret)
		return ret;
	return __choose_coins(bowl.n_right, bowl.n_unknown + bowl.n_left,
			      chosen, RESULT_RIGHT, *prev_state,
			      &prev_state->first_available_right, 1);
}

static int pick_fake_foo(int j, coin13_weigh_state_t states[3],
			 pick_fake_t pick_fake)
{
	int err;
	switch (pick_fake.fake) {
	case ONLY_UNKNOWN:
		for (int i = 0; i < 13; i++)
			if (unknowns[i] == UNKNOWN)
				return i;
		return my_assert(0, "Only unknown not found\n");
	case NOT_EQ_FROM_PREV_WEIGH:
		err = my_assert(states[2].n_coins == 1,
				"Not by 1 coin on 3rd weigh");
		if (err)
			return err;

		return states[2].coins_l[0];
	case EQ_FROM_PREV_WEIGH:
		err = my_assert(states[1].n_coins == 3,
				"Not by 3 coins on 2nd weigh");
		if (err)
			return err;

		return states[1].coins_l[2];
	case LIGHT_OR_HEAVY_OF_PREV_WEIGH:
		err = my_assert(states[2].n_coins == 1,
				"Not by 1 coin on 3rd weigh");
		if (err)
			return err;

		return states[1].result == states[2].result ?
			       states[2].coins_l[0] :
			       states[2].coins_r[0];
	case ON_LIGHT_OF_PREV_PREV_WEIGH:
		err = my_assert(states[2].n_coins == 1,
				"Not by 1 coin on 3rd weigh");
		if (err)
			return err;

		return states[2].result == RESULT_EQ ?
			       (states[1].result == RESULT_LEFT ?
					states[1].coins_r[0] :
					states[1].coins_l[0]) :
			       (states[2].result == RESULT_LEFT ?
					states[2].coins_r[0] :
					states[2].coins_l[0]);
	case ON_LIGHT_OF_PREV_WEIGH:
		return states[2].result == RESULT_EQ ?
			       (states[1].result == RESULT_LEFT ?
					states[1].coins_r[0] :
					states[1].coins_l[0]) :
			       (states[2].result == RESULT_LEFT ?
					states[2].coins_r[0] :
					states[2].coins_l[0]);
	case ON_HEAVY:
		err = my_assert(states[2].n_coins == 1,
				"Not by 1 coin on 3rd weigh");
		if (err)
			return err;

		return states[2].result == RESULT_RIGHT ? states[2].coins_r[0] :
							  states[2].coins_l[0];
	default:
		return my_assert(0, "Not implemented\n");
	}
}

static int c13_run(const instr_t program[static 15])
{
	coin13_weigh_state_t states[3] = {
		{ .n_coins = 0, .coins_l = { UNKNOWN }, .coins_r = { UNKNOWN } },
		{ .n_coins = 0, .coins_l = { UNKNOWN }, .coins_r = { UNKNOWN } },
		{ .n_coins = 0, .coins_l = { UNKNOWN }, .coins_r = { UNKNOWN } },
	};
	int j = 0;
	pick_fake_t pick_fake;
	int err = 0;

	for (size_t i = 0; i < 3; i++) {
		choice_t choice;
		int n_coins;
		int first_available_light = 0;
		int first_available_heavy = 0;
		int first_available_left = 0;
		int first_available_right = 0;

		//printf("№ %ld\n", i);
		choice = program[j].action.choice;

		// total amount of coins to be weighed on left and right
		// bowls should be equal
		states[i].n_coins = choice.left.n_unknown +
				    choice.left.n_light + choice.left.n_heavy +
				    choice.left.n_left + choice.left.n_right;
		if (states[i].n_coins == 0)
			return -1;

		/*printf("Gonna choose for left bowl: %d from unknown, %d from light, %d from heavy, %d from left, %d from right\n",
		       choice.left.n_unknown, choice.left.n_light,
		       choice.left.n_heavy, choice.left.n_left,
		       choice.left.n_right);*/

		if (choice.left.n_ref == -1 &&
		    states[i - 1].result == RESULT_LEFT) {
			states[i - 2].first_available_light = 0;
			states[i - 2].first_available_heavy = 0;
		} else if (choice.left.n_ref == -1 &&
			   states[i - 1].result == RESULT_RIGHT) {
			states[i - 2].first_available_light =
				states[i - 2].n_coins / 2;
			states[i - 2].first_available_heavy =
				states[i - 2].n_coins / 2;
		}

		err = choose_coins(choice.left, states[i].coins_l,
				   (i == 0) ? &states[0] :
					      (choice.left.n_ref < 0 ?
						       &states[i - 2] :
						       &states[i - 1]));
		if (err)
			return err;

		/*printf("Gonna choose for right bowl: %d from unknown, %d from light, %d from heavy, %d from left, %d from right\n",
		       choice.right.n_unknown, choice.right.n_light,
		       choice.right.n_heavy, choice.right.n_left,
		       choice.right.n_right);*/

		err = choose_coins(choice.right, states[i].coins_r,
				   (i == 0) ? &states[0] :
					      (choice.right.n_ref < 0 ?
						       &states[i - 2] :
						       &states[i - 1]));
		if (err)
			return err;

		err = weigh(&states[i]);
		if (err)
			return err;
		j = 2 * j + 1 + (states[i].result != RESULT_EQ);
	}

	pick_fake = program[j].action.pick_fake;

	return pick_fake_foo(j, states, pick_fake);
}

static void prepare(int fake_idx, int fake_weight)
{
	for (int j = 0; j < sizeof(unknowns) / sizeof(unknowns[0]); j++)
		unknowns[j] = UNKNOWN;
	for (int j = 0; j < sizeof(coins) / sizeof(coins[0]); j++)
		coins[j] = OK;

	coins[fake_idx] = fake_weight;
}
