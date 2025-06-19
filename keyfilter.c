/**
 * @file keyfilter.c
 * @brief Address filtering and character suggestion system
 * @author Martin Klíma (xklimam00@stud.fit.vutbr.cz)
 * @date 30.10.2023
 * 
 * This program reads addresses from stdin and provides character suggestions
 * based on a given prefix. It can either find an exact match or suggest
 * the next possible characters to continue typing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ADDRESSES 42
#define MAX_ADDRESS_LENGTH 101
#define ASCII_SIZE 256

/**
 * @brief Removes duplicate characters from a string in-place
 * @param arr Input string to process
 * @return Length of the resulting string without duplicates
 */
int removeAllDuplicates(char *arr) {
    int check[ASCII_SIZE] = {0};
    int input = 0;
    int end = 0;

    while (arr[input] != '\0') {
        if (check[(unsigned char)arr[input]] == 0) {
            check[(unsigned char)arr[input]] = 1;
            arr[end] = arr[input];
            end++;
        }
        input++;
    }
    arr[end] = '\0';
    return end;
}

/**
 * @brief Sorts characters in a string by their ASCII values using selection sort
 * @param arr String to sort
 */
void sortASCII(char *arr) {
    size_t len = strlen(arr);
    
    for (size_t i = 0; i < len - 1; i++) {
        size_t min = i;
        for (size_t j = i + 1; j < len; j++) {
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        if (min != i) {
            char temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
    }
}

/**
 * @brief Finds all allowed characters that can follow the given prefix
 * @param prefix The prefix to match against addresses
 * @param addresses Array of addresses to search through
 * @param numAddresses Number of addresses in the array
 * @param allowedChars Output buffer for allowed characters
 */
void showALLOWED_CHARS(const char *prefix, char addresses[][MAX_ADDRESS_LENGTH], 
                       int numAddresses, char *allowedChars) {
    int allowed[ASCII_SIZE] = {0};
    size_t prefixLen = strlen(prefix);

    for (int i = 0; i < numAddresses; i++) {
        if (strncmp(prefix, addresses[i], prefixLen) == 0) {
            if (addresses[i][prefixLen] != '\0') {
                allowed[(unsigned char)tolower(addresses[i][prefixLen])] = 1;
            }
        }
    }

    int end = 0;
    for (int i = 0; i < ASCII_SIZE; i++) {
        if (allowed[i] == 1) {
            allowedChars[end] = (char)i;
            end++;
        }
    }
    allowedChars[end] = '\0';
    
    removeAllDuplicates(allowedChars);
    sortASCII(allowedChars);

    for (int i = 0; allowedChars[i] != '\0'; i++) {
        allowedChars[i] = (char)toupper(allowedChars[i]);
    }
}

/**
 * @brief Converts entire string to uppercase
 * @param str String to convert
 */
void toUpperCase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

/**
 * @brief Searches for addresses matching the prefix and determines if exactly one match exists
 * @param prefix The prefix to search for
 * @param addresses Array of addresses to search through
 * @param numAddresses Number of addresses in the array
 * @param found Output buffer for the found address (if exactly one match)
 * @return Number of matching addresses found
 */
int findAddress(const char *prefix, char addresses[][MAX_ADDRESS_LENGTH], 
                int numAddresses, char *found) {
    int matchCount = 0;
    size_t prefixLen = strlen(prefix);
    
    for (int i = 0; i < numAddresses; i++) {
        if (strncmp(prefix, addresses[i], prefixLen) == 0) {
            if (matchCount == 0) {
                strcpy(found, addresses[i]);
            }
            matchCount++;
            if (matchCount > 1) {
                return matchCount;
            }
        }
    }
    return matchCount;
}

/**
 * @brief Main function - orchestrates the address filtering process
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Exit status (0 for success)
 */
int main(int argc, char *argv[]) {
    char addresses[MAX_ADDRESSES][MAX_ADDRESS_LENGTH];
    int numAddresses = 0;
    char allowedChars[MAX_ADDRESS_LENGTH];
    char found[MAX_ADDRESS_LENGTH];

    while (numAddresses < MAX_ADDRESSES && 
           fgets(addresses[numAddresses], MAX_ADDRESS_LENGTH, stdin) != NULL) {
        size_t len = strlen(addresses[numAddresses]);
        if (len > 0 && addresses[numAddresses][len - 1] == '\n') {
            addresses[numAddresses][len - 1] = '\0';
        }
        toUpperCase(addresses[numAddresses]);
        numAddresses++;
    }

    char *prefix = (argc > 1) ? argv[1] : "";
    
    char prefixCopy[MAX_ADDRESS_LENGTH];
    strncpy(prefixCopy, prefix, MAX_ADDRESS_LENGTH - 1);
    prefixCopy[MAX_ADDRESS_LENGTH - 1] = '\0';
    toUpperCase(prefixCopy);

    showALLOWED_CHARS(prefixCopy, addresses, numAddresses, allowedChars);
    int matchCount = findAddress(prefixCopy, addresses, numAddresses, found);

    if (matchCount == 1) {
        printf("Found: %s\n", found);
    } else if (allowedChars[0] != '\0') {
        printf("Enable: %s\n", allowedChars);
    } else {
        printf("Not found\n");
    }

    return 0;
}