#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Control.c"

// PassWords
#define ADMIN_PASS "admin"
#define DESC_PASS "desc"

void Admin();
void Reception();

void login() {
    char password[20];
    int attempts = 3;

    printf("\n\n\t *** MY HOTEL ***\n\n\n");
    printf("Enter your password to log in.\n\n");

    while (attempts > 0) {
        printf("Password:- ");
        scanf("%s", password);

        if (strcmp(password, ADMIN_PASS) == 0) {
            printf(" Welcome Admin.\n");
            StartProject();
            Admin();
            return;
        } else if (strcmp(password, DESC_PASS) == 0) {
            printf("Welcome Desc.\n");
            StartProject();
            Reception();
            return;
        } else {
            printf("Invalid password. You have %d attempt left.\n", attempts);
            attempts=attempts-1;
        }
    }

    printf("Too many failed attempts. Exiting...\n");
}


void Admin(){
    int choice = 0;
    int loop = 1;

    while (loop!=0){
        printf("Admin Pannel");
        REWRITE:
        printf("\n 1. View Customers");
        printf("\n 2. View Queue");
        printf("\n 3. View Tables");
        printf("\n 101. For Reset data");
        printf("\n 4. Exit");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                displayCustomers();
                goto REWRITE;
                break;
            case 2:
                displayWaitingQueue();
                goto REWRITE;
                break;
            case 3:
                displayTables();
                goto REWRITE;
                break;
            case 101:
                ResetAll();
                goto REWRITE;
                break;
            case 4:
                exit(1);
                break;
            default:
                goto REWRITE;
                break;

        }
    }
}

void Reception(){
    int choice = 0;
    int loop = 1;

    while (loop!=0){
        printf("Reception Pannel");
        REWRITE:
        printf("\n 1. Add Customer");
        printf("\n 2. Edit Customer");
        printf("\n 3. View Customers");
        printf("\n 4. View Waiting Customers");
        printf("\n 5. View Dining Customers");
        printf("\n 6. Exit");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                addCustomer();
                goto REWRITE;
                break;
            case 2:
                editCustomer();
                goto REWRITE;
                break;
            case 3:
                displayCustomers();
                goto REWRITE;
                break;
            case 4:
                displayWaitingQueue();
                goto REWRITE;
                break;
            case 5:
                displayTables();
                goto REWRITE;
                break;
            case 6:
                exit(1);
                break;
            default:
                goto REWRITE;
                break;

        }
    }
}


void ResetAll() {
    FILE *waitingFile = fopen("Waiting.csv", "w");
    if (waitingFile) {
        fprintf(waitingFile, "CustomerID,Name,MobileNumber,TotalPersons,DiningStatus\n");
        fclose(waitingFile);
        printf("Waiting.csv cleaned successfully.\n");
    } else {
        printf("Error cleaning Waiting.csv.\n");
    }

    FILE *tablesFile = fopen("Tables.csv", "w");
    if (tablesFile) {
        fprintf(tablesFile, "TableNumber,Capacity,DiningStatus,CustomerID\n");
        fclose(tablesFile);
        printf("Tables.csv cleaned successfully.\n");
    } else {
        printf("Error cleaning Tables.csv.\n");
    }

    FILE *customersFile = fopen("Customers.csv", "w");
    if (customersFile) {
        fprintf(customersFile, "CustomerID,Name,MobileNumber,TotalPersons,DiningStatus\n");
        fclose(customersFile);
        printf("Customers.csv cleaned successfully.\n");
    } else {
        printf("Error cleaning Customers.csv.\n");
    }
}
