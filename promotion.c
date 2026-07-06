/******************************************************************************
 * File        : promotion.c
 * Folder      : promotion
 * Project     : Mart Management System
 *
 * Part 1 / 9
 *      ✓ Promotion Menu
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "promotion.h"
#include "../product/product.h"
#include "../csv/csv.h"
#include "../utils/utils.h"

/*===========================================================================
    GLOBAL DATA
===========================================================================*/

extern Product productList[MAX_PRODUCTS];
extern StockBatch stockList[MAX_BATCHES];

extern Promotion promotionList[MAX_PROMOTIONS];
extern PromotionItem promotionItemList[MAX_PROMOTION_ITEMS];

extern int totalProducts;
extern int totalBatches;

extern int totalPromotions;
extern int totalPromotionItems;

/*===========================================================================
    PROMOTION MENU
===========================================================================*/

void promotionMenu()
{
    totalProducts = loadProducts(productList);
    totalBatches = loadStock(stockList);

    totalPromotions = loadPromotions(promotionList);
    totalPromotionItems = loadPromotionItems(promotionItemList);

    int choice;

    while (1)
    {
        clearScreen();

        printLine(75);
        printf("                  PROMOTION MANAGEMENT\n");
        printLine(75);

        printf("1. View Promotions\n");
        printf("2. Create Promotion\n");
        printf("3. Edit Promotion\n");
        printf("4. Delete Promotion\n");
        printf("5. Back\n");

        printSubLine(75);

        printf("Choice : ");
        choice = inputInteger();

        switch (choice)
        {
            case 1:
                viewPromotions();
                break;

            case 2:
                createPromotion();
                break;

            case 3:
                editPromotion();
                break;

            case 4:
                deletePromotion();
                break;

            case 5:
                return;

            default:
                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    VIEW PROMOTIONS
===========================================================================*/

void viewPromotions()
{
    while (1)
    {
        clearScreen();

        printLine(125);
        printf("                           PROMOTION LIST\n");
        printLine(125);

        printf("%-4s %-8s %-22s %-15s %-12s %-12s %-8s %-8s %-10s\n",
               "No",
               "ID",
               "Promotion",
               "Type",
               "Start",
               "End",
               "Priority",
               "Combine",
               "Status");

        printSubLine(125);

        int no = 1;

        for (int i = 0; i < totalPromotions; i++)
        {
            if (promotionList[i].status == 0)
                continue;

            printf("%-4d ",
                   no++);

            printf("%-8s ",
                   promotionList[i].promotionID);

            printf("%-22s ",
                   promotionList[i].promotionName);

            printf("%-15s ",
                   promotionList[i].promotionType);

            printf("%-12s ",
                   promotionList[i].startDate);

            printf("%-12s ",
                   promotionList[i].endDate);

            printf("%-8d ",
                   promotionList[i].priority);

            if (promotionList[i].canCombine)
                printf("%-8s ", "YES");
            else
                printf("%-8s ", "NO");

            if (promotionList[i].status == 1)
                printf("ACTIVE");
            else
                printf("INACTIVE");

            printf("\n");
        }

        printSubLine(125);

        printf("\nEnter Promotion No. = View Products");

        printf("\n\n1. Refresh");
        printf("\n2. Back");

        printLine(125);

        printf("Choice : ");

        int choice = inputInteger();

        if (choice >= 1 && choice < no)
        {
            displayPromotionProducts(
                promotionList[choice - 1].promotionID);

            continue;
        }

        switch (choice)
        {
            case 1:
                break;

            case 2:
                return;

            default:
                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    CREATE PROMOTION
===========================================================================*/

void createPromotion()
{
    Promotion newPromotion;

    clearScreen();

    printLine(100);
    printf("                         CREATE PROMOTION\n");
    printLine(100);

    printf("Promotion ID : ");
    inputString(newPromotion.promotionID,
                MAX_ID_LENGTH);

    if (promotionExists(newPromotion.promotionID))
    {
        printf("\nPromotion ID already exists.\n");
        pressEnter();
        return;
    }

    printf("Promotion Name : ");
    inputString(newPromotion.promotionName,
                MAX_PROMOTION_NAME);

    printf("\nPromotion Type\n");
    printf("1. Percentage\n");
    printf("2. Fixed Amount\n");
    printf("3. Buy X Get Y\n");

    printf("\nChoice : ");

    int choice = inputInteger();

    switch(choice)
    {
        case 1:
            strcpy(newPromotion.promotionType,
                   "Percentage");
            break;

        case 2:
            strcpy(newPromotion.promotionType,
                   "Fixed");
            break;

        case 3:
            strcpy(newPromotion.promotionType,
                   "Buy X Get Y");
            break;

        default:
            printf("\nInvalid choice.");
            pressEnter();
            return;
    }

    printf("\nApply Method\n");
    printf("1. Product\n");
    printf("2. Product Type\n");
    printf("3. Near Expiry\n");
    printf("4. Low Stock\n");
    printf("5. High Profit\n");

    printf("\nChoice : ");

    choice = inputInteger();

    switch(choice)
    {
        case 1:
            strcpy(newPromotion.applyMethod,
                   "Product");
            break;

        case 2:
            strcpy(newPromotion.applyMethod,
                   "Product Type");
            break;

        case 3:
            strcpy(newPromotion.applyMethod,
                   "Near Expiry");
            break;

        case 4:
            strcpy(newPromotion.applyMethod,
                   "Low Stock");
            break;

        case 5:
            strcpy(newPromotion.applyMethod,
                   "High Profit");
            break;

        default:
            printf("\nInvalid choice.");
            pressEnter();
            return;
    }

    printf("Start Date : ");
    inputString(newPromotion.startDate,
                MAX_DATE_LENGTH);

    printf("End Date   : ");
    inputString(newPromotion.endDate,
                MAX_DATE_LENGTH);

    printf("Priority : ");
    newPromotion.priority = inputInteger();

    printf("\nAllow Combine ?");
    printf("\n1. Yes");
    printf("\n2. No");

    printf("\n\nChoice : ");

    choice = inputInteger();

    newPromotion.canCombine = (choice == 1);

    newPromotion.status = 1;

    promotionList[totalPromotions++] = newPromotion;

    savePromotions(promotionList,
                   totalPromotions);

    addProductsToPromotion();

    printf("\nPromotion created successfully.\n");

    pressEnter();
}
/*===========================================================================
    ADD PRODUCTS TO PROMOTION
===========================================================================*/

void addProductsToPromotion()
{
    while (1)
    {
        clearScreen();

        printLine(120);
        printf("                     ADD PRODUCTS TO PROMOTION\n");
        printLine(120);

        printf("%-4s %-10s %-25s %-10s %-10s\n",
               "No",
               "ID",
               "Product",
               "Sell",
               "Stock");

        printSubLine(120);

        int no = 1;

        for (int i = 0; i < totalProducts; i++)
        {
            if (productList[i].status == 0)
                continue;

            printf("%-4d ",
                   no++);

            printf("%-10s ",
                   productList[i].productID);

            printf("%-25s ",
                   productList[i].productName);

            printf("$%-9.2lf ",
                   productList[i].sellPrice);

            printf("%-10d\n",
                   productList[i].totalStock);
        }

        printSubLine(120);

        printf("Enter Product No. (0 = Finish) : ");

        int choice = inputInteger();

        if (choice == 0)
            return;

        if (choice < 1 || choice >= no)
        {
            printf("\nInvalid Product.\n");
            pressEnter();
            continue;
        }

        PromotionItem item;

        strcpy(item.promotionID,
               promotionList[totalPromotions - 1].promotionID);

        strcpy(item.productID,
               productList[choice - 1].productID);

        int duplicate = 0;

        for (int i = 0; i < totalPromotionItems; i++)
        {
            if (strcmp(item.promotionID,
                       promotionItemList[i].promotionID) == 0 &&
                strcmp(item.productID,
                       promotionItemList[i].productID) == 0)
            {
                duplicate = 1;
                break;
            }
        }

        if (duplicate)
        {
            printf("\nProduct already added.\n");
            pressEnter();
            continue;
        }

        promotionItemList[totalPromotionItems++] = item;

        savePromotionItems(promotionItemList,
                           totalPromotionItems);

        printf("\nProduct added successfully.\n");

        pressEnter();
    }
}
/*===========================================================================
    EDIT PROMOTION
===========================================================================*/

void editPromotion()
{
    char promotionID[MAX_ID_LENGTH];

    clearScreen();

    printLine(100);
    printf("                          EDIT PROMOTION\n");
    printLine(100);

    printf("Promotion ID : ");
    inputString(promotionID, MAX_ID_LENGTH);

    int index = findPromotionByID(promotionID);

    if (index == -1)
    {
        printf("\nPromotion not found.\n");
        pressEnter();
        return;
    }

    printf("\nCurrent Information\n");

    printSubLine(100);

    printf("Promotion Name : %s\n",
           promotionList[index].promotionName);

    printf("Type           : %s\n",
           promotionList[index].promotionType);

    printf("Apply Method   : %s\n",
           promotionList[index].applyMethod);

    printf("Start Date     : %s\n",
           promotionList[index].startDate);

    printf("End Date       : %s\n",
           promotionList[index].endDate);

    printf("Priority       : %d\n",
           promotionList[index].priority);

    printf("Combine        : %s\n",
           promotionList[index].canCombine ? "YES" : "NO");

    printSubLine(100);

    printf("New Promotion Name : ");
    inputString(promotionList[index].promotionName,
                MAX_PROMOTION_NAME);

    printf("New Start Date : ");
    inputString(promotionList[index].startDate,
                MAX_DATE_LENGTH);

    printf("New End Date : ");
    inputString(promotionList[index].endDate,
                MAX_DATE_LENGTH);

    printf("New Priority : ");
    promotionList[index].priority = inputInteger();

    printf("\nAllow Combine ?");
    printf("\n1. Yes");
    printf("\n2. No");

    printf("\n\nChoice : ");

    int choice = inputInteger();

    promotionList[index].canCombine = (choice == 1);

    printf("\nEdit Products?");
    printf("\n1. Yes");
    printf("\n2. No");

    printf("\n\nChoice : ");

    choice = inputInteger();

    if (choice == 1)
    {
        addProductsToPromotion();
    }

    savePromotions(promotionList,
                   totalPromotions);

    printf("\nPromotion updated successfully.\n");

    pressEnter();
}
/*===========================================================================
    DELETE PROMOTION
===========================================================================*/

void deletePromotion()
{
    char promotionID[MAX_ID_LENGTH];

    clearScreen();

    printLine(100);
    printf("                         DELETE PROMOTION\n");
    printLine(100);

    printf("Promotion ID : ");
    inputString(promotionID, MAX_ID_LENGTH);

    int index = findPromotionByID(promotionID);

    if (index == -1)
    {
        printf("\nPromotion not found.\n");
        pressEnter();
        return;
    }

    printf("\nPromotion Information\n");

    printSubLine(100);

    printf("Promotion ID   : %s\n",
           promotionList[index].promotionID);

    printf("Promotion Name : %s\n",
           promotionList[index].promotionName);

    printf("Type           : %s\n",
           promotionList[index].promotionType);

    printSubLine(100);

    printf("1. Delete Promotion\n");
    printf("2. Cancel\n");

    printLine(100);

    printf("Choice : ");

    int choice = inputInteger();

    if (choice != 1)
        return;

    promotionList[index].status = 0;

    for (int i = 0; i < totalPromotionItems; i++)
    {
        if (strcmp(promotionItemList[i].promotionID,
                   promotionID) == 0)
        {
            strcpy(promotionItemList[i].promotionID, "");
            strcpy(promotionItemList[i].productID, "");
        }
    }

    savePromotions(promotionList,
                   totalPromotions);

    savePromotionItems(promotionItemList,
                       totalPromotionItems);

    printf("\nPromotion deleted successfully.\n");

    pressEnter();
}
/*===========================================================================
    PROMOTION VALIDATION
===========================================================================*/

int verifyNegativeProfit(double newPrice,
                         double buyPrice)
{
    double profit = newPrice - buyPrice;

    if (profit >= 0)
        return 1;

    clearScreen();

    printLine(90);
    printf("                    PROFIT WARNING\n");
    printLine(90);

    printf("Buy Price     : $%.2lf\n", buyPrice);
    printf("Selling Price : $%.2lf\n", newPrice);
    printf("Profit        : $%.2lf\n", profit);

    printSubLine(90);

    printf("This promotion will lose money.\n");
    printf("Please verify before saving.\n");

    printSubLine(90);

    printf("1. Continue\n");
    printf("2. Cancel\n");

    printLine(90);

    printf("Choice : ");

    int choice = inputInteger();

    return (choice == 1);
}

/*---------------------------------------------------------------------------*/

int promotionExists(const char promotionID[])
{
    for (int i = 0; i < totalPromotions; i++)
    {
        if (promotionList[i].status == 1 &&
            strcmp(promotionList[i].promotionID,
                   promotionID) == 0)
        {
            return 1;
        }
    }

    return 0;
}

/*---------------------------------------------------------------------------*/

int findPromotionByID(const char promotionID[])
{
    for (int i = 0; i < totalPromotions; i++)
    {
        if (promotionList[i].status == 1 &&
            strcmp(promotionList[i].promotionID,
                   promotionID) == 0)
        {
            return i;
        }
    }

    return -1;
}
/*===========================================================================
    HELPER FUNCTIONS
===========================================================================*/

void calculatePromotionPrice()
{
    /* Reserved for future calculation engine.
       Different promotion types can be calculated here. */
}

/*---------------------------------------------------------------------------*/

void displayPromotionTable()
{
    viewPromotions();
}

/*---------------------------------------------------------------------------*/

void displayPromotionProducts(const char promotionID[])
{
    clearScreen();

    printLine(120);
    printf("                    PROMOTION PRODUCTS\n");
    printLine(120);

    printf("%-4s %-10s %-25s %-10s\n",
           "No",
           "ID",
           "Product",
           "Sell");

    printSubLine(120);

    int no = 1;

    for (int i = 0; i < totalPromotionItems; i++)
    {
        if (strcmp(promotionItemList[i].promotionID,
                   promotionID) != 0)
            continue;

        int index = findProductByID(
            promotionItemList[i].productID);

        if (index == -1)
            continue;

        printf("%-4d ",
               no++);

        printf("%-10s ",
               productList[index].productID);

        printf("%-25s ",
               productList[index].productName);

        printf("$%.2lf\n",
               productList[index].sellPrice);
    }

    printSubLine(120);

    printf("1. Remove Product\n");
    printf("2. Back\n");

    printLine(120);

    printf("Choice : ");

    int choice = inputInteger();

    if (choice == 1)
    {
        char productID[MAX_ID_LENGTH];

        printf("Product ID : ");
        inputString(productID, MAX_ID_LENGTH);

        removeProductFromPromotion(
            promotionID,
            productID);
    }
}

/*---------------------------------------------------------------------------*/

void removeProductFromPromotion(const char promotionID[],
                                const char productID[])
{
    for (int i = 0; i < totalPromotionItems; i++)
    {
        if (strcmp(promotionItemList[i].promotionID,
                   promotionID) == 0 &&
            strcmp(promotionItemList[i].productID,
                   productID) == 0)
        {
            for (int j = i; j < totalPromotionItems - 1; j++)
            {
                promotionItemList[j] =
                    promotionItemList[j + 1];
            }

            totalPromotionItems--;

            savePromotionItems(
                promotionItemList,
                totalPromotionItems);

            printf("\nProduct removed successfully.\n");

            pressEnter();

            return;
        }
    }

    printf("\nProduct not found in this promotion.\n");

    pressEnter();
}
/******************************************************************************
 * End of File : promotion.c
 ******************************************************************************/