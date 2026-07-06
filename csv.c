/******************************************************************************
 * File        : csv.c
 * Folder      : csv
 * Project     : Mart Management System
 *
 * Description:
 *     Read and write CSV files.
 *
 *     All loaders go through splitLine(), a small helper that safely
 *     breaks a raw CSV line into fields. This replaces the old
 *     sscanf("%[^,]...") approach, which silently failed (and produced
 *     corrupt "ghost" records) whenever a field was empty. splitLine()
 *     also strips \r\n and skips lines with no ID in the first column,
 *     so blank / broken rows in a CSV file can no longer pollute the
 *     in-memory tables.
 *
 * Part 1 / 2
 *     - Global data tables (defined here, only here)
 *     - splitLine() helper
 *     - Product Type / Product / Stock
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv.h"

/*===========================================================================
    GLOBAL DATA TABLES
    (defined ONCE here - every other file reaches them through csv.h)
===========================================================================*/

ProductType     typeList[MAX_TYPES];
Product         productList[MAX_PRODUCTS];
StockBatch       stockList[MAX_BATCHES];
Transaction      transactionList[MAX_TRANSACTIONS];
TransactionItem  transactionItemList[MAX_TRANSACTION_ITEMS];
Promotion        promotionList[MAX_PROMOTIONS];
PromotionItem    promotionItemList[MAX_PROMOTION_ITEMS];
Settings         setting;

int totalTypes             = 0;
int totalProducts          = 0;
int totalBatches           = 0;
int totalTransactions      = 0;
int totalTransactionItems  = 0;
int totalPromotions        = 0;
int totalPromotionItems    = 0;

/*===========================================================================
    CSV LINE SPLITTER  (internal helper - not exposed in csv.h)
===========================================================================*/

#define MAX_CSV_FIELDS   16

/*
 * splitLine()
 *
 * Breaks "line" into up to maxFields pieces separated by commas.
 * - Strips trailing \r and \n first.
 * - A completely empty line returns 0 fields (caller should skip it).
 * - Works in place: "fields[i]" point INSIDE "line", so "line" must
 *   stay alive as long as the fields are being used.
 * - Missing / empty fields are perfectly fine - they just become "".
 */
static int splitLine(char *line, char *fields[], int maxFields)
{
    line[strcspn(line, "\r\n")] = '\0';

    if (line[0] == '\0')
        return 0;

    int count = 0;
    char *cursor = line;

    fields[count++] = cursor;

    while (count < maxFields)
    {
        char *comma = strchr(cursor, ',');

        if (comma == NULL)
            break;

        *comma = '\0';
        cursor = comma + 1;
        fields[count++] = cursor;
    }

    return count;
}

/* Safe string copy into a fixed-size struct field. */
static void copyField(char dest[], size_t destSize, const char *src)
{
    if (src == NULL)
    {
        dest[0] = '\0';
        return;
    }

    strncpy(dest, src, destSize - 1);
    dest[destSize - 1] = '\0';
}

/* atoi()/atof() already return 0 for "" or NULL-ish input, but we still
   guard against NULL to be explicit and safe. */
static int fieldInt(const char *src)
{
    return (src == NULL) ? 0 : atoi(src);
}

static double fieldDouble(const char *src)
{
    return (src == NULL) ? 0.0 : atof(src);
}

/*===========================================================================
    PRODUCT TYPE
===========================================================================*/

int loadProductTypes(ProductType typeList[])
{
    FILE *fp = fopen("data/types.csv", "r");

    if (fp == NULL)
        return 0;

    char line[256];
    char *f[MAX_CSV_FIELDS];

    fgets(line, sizeof(line), fp);      /* skip header row */

    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < MAX_TYPES)
    {
        int n = splitLine(line, f, MAX_CSV_FIELDS);

        if (n < 1 || f[0][0] == '\0')
            continue;                   /* blank / broken row -> skip */

        copyField(typeList[count].typeID,   MAX_ID_LENGTH,   f[0]);
        copyField(typeList[count].typeName, MAX_TYPE_NAME,   (n > 1) ? f[1] : "");
        typeList[count].status = (n > 2) ? fieldInt(f[2]) : 0;

        count++;
    }

    fclose(fp);

    return count;
}

void saveProductTypes(ProductType typeList[], int count)
{
    FILE *fp = fopen("data/types.csv", "w");

    if (fp == NULL)
        return;

    fprintf(fp, "TypeID,TypeName,Status\n");

    for (int i = 0; i < count; i++)
    {
        fprintf(fp,
                "%s,%s,%d\n",
                typeList[i].typeID,
                typeList[i].typeName,
                typeList[i].status);
    }

    fclose(fp);
}

