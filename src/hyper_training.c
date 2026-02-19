#include "global.h"
#include "sound.h"
#include "hyper_training.h"
#include "constants/items.h"
#include "event_data.h"
#include "field_message_box.h"
#include "item.h"
#include "party_menu.h"
#include "pokemon.h"
#include "menu.h"
#include "menu_helpers.h"
#include "script.h"
#include "script_menu.h"
#include "task.h"
#include "text.h"
#include "window.h"

static void Task_ShowStatSelectionMenu(u8 taskId);
static void Task_HandleStatSelection(u8 taskId);
static void Task_WaitForMessageThenReopenMenu(u8 taskId);

void DoHyperTraining(void)
{
    ChooseMonForHyperTraining();
}

void ShowMaxIVMenu(void)
{
    LockPlayerFieldControls();
    u8 taskId = CreateTask(Task_ShowStatSelectionMenu, 8);
    gTasks[taskId].data[15] = gSpecialVar_0x8004;
}

static void Task_ShowStatSelectionMenu(u8 taskId)
{
    static const u8 sText_HP[] = _("HP");
    static const u8 sText_Attack[] = _("Attack");
    static const u8 sText_Defense[] = _("Defense");
    static const u8 sText_SpAtk[] = _("Sp. Atk");
    static const u8 sText_SpDef[] = _("Sp. Def");
    static const u8 sText_Speed[] = _("Speed");
    static const u8 sText_Cancel[] = _("Cancel");

    static const u8 *const sStatMenuItems[] = {
        sText_HP,
        sText_Attack,
        sText_Defense,
        sText_SpAtk,
        sText_SpDef,
        sText_Speed,
        sText_Cancel};

    u8 windowId = CreateWindowFromRect(0, 0, 14, 14);
    SetStandardWindowBorderStyle(windowId, FALSE);

    u8 i;
    for (i = 0; i < ARRAY_COUNT(sStatMenuItems); i++)
    {
        AddTextPrinterParameterized(windowId, FONT_NORMAL, sStatMenuItems[i], 8, (i * 16) + 1, TEXT_SKIP_DRAW, NULL);
    }

    InitMenuInUpperLeftCornerNormal(windowId, ARRAY_COUNT(sStatMenuItems), 0);
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_FULL);

    gTasks[taskId].data[0] = windowId;
    gTasks[taskId].func = Task_HandleStatSelection;
}

static void Task_HandleStatSelection(u8 taskId)
{

    s8 input = Menu_ProcessInput();
    u8 monId = gTasks[taskId].data[15];

    if (input == MENU_B_PRESSED || input == 6) // Cancel
    {
        gSpecialVar_Result = FALSE;
        ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
        RemoveWindow(gTasks[taskId].data[0]);
        UnlockPlayerFieldControls();
        ScriptContext_Enable();
        DestroyTask(taskId);
        return;
    }
    else if (input >= 0 && input < 6) // Stat selected
    {
        struct Pokemon *mon = &gPlayerParty[monId];
        u8 maxIV = 31;

        static const u8 sStatToMonData[] = {
            MON_DATA_HP_IV,
            MON_DATA_ATK_IV,
            MON_DATA_DEF_IV,
            MON_DATA_SPATK_IV,
            MON_DATA_SPDEF_IV,
            MON_DATA_SPEED_IV};

        u8 currentIV = GetMonData(mon, sStatToMonData[input]);

        if (currentIV >= 31)
        {

            ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
            RemoveWindow(gTasks[taskId].data[0]);

            // Use the standard message box system
            static const u8 sText_AlreadyPerfect[] = _("That stat is already perfect!");

            // Lock controls while showing message
            LockPlayerFieldControls();

            // Show message in a standard msgbox
            ShowFieldMessage(sText_AlreadyPerfect);

            // Change task to wait for message to close, then reopen menu
            gTasks[taskId].func = Task_WaitForMessageThenReopenMenu;
            return;
        }
        else
        {
            SetMonData(mon, sStatToMonData[input], &maxIV);
            CalculateMonStats(mon);
            RemoveBagItem(ITEM_HEART_SCALE, 1);
            gSpecialVar_Result = TRUE;
        }

        ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
        RemoveWindow(gTasks[taskId].data[0]);

        UnlockPlayerFieldControls();
        ScriptContext_Enable();
        DestroyTask(taskId);
    }
}

static void Task_WaitForMessageThenReopenMenu(u8 taskId)
{
    // Wait for the message to be closed
    if (!IsTextPrinterActive(0) && JOY_NEW(A_BUTTON | B_BUTTON))
    {
        HideFieldMessageBox();

        // Reopen the stat selection menu
        gTasks[taskId].func = Task_ShowStatSelectionMenu;
    }
}
