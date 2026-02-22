Global_EventScript_Bel::
	lock
	faceplayer
	msgbox Global_Text_NotAvailable, MSGBOX_DEFAULT
	release
	end

Global_Text_NotAvailable:
	.string "This is the end of the split!\n"
	.string "There's no more content beyond here,\p"
	.string "and thanks for testing!$"

Global_Text_CantPass:
	.string "Azumarill says you can't pass!$"

Global_EventScript_Azumarill::
	lock
    applymovement LOCALID_AZUMARILL_116, Global_Movement_Azumarill
	faceplayer
	waitse
	playmoncry SPECIES_AZUMARILL, CRY_MODE_NORMAL
	msgbox Global_Text_CantPass, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Global_Movement_Azumarill:
    emote_x
	step_end