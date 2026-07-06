/******************************************************************************
 * File        : ui.h
 * Folder      : ui
 * Project     : Mart Management System
 *
 * Description:
 *     Shared UI toolkit. Every module (product, stock, sales, promotion,
 *     report) calls into these functions instead of printing its own
 *     ad-hoc "====" headers, so the whole app looks and behaves the
 *     same everywhere - like it was built by one team, not five.
 *
 *     This file does NOT duplicate anything from utils.h. It builds on
 *     top of it (clearScreen, printLine, inputString, ...) the same
 *     way product.c/stock.c already do.
 ******************************************************************************/

#ifndef UI_H
#define UI_H

#include <stddef.h>

#define UI_WIDTH   80   /* standard screen width used across the app */

/*===========================================================================
    COLOR
===========================================================================*/

typedef enum
{
    UI_DEFAULT = 0,
    UI_GREY,
    UI_RED,
    UI_GREEN,
    UI_YELLOW,
    UI_CYAN

} UiColor;

void uiSetColor(UiColor color);
void uiResetColor(void);

/*===========================================================================
    DATE / TIME
===========================================================================*/

void uiGetDateTimeNow(char dateOut[], char timeOut[]);

/*===========================================================================
    SCREEN LAYOUT
===========================================================================*/

/* Full page header: clears the screen, shows store name + live date/time,
   then the page title. Use this once at the top of every menu function. */
void uiHeader(const char *pageTitle);

/* A smaller divider used mid-screen, e.g. between steps of a wizard. */
void uiSectionTitle(const char *sectionTitle);

/* Small grey hint line, e.g. "0. Back" reminders or usage tips. */
void uiFooterHint(const char *hintText);

/*===========================================================================
    MESSAGES
===========================================================================*/

void uiSuccess(const char *message);
void uiError(const char *message);
void uiWarning(const char *message);
void uiInfo(const char *message);

/* Yes / No confirmation. Returns 1 for Yes, 0 for No. Keeps asking
   until the owner types a valid answer. */
int uiConfirm(const char *question);

/*===========================================================================
    FORMATTING HELPERS
===========================================================================*/

/* Formats amount using the currency symbol from settings.csv,
   e.g. 12.5 -> "$12.50". "out" must be at least 20 bytes. */
void uiFormatCurrency(double amount, char out[], size_t outSize);

/* Prints a short colored tag with no trailing newline - handy inside
   table rows, e.g. uiPrintTag("LOW STOCK", UI_RED); */
void uiPrintTag(const char *label, UiColor color);

#endif