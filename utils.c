/******************************************************************************
 * File        : utils.c
 * Folder      : utils
 * Project     : Mart Management System
 *
 * Description:
 *     Utility functions used by all modules.
 *
 * Part 1 / 2
 *     ✓ Screen
 *     ✓ Input
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

/*===========================================================================
    SCREEN
===========================================================================*/

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen()
{
    printf("\nPress Enter to continue...");
    getchar();
}

void printLine(int length)
{
    for(int i = 0; i < length; i++)
        printf("=");

    printf("\n");
}

void printSubLine(int length)
{
    for(int i = 0; i < length; i++)
        printf("-");

    printf("\n");
}

/*===========================================================================
    INPUT
===========================================================================*/

void inputString(char text[], int size)
{
    fgets(text, size, stdin);

    text[strcspn(text, "\n")] = '\0';
}

int inputInteger()
{
    int value;

    scanf("%d", &value);

    while(getchar() != '\n');

    return value;
}

double inputDouble()
{
    double value;

    scanf("%lf", &value);

    while(getchar() != '\n');

    return value;
}

char inputChar()
{
    char ch;

    scanf("%c", &ch);

    while(getchar() != '\n');

    return ch;
}

void pressEnter()
{
    printf("\nPress ENTER to continue...");
    getchar();
}
/*===========================================================================
    VALIDATION
===========================================================================*/

int isEmptyString(const char text[])
{
    return strlen(text) == 0;
}

int isPositiveInteger(int number)
{
    return number > 0;
}

int isPositiveDouble(double number)
{
    return number >= 0.0;
}

int isValidChoice(int choice, int min, int max)
{
    return (choice >= min && choice <= max);
}

int isValidDate(const char date[])
{
    /* Expected format : dd/mm/yyyy */

    if (strlen(date) != 10)
        return 0;

    if (date[2] != '/' || date[5] != '/')
        return 0;

    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5)
            continue;

        if (!isdigit(date[i]))
            return 0;
    }

    return 1;
}

int isValidPrice(double price)
{
    return price >= 0;
}

int isValidQuantity(int quantity)
{
    return quantity >= 0;
}

/*===========================================================================
    ID GENERATION
===========================================================================*/

void generateProductID(char id[], const char prefix[], int number)
{
    sprintf(id, "%s%04d", prefix, number);
}

void generateBatchID(char batchID[], int number)
{
    sprintf(batchID, "B%03d", number);
}

void generateTransactionID(char id[], int number)
{
    sprintf(id, "TR%06d", number);
}

void generateReturnID(char id[], int number)
{
    sprintf(id, "RT%06d", number);
}

void generatePromotionID(char id[], int number)
{
    sprintf(id, "PR%04d", number);
}

/******************************************************************************
 * End of File : utils.c
 ******************************************************************************/