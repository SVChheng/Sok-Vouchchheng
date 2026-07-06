/******************************************************************************
 * File        : stock.h
 * Folder      : stock
 * Project     : Mart Management System
 *
 * Description:
 *     Function declarations for Stock Management Module.
 ******************************************************************************/

#ifndef STOCK_H
#define STOCK_H

#include "../common/struct.h"

/*===========================================================================
    STOCK MENU
===========================================================================*/

void stockMenu();

/*===========================================================================
    STOCK
===========================================================================*/

void receiveStock();

void stockAdjustment();

void viewCurrentBatches();

void viewNearExpiryProducts();

void viewLowStockProducts();

/*===========================================================================
    HELPER
===========================================================================*/

int findBatchByID(const char batchID[]);

int findProductBatch(const char productID[],
                     const char batchID[]);

void sortBatchByExpiry();

void sortBatchByProduct();

void displayBatchTable();

void displayNearExpiryTable();

void displayLowStockTable();

void updateProductStock();

#endif