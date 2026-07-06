/******************************************************************************
 * File        : utils.h
 * Folder      : utils
 * Project     : Mart Management System
 *
 * Description:
 *     Function declarations for utility functions.
 ******************************************************************************/

#ifndef UTILS_H
#define UTILS_H

/*===========================================================================
    SCREEN
===========================================================================*/

void clearScreen();

void pauseScreen();

void printLine(int length);

void printSubLine(int length);

void pressEnter();

/*===========================================================================
    INPUT
===========================================================================*/

void inputString(char text[], int size);

int inputInteger();

double inputDouble();

char inputChar();

/*===========================================================================
    VALIDATION
===========================================================================*/

int isEmptyString(const char text[]);

int isPositiveInteger(int number);

int isPositiveDouble(double number);

int isValidChoice(int choice, int min, int max);

int isValidDate(const char date[]);

int isValidPrice(double price);

int isValidQuantity(int quantity);

/*===========================================================================
    ID GENERATION
===========================================================================*/

void generateProductID(char id[], const char prefix[], int number);

void generateBatchID(char batchID[], int number);

void generateTransactionID(char id[], int number);

void generateReturnID(char id[], int number);

void generatePromotionID(char id[], int number);

#endif