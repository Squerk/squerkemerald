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
    case 2, HeartScaleTutor_EventScript_Decline
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
    goto_if_eq VAR_RESULT, FALSE, HeartScaleTutor_EventScript_NoHeartScale

    special DoHyperTraining
    waitstate
    
    compare VAR_0x8004, PARTY_NOTHING_CHOSEN
    goto_if_eq HeartScaleTutor_EventScript_Decline
    
    special ShowMaxIVMenu
    waitstate
    
    compare VAR_RESULT, FALSE
    goto_if_eq HeartScaleTutor_EventScript_Decline

    removeitem ITEM_HEART_SCALE
    msgbox HeartScaleTutor_Text_IVMaxed, MSGBOX_DEFAULT
    release
    end

HeartScaleTutor_EventScript_NoHeartScale::
    msgbox HeartScaleTutor_Text_NoHeartScale, MSGBOX_DEFAULT
    release
    end

HeartScaleTutor_EventScript_Decline::
    msgbox HeartScaleTutor_Text_Decline, MSGBOX_DEFAULT
    release
    end

// Text strings
HeartScaleTutor_Text_Intro:
    .string "I can help strengthen your Pokémon\n"
    .string "for just one heart scale!$"

HeartScaleTutor_Text_WhatService:
    .string "What would you like me to do?$"

HeartScaleTutor_Text_TeachMove:
    .string "I can teach forgotten moves.$"

HeartScaleTutor_Text_MaxIV:
    .string "I can maximize one stat's potential.$"

HeartScaleTutor_Text_NoHeartScale:
    .string "You don't have a heart scale!$"

HeartScaleTutor_Text_MoveTaught:
    .string "There! Your Pokémon remembered!$"

HeartScaleTutor_Text_IVMaxed:
    .string "There! That stat is now perfect!$"

HeartScaleTutor_Text_Decline:
    .string "Come back if you need my help!$"
