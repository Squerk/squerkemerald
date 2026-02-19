HeartScaleTutor_EventScript_Main::
    lock
    faceplayer
    msgbox HeartScaleTutor_Text_Intro, MSGBOX_DEFAULT
    goto HeartScaleTutor_EventScript_ChooseService
    end

@ case 1, HeartScaleTutor_EventScript_AbilityChange to add ability change
HeartScaleTutor_EventScript_ChooseService::
    message HeartScaleTutor_Text_WhatService
    waitmessage
    multichoice 0, 0, MULTI_HEART_SCALE_SERVICES, FALSE
    switch VAR_RESULT
    case 0, HeartScaleTutor_EventScript_MoveTutor
    case 1, HeartScaleTutor_EventScript_MaxIV
    case 2, HeartScaleTutor_EventScript_ChangeNature
    case 3, HeartScaleTutor_EventScript_MoveDeleter
    case 4, HeartScaleTutor_EventScript_Decline
    case MULTI_B_PRESSED, HeartScaleTutor_EventScript_Decline
    end

HeartScaleTutor_EventScript_MoveTutor::
    msgbox HeartScaleTutor_Text_TeachMove, MSGBOX_DEFAULT
    checkitem ITEM_HEART_SCALE
    goto_if_eq VAR_RESULT, FALSE, HeartScaleTutor_EventScript_NoHeartScale
	msgbox MoveRelearner_Text_ChoosePkmn, MSGBOX_DEFAULT
	special ChooseMonForMoveRelearner
	waitstate
	call_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartScaleTutor_EventScript_Decline
	special IsSelectedMonEgg
	call_if_eq VAR_RESULT, YES, MoveRelearner_EventScript_CantTeachMoveToEgg
	call_if_eq VAR_0x8005, NO, MoveRelearner_EventScript_CantTeachMoveToPkmn
    special TeachMoveRelearnerMove
    waitstate
    goto_if_eq VAR_0x8004, FALSE, HeartScaleTutor_EventScript_Decline
    removeitem ITEM_HEART_SCALE
    msgbox HeartScaleTutor_Text_MoveTaught, MSGBOX_DEFAULT
    release
    end

HeartScaleTutor_EventScript_MaxIV::
    msgbox HeartScaleTutor_Text_MaxIV, MSGBOX_DEFAULT
    checkitem ITEM_HEART_SCALE
    goto_if_eq VAR_RESULT, FALSE, HeartScaleTutor_EventScript_NoThreeHeartScale

    special DoHyperTraining
    waitstate
    
    compare VAR_0x8004, PARTY_NOTHING_CHOSEN
    goto_if_eq HeartScaleTutor_EventScript_Decline
    
    special ShowMaxIVMenu
    waitstate
    
    compare VAR_RESULT, FALSE
    goto_if_eq HeartScaleTutor_EventScript_Decline

    msgbox HeartScaleTutor_Text_IVMaxed, MSGBOX_DEFAULT
    release
    end

HeartScaleTutor_EventScript_ChangeNature::
    msgbox HeartScaleTutor_Text_ChangeNature, MSGBOX_DEFAULT
    checkitem ITEM_HEART_SCALE, 3
    goto_if_eq VAR_RESULT, FALSE, HeartScaleTutor_EventScript_NoThreeHeartScale

    special DoHyperTraining
    waitstate
    
    compare VAR_0x8004, PARTY_NOTHING_CHOSEN
    goto_if_eq HeartScaleTutor_EventScript_Decline
    
    special ShowNatureChangeMenu
    waitstate
    
    compare VAR_RESULT, FALSE
    goto_if_eq HeartScaleTutor_EventScript_Decline

    msgbox HeartScaleTutor_Text_NatureChanged, MSGBOX_DEFAULT
    release
    end

HeartScaleTutor_EventScript_NoHeartScale::
    msgbox HeartScaleTutor_Text_NoHeartScale, MSGBOX_DEFAULT
    release
    end

HeartScaleTutor_EventScript_NoThreeHeartScale::
    msgbox HeartScaleTutor_Text_NoThreeHeartScale, MSGBOX_DEFAULT
    release
    end

HeartScaleTutor_EventScript_Decline::
    msgbox HeartScaleTutor_Text_Decline, MSGBOX_DEFAULT
    release
    end

HeartScaleTutor_Text_Intro:
    .string "I can help strengthen your Pokémon\n"
    .string "for heart scales!$"

HeartScaleTutor_Text_WhatService:
    .string "What would you like me to do?$"

