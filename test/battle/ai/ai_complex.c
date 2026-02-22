#include "global.h"
#include "test/battle.h"

AI_SINGLE_BATTLE_TEST("AI_Complex: Stealth Rock scores higher than normal move on first turn")
{
    u32 turnCount;

    PARAMETRIZE { turnCount = 0; }
    PARAMETRIZE { turnCount = 1; }

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_STEALTH_ROCK) == EFFECT_STEALTH_ROCK);
        AI_FLAGS(AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_GARCHOMP) { Moves(MOVE_STEALTH_ROCK, MOVE_TACKLE); }
    } WHEN {
        if (turnCount == 1)
            TURN { MOVE(player, MOVE_SPLASH); SCORE_GT(opponent, MOVE_STEALTH_ROCK, MOVE_TACKLE); }
        else
            TURN { MOVE(player, MOVE_SPLASH); SCORE_GT(opponent, MOVE_STEALTH_ROCK, MOVE_TACKLE); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Sticky Web scores higher than Stealth Rock on first turn")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_STICKY_WEB) == EFFECT_STICKY_WEB);
        ASSUME(GetMoveEffect(MOVE_STEALTH_ROCK) == EFFECT_STEALTH_ROCK);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_SMEARGLE) { Moves(MOVE_STICKY_WEB, MOVE_STEALTH_ROCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); SCORE_GT(opponent, MOVE_STICKY_WEB, MOVE_STEALTH_ROCK); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Protect scores lower on first turn in singles")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_PROTECT) == EFFECT_PROTECT);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_PROTECT, MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); SCORE_LT(opponent, MOVE_PROTECT, MOVE_TACKLE); }
    }
}

// AI_SINGLE_BATTLE_TEST("AI_Complex: AI never uses Protect three turns in a row")
// {
//     GIVEN {
//         ASSUME(GetMoveEffect(MOVE_PROTECT) == EFFECT_PROTECT);
//         AI_FLAGS(AI_FLAG_COMPLEX);
//         PLAYER(SPECIES_GENGAR) { Moves(MOVE_SPLASH); }
//         OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_PROTECT, MOVE_TACKLE); }
//     } WHEN {
//         TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_PROTECT); }
//         TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_PROTECT); }
//         TURN { MOVE(player, MOVE_SPLASH); SCORE_LT(opponent, MOVE_PROTECT, MOVE_TACKLE); }
//     }
// }

