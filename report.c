/******************************************************************************
 * File        : report.c
 * Folder      : report
 * Project     : Mart Management System
 *
 * Part 1 / 8
 *      ✓ Report Menu
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "report.h"
#include "../csv/csv.h"
#include "../utils/utils.h"
#include "../product/product.h"
#include "../stock/stock.h"

/*===========================================================================
    GLOBAL DATA
===========================================================================*/

extern Product productList[MAX_PRODUCTS];
extern StockBatch stockList[MAX_BATCHES];

extern Transaction transactionList[MAX_TRANSACTIONS];
extern TransactionItem transactionItemList[MAX_TRANSACTION_ITEMS];

extern int totalProducts;
extern int totalBatches;

extern int totalTransactions;
extern int totalTransactionItems;

/*===========================================================================
    REPORT MENU
===========================================================================*/

void reportMenu()
{
    totalProducts = loadProducts(productList);
    totalBatches = loadStock(stockList);

    totalTransactions = loadTransactions(transactionList);
    totalTransactionItems = loadTransactionItems(transactionItemList);

    int choice;

    while (1)
    {
        clearScreen();

        printLine(70);
        printf("                    REPORT MANAGEMENT\n");
        printLine(70);

        printf("1. Sales Report\n");
        printf("2. Product Report\n");
        printf("3. Profit Report\n");
        printf("4. Inventory Report\n");
        printf("5. Back\n");

        printSubLine(70);

        printf("Choice : ");
        choice = inputInteger();

        switch (choice)
        {
            case 1:
                salesReport();
                break;

            case 2:
                productReport();
                break;

            case 3:
                profitReport();
                break;

            case 4:
                inventoryReport();
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
    SALES REPORT
===========================================================================*/

void salesReport()
{
    while (1)
    {
        clearScreen();

        printLine(120);
        printf("                           SALES REPORT\n");
        printLine(120);

        printf("%-4s %-12s %-12s %-10s %-10s %-10s %-10s %-10s\n",
               "No",
               "Transaction",
               "Date",
               "Time",
               "Total",
               "Received",
               "Change",
               "Profit");

        printSubLine(120);

        int no = 1;

        for (int i = 0; i < totalTransactions; i++)
        {
            if (transactionList[i].status == 0)
                continue;

            printf("%-4d ",
                   no++);

            printf("%-12s ",
                   transactionList[i].transactionID);

            printf("%-12s ",
                   transactionList[i].date);

            printf("%-10s ",
                   transactionList[i].time);

            printf("$%-9.2lf ",
                   transactionList[i].total);

            printf("$%-9.2lf ",
                   transactionList[i].received);

            printf("$%-9.2lf ",
                   transactionList[i].change);

            printf("$%.2lf\n",
                   transactionList[i].totalProfit);
        }

        printSubLine(120);

        printf("Revenue : $%.2lf\n",
               calculateRevenue());

        printf("Cost    : $%.2lf\n",
               calculateCost());

        printf("Profit  : $%.2lf\n",
               calculateTotalProfit());

        printLine(120);

        printf("1. Today\n");
        printf("2. Specific Date\n");
        printf("3. Date Range\n");
        printf("4. Transaction ID\n");
        printf("5. Back\n");

        printSubLine(120);

        printf("Choice : ");

        int choice = inputInteger();

        switch (choice)
        {
            case 1:
                reportByToday();
                break;

            case 2:
                reportByDate();
                break;

            case 3:
                reportByDateRange();
                break;

            case 4:
                reportByTransactionID();
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
    PRODUCT REPORT
===========================================================================*/

void productReport()
{
    while (1)
    {
        clearScreen();

        updateProductStock();

        printLine(125);
        printf("                          PRODUCT REPORT\n");
        printLine(125);

        printf("%-4s %-10s %-25s %-10s %-10s %-10s %-10s\n",
               "No",
               "ID",
               "Product",
               "Stock",
               "Sold",
               "Revenue",
               "Profit");

        printSubLine(125);

        int no = 1;

        for (int i = 0; i < totalProducts; i++)
        {
            if (productList[i].status == 0)
                continue;

            double revenue = 0;
            double profit = 0;
            int sold = 0;

            for (int j = 0; j < totalTransactionItems; j++)
            {
                if (strcmp(transactionItemList[j].productID,
                           productList[i].productID) == 0)
                {
                    sold += transactionItemList[j].quantity;

                    revenue +=
                        transactionItemList[j].finalPrice *
                        transactionItemList[j].quantity;

                    profit +=
                        transactionItemList[j].profit;
                }
            }

            printf("%-4d ",
                   no++);

            printf("%-10s ",
                   productList[i].productID);

            printf("%-25s ",
                   productList[i].productName);

            printf("%-10d ",
                   productList[i].totalStock);

            printf("%-10d ",
                   sold);

            printf("$%-9.2lf ",
                   revenue);

            printf("$%.2lf\n",
                   profit);
        }

        printSubLine(125);

        printf("1. Refresh\n");
        printf("2. Back\n");

        printLine(125);

        printf("Choice : ");

        int choice = inputInteger();

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
    PROFIT REPORT
===========================================================================*/

void profitReport()
{
    while (1)
    {
        clearScreen();

        printLine(120);
        printf("                          PROFIT REPORT\n");
        printLine(120);

        printf("%-4s %-10s %-25s %-10s %-10s %-10s %-10s\n",
               "No",
               "ID",
               "Product",
               "Revenue",
               "Cost",
               "Profit",
               "Margin");

        printSubLine(120);

        int no = 1;

        double totalRevenue = 0;
        double totalCost = 0;
        double totalProfit = 0;

        for (int i = 0; i < totalProducts; i++)
        {
            if (productList[i].status == 0)
                continue;

            double revenue = 0;
            double cost = 0;
            double profit = 0;

            for (int j = 0; j < totalTransactionItems; j++)
            {
                if (strcmp(transactionItemList[j].productID,
                           productList[i].productID) == 0)
                {
                    revenue += transactionItemList[j].finalPrice *
                               transactionItemList[j].quantity;

                    cost += transactionItemList[j].buyPrice *
                            transactionItemList[j].quantity;

                    profit += transactionItemList[j].profit;
                }
            }

            double margin = 0;

            if (revenue > 0)
                margin = (profit / revenue) * 100;

            printf("%-4d ",
                   no++);

            printf("%-10s ",
                   productList[i].productID);

            printf("%-25s ",
                   productList[i].productName);

            printf("$%-9.2lf ",
                   revenue);

            printf("$%-9.2lf ",
                   cost);

            printf("$%-9.2lf ",
                   profit);

            printf("%.2lf%%\n",
                   margin);

            totalRevenue += revenue;
            totalCost += cost;
            totalProfit += profit;
        }

        printSubLine(120);

        printf("Total Revenue : $%.2lf\n", totalRevenue);
        printf("Total Cost    : $%.2lf\n", totalCost);
        printf("Total Profit  : $%.2lf\n", totalProfit);

        printLine(120);

        printf("1. Refresh\n");
        printf("2. Back\n");

        printSubLine(120);

        printf("Choice : ");

        int choice = inputInteger();

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
    INVENTORY REPORT
===========================================================================*/

void inventoryReport()
{
    while (1)
    {
        clearScreen();

        updateProductStock();

        printLine(130);
        printf("                         INVENTORY REPORT\n");
        printLine(130);

        printf("%-4s %-10s %-25s %-8s %-10s %-10s %-12s %-10s\n",
               "No",
               "ID",
               "Product",
               "Batch",
               "Stock",
               "Buy",
               "Expiry",
               "Status");

        printSubLine(130);

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

            printf("%-25s ",
                   productList[productIndex].productName);

            printf("%-8s ",
                   stockList[i].batchID);

            printf("%-10d ",
                   stockList[i].quantity);

            printf("$%-9.2lf ",
                   stockList[i].buyPrice);

            printf("%-12s ",
                   stockList[i].expiryDate);

            if (stockList[i].promotionID[0] != '\0')
                printf("PROMO");
            else if (stockList[i].quantity <= 10)
                printf("LOW");
            else
                printf("NORMAL");

            printf("\n");
        }

        printSubLine(130);

        printf("Status\n");
        printf("LOW    = Low Stock\n");
        printf("PROMO  = Promotion Applied\n");
        printf("NORMAL = Normal Stock\n");

        printLine(130);

        printf("1. Refresh\n");
        printf("2. Back\n");

        printSubLine(130);

        printf("Choice : ");

        int choice = inputInteger();

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
    SEARCH & FILTER
===========================================================================*/

void reportByToday()
{
    printf("\nFeature will use current system date.\n");
    pressEnter();
}

/*---------------------------------------------------------------------------*/

void reportByDate()
{
    char date[MAX_DATE_LENGTH];

    clearScreen();

    printLine(90);
    printf("                     REPORT BY DATE\n");
    printLine(90);

    printf("Date (dd/mm/yyyy) : ");
    inputString(date, MAX_DATE_LENGTH);

    printSubLine(90);

    for (int i = 0; i < totalTransactions; i++)
    {
        if (strcmp(transactionList[i].date, date) == 0)
        {
            printf("%-12s %-12s %-10s $%.2lf\n",
                   transactionList[i].transactionID,
                   transactionList[i].date,
                   transactionList[i].time,
                   transactionList[i].total);
        }
    }

    pressEnter();
}

/*---------------------------------------------------------------------------*/

void reportByDateRange()
{
    char startDate[MAX_DATE_LENGTH];
    char endDate[MAX_DATE_LENGTH];

    clearScreen();

    printLine(90);
    printf("                  REPORT BY DATE RANGE\n");
    printLine(90);

    printf("Start Date : ");
    inputString(startDate, MAX_DATE_LENGTH);

    printf("End Date   : ");
    inputString(endDate, MAX_DATE_LENGTH);

    printSubLine(90);

    for (int i = 0; i < totalTransactions; i++)
    {
        if (strcmp(transactionList[i].date, startDate) >= 0 &&
            strcmp(transactionList[i].date, endDate) <= 0)
        {
            printf("%-12s %-12s %-10s $%.2lf\n",
                   transactionList[i].transactionID,
                   transactionList[i].date,
                   transactionList[i].time,
                   transactionList[i].total);
        }
    }

    pressEnter();
}

/*---------------------------------------------------------------------------*/

void reportByTransactionID()
{
    char id[MAX_ID_LENGTH];

    clearScreen();

    printLine(90);
    printf("                REPORT BY TRANSACTION ID\n");
    printLine(90);

    printf("Transaction ID : ");
    inputString(id, MAX_ID_LENGTH);

    printSubLine(90);

    for (int i = 0; i < totalTransactions; i++)
    {
        if (strcmp(transactionList[i].transactionID, id) == 0)
        {
            printf("Transaction : %s\n",
                   transactionList[i].transactionID);

            printf("Date        : %s\n",
                   transactionList[i].date);

            printf("Time        : %s\n",
                   transactionList[i].time);

            printf("Subtotal    : $%.2lf\n",
                   transactionList[i].subtotal);

            printf("Discount    : $%.2lf\n",
                   transactionList[i].discount);

            printf("Total       : $%.2lf\n",
                   transactionList[i].total);

            printf("Profit      : $%.2lf\n",
                   transactionList[i].totalProfit);

            break;
        }
    }

    pressEnter();
}
/*===========================================================================
    HELPER FUNCTIONS
===========================================================================*/

double calculateRevenue()
{
    double revenue = 0;

    for (int i = 0; i < totalTransactions; i++)
    {
        if (transactionList[i].status == 1)
        {
            revenue += transactionList[i].total;
        }
    }

    return revenue;
}

/*---------------------------------------------------------------------------*/

double calculateCost()
{
    double cost = 0;

    for (int i = 0; i < totalTransactionItems; i++)
    {
        cost += transactionItemList[i].buyPrice *
                transactionItemList[i].quantity;
    }

    return cost;
}

/*---------------------------------------------------------------------------*/

double calculateTotalProfit()
{
    return calculateRevenue() - calculateCost();
}

/*---------------------------------------------------------------------------*/

void displayTransactionTable()
{
    salesReport();
}

/*---------------------------------------------------------------------------*/

void displayProductReport()
{
    productReport();
}

/*---------------------------------------------------------------------------*/

void displayInventoryReport()
{
    inventoryReport();
}
/******************************************************************************
 * End of File : report.c
 ******************************************************************************/