HeartScaleTutor_Text_TeachMove:
    .string "I can teach moves for one scale.$"

HeartScaleTutor_Text_MaxIV:
    .string "I can maximize an IV for one scale.$"

HeartScaleTutor_Text_ChangeNature:
    .string "I can change a nature for 3 scales.$"

HeartScaleTutor_Text_NoHeartScale:
    .string "You don't have a heart scale!$"

HeartScaleTutor_Text_NoThreeHeartScale:
    .string "You don't have 3 heart scales!$"

HeartScaleTutor_Text_MoveTaught:
    .string "There! Your Pokémon remembered!$"

HeartScaleTutor_Text_IVMaxed:
    .string "There! That stat is now perfect!$"

HeartScaleTutor_Text_NatureChanged:
    .string "There! The nature has been changed!$"

HeartScaleTutor_Text_Decline:
    .string "Come back if you need my help!$"

HeartScaleTutor_EventScript_MoveDeleter::
	lockall
	applymovement LOCALID_MOVE_DELETER, Common_Movement_FacePlayer
	waitmovement 0
	msgbox HeartScaleTutor_Text_ICanMakeMonForgetMove, MSGBOX_YESNO
	switch VAR_RESULT
	case YES, HeartScaleTutor_EventScript_ChooseMonAndMoveToForget
	case NO, HeartScaleTutor_Text_Decline
	releaseall
	end

HeartScaleTutor_EventScript_ChooseMonAndMoveToForget::
	msgbox HeartScaleTutor_Text_WhichMonShouldForget, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartScaleTutor_Text_Decline
	special IsSelectedMonEgg
	goto_if_eq VAR_RESULT, TRUE, HeartScaleTutor_EventScript_EggCantForgetMoves
	special GetNumMovesSelectedMonHas
	goto_if_eq VAR_RESULT, 1, HeartScaleTutor_EventScript_MonOnlyKnowsOneMove
	msgbox HeartScaleTutor_Text_WhichMoveShouldBeForgotten, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	special MoveDeleterChooseMoveToForget
	fadescreen FADE_FROM_BLACK
	goto_if_eq VAR_0x8005, MAX_MON_MOVES, HeartScaleTutor_EventScript_ChooseMonAndMoveToForget
	special BufferMoveDeleterNicknameAndMove
	msgbox HeartScaleTutor_Text_MonsMoveShouldBeForgotten, MSGBOX_YESNO
	switch VAR_RESULT
	case YES, HeartScaleTutor_EventScript_TryForgetMove
	case NO, HeartScaleTutor_Text_Decline
	releaseall
	end

HeartScaleTutor_EventScript_TryForgetMove::
	special MoveDeleterForgetMove
	playfanfare MUS_MOVE_DELETED
	waitfanfare
	msgbox HeartScaleTutor_Text_MonHasForgottenMove, MSGBOX_DEFAULT
	releaseall
	end

HeartScaleTutor_EventScript_MonOnlyKnowsOneMove::
	special BufferMoveDeleterNicknameAndMove
	msgbox HeartScaleTutor_Text_MonOnlyKnowsOneMove, MSGBOX_DEFAULT
	releaseall
	end

HeartScaleTutor_EventScript_EggCantForgetMoves::
	msgbox HeartScaleTutor_Text_EggCantForgetMoves, MSGBOX_DEFAULT
	releaseall
	end

HeartScaleTutor_Text_ICanMakeMonForgetMove:
	.string "I can make Pokémon forget their moves.\p"
	.string "Would you like me to do that?$"

HeartScaleTutor_Text_WhichMonShouldForget:
	.string "Which Pokémon should forget a move?$"

HeartScaleTutor_Text_WhichMoveShouldBeForgotten:
	.string "Which move should be forgotten?$"

HeartScaleTutor_Text_MonOnlyKnowsOneMove:
	.string "{STR_VAR_1} knows only one move\n"
	.string "so it can't be forgotten…$"

HeartScaleTutor_Text_MonsMoveShouldBeForgotten:
	.string "Hm! {STR_VAR_1}'s {STR_VAR_2}?\n"
	.string "That move should be forgotten?$"

HeartScaleTutor_Text_MonHasForgottenMove:
	.string "It worked to perfection!\p"
	.string "{STR_VAR_1} has forgotten\n"
	.string "{STR_VAR_2} completely.$"

HeartScaleTutor_Text_EggCantForgetMoves:
	.string "What?\n"
	.string "No EGG should know any moves.$"
