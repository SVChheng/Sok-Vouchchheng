/******************************************************************************
 * File        : sales.c
 * Folder      : sales
 * Project     : Mart Management System
 *
 * Part 1 / 12
 *      ✓ Sales Menu
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sales.h"
#include "../csv/csv.h"
#include "../utils/utils.h"
#include "../product/product.h"
#include "../stock/stock.h"

/*===========================================================================
    GLOBAL DATA
===========================================================================*/

extern Product productList[MAX_PRODUCTS];
extern ProductType typeList[MAX_TYPES];
extern StockBatch stockList[MAX_BATCHES];

extern Transaction transactionList[MAX_TRANSACTIONS];
extern TransactionItem transactionItemList[MAX_TRANSACTION_ITEMS];

extern Promotion promotionList[MAX_PROMOTIONS];
extern PromotionItem promotionItemList[MAX_PROMOTION_ITEMS];

extern Settings setting;

extern int totalProducts;
extern int totalTypes;
extern int totalBatches;

extern int totalTransactions;
extern int totalTransactionItems;

extern int totalPromotions;
extern int totalPromotionItems;

/* Shopping Cart */

CartItem cart[MAX_CART_ITEMS];
int totalCart = 0;

/*===========================================================================
    SALES MENU
===========================================================================*/