/*===========================================================================
    PRODUCT
===========================================================================*/

int loadProducts(Product productList[])
{
    FILE *fp = fopen("data/products.csv", "r");

    if (fp == NULL)
        return 0;

    char line[512];
    char *f[MAX_CSV_FIELDS];

    fgets(line, sizeof(line), fp);

    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < MAX_PRODUCTS)
    {
        int n = splitLine(line, f, MAX_CSV_FIELDS);

        /* Need at least a ProductID - anything else is a broken/blank
           row left over from earlier bugs, so we drop it instead of
           turning it into a "ghost" product. */
        if (n < 1 || f[0][0] == '\0')
            continue;

        copyField(productList[count].productID,   MAX_ID_LENGTH,     f[0]);
        copyField(productList[count].typeID,      MAX_ID_LENGTH,     (n > 1) ? f[1] : "");
        copyField(productList[count].productName, MAX_NAME_LENGTH,   (n > 2) ? f[2] : "");

        productList[count].sellPrice       = (n > 3) ? fieldDouble(f[3]) : 0.0;
        productList[count].discountPrice   = (n > 4) ? fieldDouble(f[4]) : 0.0;
        productList[count].totalStock      = (n > 5) ? fieldInt(f[5])    : 0;
        productList[count].soldThisMonth   = (n > 6) ? fieldInt(f[6])    : 0;
        productList[count].profitThisMonth = (n > 7) ? fieldDouble(f[7]) : 0.0;
        productList[count].status          = (n > 8) ? fieldInt(f[8])    : 1;

        count++;
    }

    fclose(fp);

    return count;
}

void saveProducts(Product productList[], int count)
{
    FILE *fp = fopen("data/products.csv", "w");

    if (fp == NULL)
        return;

    fprintf(fp,
            "ProductID,TypeID,ProductName,SellPrice,DiscountPrice,TotalStock,SoldThisMonth,ProfitThisMonth,Status\n");

    for (int i = 0; i < count; i++)
    {
        /* Never write a row with no ProductID - keeps the file clean
           even if a stale/blank slot ever ends up in the array. */
        if (productList[i].productID[0] == '\0')
            continue;

        fprintf(fp,
                "%s,%s,%s,%.2lf,%.2lf,%d,%d,%.2lf,%d\n",
                productList[i].productID,
                productList[i].typeID,
                productList[i].productName,
                productList[i].sellPrice,
                productList[i].discountPrice,
                productList[i].totalStock,
                productList[i].soldThisMonth,
                productList[i].profitThisMonth,
                productList[i].status);
    }

    fclose(fp);
}

/*===========================================================================
    STOCK
===========================================================================*/

int loadStock(StockBatch stockList[])
{
    FILE *fp = fopen("data/stock.csv", "r");

    if (fp == NULL)
        return 0;

    char line[512];
    char *f[MAX_CSV_FIELDS];

    fgets(line, sizeof(line), fp);

    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < MAX_BATCHES)
    {
        int n = splitLine(line, f, MAX_CSV_FIELDS);

        if (n < 1 || f[0][0] == '\0')
            continue;

        copyField(stockList[count].batchID,     MAX_ID_LENGTH,     f[0]);
        copyField(stockList[count].productID,   MAX_ID_LENGTH,     (n > 1) ? f[1] : "");

        stockList[count].buyPrice = (n > 2) ? fieldDouble(f[2]) : 0.0;
        stockList[count].quantity = (n > 3) ? fieldInt(f[3])    : 0;

        copyField(stockList[count].expiryDate,  MAX_DATE_LENGTH,   (n > 4) ? f[4] : "");
        copyField(stockList[count].supplier,    MAX_SUPPLIER_NAME, (n > 5) ? f[5] : "");
        copyField(stockList[count].promotionID, MAX_ID_LENGTH,     (n > 6) ? f[6] : "");

        stockList[count].status = (n > 7) ? fieldInt(f[7]) : 1;

        count++;
    }

    fclose(fp);

    return count;
}

void saveStock(StockBatch stockList[], int count)
{
    FILE *fp = fopen("data/stock.csv", "w");

    if (fp == NULL)
        return;

    fprintf(fp,
            "BatchID,ProductID,BuyPrice,Quantity,ExpiryDate,Supplier,PromotionID,Status\n");

    for (int i = 0; i < count; i++)
    {
        if (stockList[i].batchID[0] == '\0')
            continue;

        fprintf(fp,
                "%s,%s,%.2lf,%d,%s,%s,%s,%d\n",
                stockList[i].batchID,
                stockList[i].productID,
                stockList[i].buyPrice,
                stockList[i].quantity,
                stockList[i].expiryDate,
                stockList[i].supplier,
                stockList[i].promotionID,
                stockList[i].status);
    }

    fclose(fp);
}

