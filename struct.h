/******************************************************************************
 * File        : struct.h
 * Project     : Mart Management System
 * Description : Common structures used by all modules.
 ******************************************************************************/

#ifndef STRUCT_H
#define STRUCT_H

/*===========================================================================
    CONSTANTS
===========================================================================*/

#define MAX_TYPES               50
#define MAX_PRODUCTS            1000
#define MAX_BATCHES             5000
#define MAX_CART_ITEMS          100
#define MAX_TRANSACTION_ITEMS   5000
#define MAX_TRANSACTIONS        10000
#define MAX_PROMOTIONS          200
#define MAX_PROMOTION_ITEMS     5000

#define MAX_ID_LENGTH           20
#define MAX_NAME_LENGTH         100
#define MAX_TYPE_NAME           50
#define MAX_SUPPLIER_NAME       100
#define MAX_PASSWORD_LENGTH     50
#define MAX_STATUS_LENGTH       20
#define MAX_DATE_LENGTH         11      /* dd/mm/yyyy */
#define MAX_TIME_LENGTH         9       /* hh:mm:ss */
#define MAX_PROMOTION_NAME      100

/*===========================================================================
    PRODUCT TYPE
===========================================================================*/

typedef struct
{
    char typeID[MAX_ID_LENGTH];
    char typeName[MAX_TYPE_NAME];

    int status;            /* 1 = Active, 0 = Deleted */

} ProductType;

/*===========================================================================
    PRODUCT
===========================================================================*/

typedef struct
{
    char productID[MAX_ID_LENGTH];

    char typeID[MAX_ID_LENGTH];

    char productName[MAX_NAME_LENGTH];

    double sellPrice;

    double discountPrice;

    int totalStock;

    int soldThisMonth;

    double profitThisMonth;

    int status;            /* Active / Inactive / Deleted */

} Product;

/*===========================================================================
    STOCK BATCH
===========================================================================*/

typedef struct
{
    char batchID[MAX_ID_LENGTH];

    char productID[MAX_ID_LENGTH];

    double buyPrice;

    int quantity;

    char expiryDate[MAX_DATE_LENGTH];

    char supplier[MAX_SUPPLIER_NAME];

    char promotionID[MAX_ID_LENGTH];

    int status;            /* Active / Deleted */

} StockBatch;
/*===========================================================================
    SHOPPING CART
===========================================================================*/

typedef struct
{
    char productID[MAX_ID_LENGTH];

    char productName[MAX_NAME_LENGTH];

    char batchID[MAX_ID_LENGTH];

    int quantity;

    double normalPrice;

    double finalPrice;

    char promotionName[MAX_PROMOTION_NAME];

    double subtotal;

    double profit;

} CartItem;

/*===========================================================================
    TRANSACTION (Receipt Header)
===========================================================================*/

typedef struct
{
    char transactionID[MAX_ID_LENGTH];

    char transactionType[20];      /* SALE / RETURN */

    char date[MAX_DATE_LENGTH];

    char time[MAX_TIME_LENGTH];

    double subtotal;

    double discount;

    double total;

    double received;

    double change;

    double totalProfit;

    int status;

} Transaction;

/*===========================================================================
    TRANSACTION ITEM (Receipt Detail / Snapshot)
===========================================================================*/

typedef struct
{
    char transactionID[MAX_ID_LENGTH];

    char productID[MAX_ID_LENGTH];

    char productName[MAX_NAME_LENGTH];

    char batchID[MAX_ID_LENGTH];

    int quantity;

    double buyPrice;

    double normalPrice;

    double finalPrice;

    char promotionName[MAX_PROMOTION_NAME];

    double profit;

} TransactionItem;
/*===========================================================================
    PROMOTION
===========================================================================*/

typedef struct
{
    char promotionID[MAX_ID_LENGTH];

    char promotionName[MAX_PROMOTION_NAME];

    char promotionType[50];
    /* Percentage / Fixed Amount / Buy X Get Y */

    char applyMethod[50];
    /* Product / Product Type / Near Expiry / Low Stock / High Profit */

    char startDate[MAX_DATE_LENGTH];

    char endDate[MAX_DATE_LENGTH];

    int priority;

    int canCombine;

    int status;
    /* Upcoming / Active / Expired / Deleted */

} Promotion;

/*===========================================================================
    PROMOTION ITEM
===========================================================================*/

typedef struct
{
    char promotionID[MAX_ID_LENGTH];

    char productID[MAX_ID_LENGTH];

} PromotionItem;

/*===========================================================================
    SETTINGS
===========================================================================*/

typedef struct
{
    char storeName[MAX_NAME_LENGTH];

    char password[MAX_PASSWORD_LENGTH];

    char currency[10];

    int lowStockWarning;

    int nearExpiryWarningDays;

    int allowNegativeProfitPromotion;

    double defaultTaxRate;

    char receiptFooter[200];

    int maxLoginAttempt;

    char dateFormat[20];

    char timeFormat[20];

    int autoBackup;

    int defaultPromotionPriority;

} Settings;
#endif