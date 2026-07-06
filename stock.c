/******************************************************************************
 * File        : stock.c
 * Folder      : stock
 * Project     : Mart Management System
 *
 * Part 1 / 8
 *      ✓ Stock Menu
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stock.h"
#include "../csv/csv.h"
#include "../utils/utils.h"
#include "../product/product.h"

/*===========================================================================
    GLOBAL DATA
===========================================================================*/

extern Product productList[MAX_PRODUCTS];
extern ProductType typeList[MAX_TYPES];
extern StockBatch stockList[MAX_BATCHES];

extern int totalProducts;
extern int totalTypes;
extern int totalBatches;

/*===========================================================================
    STOCK MENU
===========================================================================*/

void stockMenu()
{
    totalProducts = loadProducts(productList);
    totalTypes = loadProductTypes(typeList);
    totalBatches = loadStock(stockList);

    int choice;

    while (1)
    {
        clearScreen();

        printLine(70);
        printf("                    STOCK MANAGEMENT\n");
        printLine(70);

        printf("1. Receive Stock\n");
        printf("2. Stock Adjustment\n");
        printf("3. Current Batches\n");
        printf("4. Near Expiry\n");
        printf("5. Low Stock\n");
        printf("6. Back\n");

        printSubLine(70);

        printf("Choice : ");
        choice = inputInteger();

        switch (choice)
        {
            case 1:
                receiveStock();
                break;

            case 2:
                stockAdjustment();
                break;

            case 3:
                viewCurrentBatches();
                break;

            case 4:
                viewNearExpiryProducts();
                break;

            case 5:
                viewLowStockProducts();
                break;

            case 6:
                return;

            default:
                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    RECEIVE STOCK
===========================================================================*/

void receiveStock()
{
    char productID[MAX_ID_LENGTH];

    clearScreen();

    printLine(105);
    printf("                              RECEIVE STOCK\n");
    printLine(105);

    printf("Product ID : ");
    inputString(productID, MAX_ID_LENGTH);

    int productIndex = findProductByID(productID);

    if (productIndex == -1)
    {
        printf("\nProduct not found.\n");
        pressEnter();
        return;
    }

    printf("\nProduct Information\n");

    printSubLine(105);

    printf("ID           : %s\n",
           productList[productIndex].productID);

    printf("Product Name : %s\n",
           productList[productIndex].productName);

    printf("Current Stock: %d\n",
           productList[productIndex].totalStock);

    printSubLine(105);

    printf("%-6s %-8s %-10s %-12s %-15s\n",
           "Batch",
           "Stock",
           "Buy",
           "Expiry",
           "Supplier");

    printSubLine(105);

    for (int i = 0; i < totalBatches; i++)
    {
        if (stockList[i].status == 0)
            continue;

        if (strcmp(stockList[i].productID,
                   productID) == 0)
        {
            printf("%-6s %-8d $%-9.2lf %-12s %-15s\n",
                   stockList[i].batchID,
                   stockList[i].quantity,
                   stockList[i].buyPrice,
                   stockList[i].expiryDate,
                   stockList[i].supplier);
        }
    }

    printSubLine(105);

    StockBatch newBatch;

    strcpy(newBatch.productID,
           productID);

    generateNextBatchID(newBatch.batchID,
                        productID);

    printf("Batch ID     : %s\n",
           newBatch.batchID);

    printf("Buy Price    : ");
    newBatch.buyPrice = inputDouble();

    printf("Quantity     : ");
    newBatch.quantity = inputInteger();

    printf("Supplier     : ");
    inputString(newBatch.supplier,
                MAX_SUPPLIER_NAME);

    printf("Expiry Date  : ");
    inputString(newBatch.expiryDate,
                MAX_DATE_LENGTH);

    strcpy(newBatch.promotionID, "");

    newBatch.status = 1;

    stockList[totalBatches++] = newBatch;

    updateProductStock();

    saveStock(stockList, totalBatches);
    saveProducts(productList, totalProducts);

    printf("\nStock received successfully.\n");

    pressEnter();
}
/*===========================================================================
    STOCK ADJUSTMENT
===========================================================================*/

void stockAdjustment()
{
    char batchID[MAX_ID_LENGTH];

    clearScreen();

    printLine(105);
    printf("                            STOCK ADJUSTMENT\n");
    printLine(105);

    printf("Batch ID : ");
    inputString(batchID, MAX_ID_LENGTH);

    int index = findBatchByID(batchID);

    if (index == -1)
    {
        printf("\nBatch not found.\n");
        pressEnter();
        return;
    }

    printSubLine(105);

    printf("Product ID   : %s\n",
           stockList[index].productID);

    printf("Batch ID     : %s\n",
           stockList[index].batchID);

    printf("Current Stock: %d\n",
           stockList[index].quantity);

    printf("Buy Price    : $%.2lf\n",
           stockList[index].buyPrice);

    printf("Expiry Date  : %s\n",
           stockList[index].expiryDate);

    printf("Supplier     : %s\n",
           stockList[index].supplier);

    printSubLine(105);

    printf("1. Change Quantity\n");
    printf("2. Change Buy Price\n");
    printf("3. Change Expiry Date\n");
    printf("4. Change Supplier\n");
    printf("5. Back\n");

    printLine(105);

    printf("Choice : ");

    int choice = inputInteger();

    switch (choice)
    {
        case 1:

            printf("New Quantity : ");
            stockList[index].quantity = inputInteger();
            break;

        case 2:

            printf("New Buy Price : ");
            stockList[index].buyPrice = inputDouble();
            break;

        case 3:

            printf("New Expiry Date : ");
            inputString(stockList[index].expiryDate,
                        MAX_DATE_LENGTH);
            break;

        case 4:

            printf("New Supplier : ");
            inputString(stockList[index].supplier,
                        MAX_SUPPLIER_NAME);
            break;

        case 5:
            return;

        default:
            printf("\nInvalid choice.\n");
            pressEnter();
            return;
    }

    updateProductStock();

    saveStock(stockList, totalBatches);
    saveProducts(productList, totalProducts);

    printf("\nStock updated successfully.\n");

    pressEnter();
}
/*===========================================================================
    CURRENT BATCH LIST
===========================================================================*/

void viewCurrentBatches()
{
    while (1)
    {
        clearScreen();

        sortBatchByProduct();

        printLine(120);
        printf("                            CURRENT BATCHES\n");
        printLine(120);

        printf("%-4s %-10s %-8s %-25s %-8s %-10s %-12s %-15s %-10s\n",
               "No",
               "ID",
               "Batch",
               "Product",
               "Stock",
               "Buy",
               "Expiry",
               "Supplier",
               "Status");

        printSubLine(120);

        int no = 1;

        for (int i = 0; i < totalBatches; i++)
        {
            if (stockList[i].status == 0)
                continue;

            int productIndex = findProductByID(stockList[i].productID);

            if (productIndex == -1)
                continue;

            printf("%-4d ",
                   no++);

            printf("%-10s ",
                   stockList[i].productID);

            printf("%-8s ",
                   stockList[i].batchID);

            printf("%-25s ",
                   productList[productIndex].productName);

            printf("%-8d ",
                   stockList[i].quantity);

            printf("$%-9.2lf ",
                   stockList[i].buyPrice);

            printf("%-12s ",
                   stockList[i].expiryDate);

            printf("%-15s ",
                   stockList[i].supplier);

            if (stockList[i].promotionID[0] != '\0')
                printf("PROMO");
            else if (stockList[i].quantity <= 10)
                printf("LOW");
            else
                printf("-");

            printf("\n");
        }

        printSubLine(120);

        printf("\nStatus\n");
        printf("LOW   = Low Stock\n");
        printf("PROMO = Promotion Applied\n");

        printSubLine(120);

        printf("1. Stock Adjustment\n");
        printf("2. Refresh\n");
        printf("3. Back\n");

        printLine(120);

        printf("Choice : ");

        int choice = inputInteger();

        switch (choice)
        {
            case 1:
                stockAdjustment();
                break;

            case 2:
                break;

            case 3:
                return;

            default:
                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    NEAR EXPIRY PRODUCTS
===========================================================================*/

void viewNearExpiryProducts()
{
    while (1)
    {
        clearScreen();

        sortBatchByExpiry();

        printLine(120);
        printf("                         NEAR EXPIRY PRODUCTS\n");
        printLine(120);

        printf("%-4s %-10s %-8s %-25s %-8s %-12s %-15s %-10s\n",
               "No",
               "ID",
               "Batch",
               "Product",
               "Stock",
               "Expiry",
               "Supplier",
               "Status");

        printSubLine(120);

        int no = 1;

        for (int i = 0; i < totalBatches; i++)
        {
            if (stockList[i].status == 0)
                continue;

            int productIndex = findProductByID(stockList[i].productID);

            if (productIndex == -1)
                continue;

            /* Only display products near expiry.
               Later this will use Settings.nearExpiryWarningDays */

            printf("%-4d ",
                   no++);

            printf("%-10s ",
                   stockList[i].productID);

            printf("%-8s ",
                   stockList[i].batchID);

            printf("%-25s ",
                   productList[productIndex].productName);

            printf("%-8d ",
                   stockList[i].quantity);

            printf("%-12s ",
                   stockList[i].expiryDate);

            printf("%-15s ",
                   stockList[i].supplier);

            printf("WARNING");

            printf("\n");
        }

        printSubLine(120);

        printf("\nStatus");
        printf("\nWARNING = Near Expiry");

        printSubLine(120);

        printf("1. Refresh\n");
        printf("2. Stock Adjustment\n");
        printf("3. Back\n");

        printLine(120);

        printf("Choice : ");

        int choice = inputInteger();

        switch (choice)
        {
            case 1:
                break;

            case 2:
                stockAdjustment();
                break;

            case 3:
                return;

            default:
                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    LOW STOCK PRODUCTS
===========================================================================*/

void viewLowStockProducts()
{
    while (1)
    {
        clearScreen();

        updateProductStock();

        printLine(120);
        printf("                           LOW STOCK PRODUCTS\n");
        printLine(120);

        printf("%-4s %-10s %-25s %-10s %-10s\n",
               "No",
               "ID",
               "Product",
               "Stock",
               "Status");

        printSubLine(120);

        int no = 1;

        for (int i = 0; i < totalProducts; i++)
        {
            if (productList[i].status == 0)
                continue;

            if (productList[i].totalStock <= 10)
            {
                printf("%-4d ",
                       no++);

                printf("%-10s ",
                       productList[i].productID);

                printf("%-25s ",
                       productList[i].productName);

                printf("%-10d ",
                       productList[i].totalStock);

                printf("LOW\n");
            }
        }

        printSubLine(120);

        printf("\nStatus");
        printf("\nLOW = Stock is lower than warning quantity.");

        printSubLine(120);

        printf("1. Receive Stock\n");
        printf("2. Refresh\n");
        printf("3. Back\n");

        printLine(120);

        printf("Choice : ");

        int choice = inputInteger();

        switch (choice)
        {
            case 1:
                receiveStock();
                break;

            case 2:
                break;

            case 3:
                return;

            default:
                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    HELPER FUNCTIONS
===========================================================================*/

int findBatchByID(const char batchID[])
{
    for (int i = 0; i < totalBatches; i++)
    {
        if (stockList[i].status == 1 &&
            strcmp(stockList[i].batchID, batchID) == 0)
        {
            return i;
        }
    }

    return -1;
}

/*---------------------------------------------------------------------------*/

int findProductBatch(const char productID[],
                     const char batchID[])
{
    for (int i = 0; i < totalBatches; i++)
    {
        if (stockList[i].status == 1 &&
            strcmp(stockList[i].productID, productID) == 0 &&
            strcmp(stockList[i].batchID, batchID) == 0)
        {
            return i;
        }
    }

    return -1;
}

/*---------------------------------------------------------------------------*/

void sortBatchByProduct()
{
    StockBatch temp;

    for (int i = 0; i < totalBatches - 1; i++)
    {
        for (int j = i + 1; j < totalBatches; j++)
        {
            if (strcmp(stockList[i].productID,
                       stockList[j].productID) > 0)
            {
                temp = stockList[i];
                stockList[i] = stockList[j];
                stockList[j] = temp;
            }
        }
    }
}

/*---------------------------------------------------------------------------*/

void sortBatchByExpiry()
{
    StockBatch temp;

    for (int i = 0; i < totalBatches - 1; i++)
    {
        for (int j = i + 1; j < totalBatches; j++)
        {
            if (strcmp(stockList[i].expiryDate,
                       stockList[j].expiryDate) > 0)
            {
                temp = stockList[i];
                stockList[i] = stockList[j];
                stockList[j] = temp;
            }
        }
    }
}

/*---------------------------------------------------------------------------*/

void updateProductStock()
{
    for (int i = 0; i < totalProducts; i++)
    {
        productList[i].totalStock = 0;

        for (int j = 0; j < totalBatches; j++)
        {
            if (stockList[j].status == 1 &&
                strcmp(productList[i].productID,
                       stockList[j].productID) == 0)
            {
                productList[i].totalStock += stockList[j].quantity;
            }
        }
    }
}
/*---------------------------------------------------------------------------*/

void displayBatchTable()
{
    viewCurrentBatches();
}

/*---------------------------------------------------------------------------*/

void displayNearExpiryTable()
{
    viewNearExpiryProducts();
}

/*---------------------------------------------------------------------------*/

void displayLowStockTable()
{
    viewLowStockProducts();
}

/******************************************************************************
 * End of File : stock.c
 ******************************************************************************/