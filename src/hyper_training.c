#include "global.h"
#include "sound.h"
#include "hyper_training.h"
#include "constants/items.h"
#include "list_menu.h"
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
static void Task_ShowNatureTypeMenu(u8 taskId);
static void Task_HandleNatureTypeSelection(u8 taskId);
static void Task_ShowSpecificNatureMenu(u8 taskId);
static void Task_HandleSpecificNatureSelection(u8 taskId);
static void Task_WaitForMessageThenReopenNatureTypeMenu(u8 taskId);

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

void ShowNatureChangeMenu(void)
{
    LockPlayerFieldControls();
    u8 taskId = CreateTask(Task_ShowNatureTypeMenu, 8);
    gTasks[taskId].data[15] = gSpecialVar_0x8004;
}

static void Task_ShowNatureTypeMenu(u8 taskId)
{
    static const u8 sText_Attack[] = _("+ Attack");
    static const u8 sText_Defense[] = _("+ Defense");
    static const u8 sText_SpAtk[] = _("+ Sp. Atk");
    static const u8 sText_SpDef[] = _("+ Sp. Def");
    static const u8 sText_Speed[] = _("+ Speed");
    static const u8 sText_Neutral[] = _("Neutral");
    static const u8 sText_Cancel[] = _("Cancel");
    
    static const u8 *const sNatureTypeMenuItems[] = {
        sText_Attack,
        sText_Defense,
        sText_SpAtk,
        sText_SpDef,
        sText_Speed,
        sText_Neutral,
        sText_Cancel
    };
    
    u8 windowId = CreateWindowFromRect(0, 0, 14, 14);
    SetStandardWindowBorderStyle(windowId, FALSE);
    
    u8 i;
    for (i = 0; i < ARRAY_COUNT(sNatureTypeMenuItems); i++)
    {
        AddTextPrinterParameterized(windowId, FONT_NORMAL, sNatureTypeMenuItems[i], 8, (i * 16) + 1, TEXT_SKIP_DRAW, NULL);
    }
    
    InitMenuInUpperLeftCornerNormal(windowId, ARRAY_COUNT(sNatureTypeMenuItems), 0);
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_FULL);
    
    gTasks[taskId].data[0] = windowId;
    gTasks[taskId].func = Task_HandleNatureTypeSelection;
}

static void Task_HandleNatureTypeSelection(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        gSpecialVar_Result = FALSE;
        
        ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
        RemoveWindow(gTasks[taskId].data[0]);
        
        UnlockPlayerFieldControls();
        ScriptContext_Enable();
        DestroyTask(taskId);
        return;
    }
    
    s8 input = Menu_ProcessInputNoWrap();
    
    if (input == MENU_NOTHING_CHOSEN)
        return;
    
    if (input == 6) // Cancel
    {
        gSpecialVar_Result = FALSE;
        
        ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
        RemoveWindow(gTasks[taskId].data[0]);
        
        UnlockPlayerFieldControls();
        ScriptContext_Enable();
        DestroyTask(taskId);
        return;
    }
    
    // Store the selected stat type (0=Atk, 1=Def, 2=SpA, 3=SpD, 4=Spe, 5=Neutral)
    gTasks[taskId].data[14] = input;
    
    // Close current menu and open specific nature menu
    ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
    RemoveWindow(gTasks[taskId].data[0]);
    
    gTasks[taskId].func = Task_ShowSpecificNatureMenu;
}

