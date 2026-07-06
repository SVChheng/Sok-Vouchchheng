/******************************************************************************
 * File        : main.c
 * Project     : Mart Management System
 *
 * Part 1 / 2
 *      - Include
 *      - Login (password now comes from settings.csv, masked with *)
 *      - Dashboard stat helpers
 * Part 2 / 2
 *      - main()
 *      - Home Dashboard / Main Menu
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "product/product.h"
#include "stock/stock.h"
#include "sales/sales.h"
#include "promotion/promotion.h"
#include "report/report.h"
#include "utils/utils.h"
#include "ui/ui.h"
#include "csv/csv.h"

#ifdef _WIN32
    #include <conio.h>
#endif

void mainMenu(void);

/*===========================================================================
    PASSWORD INPUT (masked with * on Windows)
===========================================================================*/

#ifdef _WIN32

static void inputPasswordMasked(char text[], int size)
{
    int i = 0;
    int ch;

    while (1)
    {
        ch = _getch();

        if (ch == '\r' || ch == '\n')
        {
            break;
        }
        else if (ch == '\b')
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else if (i < size - 1 && ch >= 32 && ch <= 126)
        {
            text[i++] = (char)ch;
            printf("*");
        }
    }

    text[i] = '\0';
    printf("\n");
}

#endif

static void readPassword(char text[], int size)
{
#ifdef _WIN32
    inputPasswordMasked(text, size);
#else
    inputString(text, size);
#endif
}

/*===========================================================================
    LOGIN
===========================================================================*/

int login()
{
    char password[MAX_PASSWORD_LENGTH];

    int attempt = 0;

    while (attempt < setting.maxLoginAttempt)
    {
        uiHeader("STORE LOGIN");

        printf(" Enter store password to continue.\n\n");
        printf(" Password : ");

        readPassword(password, sizeof(password));

        if (strcmp(password, setting.password) == 0)
        {
            uiSuccess("Login successful. Welcome back!");
            pressEnter();
            return 1;
        }

        attempt++;

        uiError("Wrong password.");
        printf(" Remaining attempt(s) : %d\n", setting.maxLoginAttempt - attempt);

        pressEnter();
    }

    uiError("Too many failed attempts. The application will now close.");

    return 0;
}

/*===========================================================================
    DASHBOARD STAT HELPERS
===========================================================================*/

static int countActiveProducts(void)
{
    int count = 0;

    for (int i = 0; i < totalProducts; i++)
        if (productList[i].status == 1)
            count++;

    return count;
}

static int countLowStock(void)
{
    int count = 0;

    for (int i = 0; i < totalProducts; i++)
        if (productList[i].status == 1 &&
            productList[i].totalStock <= setting.lowStockWarning)
            count++;

    return count;
}

static int countActivePromotions(void)
{
    int count = 0;

    for (int i = 0; i < totalPromotions; i++)
        if (promotionList[i].status == 1)
            count++;

    return count;
}

static void todaysSalesSummary(int *outCount, double *outRevenue)
{
    char today[MAX_DATE_LENGTH];
    char clock[MAX_TIME_LENGTH];

    uiGetDateTimeNow(today, clock);

    int count = 0;
    double revenue = 0.0;

    for (int i = 0; i < totalTransactions; i++)
    {
        if (transactionList[i].status == 1 &&
            strcmp(transactionList[i].date, today) == 0 &&
            strcmp(transactionList[i].transactionType, "SALE") == 0)
        {
            count++;
            revenue += transactionList[i].total;
        }
    }

    *outCount   = count;
    *outRevenue = revenue;
}

/*===========================================================================
    MAIN
===========================================================================*/

int main()
{
    loadSettings(&setting);

    if (!login())
        return 0;

    mainMenu();

    return 0;
}

/*===========================================================================
    HOME DASHBOARD / MAIN MENU
===========================================================================*/

void mainMenu()
{
    int choice;

    while (1)
    {
        /* Refresh every time we land back on the dashboard, so numbers
           are always current after coming back from any module. */
        totalProducts     = loadProducts(productList);
        totalPromotions   = loadPromotions(promotionList);
        totalTransactions = loadTransactions(transactionList);

        int todayCount;
        double todayRevenue;
        todaysSalesSummary(&todayCount, &todayRevenue);

        char revenueText[20];
        uiFormatCurrency(todayRevenue, revenueText, sizeof(revenueText));

        uiHeader("HOME DASHBOARD");

        printf(" %-30s %d\n", "Active Products", countActiveProducts());

        printf(" %-30s ", "Low Stock Alerts");

        if (countLowStock() > 0)
            uiPrintTag("!! ", UI_RED);

        printf("%d\n", countLowStock());

        printf(" %-30s %d\n", "Active Promotions", countActivePromotions());
        printf(" %-30s %d transaction(s), %s\n", "Today's Sales", todayCount, revenueText);

        printSubLine(UI_WIDTH);

        printf("\n");
        printf(" 1. Product Management\n");
        printf(" 2. Stock Management\n");
        printf(" 3. Sales Management\n");
        printf(" 4. Promotion Management\n");
        printf(" 5. Reports\n");
        printf(" 6. Exit\n");

        uiFooterHint(" Tip : Low Stock Alerts shows products at or below your low-stock setting.");

        printf("\nChoice : ");
        choice = inputInteger();

        switch (choice)
        {
            case 1:
                productMenu();
                break;

            case 2:
                stockMenu();
                break;

            case 3:
                salesMenu();
                break;

            case 4:
                promotionMenu();
                break;

            case 5:
                reportMenu();
                break;

            case 6:

                uiHeader("GOODBYE");
                printf(" Thank you for using Mart Management System.\n");
                printf(" See you next time!\n\n");

                exit(0);

            default:

                uiError("Invalid choice.");
                pressEnter();
        }
    }
}

/******************************************************************************
 * End of File : main.c
 ******************************************************************************/