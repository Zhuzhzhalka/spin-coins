#define SEP "--------------------------------------------------------------\n"

c_decl {
#include "vm.h"

#define BOWL2INITLIST(bowl) \
  {                         \
    (bowl).n_unknown,       \
    (bowl).n_light,         \
    (bowl).n_heavy,         \
    (bowl).n_left,          \
    (bowl).n_right,         \
    (bowl).n_ref,           \
  }

#define CHOICE2INITLIST(choice)    \
  {                                \
    BOWL2INITLIST((choice).left),  \
    BOWL2INITLIST((choice).right)  \
  }

#define U128ZP(x) \
    ((__uint128_t) ((unsigned) (x)))

#define U128_FROM_I32X4(arr) \
    ((U128ZP((arr)[0]) << 96) | (U128ZP((arr)[1]) << 64) | (U128ZP((arr)[2]) << 32) | (U128ZP((arr)[3])))

#define U128_FROM_U64X2(a, b) \
    ((((__uint128_t)(a)) << 64) |  ((__uint128_t)(b)))
}

c_code {
#include "vm_impl.c"
#include "serialize_impl.c"

instr_t program[15] = {};

__uint128_t seed = U128_FROM_U64X2(0x00000000000011c4LU, 0xc417052e4505352eLU);
}

typedef C13Bowl {
    int n_unkn;
    int n_light;
    int n_heavy;
    int n_left;
    int n_right;
    int n_ref;
}

typedef C13Choice {
    C13Bowl left;
    C13Bowl right;
}

mtype:C13Pick = {
    OOPS,
    ONLY_UNKNOWN,
    NOT_EQ_FROM_PREV_WEIGH,
    EQ_FROM_PREV_WEIGH,
    LIGHT_OR_HEAVY_OF_PREV_WEIGH,
    ON_LIGHT_OF_PREV_PREV_WEIGH,
    ON_LIGHT_OF_PREV_WEIGH,
    ON_HEAVY,
}

C13Choice prog_choice[7]; // первые 7 инструкций - взвешивания
mtype:C13Pick prog_pick[8]; // остальные 8 - выбор фальшивой

c_state "instr_t program[15]" "Global"

init {
    int i = 0;
    int run_result = -1;
    int fake_idx, fake_weight;
    int ok = 0;
    int offs = 0;

    for (offs: 0..100000000) {
        ok = 0;
        c_code {
            __uint128_t pr = seed + Pinit->offs;
            deserialize(program, pr);
            print_prog(program);
        }

        for (i: 0..25) {
            fake_idx = i % 13;
            fake_weight = (i % 2) * 2 - 1;

            printf(SEP);
            printf("%d: Fake idx %d, weight %d\n", i, fake_idx, fake_weight);

            c_code {
                prepare(Pinit->fake_idx, Pinit->fake_weight);
                Pinit->run_result = c13_run(program);
            };
            if
            :: run_result == -1 -> goto L1;
            :: else -> skip;
            fi

            c_code {
                if (Pinit->i == 25) {
                    //printf("\nOK: %d, F: %d, OK/ALL %lf\n", Pinit->ok, 26 - Pinit->ok, (double)Pinit->ok / 26);
                    size_t i;
                    for (i = 0; i < 7; i++) {
                        now.prog_choice[i] = (struct C13Choice) CHOICE2INITLIST(program[i].action.choice);
                    }

                    for (; i < 15; i++) {
                        now.prog_pick[i - 7] = ON_HEAVY + 1 - program[i].action.pick_fake.fake;
                    }
                } else {
                        //printf("\nOK: %d, F: %d, OK/ALL %lf\n", Pinit->ok, 26 - Pinit->ok, (double)Pinit->ok / 26);
                }
            }

            if
            :: run_result == fake_idx -> ok = ok + 1;
            :: run_result != fake_idx -> break;
            fi

            assert(ok != 26);
        }
L1:

    }
}