// Second menu: Choose specific nature based on stat type
static void Task_ShowSpecificNatureMenu(u8 taskId)
{
    u8 statType = gTasks[taskId].data[14];
    
    // Define natures for each stat type
    // Attack+: Lonely(Def-), Brave(Spe-), Adamant(SpA-), Naughty(SpD-)
    static const u8 sText_Lonely[] = _("Lonely (-Def)");
    static const u8 sText_Brave[] = _("Brave (-Spe)");
    static const u8 sText_Adamant[] = _("Adamant (-SpA)");
    static const u8 sText_Naughty[] = _("Naughty (-SpD)");
    
    // Defense+: Bold(Atk-), Relaxed(Spe-), Impish(SpA-), Lax(SpD-)
    static const u8 sText_Bold[] = _("Bold (-Atk)");
    static const u8 sText_Relaxed[] = _("Relaxed (-Spe)");
    static const u8 sText_Impish[] = _("Impish (-SpA)");
    static const u8 sText_Lax[] = _("Lax (-SpD)");
    
    // SpAtk+: Modest(Atk-), Mild(Def-), Quiet(Spe-), Rash(SpD-)
    static const u8 sText_Modest[] = _("Modest (-Atk)");
    static const u8 sText_Mild[] = _("Mild (-Def)");
    static const u8 sText_Quiet[] = _("Quiet (-Spe)");
    static const u8 sText_Rash[] = _("Rash (-SpD)");
    
    // SpDef+: Calm(Atk-), Gentle(Def-), Sassy(Spe-), Careful(SpA-)
    static const u8 sText_Calm[] = _("Calm (-Atk)");
    static const u8 sText_Gentle[] = _("Gentle (-Def)");
    static const u8 sText_Sassy[] = _("Sassy (-Spe)");
    static const u8 sText_Careful[] = _("Careful (-SpA)");
    
    // Speed+: Timid(Atk-), Hasty(Def-), Jolly(SpA-), Naive(SpD-)
    static const u8 sText_Timid[] = _("Timid (-Atk)");
    static const u8 sText_Hasty[] = _("Hasty (-Def)");
    static const u8 sText_Jolly[] = _("Jolly (-SpA)");
    static const u8 sText_Naive[] = _("Naive (-SpD)");
    
    // Neutral: Hardy, Bashful
    static const u8 sText_Hardy[] = _("Hardy");
    static const u8 sText_Bashful[] = _("Bashful");
    
    static const u8 sText_Cancel[] = _("Cancel");
    
    const u8 *menuItems[5];
    u8 natures[5] = {NATURE_HARDY, NATURE_BASHFUL, 0, 0, 0};
    u8 menuCount = 2;
    
    if (statType == 0) // Attack+
    {
        menuItems[0] = sText_Lonely; natures[0] = NATURE_LONELY;
        menuItems[1] = sText_Brave; natures[1] = NATURE_BRAVE;
        menuItems[2] = sText_Adamant; natures[2] = NATURE_ADAMANT;
        menuItems[3] = sText_Naughty; natures[3] = NATURE_NAUGHTY;
        menuCount = 4;
    }
    else if (statType == 1) // Defense+
    {
        menuItems[0] = sText_Bold; natures[0] = NATURE_BOLD;
        menuItems[1] = sText_Relaxed; natures[1] = NATURE_RELAXED;
        menuItems[2] = sText_Impish; natures[2] = NATURE_IMPISH;
        menuItems[3] = sText_Lax; natures[3] = NATURE_LAX;
        menuCount = 4;
    }
    else if (statType == 2) // SpAtk+
    {
        menuItems[0] = sText_Modest; natures[0] = NATURE_MODEST;
        menuItems[1] = sText_Mild; natures[1] = NATURE_MILD;
        menuItems[2] = sText_Quiet; natures[2] = NATURE_QUIET;
        menuItems[3] = sText_Rash; natures[3] = NATURE_RASH;
        menuCount = 4;
    }
    else if (statType == 3) // SpDef+
    {
        menuItems[0] = sText_Calm; natures[0] = NATURE_CALM;
        menuItems[1] = sText_Gentle; natures[1] = NATURE_GENTLE;
        menuItems[2] = sText_Sassy; natures[2] = NATURE_SASSY;
        menuItems[3] = sText_Careful; natures[3] = NATURE_CAREFUL;
        menuCount = 4;
    }
    else if (statType == 4) // Speed+
    {
        menuItems[0] = sText_Timid; natures[0] = NATURE_TIMID;
        menuItems[1] = sText_Hasty; natures[1] = NATURE_HASTY;
        menuItems[2] = sText_Jolly; natures[2] = NATURE_JOLLY;
        menuItems[3] = sText_Naive; natures[3] = NATURE_NAIVE;
        menuCount = 4;
    }
    else if (statType == 5) // Neutral
    {
        menuItems[0] = sText_Hardy; natures[0] = NATURE_HARDY;
        menuItems[1] = sText_Bashful; natures[1] = NATURE_BASHFUL;
        menuCount = 2;
    }
    
    menuItems[menuCount] = sText_Cancel;
    menuCount++;
    
    // Store nature mapping in task data
    gTasks[taskId].data[5] = natures[0];
    gTasks[taskId].data[6] = natures[1];
    gTasks[taskId].data[7] = (menuCount > 3) ? natures[2] : 0;
    gTasks[taskId].data[8] = (menuCount > 4) ? natures[3] : 0;
    gTasks[taskId].data[9] = menuCount;
    
    u8 windowId = CreateWindowFromRect(0, 0, 18, 12);
    SetStandardWindowBorderStyle(windowId, FALSE);
    
    u8 i;
    for (i = 0; i < menuCount; i++)
    {
        AddTextPrinterParameterized(windowId, FONT_NORMAL, menuItems[i], 8, (i * 16) + 1, TEXT_SKIP_DRAW, NULL);
    }
    
    InitMenuInUpperLeftCornerNormal(windowId, menuCount, 0);
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_FULL);
    
    gTasks[taskId].data[0] = windowId;
    gTasks[taskId].func = Task_HandleSpecificNatureSelection;
}

