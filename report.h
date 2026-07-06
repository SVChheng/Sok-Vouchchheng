/******************************************************************************
 * File        : report.h
 * Folder      : report
 * Project     : Mart Management System
 *
 * Description:
 *     Function declarations for Report Module.
 ******************************************************************************/

#ifndef REPORT_H
#define REPORT_H

#include "../common/struct.h"

/*===========================================================================
    REPORT MENU
===========================================================================*/

void reportMenu();

/*===========================================================================
    REPORTS
===========================================================================*/

void salesReport();

void productReport();

void profitReport();

void inventoryReport();

/*===========================================================================
    SEARCH & FILTER
===========================================================================*/

void reportByToday();

void reportByDate();

void reportByDateRange();

void reportByTransactionID();

/*===========================================================================
    HELPER
===========================================================================*/

double calculateRevenue();

double calculateCost();

double calculateTotalProfit();

void displayTransactionTable();

void displayProductReport();

void displayInventoryReport();

#endif