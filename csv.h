/******************************************************************************
 * File        : csv.h
 * Folder      : csv
 * Project     : Mart Management System
 *
 * Description:
 *     Function declarations for reading and writing CSV files.
 *     This is the ONLY place the shared data-table variables are DEFINED
 *     (created), inside csv.c. Every other file only sees them as "extern"
 *     through this header, so there is one single source of truth.
 ******************************************************************************/

#ifndef CSV_H
#define CSV_H

#include "../common/struct.h"

/*===========================================================================
    GLOBAL DATA TABLES (defined once in csv.c)
===========================================================================*/

extern ProductType     typeList[MAX_TYPES];
extern Product         productList[MAX_PRODUCTS];
extern StockBatch       stockList[MAX_BATCHES];
extern Transaction      transactionList[MAX_TRANSACTIONS];
extern TransactionItem  transactionItemList[MAX_TRANSACTION_ITEMS];
extern Promotion        promotionList[MAX_PROMOTIONS];
extern PromotionItem    promotionItemList[MAX_PROMOTION_ITEMS];
extern Settings         setting;

extern int totalTypes;
extern int totalProducts;
extern int totalBatches;
extern int totalTransactions;
extern int totalTransactionItems;
extern int totalPromotions;
extern int totalPromotionItems;

/*===========================================================================
    PRODUCT TYPE
===========================================================================*/

int  loadProductTypes(ProductType typeList[]);
void saveProductTypes(ProductType typeList[], int count);

/*===========================================================================
    PRODUCT
===========================================================================*/

int  loadProducts(Product productList[]);
void saveProducts(Product productList[], int count);

/*===========================================================================
    STOCK
===========================================================================*/

int  loadStock(StockBatch stockList[]);
void saveStock(StockBatch stockList[], int count);

/*===========================================================================
    TRANSACTION
===========================================================================*/

int  loadTransactions(Transaction transactionList[]);
void saveTransactions(Transaction transactionList[], int count);

/*===========================================================================
    TRANSACTION ITEM
===========================================================================*/

int  loadTransactionItems(TransactionItem itemList[]);
void saveTransactionItems(TransactionItem itemList[], int count);

/*===========================================================================
    PROMOTION
===========================================================================*/

int  loadPromotions(Promotion promotionList[]);
void savePromotions(Promotion promotionList[], int count);

/*===========================================================================
    PROMOTION ITEM
===========================================================================*/

int  loadPromotionItems(PromotionItem itemList[]);
void savePromotionItems(PromotionItem itemList[], int count);

/*===========================================================================
    SETTINGS
===========================================================================*/

int  loadSettings(Settings *setting);
void saveSettings(Settings setting);

#endif