void salesMenu()
{
    totalProducts = loadProducts(productList);
    totalTypes = loadProductTypes(typeList);
    totalBatches = loadStock(stockList);

    totalTransactions = loadTransactions(transactionList);
    totalTransactionItems = loadTransactionItems(transactionItemList);

    totalPromotions = loadPromotions(promotionList);
    totalPromotionItems = loadPromotionItems(promotionItemList);

    loadSettings(&setting);

    int choice;

    while (1)
    {
        clearScreen();

        printLine(70);
        printf("                    SALES MANAGEMENT\n");
        printLine(70);

        printf("1. New Sale\n");
        printf("2. Return Product\n");
        printf("3. Back\n");

        printSubLine(70);

        printf("Choice : ");
        choice = inputInteger();

        switch (choice)
        {
            case 1:
                newSale();
                break;

            case 2:
                returnProduct();
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
    NEW SALE
===========================================================================*/

void newSale()
{
    clearCart();

    while (1)
    {
        clearScreen();

        printLine(125);
        printf("                               NEW SALE\n");
        printLine(125);

        if (totalCart == 0)
        {
            printf("Shopping Cart : Empty\n");
        }
        else
        {
            printf("%-4s %-10s %-25s %-6s %-10s %-10s %-10s\n",
                   "No",
                   "ID",
                   "Product",
                   "Qty",
                   "Price",
                   "Total",
                   "Promotion");

            printSubLine(125);

            double grandTotal = 0;

            for (int i = 0; i < totalCart; i++)
            {
                printf("%-4d ",
                       i + 1);

                printf("%-10s ",
                       cart[i].productID);

                printf("%-25s ",
                       cart[i].productName);

                printf("%-6d ",
                       cart[i].quantity);

                printf("$%-9.2lf ",
                       cart[i].finalPrice);

                printf("$%-9.2lf ",
                       cart[i].subtotal);

                if(strlen(cart[i].promotionName) == 0)
                    printf("-");
                else
                    printf("%s", cart[i].promotionName);

                printf("\n");

                grandTotal += cart[i].subtotal;
            }

            printSubLine(125);

            printf("Grand Total : $%.2lf\n",
                   grandTotal);
        }

        printLine(125);

        printf("1. Add Product\n");
        printf("2. Edit Quantity\n");
        printf("3. Remove Product\n");
        printf("4. Checkout\n");
        printf("5. Cancel Sale\n");

        printSubLine(125);

        printf("Choice : ");

        int choice = inputInteger();

        switch(choice)
        {
            case 1:
                addProductToCart();
                break;

            case 2:
                editCartQuantity();
                break;

            case 3:
                removeProductFromCart();
                break;

            case 4:

                if(totalCart == 0)
                {
                    printf("\nCart is empty.\n");
                    pressEnter();
                }
                else
                {
                    checkout();
                    return;
                }

                break;

            case 5:

                clearCart();
                return;

            default:

                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    ADD PRODUCT TO CART
===========================================================================*/

void addProductToCart()
{
    char productID[MAX_ID_LENGTH];

    clearScreen();

    printLine(120);
    printf("                          ADD PRODUCT TO CART\n");
    printLine(120);

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

    printSubLine(120);

    printf("Product ID   : %s\n",
           productList[productIndex].productID);

    printf("Product Name : %s\n",
           productList[productIndex].productName);

    printf("Sell Price   : $%.2lf\n",
           productList[productIndex].sellPrice);

    if (productList[productIndex].discountPrice > 0)
        printf("Discount     : $%.2lf\n",
               productList[productIndex].discountPrice);
    else
        printf("Discount     : -\n");

    printf("Stock        : %d\n",
           productList[productIndex].totalStock);

    printSubLine(120);

    int quantity;

    printf("Quantity : ");
    quantity = inputInteger();

    if (quantity <= 0)
    {
        printf("\nInvalid quantity.\n");
        pressEnter();
        return;
    }

    if (quantity > getAvailableStock(productID))
    {
        printf("\nNot enough stock.\n");
        pressEnter();
        return;
    }

    int cartIndex = findCartItem(productID);

    if (cartIndex != -1)
    {
        cart[cartIndex].quantity += quantity;
        cart[cartIndex].subtotal =
            cart[cartIndex].quantity *
            cart[cartIndex].finalPrice;
    }
    else
    {
        strcpy(cart[totalCart].productID,
               productList[productIndex].productID);

        strcpy(cart[totalCart].productName,
               productList[productIndex].productName);

        strcpy(cart[totalCart].batchID, "");

        cart[totalCart].quantity = quantity;

        cart[totalCart].normalPrice =
            productList[productIndex].sellPrice;

        if (productList[productIndex].discountPrice > 0)
            cart[totalCart].finalPrice =
                productList[productIndex].discountPrice;
        else
            cart[totalCart].finalPrice =
                productList[productIndex].sellPrice;

        strcpy(cart[totalCart].promotionName, "");

        cart[totalCart].subtotal =
            cart[totalCart].quantity *
            cart[totalCart].finalPrice;

        cart[totalCart].profit =
            calculateProfit(&cart[totalCart]);

        totalCart++;
    }

    applyPromotion();

    printf("\nProduct added successfully.\n");

    pressEnter();
}
/*===========================================================================
    EDIT CART QUANTITY
===========================================================================*/

void editCartQuantity()
{
    if (totalCart == 0)
    {
        printf("\nShopping cart is empty.\n");
        pressEnter();
        return;
    }

    clearScreen();

    printLine(120);
    printf("                         EDIT CART QUANTITY\n");
    printLine(120);

    printf("%-4s %-10s %-25s %-8s %-10s\n",
           "No",
           "ID",
           "Product",
           "Qty",
           "Total");

    printSubLine(120);

    for (int i = 0; i < totalCart; i++)
    {
        printf("%-4d ",
               i + 1);

        printf("%-10s ",
               cart[i].productID);

        printf("%-25s ",
               cart[i].productName);

        printf("%-8d ",
               cart[i].quantity);

        printf("$%.2lf\n",
               cart[i].subtotal);
    }

    printSubLine(120);

    printf("Item No. : ");

    int item = inputInteger();

    if (item < 1 || item > totalCart)
    {
        printf("\nInvalid item.\n");
        pressEnter();
        return;
    }

    item--;

    printf("Current Quantity : %d\n",
           cart[item].quantity);

    printf("New Quantity : ");

    int quantity = inputInteger();

    if (quantity <= 0)
    {
        printf("\nInvalid quantity.\n");
        pressEnter();
        return;
    }

    if (quantity > getAvailableStock(cart[item].productID))
    {
        printf("\nNot enough stock.\n");
        pressEnter();
        return;
    }

    cart[item].quantity = quantity;

    cart[item].subtotal =
        cart[item].quantity *
        cart[item].finalPrice;

    cart[item].profit =
        calculateProfit(&cart[item]);

    applyPromotion();

    printf("\nQuantity updated successfully.\n");

    pressEnter();
}
/*===========================================================================
    REMOVE PRODUCT FROM CART
===========================================================================*/

void removeProductFromCart()
{
    if (totalCart == 0)
    {
        printf("\nShopping cart is empty.\n");
        pressEnter();
        return;
    }

    clearScreen();

    printLine(120);
    printf("                       REMOVE PRODUCT FROM CART\n");
    printLine(120);

    printf("%-4s %-10s %-25s %-8s %-10s\n",
           "No",
           "ID",
           "Product",
           "Qty",
           "Total");

    printSubLine(120);

    for (int i = 0; i < totalCart; i++)
    {
        printf("%-4d ",
               i + 1);

        printf("%-10s ",
               cart[i].productID);

        printf("%-25s ",
               cart[i].productName);

        printf("%-8d ",
               cart[i].quantity);

        printf("$%.2lf\n",
               cart[i].subtotal);
    }

    printSubLine(120);

    printf("Item No. : ");

    int item = inputInteger();

    if (item < 1 || item > totalCart)
    {
        printf("\nInvalid item.\n");
        pressEnter();
        return;
    }

    item--;

    printf("\nRemove \"%s\" ?\n",
           cart[item].productName);

    printf("\n1. Yes");
    printf("\n2. No");

    printf("\n\nChoice : ");

    int choice = inputInteger();

    if (choice != 1)
        return;

    for (int i = item; i < totalCart - 1; i++)
    {
        cart[i] = cart[i + 1];
    }

    totalCart--;

    applyPromotion();

    printf("\nProduct removed successfully.\n");

    pressEnter();
}
/*===========================================================================
    PROMOTION ENGINE
===========================================================================*/

void applyPromotion()
{
    for (int i = 0; i < totalCart; i++)
    {
        /* Reset */

        cart[i].finalPrice = cart[i].normalPrice;

        strcpy(cart[i].promotionName, "");

        /*-------------------------------------------------------
            Product Discount
        -------------------------------------------------------*/

        int productIndex = findProductByID(cart[i].productID);

        if (productIndex != -1)
        {
            if (productList[productIndex].discountPrice > 0)
            {
                cart[i].finalPrice =
                    productList[productIndex].discountPrice;

                strcpy(cart[i].promotionName,
                       "Product Discount");
            }
        }

        /*-------------------------------------------------------
            Promotion Event
        -------------------------------------------------------*/

        for (int p = 0; p < totalPromotions; p++)
        {
            if (promotionList[p].status == 0)
                continue;

            int apply = 0;

            for (int j = 0; j < totalPromotionItems; j++)
            {
                if (strcmp(promotionItemList[j].promotionID,
                           promotionList[p].promotionID) == 0 &&
                    strcmp(promotionItemList[j].productID,
                           cart[i].productID) == 0)
                {
                    apply = 1;
                    break;
                }
            }

            if (!apply)
                continue;

            /*---------------------------------------
                Percentage Discount
            ---------------------------------------*/

            if (strcmp(promotionList[p].promotionType,
                       "Percentage") == 0)
            {
                double percent;

                printf("\n%s (%%): ",
                       promotionList[p].promotionName);

                percent = inputDouble();

                double price =
                    cart[i].normalPrice *
                    (100.0 - percent) / 100.0;

                if (price < cart[i].finalPrice)
                {
                    cart[i].finalPrice = price;

                    strcpy(cart[i].promotionName,
                           promotionList[p].promotionName);
                }
            }

            /*---------------------------------------
                Fixed Amount
            ---------------------------------------*/

            else if (strcmp(promotionList[p].promotionType,
                            "Fixed") == 0)
            {
                double amount;

                printf("\n%s Amount : ",
                       promotionList[p].promotionName);

                amount = inputDouble();

                double price =
                    cart[i].normalPrice - amount;

                if (price < 0)
                    price = 0;

                if (price < cart[i].finalPrice)
                {
                    cart[i].finalPrice = price;

                    strcpy(cart[i].promotionName,
                           promotionList[p].promotionName);
                }
            }
        }

        cart[i].subtotal =
            cart[i].quantity *
            cart[i].finalPrice;

        cart[i].profit =
            calculateProfit(&cart[i]);
    }

    calculateCartTotal();
}

/*===========================================================================*/

void calculateCartTotal()
{
    double total = 0;

    for (int i = 0; i < totalCart; i++)
    {
        total += cart[i].subtotal;
    }
}
/*===========================================================================
    CHECKOUT
===========================================================================*/

void checkout()
{
    if (totalCart == 0)
    {
        printf("\nShopping cart is empty.\n");
        pressEnter();
        return;
    }

    clearScreen();

    calculateCartTotal();

    printLine(125);
    printf("                               CHECKOUT\n");
    printLine(125);

    printf("%-4s %-10s %-25s %-6s %-10s %-10s %-12s\n",
           "No",
           "ID",
           "Product",
           "Qty",
           "Price",
           "Total",
           "Promotion");

    printSubLine(125);

    double subtotal = 0;
    double totalProfit = 0;

    for (int i = 0; i < totalCart; i++)
    {
        printf("%-4d ",
               i + 1);

        printf("%-10s ",
               cart[i].productID);

        printf("%-25s ",
               cart[i].productName);

        printf("%-6d ",
               cart[i].quantity);

        printf("$%-9.2lf ",
               cart[i].finalPrice);

        printf("$%-9.2lf ",
               cart[i].subtotal);

        if (strlen(cart[i].promotionName) == 0)
            printf("-");
        else
            printf("%s", cart[i].promotionName);

        printf("\n");

        subtotal += cart[i].subtotal;
        totalProfit += cart[i].profit;
    }

    printSubLine(125);

    printf("Subtotal      : $%.2lf\n", subtotal);
    printf("Discount      : $%.2lf\n", 0.00);
    printf("Total         : $%.2lf\n", subtotal);
    printf("Profit        : $%.2lf\n", totalProfit);

    printLine(125);

    printf("1. Payment\n");
    printf("2. Back\n");

    printSubLine(125);

    printf("Choice : ");

    int choice = inputInteger();

    switch (choice)
    {
        case 1:
            payment();
            break;

        case 2:
            return;

        default:
            printf("\nInvalid choice.\n");
            pressEnter();
    }
}
/*===========================================================================
    PAYMENT
===========================================================================*/

void payment()
{
    clearScreen();

    double subtotal = 0;
    double totalProfit = 0;

    for (int i = 0; i < totalCart; i++)
    {
        subtotal += cart[i].subtotal;
        totalProfit += cart[i].profit;
    }

    double total = subtotal;
    double received;
    double change;

    printLine(80);
    printf("                              PAYMENT\n");
    printLine(80);

    printf("Subtotal : $%.2lf\n", subtotal);
    printf("Discount : $0.00\n");
    printf("Total    : $%.2lf\n", total);

    printSubLine(80);

    do
    {
        printf("Received : $");
        received = inputDouble();

        if (received < total)
        {
            printf("\nNot enough money.\n\n");
        }

    } while (received < total);

    change = received - total;

    printSubLine(80);

    printf("Change   : $%.2lf\n", change);

    printLine(80);

    Transaction sale;

    generateTransactionID(sale.transactionID,
                          totalTransactions + 1);

    strcpy(sale.transactionType, "SALE");

    strcpy(sale.date, __DATE__);
    strcpy(sale.time, __TIME__);

    sale.subtotal = subtotal;
    sale.discount = 0;
    sale.total = total;
    sale.received = received;
    sale.change = change;
    sale.totalProfit = totalProfit;
    sale.status = 1;

    transactionList[totalTransactions++] = sale;

    saveSale();

    updateStockAfterSale();

    saveTransactions(transactionList,
                     totalTransactions);

    saveProducts(productList,
                 totalProducts);

    saveStock(stockList,
              totalBatches);

    printReceipt();

    clearCart();

    printf("\nSale completed successfully.\n");

    pressEnter();
}
/*===========================================================================
    PRINT RECEIPT
===========================================================================*/

void printReceipt()
{
    clearScreen();

    printLine(60);
    printf("                    MART RECEIPT\n");
    printLine(60);

    printf("Transaction : %s\n",
           transactionList[totalTransactions - 1].transactionID);

    printf("Date        : %s\n",
           transactionList[totalTransactions - 1].date);

    printf("Time        : %s\n",
           transactionList[totalTransactions - 1].time);

    printSubLine(60);

    printf("%-20s %-5s %-8s %-8s\n",
           "Product",
           "Qty",
           "Price",
           "Total");

    printSubLine(60);

    for (int i = 0; i < totalCart; i++)
    {
        printf("%-20s %-5d $%-7.2lf $%-7.2lf\n",
               cart[i].productName,
               cart[i].quantity,
               cart[i].finalPrice,
               cart[i].subtotal);

        if (strlen(cart[i].promotionName) > 0)
        {
            printf("   Promotion : %s\n",
                   cart[i].promotionName);
        }
    }

    printSubLine(60);

    printf("Subtotal : $%.2lf\n",
           transactionList[totalTransactions - 1].subtotal);

    printf("Discount : $%.2lf\n",
           transactionList[totalTransactions - 1].discount);

    printf("Total    : $%.2lf\n",
           transactionList[totalTransactions - 1].total);

    printf("Received : $%.2lf\n",
           transactionList[totalTransactions - 1].received);

    printf("Change   : $%.2lf\n",
           transactionList[totalTransactions - 1].change);

    printLine(60);

    printf("Thank you. Please come again.\n");

    printLine(60);
}
/*===========================================================================
    RETURN PRODUCT
===========================================================================*/

void returnProduct()
{
    char transactionID[MAX_ID_LENGTH];

    clearScreen();

    printLine(90);
    printf("                         RETURN PRODUCT\n");
    printLine(90);

    printf("Transaction ID : ");
    inputString(transactionID, MAX_ID_LENGTH);

    int transactionIndex = -1;

    for (int i = 0; i < totalTransactions; i++)
    {
        if (strcmp(transactionList[i].transactionID,
                   transactionID) == 0)
        {
            transactionIndex = i;
            break;
        }
    }

    if (transactionIndex == -1)
    {
        printf("\nTransaction not found.\n");
        pressEnter();
        return;
    }

    printSubLine(90);

    printf("Transaction : %s\n",
           transactionList[transactionIndex].transactionID);

    printf("Date        : %s\n",
           transactionList[transactionIndex].date);

    printf("Time        : %s\n",
           transactionList[transactionIndex].time);

    printf("Total       : $%.2lf\n",
           transactionList[transactionIndex].total);

    printSubLine(90);

    printf("%-4s %-10s %-25s %-6s\n",
           "No",
           "ID",
           "Product",
           "Qty");

    printSubLine(90);

    int no = 1;

    for (int i = 0; i < totalTransactionItems; i++)
    {
        if (strcmp(transactionItemList[i].transactionID,
                   transactionID) == 0)
        {
            printf("%-4d %-10s %-25s %-6d\n",
                   no++,
                   transactionItemList[i].productID,
                   transactionItemList[i].productName,
                   transactionItemList[i].quantity);
        }
    }

    printSubLine(90);

    printf("1. Confirm Return\n");
    printf("2. Cancel\n");

    printLine(90);

    printf("Choice : ");

    int choice = inputInteger();

    if (choice != 1)
        return;

    for (int i = 0; i < totalTransactionItems; i++)
    {
        if (strcmp(transactionItemList[i].transactionID,
                   transactionID) == 0)
        {
            for (int j = 0; j < totalBatches; j++)
            {
                if (strcmp(stockList[j].batchID,
                           transactionItemList[i].batchID) == 0)
                {
                    stockList[j].quantity +=
                        transactionItemList[i].quantity;

                    break;
                }
            }
        }
    }

    updateProductStock();

    saveStock(stockList, totalBatches);
    saveProducts(productList, totalProducts);

    printf("\nReturn completed successfully.\n");

    pressEnter();
}
/*===========================================================================
    HELPER FUNCTIONS
===========================================================================*/

void clearCart()
{
    totalCart = 0;
}

/*---------------------------------------------------------------------------*/

int findCartItem(const char productID[])
{
    for (int i = 0; i < totalCart; i++)
    {
        if (strcmp(cart[i].productID, productID) == 0)
        {
            return i;
        }
    }

    return -1;
}

/*---------------------------------------------------------------------------*/

int cartProductExists(const char productID[])
{
    return (findCartItem(productID) != -1);
}

/*---------------------------------------------------------------------------*/

int getAvailableStock(const char productID[])
{
    int total = 0;

    for (int i = 0; i < totalBatches; i++)
    {
        if (stockList[i].status == 1 &&
            strcmp(stockList[i].productID,
                   productID) == 0)
        {
            total += stockList[i].quantity;
        }
    }

    return total;
}

/*---------------------------------------------------------------------------*/

double calculateProfit(const CartItem *item)
{
    double buyPrice = 0;

    for (int i = 0; i < totalBatches; i++)
    {
        if (stockList[i].status == 1 &&
            strcmp(stockList[i].productID,
                   item->productID) == 0)
        {
            buyPrice = stockList[i].buyPrice;
            break;
        }
    }

    return (item->finalPrice - buyPrice) * item->quantity;
}

/*---------------------------------------------------------------------------*/

void updateStockAfterSale()
{
    for (int i = 0; i < totalCart; i++)
    {
        int remain = cart[i].quantity;

        for (int j = 0; j < totalBatches && remain > 0; j++)
        {
            if (stockList[j].status == 0)
                continue;

            if (strcmp(stockList[j].productID,
                       cart[i].productID) != 0)
                continue;

            if (stockList[j].quantity >= remain)
            {
                stockList[j].quantity -= remain;
                remain = 0;
            }
            else
            {
                remain -= stockList[j].quantity;
                stockList[j].quantity = 0;
            }
        }
    }

    updateProductStock();
}

/*---------------------------------------------------------------------------*/

void saveSale()
{
    for (int i = 0; i < totalCart; i++)
    {
        TransactionItem item;

        strcpy(item.transactionID,
               transactionList[totalTransactions - 1].transactionID);

        strcpy(item.productID,
               cart[i].productID);

        strcpy(item.productName,
               cart[i].productName);

        strcpy(item.batchID,
               cart[i].batchID);

        item.quantity = cart[i].quantity;

        item.buyPrice = 0;

        for (int j = 0; j < totalBatches; j++)
        {
            if (strcmp(stockList[j].productID,
                       cart[i].productID) == 0)
            {
                item.buyPrice = stockList[j].buyPrice;
                break;
            }
        }

        item.normalPrice = cart[i].normalPrice;
        item.finalPrice = cart[i].finalPrice;

        strcpy(item.promotionName,
               cart[i].promotionName);

        item.profit = cart[i].profit;

        transactionItemList[totalTransactionItems++] = item;
    }

    saveTransactionItems(transactionItemList,
                         totalTransactionItems);
}
/******************************************************************************
 * End of File : sales.c
 ******************************************************************************/