AI_SINGLE_BATTLE_TEST("AI_Complex: Icy Wind preferred when AI is slower than target")
{
    u32 speedPlayer, speedAi;

    PARAMETRIZE { speedPlayer = 4; speedAi = 2; }  // AI slower
    PARAMETRIZE { speedPlayer = 2; speedAi = 4; }  // AI faster

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ICY_WIND) == EFFECT_HIT);
        AI_FLAGS(AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Speed(speedPlayer); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_LAPRAS) { Speed(speedAi); Moves(MOVE_ICY_WIND, MOVE_SURF); }
    } WHEN {
        if (speedPlayer > speedAi)
            TURN { MOVE(player, MOVE_SPLASH); SCORE_EQ(opponent, MOVE_ICY_WIND, MOVE_SURF); }
        else
            TURN { MOVE(player, MOVE_SPLASH); SCORE_LT(opponent, MOVE_ICY_WIND, MOVE_SURF); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Trapping move scores higher than regular move")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FIRE_SPIN) == EFFECT_HIT);
        AI_FLAGS(AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_MAGMAR) { Moves(MOVE_FIRE_SPIN, MOVE_EMBER); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); SCORE_GT(opponent, MOVE_FIRE_SPIN, MOVE_EMBER); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Trick Room preferred when AI is slower")
{
    u32 speedPlayer, speedAi;

    PARAMETRIZE { speedPlayer = 200; speedAi = 10; }
    PARAMETRIZE { speedPlayer = 10; speedAi = 200; }

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TRICK_ROOM) == EFFECT_TRICK_ROOM);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Speed(speedPlayer); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_REUNICLUS) { Speed(speedAi); Moves(MOVE_TRICK_ROOM, MOVE_PSYSHOCK); }
    } WHEN {
        if (speedPlayer > speedAi)
            TURN { MOVE(player, MOVE_SPLASH); SCORE_GT(opponent, MOVE_TRICK_ROOM, MOVE_PSYSHOCK); }
        else
            TURN { MOVE(player, MOVE_SPLASH); SCORE_LT(opponent, MOVE_TRICK_ROOM, MOVE_PSYSHOCK); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Trick Room never used if already active")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TRICK_ROOM) == EFFECT_TRICK_ROOM);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Speed(200); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_REUNICLUS) { Speed(10); Moves(MOVE_TRICK_ROOM, MOVE_PSYSHOCK); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); SCORE_GT(opponent, MOVE_TRICK_ROOM, MOVE_PSYSHOCK); }
        TURN { MOVE(player, MOVE_SPLASH); SCORE_LT(opponent, MOVE_TRICK_ROOM, MOVE_PSYSHOCK); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Imprison preferred if opponent shares a move")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_IMPRISON) == EFFECT_IMPRISON);
        AI_FLAGS(AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE, MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_IMPRISON, MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); SCORE_GT(opponent, MOVE_IMPRISON, MOVE_TACKLE); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Imprison never used if opponent shares no moves")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_IMPRISON) == EFFECT_IMPRISON);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH, MOVE_WATER_GUN); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_IMPRISON, MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); SCORE_LT(opponent, MOVE_IMPRISON, MOVE_TACKLE); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Fake Out preferred on first turn out")
{
    u32 turnCount;

    PARAMETRIZE { turnCount = 0; }
    PARAMETRIZE { turnCount = 1; }

    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_LOPUNNY) { Moves(MOVE_FAKE_OUT, MOVE_TACKLE); }
    } WHEN {
        if (turnCount == 1)
        {
            TURN { MOVE(player, MOVE_SPLASH); }
            TURN { MOVE(player, MOVE_SPLASH); SCORE_LT(opponent, MOVE_FAKE_OUT, MOVE_TACKLE); }
        }
        else
        {
            TURN { MOVE(player, MOVE_SPLASH); SCORE_GT(opponent, MOVE_FAKE_OUT, MOVE_TACKLE); }
        }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Sucker Punch less likely after using it last turn")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUCKER_PUNCH) == EFFECT_SUCKER_PUNCH);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_BISHARP) { Moves(MOVE_SUCKER_PUNCH, MOVE_TACKLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SUCKER_PUNCH); }
        TURN { MOVE(player, MOVE_SPLASH); SCORE_LT(opponent, MOVE_SUCKER_PUNCH, MOVE_TACKLE); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Protect scores higher when player has a negative condition")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_PROTECT) == EFFECT_PROTECT);
        AI_FLAGS(AI_FLAG_COMPLEX);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_PROTECT, MOVE_WILL_O_WISP); }
    } WHEN {
            TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_WILL_O_WISP); }
            TURN { MOVE(player, MOVE_SPLASH); SCORE_EQ_VAL(opponent, MOVE_PROTECT, 107); }
    }
}

AI_SINGLE_BATTLE_TEST("AI_Complex: Protect scores higher when player has a negative condition")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_PROTECT) == EFFECT_PROTECT);
        AI_FLAGS(AI_FLAG_COMPLEX | AI_FLAG_OMNISCIENT | AI_FLAG_CHECK_BAD_MOVE);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_PROTECT, MOVE_INFESTATION); }
    } WHEN {
            TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_INFESTATION); }
            TURN { MOVE(player, MOVE_SPLASH); SCORE_EQ_VAL(opponent, MOVE_PROTECT, 107); SCORE_LT(opponent, MOVE_INFESTATION, MOVE_PROTECT);}
    }
}