/******************************************************************************
 * End of Part 1 / 2 : csv.c
 ******************************************************************************/
 /*===========================================================================
    TRANSACTION
===========================================================================*/

int loadTransactions(Transaction transactionList[])
{
    FILE *fp = fopen("data/transactions.csv", "r");

    if (fp == NULL)
        return 0;

    char line[512];
    char *f[MAX_CSV_FIELDS];

    fgets(line, sizeof(line), fp);

    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < MAX_TRANSACTIONS)
    {
        int n = splitLine(line, f, MAX_CSV_FIELDS);

        if (n < 1 || f[0][0] == '\0')
            continue;

        copyField(transactionList[count].transactionID,   MAX_ID_LENGTH,   f[0]);
        copyField(transactionList[count].transactionType,  20,             (n > 1) ? f[1] : "");
        copyField(transactionList[count].date,             MAX_DATE_LENGTH,(n > 2) ? f[2] : "");
        copyField(transactionList[count].time,             MAX_TIME_LENGTH,(n > 3) ? f[3] : "");

        transactionList[count].subtotal     = (n > 4)  ? fieldDouble(f[4])  : 0.0;
        transactionList[count].discount     = (n > 5)  ? fieldDouble(f[5])  : 0.0;
        transactionList[count].total        = (n > 6)  ? fieldDouble(f[6])  : 0.0;
        transactionList[count].received     = (n > 7)  ? fieldDouble(f[7])  : 0.0;
        transactionList[count].change       = (n > 8)  ? fieldDouble(f[8])  : 0.0;
        transactionList[count].totalProfit  = (n > 9)  ? fieldDouble(f[9])  : 0.0;
        transactionList[count].status       = (n > 10) ? fieldInt(f[10])    : 1;

        count++;
    }

    fclose(fp);

    return count;
}

void saveTransactions(Transaction transactionList[], int count)
{
    FILE *fp = fopen("data/transactions.csv", "w");

    if (fp == NULL)
        return;

    fprintf(fp,
        "TransactionID,TransactionType,Date,Time,Subtotal,Discount,Total,Received,Change,TotalProfit,Status\n");

    for (int i = 0; i < count; i++)
    {
        if (transactionList[i].transactionID[0] == '\0')
            continue;

        fprintf(fp,
            "%s,%s,%s,%s,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%d\n",
            transactionList[i].transactionID,
            transactionList[i].transactionType,
            transactionList[i].date,
            transactionList[i].time,
            transactionList[i].subtotal,
            transactionList[i].discount,
            transactionList[i].total,
            transactionList[i].received,
            transactionList[i].change,
            transactionList[i].totalProfit,
            transactionList[i].status);
    }

    fclose(fp);
}

/*===========================================================================
    TRANSACTION ITEM
===========================================================================*/

int loadTransactionItems(TransactionItem itemList[])
{
    FILE *fp = fopen("data/transaction_items.csv", "r");

    if (fp == NULL)
        return 0;

    char line[512];
    char *f[MAX_CSV_FIELDS];

    fgets(line, sizeof(line), fp);

    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < MAX_TRANSACTION_ITEMS)
    {
        int n = splitLine(line, f, MAX_CSV_FIELDS);

        if (n < 1 || f[0][0] == '\0')
            continue;

        copyField(itemList[count].transactionID, MAX_ID_LENGTH,   f[0]);
        copyField(itemList[count].productID,     MAX_ID_LENGTH,   (n > 1) ? f[1] : "");
        copyField(itemList[count].productName,   MAX_NAME_LENGTH, (n > 2) ? f[2] : "");
        copyField(itemList[count].batchID,       MAX_ID_LENGTH,   (n > 3) ? f[3] : "");

        itemList[count].quantity    = (n > 4) ? fieldInt(f[4])    : 0;
        itemList[count].buyPrice    = (n > 5) ? fieldDouble(f[5]) : 0.0;
        itemList[count].normalPrice = (n > 6) ? fieldDouble(f[6]) : 0.0;
        itemList[count].finalPrice  = (n > 7) ? fieldDouble(f[7]) : 0.0;

        copyField(itemList[count].promotionName, MAX_PROMOTION_NAME, (n > 8) ? f[8] : "");

        itemList[count].profit = (n > 9) ? fieldDouble(f[9]) : 0.0;

        count++;
    }

    fclose(fp);

    return count;
}

