/******************************************************************************
 * File        : sales.h
 * Folder      : sales
 * Project     : Mart Management System
 *
 * Description:
 *     Function declarations for Sales Management Module.
 ******************************************************************************/

#ifndef SALES_H
#define SALES_H

#include "../common/struct.h"

/*===========================================================================
    SALES MENU
===========================================================================*/

void salesMenu();

/*===========================================================================
    SALES
===========================================================================*/

void newSale();

void addProductToCart();

void editCartQuantity();

void removeProductFromCart();

void checkout();

void payment();

void printReceipt();

void returnProduct();

/*===========================================================================
    PROMOTION
===========================================================================*/

void applyPromotion();

void calculateCartTotal();

/*===========================================================================
    HELPER
===========================================================================*/

void clearCart();

int findCartItem(const char productID[]);

int cartProductExists(const char productID[]);

int getAvailableStock(const char productID[]);

double calculateProfit(const CartItem *item);

void updateStockAfterSale();

void saveSale();

#endif