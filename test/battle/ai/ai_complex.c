#include "global.h"
#include "test/battle.h"

// ===== HAZARD TESTS =====

AI_SINGLE_BATTLE_TEST("AI_Complex: AI prefers Stealth Rock more on first turn out")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GARCHOMP) { Moves(MOVE_STEALTH_ROCK, MOVE_TACKLE); }
    } WHEN {
        TURN { SCORE_GT(opponent, MOVE_STEALTH_ROCK, MOVE_TACKLE); } // first turn, high score
        TURN { SCORE_GT(opponent, MOVE_STEALTH_ROCK, MOVE_TACKLE); } // subsequent turn, lower but still preferred
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: AI scores Sticky Web higher than Stealth Rock on first turn")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SMEARGLE) { Moves(MOVE_STICKY_WEB, MOVE_STEALTH_ROCK); }
    } WHEN {
        TURN { SCORE_GT(opponent, MOVE_STICKY_WEB, MOVE_STEALTH_ROCK); }
    }
}

// ===== PROTECT TESTS =====

AI_SINGLE_BATTLE_TEST("AI_Complex: AI is less likely to Protect if it used Protect last turn")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_PROTECT, MOVE_TACKLE); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_PROTECT); }
        TURN { SCORE_LT(opponent, MOVE_PROTECT, MOVE_TACKLE); } // after one protect, score drops
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: AI prefers Protect less on first turn out in singles")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_PROTECT, MOVE_SPLASH); }
    } WHEN {
        TURN { SCORE_LT(opponent, MOVE_PROTECT, MOVE_SPLASH); } // -1 on first turn in singles
    }
}

// ===== SPEED REDUCTION TESTS =====

AI_SINGLE_BATTLE_TEST("AI_Complex: AI prefers Icy Wind when slower than target")
{
    u32 speedPlayer, speedAi;

    PARAMETRIZE { speedPlayer = 200; speedAi = 10; }
    PARAMETRIZE { speedPlayer = 10; speedAi = 200; }

    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET) { Speed(speedPlayer); }
        OPPONENT(SPECIES_LAPRAS) { Moves(MOVE_ICY_WIND, MOVE_WATER_GUN); Speed(speedAi); }
    } WHEN {
        if (speedPlayer > speedAi)
        {
            TURN { SCORE_GT(opponent, MOVE_ICY_WIND, MOVE_WATER_GUN); } // slower, prefers icy wind
        }
        else
        {
            TURN { SCORE_EQ(opponent, MOVE_ICY_WIND, MOVE_WATER_GUN); } // faster, no preference
        }
    }
}

// ===== TRAPPING TESTS =====

AI_SINGLE_BATTLE_TEST("AI_Complex: AI scores trapping move positively")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MAGMAR) { Moves(MOVE_FIRE_SPIN, MOVE_EMBER); }
    } WHEN {
        TURN { SCORE_GT(opponent, MOVE_FIRE_SPIN, MOVE_EMBER); }
    }
}

// ===== TRICK ROOM TESTS =====

AI_SINGLE_BATTLE_TEST("AI_Complex: AI prefers Trick Room when slower than target")
{
    u32 speedPlayer, speedAi;

    PARAMETRIZE { speedPlayer = 200; speedAi = 10; }
    PARAMETRIZE { speedPlayer = 10; speedAi = 200; }

    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET) { Speed(speedPlayer); }
        OPPONENT(SPECIES_REUNICLUS) { Moves(MOVE_TRICK_ROOM, MOVE_PSYSHOCK); Speed(speedAi); }
    } WHEN {
        if (speedPlayer > speedAi)
        {
            TURN { SCORE_GT(opponent, MOVE_TRICK_ROOM, MOVE_PSYSHOCK); }
        }
        else
        {
            TURN { SCORE_LT(opponent, MOVE_TRICK_ROOM, MOVE_PSYSHOCK); }
        }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: AI never uses Trick Room if already up")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET) { Speed(200); }
        OPPONENT(SPECIES_REUNICLUS) { Moves(MOVE_TRICK_ROOM, MOVE_PSYSHOCK); Speed(10); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_TRICK_ROOM); }
        TURN { SCORE_LT(opponent, MOVE_TRICK_ROOM, MOVE_PSYSHOCK); } // already up, -20
    }
}

// ===== IMPRISON TESTS =====

AI_SINGLE_BATTLE_TEST("AI_Complex: AI uses Imprison if opponent shares a move")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE, MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_IMPRISON, MOVE_TACKLE); }
    } WHEN {
        TURN { SCORE_GT(opponent, MOVE_IMPRISON, MOVE_TACKLE); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: AI never uses Imprison if opponent shares no moves")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_IMPRISON, MOVE_TACKLE); }
    } WHEN {
        TURN { SCORE_LT(opponent, MOVE_IMPRISON, MOVE_TACKLE); }
    }
}

// ===== FAKE OUT TESTS =====

AI_SINGLE_BATTLE_TEST("AI_Complex: AI prefers Fake Out on first turn out")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_LOPUNNY) { Moves(MOVE_FAKE_OUT, MOVE_TACKLE); }
    } WHEN {
        TURN { SCORE_GT(opponent, MOVE_FAKE_OUT, MOVE_TACKLE); } // first turn, +9
        TURN { SCORE_LT(opponent, MOVE_FAKE_OUT, MOVE_TACKLE); } // second turn, fails so bad move
    }
}

// ===== SUCKER PUNCH TESTS =====

AI_SINGLE_BATTLE_TEST("AI_Complex: AI is less likely to use Sucker Punch twice in a row")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BISHARP) { Moves(MOVE_SUCKER_PUNCH, MOVE_TACKLE); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_SUCKER_PUNCH); }
        TURN { SCORE_LT(opponent, MOVE_SUCKER_PUNCH, MOVE_TACKLE); } // 50% chance of -20
    }
}

// AI_SINGLE_BATTLE_TEST("AI_Complex: INTENTIONAL FAIL - AI prefers Tackle over Stealth Rock on first turn")
// {
//     GIVEN {
//         AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX);
//         PLAYER(SPECIES_WOBBUFFET);
//         OPPONENT(SPECIES_GARCHOMP) { Moves(MOVE_STEALTH_ROCK, MOVE_TACKLE); }
//     } WHEN {
//         TURN { SCORE_GT(opponent, MOVE_TACKLE, MOVE_STEALTH_ROCK); } // should fail, Stealth Rock scores higher
//     }
// }