void saveTransactionItems(TransactionItem itemList[], int count)
{
    FILE *fp = fopen("data/transaction_items.csv", "w");

    if (fp == NULL)
        return;

    fprintf(fp,
        "TransactionID,ProductID,ProductName,BatchID,Quantity,BuyPrice,NormalPrice,FinalPrice,PromotionName,Profit\n");

    for (int i = 0; i < count; i++)
    {
        if (itemList[i].transactionID[0] == '\0')
            continue;

        fprintf(fp,
            "%s,%s,%s,%s,%d,%.2lf,%.2lf,%.2lf,%s,%.2lf\n",
            itemList[i].transactionID,
            itemList[i].productID,
            itemList[i].productName,
            itemList[i].batchID,
            itemList[i].quantity,
            itemList[i].buyPrice,
            itemList[i].normalPrice,
            itemList[i].finalPrice,
            itemList[i].promotionName,
            itemList[i].profit);
    }

    fclose(fp);
}

/*===========================================================================
    PROMOTION
===========================================================================*/

int loadPromotions(Promotion promotionList[])
{
    FILE *fp = fopen("data/promotions.csv", "r");

    if (fp == NULL)
        return 0;

    char line[512];
    char *f[MAX_CSV_FIELDS];

    fgets(line, sizeof(line), fp);

    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < MAX_PROMOTIONS)
    {
        int n = splitLine(line, f, MAX_CSV_FIELDS);

        if (n < 1 || f[0][0] == '\0')
            continue;

        copyField(promotionList[count].promotionID,   MAX_ID_LENGTH,        f[0]);
        copyField(promotionList[count].promotionName, MAX_PROMOTION_NAME,   (n > 1) ? f[1] : "");
        copyField(promotionList[count].promotionType, 50,                   (n > 2) ? f[2] : "");
        copyField(promotionList[count].applyMethod,   50,                   (n > 3) ? f[3] : "");
        copyField(promotionList[count].startDate,     MAX_DATE_LENGTH,      (n > 4) ? f[4] : "");
        copyField(promotionList[count].endDate,       MAX_DATE_LENGTH,      (n > 5) ? f[5] : "");

        promotionList[count].priority   = (n > 6) ? fieldInt(f[6]) : 0;
        promotionList[count].canCombine = (n > 7) ? fieldInt(f[7]) : 0;
        promotionList[count].status     = (n > 8) ? fieldInt(f[8]) : 1;

        count++;
    }

    fclose(fp);

    return count;
}

void savePromotions(Promotion promotionList[], int count)
{
    FILE *fp = fopen("data/promotions.csv", "w");

    if (fp == NULL)
        return;

    fprintf(fp,
        "PromotionID,PromotionName,PromotionType,ApplyMethod,StartDate,EndDate,Priority,CanCombine,Status\n");

    for (int i = 0; i < count; i++)
    {
        if (promotionList[i].promotionID[0] == '\0')
            continue;

        fprintf(fp,
            "%s,%s,%s,%s,%s,%s,%d,%d,%d\n",
            promotionList[i].promotionID,
            promotionList[i].promotionName,
            promotionList[i].promotionType,
            promotionList[i].applyMethod,
            promotionList[i].startDate,
            promotionList[i].endDate,
            promotionList[i].priority,
            promotionList[i].canCombine,
            promotionList[i].status);
    }

    fclose(fp);
}

/*===========================================================================
    PROMOTION ITEM
===========================================================================*/

int loadPromotionItems(PromotionItem itemList[])
{
    FILE *fp = fopen("data/promotion_items.csv", "r");

    if (fp == NULL)
        return 0;

    char line[256];
    char *f[MAX_CSV_FIELDS];

    fgets(line, sizeof(line), fp);

    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < MAX_PROMOTION_ITEMS)
    {
        int n = splitLine(line, f, MAX_CSV_FIELDS);

        if (n < 1 || f[0][0] == '\0')
            continue;

        copyField(itemList[count].promotionID, MAX_ID_LENGTH, f[0]);
        copyField(itemList[count].productID,   MAX_ID_LENGTH, (n > 1) ? f[1] : "");

        count++;
    }

    fclose(fp);

    return count;
}

void savePromotionItems(PromotionItem itemList[], int count)
{
    FILE *fp = fopen("data/promotion_items.csv", "w");

    if (fp == NULL)
        return;

    fprintf(fp, "PromotionID,ProductID\n");

    for (int i = 0; i < count; i++)
    {
        if (itemList[i].promotionID[0] == '\0')
            continue;

        fprintf(fp,
            "%s,%s\n",
            itemList[i].promotionID,
            itemList[i].productID);
    }

    fclose(fp);
}