static void Task_HandleSpecificNatureSelection(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        // Go back to stat type menu
        ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
        RemoveWindow(gTasks[taskId].data[0]);
        
        gTasks[taskId].func = Task_ShowNatureTypeMenu;
        return;
    }
    
    s8 input = Menu_ProcessInputNoWrap();
    u8 monId = gTasks[taskId].data[15];
    u8 menuCount = gTasks[taskId].data[9];
    
    if (input == MENU_NOTHING_CHOSEN)
        return;
    
    if (input == menuCount - 1) // Cancel
    {
        // Go back to stat type menu
        ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
        RemoveWindow(gTasks[taskId].data[0]);
        
        gTasks[taskId].func = Task_ShowNatureTypeMenu;
        return;
    }
    
    if (input >= 0 && input < menuCount - 1)
    {
        struct Pokemon *mon = &gPlayerParty[monId];
        u8 currentNature = GetMonData(mon, MON_DATA_HIDDEN_NATURE);
        u8 newNature = gTasks[taskId].data[5 + input];
        
        if (currentNature == newNature)
        {
            ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
            RemoveWindow(gTasks[taskId].data[0]);
            
            static const u8 sText_AlreadyThatNature[] = _("This Pokémon already has\nthat nature!");
            LockPlayerFieldControls();
            ShowFieldMessage(sText_AlreadyThatNature);
            
            gTasks[taskId].func = Task_WaitForMessageThenReopenNatureTypeMenu;
            return;
        }
        
        SetMonData(mon, MON_DATA_HIDDEN_NATURE, &newNature);
        CalculateMonStats(mon);
        RemoveBagItem(ITEM_HEART_SCALE, 3);
        gSpecialVar_Result = TRUE;
        
        ClearStdWindowAndFrameToTransparent(gTasks[taskId].data[0], FALSE);
        RemoveWindow(gTasks[taskId].data[0]);
        
        UnlockPlayerFieldControls();
        ScriptContext_Enable();
        DestroyTask(taskId);
    }
}

static void Task_WaitForMessageThenReopenNatureTypeMenu(u8 taskId)
{
    if (!IsTextPrinterActive(0) && JOY_NEW(A_BUTTON | B_BUTTON))
    {
        HideFieldMessageBox();
        
        gTasks[taskId].func = Task_ShowNatureTypeMenu;
    }
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
