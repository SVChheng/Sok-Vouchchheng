/******************************************************************************
 * File        : product.h
 * Folder      : product
 * Project     : Mart Management System
 *
 * Description:
 *     Function declarations for Product Management Module.
 ******************************************************************************/

#ifndef PRODUCT_H
#define PRODUCT_H

#include "../common/struct.h"

/*===========================================================================
    PRODUCT MENU
===========================================================================*/

void productMenu();

/*===========================================================================
    PRODUCT TYPE
===========================================================================*/

void productTypeMenu();

void viewProductTypes();

void addProductType();

void editProductType();

void deleteProductType();

/*===========================================================================
    PRODUCT
===========================================================================*/

void viewProductList();

void viewProductDetails();

void addProduct();

void editProduct();

void deleteProduct();

void searchProduct();

/*===========================================================================
    STOCK
===========================================================================*/

void addStock();

/*===========================================================================
    HELPER
===========================================================================*/

int findProductByID(const char productID[]);

int findTypeByID(const char typeID[]);

int productExists(const char productName[], const char typeID[]);

void updateTotalStock();

void sortProductsByType();

void displayProductTable();

void displayProductDetails(int index);

void generateNextProductID(char productID[], const char typeID[]);

void generateNextBatchID(char batchID[], const char productID[]);

#endif