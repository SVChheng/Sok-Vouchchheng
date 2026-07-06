/******************************************************************************
 * File        : product.c
 * Folder      : product
 * Project     : Mart Management System
 *
 * Description:
 *     Product Management Module
 *
 * Part 1 / 10
 *     ✓ Product Menu
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "product.h"
#include "../csv/csv.h"
#include "../utils/utils.h"
#include "../product/product.h"
#include "../stock/stock.h"

/*===========================================================================
    GLOBAL DATA
===========================================================================*/

extern ProductType typeList[MAX_TYPES];
extern Product productList[MAX_PRODUCTS];
extern StockBatch stockList[MAX_BATCHES];

extern int totalTypes;
extern int totalProducts;
extern int totalBatches;

/*===========================================================================
    PRODUCT MENU
===========================================================================*/

void productMenu()
{
    totalTypes = loadProductTypes(typeList);
    totalProducts = loadProducts(productList);
    totalBatches = loadStock(stockList);

    int choice;

    while (1)
    {
        clearScreen();

        printLine(65);
        printf("                PRODUCT MANAGEMENT\n");
        printLine(65);

        printf("1. Product Type\n");
        printf("2. Product List\n");
        printf("3. Add Product\n");
        printf("4. Back\n");

        printSubLine(65);

        printf("Choice : ");
        choice = inputInteger();

        switch (choice)
        {
            case 1:
                productTypeMenu();
                break;

            case 2:
                viewProductList();
                break;

            case 3:
                addProduct();
                break;

            case 4:
                return;

            default:
                printf("\nInvalid choice.\n");
                pressEnter();
        }
    }
}
/*===========================================================================
    PRODUCT TYPE MENU
===========================================================================*/

