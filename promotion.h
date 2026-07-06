/******************************************************************************
 * File        : promotion.h
 * Folder      : promotion
 * Project     : Mart Management System
 *
 * Description:
 *     Function declarations for Promotion Management Module.
 ******************************************************************************/

#ifndef PROMOTION_H
#define PROMOTION_H

#include "../common/struct.h"

/*===========================================================================
    PROMOTION MENU
===========================================================================*/

void promotionMenu();

/*===========================================================================
    PROMOTION
===========================================================================*/

void viewPromotions();

void createPromotion();

void addProductsToPromotion();

void editPromotion();

void deletePromotion();

/*===========================================================================
    VALIDATION
===========================================================================*/

int verifyNegativeProfit(double newPrice,
                         double buyPrice);

int promotionExists(const char promotionID[]);

int findPromotionByID(const char promotionID[]);
int findProductByID(const char productID[]);

/*===========================================================================
    HELPER
===========================================================================*/

void calculatePromotionPrice();

void displayPromotionTable();

void displayPromotionProducts(const char promotionID[]);

void removeProductFromPromotion(const char promotionID[],
                                const char productID[]);

#endif