/*===========================================================================
    SETTINGS
===========================================================================*/

/* Sensible fallback values if settings.csv is ever missing. */
static void applyDefaultSettings(Settings *s)
{
    copyField(s->storeName, MAX_NAME_LENGTH, "My Mart");
    copyField(s->password,  MAX_PASSWORD_LENGTH, "1234");
    copyField(s->currency,  10, "$");

    s->lowStockWarning              = 10;
    s->nearExpiryWarningDays        = 30;
    s->allowNegativeProfitPromotion = 0;
    s->defaultTaxRate               = 0;

    copyField(s->receiptFooter, 200, "Thank you for shopping with us!");

    s->maxLoginAttempt = 3;

    copyField(s->dateFormat, 20, "dd/mm/yyyy");
    copyField(s->timeFormat, 20, "24 Hours");

    s->autoBackup               = 1;
    s->defaultPromotionPriority = 1;
}

int loadSettings(Settings *setting)
{
    applyDefaultSettings(setting);

    FILE *fp = fopen("data/settings.csv", "r");

    if (fp == NULL)
        return 0;

    char line[256];
    char *f[MAX_CSV_FIELDS];

    fgets(line, sizeof(line), fp);      /* skip header row */

    while (fgets(line, sizeof(line), fp))
    {
        int n = splitLine(line, f, MAX_CSV_FIELDS);

        if (n < 2 || f[0][0] == '\0')
            continue;

        const char *key   = f[0];
        const char *value = f[1];

        if (strcmp(key, "storeName") == 0)
            copyField(setting->storeName, MAX_NAME_LENGTH, value);
        else if (strcmp(key, "storePassword") == 0)
            copyField(setting->password, MAX_PASSWORD_LENGTH, value);
        else if (strcmp(key, "currency") == 0)
            copyField(setting->currency, 10, value);
        else if (strcmp(key, "lowStockWarning") == 0)
            setting->lowStockWarning = fieldInt(value);
        else if (strcmp(key, "nearExpiryWarningDays") == 0)
            setting->nearExpiryWarningDays = fieldInt(value);
        else if (strcmp(key, "allowNegativeProfitPromotion") == 0)
            setting->allowNegativeProfitPromotion = fieldInt(value);
        else if (strcmp(key, "defaultTaxRate") == 0)
            setting->defaultTaxRate = fieldDouble(value);
        else if (strcmp(key, "receiptFooter") == 0)
            copyField(setting->receiptFooter, 200, value);
        else if (strcmp(key, "maxLoginAttempt") == 0)
            setting->maxLoginAttempt = fieldInt(value);
        else if (strcmp(key, "dateFormat") == 0)
            copyField(setting->dateFormat, 20, value);
        else if (strcmp(key, "timeFormat") == 0)
            copyField(setting->timeFormat, 20, value);
        else if (strcmp(key, "autoBackup") == 0)
            setting->autoBackup = fieldInt(value);
        else if (strcmp(key, "defaultPromotionPriority") == 0)
            setting->defaultPromotionPriority = fieldInt(value);
    }

    fclose(fp);

    return 1;
}

void saveSettings(Settings setting)
{
    FILE *fp = fopen("data/settings.csv", "w");

    if (fp == NULL)
        return;

    fprintf(fp, "settingName,settingValue\n");
    fprintf(fp, "storeName,%s\n",                 setting.storeName);
    fprintf(fp, "storePassword,%s\n",             setting.password);
    fprintf(fp, "currency,%s\n",                  setting.currency);
    fprintf(fp, "lowStockWarning,%d\n",           setting.lowStockWarning);
    fprintf(fp, "nearExpiryWarningDays,%d\n",     setting.nearExpiryWarningDays);
    fprintf(fp, "allowNegativeProfitPromotion,%d\n", setting.allowNegativeProfitPromotion);
    fprintf(fp, "defaultTaxRate,%.2lf\n",         setting.defaultTaxRate);
    fprintf(fp, "receiptFooter,%s\n",             setting.receiptFooter);
    fprintf(fp, "maxLoginAttempt,%d\n",           setting.maxLoginAttempt);
    fprintf(fp, "dateFormat,%s\n",                setting.dateFormat);
    fprintf(fp, "timeFormat,%s\n",                setting.timeFormat);
    fprintf(fp, "autoBackup,%d\n",                setting.autoBackup);
    fprintf(fp, "defaultPromotionPriority,%d\n",  setting.defaultPromotionPriority);

    fclose(fp);
}

/******************************************************************************
 * End of File : csv.c
 ******************************************************************************/