void productTypeMenu()
{
    int choice;

    while (1)
    {
        clearScreen();

        printLine(65);
        printf("                    PRODUCT TYPE\n");
        printLine(65);

        printf("%-5s %-12s %-25s %-10s\n",
               "No", "Type ID", "Type Name", "Products");

        printSubLine(65);

        updateTotalStock();
        sortProductsByType();

        for (int i = 0; i < totalTypes; i++)
        {
            if (typeList[i].status == 0)
                continue;

            int productCount = 0;

            for (int j = 0; j < totalProducts; j++)
            {
                if (productList[j].status == 0)
                    continue;

                if (strcmp(typeList[i].typeID,
                           productList[j].typeID) == 0)
                {
                    productCount++;
                }
            }

            printf("%-5d %-12s %-25s %-10d\n",
                   i + 1,
                   typeList[i].typeID,
                   typeList[i].typeName,
                   productCount);
        }

        printSubLine(65);

        printf("Enter No. = View Products\n\n");

        printf("1. Add Product Type\n");
        printf("2. Edit Product Type\n");
        printf("3. Delete Product Type\n");
        printf("4. Back\n");

        printLine(65);

        printf("Choice : ");
        choice = inputInteger();

        switch (choice)
        {
            case 1:
                addProductType();
                break;

            case 2:
                editProductType();
                break;

            case 3:
                deleteProductType();
                break;

            case 4:
                return;

            default:
                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    PRODUCT TYPE MANAGEMENT
===========================================================================*/

void addProductType()
{
    clearScreen();

    ProductType newType;

    printLine(60);
    printf("               ADD PRODUCT TYPE\n");
    printLine(60);

    printf("Type ID   : ");
    inputString(newType.typeID, MAX_ID_LENGTH);

    /* Check duplicate Type ID */
    for (int i = 0; i < totalTypes; i++)
    {
        if (strcmp(typeList[i].typeID, newType.typeID) == 0)
        {
            printf("\nType ID already exists.\n");
            pressEnter();
            return;
        }
    }

    printf("Type Name : ");
    inputString(newType.typeName, MAX_TYPE_NAME);

    newType.status = 1;

    typeList[totalTypes++] = newType;

    saveProductTypes(typeList, totalTypes);

    printf("\nProduct Type added successfully.\n");
    pressEnter();
}

/*---------------------------------------------------------------------------*/

void editProductType()
{
    clearScreen();

    char typeID[MAX_ID_LENGTH];

    printLine(60);
    printf("              EDIT PRODUCT TYPE\n");
    printLine(60);

    printf("Type ID : ");
    inputString(typeID, MAX_ID_LENGTH);

    int index = findTypeByID(typeID);

    if (index == -1)
    {
        printf("\nType not found.\n");
        pressEnter();
        return;
    }

    printf("\nCurrent Name : %s\n", typeList[index].typeName);

    printf("New Name     : ");
    inputString(typeList[index].typeName, MAX_TYPE_NAME);

    saveProductTypes(typeList, totalTypes);

    printf("\nUpdated successfully.\n");
    pressEnter();
}

/*---------------------------------------------------------------------------*/

void deleteProductType()
{
    clearScreen();

    char typeID[MAX_ID_LENGTH];

    printLine(60);
    printf("             DELETE PRODUCT TYPE\n");
    printLine(60);

    printf("Type ID : ");
    inputString(typeID, MAX_ID_LENGTH);

    int index = findTypeByID(typeID);

    if (index == -1)
    {
        printf("\nType not found.\n");
        pressEnter();
        return;
    }

    /* Check whether products still exist */
    for (int i = 0; i < totalProducts; i++)
    {
        if (productList[i].status == 1 &&
            strcmp(productList[i].typeID, typeID) == 0)
        {
            printf("\nThis type still contains products.");
            printf("\nMove all products to another type first.\n");
            pressEnter();
            return;
        }
    }

    printf("\nDelete \"%s\" ?\n", typeList[index].typeName);

    printf("\n1. Yes");
    printf("\n2. No");

    printf("\n\nChoice : ");

    int choice = inputInteger();

    if (choice == 1)
    {
        typeList[index].status = 0;

        saveProductTypes(typeList, totalTypes);

        printf("\nDeleted successfully.\n");
    }

    pressEnter();
}
/*===========================================================================
    PRODUCT LIST
===========================================================================*/

void viewProductList()
{
    int choice;

    while (1)
    {
        clearScreen();

        printLine(120);
        printf("                              PRODUCT LIST\n");
        printLine(120);

        printf("%-4s %-10s %-8s %-25s %-8s %-8s %-10s %-12s %-15s %-10s\n",
               "No",
               "ID",
               "Batch",
               "Product",
               "Stock",
               "Sell",
               "Discount",
               "Expiry",
               "Supplier",
               "Status");

        printSubLine(120);

        int no = 1;

        for (int i = 0; i < totalProducts; i++)
        {
            if (productList[i].status == 0)
                continue;

            int firstBatch = 1;

            for (int j = 0; j < totalBatches; j++)
            {
                if (stockList[j].status == 0)
                    continue;

                if (strcmp(productList[i].productID,
                           stockList[j].productID) == 0)
                {
                    printf("%-4d %-10s %-8s %-25s %-8d $%-7.2lf ",
                           firstBatch ? no : 0,
                           productList[i].productID,
                           stockList[j].batchID,
                           productList[i].productName,
                           stockList[j].quantity,
                           productList[i].sellPrice);

                    if (productList[i].discountPrice > 0)
                        printf("$%-8.2lf ", productList[i].discountPrice);
                    else
                        printf("%-10s ", "-");

                    printf("%-12s ",
                           stockList[j].expiryDate);

                    printf("%-15s ",
                           stockList[j].supplier);

                    if (productList[i].totalStock <= 10)
                        printf("LOW");
                    else
                        printf("-");

                    printf("\n");

                    firstBatch = 0;
                }
            }

            no++;
        }

        printSubLine(120);

        printf("\nStatus : LOW = Low Stock\n");

        printf("\nEnter Product No. : ");
        choice = inputInteger();

        if (choice > 0 && choice < no)
        {
            displayProductDetails(choice - 1);
            continue;
        }

        printf("\n1. Search Product");
        printf("\n2. Add Product");
        printf("\n3. Back");

        printf("\n\nChoice : ");

        choice = inputInteger();

        switch (choice)
        {
            case 1:
                searchProduct();
                break;

            case 2:
                addProduct();
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
    PRODUCT DETAILS
===========================================================================*/

void displayProductDetails(int index)
{
    while (1)
    {
        clearScreen();

        printLine(105);
        printf("                              PRODUCT DETAILS\n");
        printLine(105);

        printf("Product ID      : %s\n", productList[index].productID);
        printf("Product Name    : %s\n", productList[index].productName);

        for (int i = 0; i < totalTypes; i++)
        {
            if (strcmp(productList[index].typeID,
                       typeList[i].typeID) == 0)
            {
                printf("Type            : %s\n",
                       typeList[i].typeName);
                break;
            }
        }

        printSubLine(105);

        printf("%-8s %-8s %-10s %-10s %-12s %-12s %-15s %-10s\n",
               "Batch",
               "Stock",
               "Buy",
               "Sell",
               "Discount",
               "Expiry",
               "Supplier",
               "Status");

        printSubLine(105);

        for (int i = 0; i < totalBatches; i++)
        {
            if (stockList[i].status == 0)
                continue;

            if (strcmp(productList[index].productID,
                       stockList[i].productID) == 0)
            {
                printf("%-8s ",
                       stockList[i].batchID);

                printf("%-8d ",
                       stockList[i].quantity);

                printf("$%-9.2lf ",
                       stockList[i].buyPrice);

                printf("$%-9.2lf ",
                       productList[index].sellPrice);

                if (productList[index].discountPrice > 0)
                    printf("$%-11.2lf ",
                           productList[index].discountPrice);
                else
                    printf("%-12s ",
                           "-");

                printf("%-12s ",
                       stockList[i].expiryDate);

                printf("%-15s ",
                       stockList[i].supplier);

                if (stockList[i].promotionID[0] != '\0')
                    printf("PROMO");
                else if (productList[index].totalStock <= 10)
                    printf("LOW");
                else
                    printf("-");

                printf("\n");
            }
        }

        printSubLine(105);

        printf("Current Stock       : %d\n",
               productList[index].totalStock);

        printf("Sold This Month     : %d\n",
               productList[index].soldThisMonth);

        printf("Profit This Month   : $%.2lf\n",
               productList[index].profitThisMonth);

        printSubLine(105);

        printf("Status:\n");
        printf("LOW   = Low Stock\n");
        printf("PROMO = Promotion Applied\n");

        printSubLine(105);

        printf("1. Edit Product\n");
        printf("2. Delete Product\n");
        printf("3. Back\n");

        printLine(105);

        printf("Choice : ");

        int choice = inputInteger();

        switch (choice)
        {
            case 1:
                editProduct();
                break;

            case 2:
                deleteProduct();
                return;

            case 3:
                return;

            default:
                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    SEARCH PRODUCT
===========================================================================*/

void searchProduct()
{
    char keyword[MAX_NAME_LENGTH];

    while (1)
    {
        clearScreen();

        printLine(100);
        printf("                              SEARCH PRODUCT\n");
        printLine(100);

        printf("Enter Product ID / Product Name : ");
        inputString(keyword, MAX_NAME_LENGTH);

        printSubLine(100);

        printf("%-4s %-10s %-8s %-25s %-8s %-8s %-10s %-15s\n",
               "No",
               "ID",
               "Batch",
               "Product",
               "Stock",
               "Sell",
               "Discount",
               "Status");

        printSubLine(100);

        int found = 0;
        int no = 1;

        for (int i = 0; i < totalProducts; i++)
        {
            if (productList[i].status == 0)
                continue;

            if (strstr(productList[i].productID, keyword) != NULL ||
                strstr(productList[i].productName, keyword) != NULL)
            {
                for (int j = 0; j < totalBatches; j++)
                {
                    if (stockList[j].status == 0)
                        continue;

                    if (strcmp(productList[i].productID,
                               stockList[j].productID) == 0)
                    {
                        printf("%-4d %-10s %-8s %-25s %-8d $%-7.2lf ",
                               no,
                               productList[i].productID,
                               stockList[j].batchID,
                               productList[i].productName,
                               stockList[j].quantity,
                               productList[i].sellPrice);

                        if (productList[i].discountPrice > 0)
                            printf("$%-8.2lf ",
                                   productList[i].discountPrice);
                        else
                            printf("%-10s ",
                                   "-");

                        if (stockList[j].promotionID[0] != '\0')
                            printf("PROMO");
                        else if (productList[i].totalStock <= 10)
                            printf("LOW");
                        else
                            printf("-");

                        printf("\n");

                        found = 1;
                        no++;
                    }
                }
            }
        }

        if (!found)
        {
            printf("\nNo product found.\n");
        }

        printSubLine(100);

        printf("\n1. Search Again");
        printf("\n2. Product Details");
        printf("\n3. Back");

        printf("\n\nChoice : ");

        int choice = inputInteger();

        switch (choice)
        {
            case 1:
                break;

            case 2:
            {
                printf("\nEnter Product No. : ");

                int number = inputInteger();

                if (number >= 1 && number < no)
                {
                    displayProductDetails(number - 1);
                }
                else
                {
                    printf("\nInvalid Product Number.");
                    pressEnter();
                }

                break;
            }

            case 3:
                return;

            default:
                printf("\nInvalid choice.");
                pressEnter();
        }
    }
}
/*===========================================================================
    ADD PRODUCT
===========================================================================*/

void addProduct()
{
    Product newProduct;
    StockBatch newBatch;

    while (1)
    {
        clearScreen();

        printLine(105);
        printf("                               ADD PRODUCT\n");
        printLine(105);

        if (totalProducts > 0)
        {
            printf("Recently Added\n");

            printSubLine(105);

            printf("%-4s %-10s %-8s %-25s %-8s %-8s %-8s %-15s\n",
                   "No",
                   "ID",
                   "Batch",
                   "Product",
                   "Stock",
                   "Sell",
                   "Buy",
                   "Supplier");

            printSubLine(105);

            int start = totalProducts - 5;

            if (start < 0)
                start = 0;

            int no = 1;

            for (int i = start; i < totalProducts; i++)
            {
                for (int j = 0; j < totalBatches; j++)
                {
                    if (strcmp(productList[i].productID,
                               stockList[j].productID) == 0)
                    {
                        printf("%-4d %-10s %-8s %-25s %-8d $%-7.2lf $%-7.2lf %-15s\n",
                               no++,
                               productList[i].productID,
                               stockList[j].batchID,
                               productList[i].productName,
                               stockList[j].quantity,
                               productList[i].sellPrice,
                               stockList[j].buyPrice,
                               stockList[j].supplier);

                        break;
                    }
                }
            }

            printSubLine(105);
        }

        printf("Type ID         : ");
        inputString(newProduct.typeID, MAX_ID_LENGTH);

        generateNextProductID(newProduct.productID,
                              newProduct.typeID);

        printf("Product ID      : %s\n",
               newProduct.productID);

        printf("Product Name    : ");
        inputString(newProduct.productName,
                    MAX_NAME_LENGTH);

        if (productExists(newProduct.productName,
                          newProduct.typeID))
        {
            clearScreen();

            printLine(70);
            printf("             SIMILAR PRODUCT FOUND\n");
            printLine(70);

            printf("\nThis product already exists.\n\n");

            printf("1. Add Stock\n");
            printf("2. Create New Product\n");
            printf("3. Cancel\n");

            printLine(70);

            printf("Choice : ");

            int choice = inputInteger();

            if (choice == 1)
            {
                receiveStock();
                return;
            }
            else if (choice == 3)
            {
                return;
            }
        }

        printf("Sell Price      : ");
        newProduct.sellPrice = inputDouble();

        printf("Discount Price  : ");
        newProduct.discountPrice = inputDouble();

        printf("Buy Price       : ");
        newBatch.buyPrice = inputDouble();

        printf("Quantity        : ");
        newBatch.quantity = inputInteger();

        printf("Supplier        : ");
        inputString(newBatch.supplier,
                    MAX_SUPPLIER_NAME);

        printf("Expiry Date     : ");
        inputString(newBatch.expiryDate,
                    MAX_DATE_LENGTH);

        generateNextBatchID(newBatch.batchID,
                            newProduct.productID);

        strcpy(newBatch.productID,
               newProduct.productID);

        strcpy(newBatch.promotionID, "");

        newProduct.totalStock = newBatch.quantity;
        newProduct.soldThisMonth = 0;
        newProduct.profitThisMonth = 0;
        newProduct.status = 1;

        newBatch.status = 1;

        productList[totalProducts++] = newProduct;
        stockList[totalBatches++] = newBatch;

        saveProducts(productList, totalProducts);
        saveStock(stockList, totalBatches);

        printSubLine(105);

        printf("1. Save & Add More\n");
        printf("2. Save & Finish\n");
        printf("3. Cancel\n");

        printLine(105);

        printf("Choice : ");

        int choice = inputInteger();

        if (choice == 1)
            continue;

        if (choice == 2)
            return;

        if (choice == 3)
            return;
    }
}
/*===========================================================================
    EDIT PRODUCT
===========================================================================*/

void editProduct()
{
    char productID[MAX_ID_LENGTH];

    clearScreen();

    printLine(100);
    printf("                              EDIT PRODUCT\n");
    printLine(100);

    printf("Product ID : ");
    inputString(productID, MAX_ID_LENGTH);

    int index = findProductByID(productID);

    if (index == -1)
    {
        printf("\nProduct not found.\n");
        pressEnter();
        return;
    }

    printf("\nCurrent Information\n");
    printSubLine(100);

    printf("Product ID      : %s\n", productList[index].productID);
    printf("Product Name    : %s\n", productList[index].productName);
    printf("Sell Price      : $%.2lf\n", productList[index].sellPrice);

    if(productList[index].discountPrice > 0)
        printf("Discount Price  : $%.2lf\n", productList[index].discountPrice);
    else
        printf("Discount Price  : -\n");

    printf("Type ID         : %s\n", productList[index].typeID);

    printSubLine(100);

    printf("New Product Name : ");
    inputString(productList[index].productName,
                MAX_NAME_LENGTH);

    printf("New Sell Price   : ");
    productList[index].sellPrice = inputDouble();

    printf("New Discount     : ");
    productList[index].discountPrice = inputDouble();

    printf("New Type ID      : ");
    inputString(productList[index].typeID,
                MAX_ID_LENGTH);

    saveProducts(productList, totalProducts);

    printf("\nProduct updated successfully.\n");

    pressEnter();
}

/*===========================================================================
    DELETE PRODUCT
===========================================================================*/

void deleteProduct()
{
    char productID[MAX_ID_LENGTH];

    clearScreen();

    printLine(100);
    printf("                             DELETE PRODUCT\n");
    printLine(100);

    printf("Product ID : ");
    inputString(productID, MAX_ID_LENGTH);

    int index = findProductByID(productID);

    if(index == -1)
    {
        printf("\nProduct not found.\n");
        pressEnter();
        return;
    }

    printf("\nProduct : %s\n",
           productList[index].productName);

    printf("\n1. Delete");
    printf("\n2. Cancel");

    printf("\n\nChoice : ");

    int choice = inputInteger();

    if(choice == 1)
    {
        productList[index].status = 0;

        for(int i = 0; i < totalBatches; i++)
        {
            if(strcmp(stockList[i].productID,
                      productList[index].productID) == 0)
            {
                stockList[i].status = 0;
            }
        }

        saveProducts(productList, totalProducts);
        saveStock(stockList, totalBatches);

        printf("\nDeleted successfully.\n");
    }

    pressEnter();
}
/*===========================================================================
    HELPER FUNCTIONS
===========================================================================*/

int findProductByID(const char productID[])
{
    for (int i = 0; i < totalProducts; i++)
    {
        if (productList[i].status == 1 &&
            strcmp(productList[i].productID, productID) == 0)
        {
            return i;
        }
    }

    return -1;
}

/*---------------------------------------------------------------------------*/

int findTypeByID(const char typeID[])
{
    for (int i = 0; i < totalTypes; i++)
    {
        if (typeList[i].status == 1 &&
            strcmp(typeList[i].typeID, typeID) == 0)
        {
            return i;
        }
    }

    return -1;
}

/*---------------------------------------------------------------------------*/

int productExists(const char productName[], const char typeID[])
{
    for (int i = 0; i < totalProducts; i++)
    {
        if (productList[i].status == 0)
            continue;

        if (strcmp(productList[i].productName, productName) == 0 &&
            strcmp(productList[i].typeID, typeID) == 0)
        {
            return 1;
        }
    }

    return 0;
}

/*---------------------------------------------------------------------------*/

void updateTotalStock()
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

void sortProductsByType()
{
    Product temp;

    for (int i = 0; i < totalProducts - 1; i++)
    {
        for (int j = i + 1; j < totalProducts; j++)
        {
            if (strcmp(productList[i].typeID,
                       productList[j].typeID) > 0)
            {
                temp = productList[i];
                productList[i] = productList[j];
                productList[j] = temp;
            }
        }
    }
}

/*---------------------------------------------------------------------------*/

void displayProductTable()
{
    viewProductList();
}

/*---------------------------------------------------------------------------*/

void generateNextProductID(char productID[],
                           const char typeID[])
{
    int max = 0;
    int number;

    for (int i = 0; i < totalProducts; i++)
    {
        if (strncmp(productList[i].productID,
                    typeID,
                    strlen(typeID)) == 0)
        {
            sscanf(productList[i].productID + strlen(typeID),
                   "%d",
                   &number);

            if (number > max)
                max = number;
        }
    }

    sprintf(productID,
            "%s%04d",
            typeID,
            max + 1);
}

/*---------------------------------------------------------------------------*/

void generateNextBatchID(char batchID[],
                         const char productID[])
{
    int count = 0;

    for (int i = 0; i < totalBatches; i++)
    {
        if (strcmp(stockList[i].productID,
                   productID) == 0)
        {
            count++;
        }
    }

    sprintf(batchID,
            "B%03d",
            count + 1);
}
/*===========================================================================
    HELPER FUNCTIONS
===========================================================================*/

// void displayProductDetails(int index)
// {
//     if (index < 0 || index >= totalProducts)
//     {
//         printf("\nInvalid Product.\n");
//         pressEnter();
//         return;
//     }

//     /* Reuse Product Details page */
//     /* (Implemented in Part 5) */
// }

/*---------------------------------------------------------------------------*/



/******************************************************************************
 * End of File : product.c
 ******************************************************************************/