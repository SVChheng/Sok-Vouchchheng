/******************************************************************************
 * File        : ui.c
 * Folder      : ui
 * Project     : Mart Management System
 *
 * Description:
 *     Implementation of the shared UI toolkit declared in ui.h.
 *
 * Part 1 / 1
 *     - Color
 *     - Date / Time
 *     - Screen layout (header, section title, footer hint)
 *     - Messages (success / error / warning / info / confirm)
 *     - Formatting helpers (currency, tags)
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ui.h"
#include "../utils/utils.h"
#include "../csv/csv.h"     /* for the shared "setting" (store name, currency) */

#ifdef _WIN32
    #include <windows.h>
#endif

/*===========================================================================
    COLOR
===========================================================================*/

void uiSetColor(UiColor color)
{
#ifdef _WIN32

    WORD attr;

    switch (color)
    {
        case UI_RED:
            attr = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;

        case UI_GREEN:
            attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;

        case UI_YELLOW:
            attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;

        case UI_CYAN:
            attr = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;

        case UI_GREY:
            attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        default:
            attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);

#else

    const char *code;

    switch (color)
    {
        case UI_RED:    code = "\033[31m"; break;
        case UI_GREEN:  code = "\033[32m"; break;
        case UI_YELLOW: code = "\033[33m"; break;
        case UI_CYAN:   code = "\033[36m"; break;
        case UI_GREY:   code = "\033[90m"; break;
        default:        code = "\033[97m"; break;
    }

    printf("%s", code);

#endif
}

void uiResetColor(void)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                             FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    printf("\033[0m");
#endif
}

/*===========================================================================
    DATE / TIME
===========================================================================*/

void uiGetDateTimeNow(char dateOut[], char timeOut[])
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    sprintf(dateOut, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    sprintf(timeOut, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
}

/*===========================================================================
    SCREEN LAYOUT
===========================================================================*/

void uiHeader(const char *pageTitle)
{
    clearScreen();

    char date[MAX_DATE_LENGTH];
    char clock[MAX_TIME_LENGTH];

    uiGetDateTimeNow(date, clock);

    const char *storeName = (setting.storeName[0] != '\0') ? setting.storeName
                                                            : "MART MANAGEMENT SYSTEM";

    char rightText[40];
    sprintf(rightText, "%s  %s", date, clock);

    int leftLen  = (int)strlen(storeName) + 1;      /* +1 for leading space */
    int rightLen = (int)strlen(rightText);
    int gap      = UI_WIDTH - leftLen - rightLen;

    if (gap < 1)
        gap = 1;

    printLine(UI_WIDTH);

    uiSetColor(UI_CYAN);
    printf(" %s", storeName);
    uiResetColor();

    printf("%*s%s\n", gap, "", rightText);

    printSubLine(UI_WIDTH);

    uiSetColor(UI_YELLOW);
    printf(" %s\n", pageTitle);
    uiResetColor();

    printLine(UI_WIDTH);
}

void uiSectionTitle(const char *sectionTitle)
{
    printSubLine(UI_WIDTH);

    uiSetColor(UI_YELLOW);
    printf(" %s\n", sectionTitle);
    uiResetColor();

    printSubLine(UI_WIDTH);
}

void uiFooterHint(const char *hintText)
{
    uiSetColor(UI_GREY);
    printf("\n%s\n", hintText);
    uiResetColor();
}

/*===========================================================================
    MESSAGES
===========================================================================*/

void uiSuccess(const char *message)
{
    uiSetColor(UI_GREEN);
    printf("\n[OK] %s\n", message);
    uiResetColor();
}

void uiError(const char *message)
{
    uiSetColor(UI_RED);
    printf("\n[ERROR] %s\n", message);
    uiResetColor();
}

void uiWarning(const char *message)
{
    uiSetColor(UI_YELLOW);
    printf("\n[WARNING] %s\n", message);
    uiResetColor();
}

void uiInfo(const char *message)
{
    uiSetColor(UI_CYAN);
    printf("\n[INFO] %s\n", message);
    uiResetColor();
}

int uiConfirm(const char *question)
{
    char answer[10];

    while (1)
    {
        printf("\n%s (Y/N) : ", question);
        inputString(answer, sizeof(answer));

        if (answer[0] == 'Y' || answer[0] == 'y')
            return 1;

        if (answer[0] == 'N' || answer[0] == 'n')
            return 0;

        uiError("Please type Y or N.");
    }
}

/*===========================================================================
    FORMATTING HELPERS
===========================================================================*/

void uiFormatCurrency(double amount, char out[], size_t outSize)
{
    const char *symbol = (setting.currency[0] != '\0') ? setting.currency : "$";

    snprintf(out, outSize, "%s%.2f", symbol, amount);
}

void uiPrintTag(const char *label, UiColor color)
{
    uiSetColor(color);
    printf("%s", label);
    uiResetColor();
}

/******************************************************************************
 * End of File : ui.c
 ******************************************************************************/