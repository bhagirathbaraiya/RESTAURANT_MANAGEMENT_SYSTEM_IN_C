#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct WaitingCustomer {
    char id[4];
    char name[40];
    char mobile[15];
    int totalPersons;
    struct WaitingCustomer *next;
} WaitingCustomer;

WaitingCustomer *waitingHead = NULL;

void loadWaitingQueue() {
    FILE *file = fopen("Waiting.csv", "r");
    if (!file) return; 

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        WaitingCustomer *newCustomer = (WaitingCustomer *)malloc(sizeof(WaitingCustomer));
        sscanf(line, "%[^,],%[^,],%[^,],%d", newCustomer->id, newCustomer->name, 
               newCustomer->mobile, &newCustomer->totalPersons);
        newCustomer->next = waitingHead;
        waitingHead = newCustomer;
    }

    fclose(file);
}

void saveWaitingQueue() {
    FILE *file = fopen("Waiting.csv", "w");
    if (!file) {
        printf("Error saving waiting queue.\n");
        return;
    }

    WaitingCustomer *current = waitingHead;
    while (current) {
        fprintf(file, "%s,%s,%s,%d\n", current->id, current->name, current->mobile, current->totalPersons);
        current = current->next;
    }

    fclose(file);
}

void addToQueue(char id[], char name[], char mobile[], int totalPersons) {
    WaitingCustomer *newCustomer = (WaitingCustomer *)malloc(sizeof(WaitingCustomer));
    strcpy(newCustomer->id, id);
    strcpy(newCustomer->name, name);
    strcpy(newCustomer->mobile, mobile);
    newCustomer->totalPersons = totalPersons;
    newCustomer->next = waitingHead;
    waitingHead = newCustomer;

    saveWaitingQueue();
    printf("No table available. Customer %s added to the waiting queue.\n", id);
}

void displayWaitingQueue() {
    if (!waitingHead) {
        printf("No customers in the waiting queue.\n");
        return;
    }

    printf("=====================================================\n");
    printf("| %-5s | %-40s | %-12s | %-8s |\n", 
           "ID", "Name", "Mobile", "Persons");
    printf("=====================================================\n");

    WaitingCustomer *current = waitingHead;
    while (current) {
        printf("| %-5s | %-40s | %-12s | %-8d |\n", 
               current->id, current->name, current->mobile, current->totalPersons);
        current = current->next;
    }

    printf("=====================================================\n");
}

int assignTableToCustomer(char id[], int totalPersons) {
    FILE *file = fopen("Tables.csv", "r+");
    if (!file) {
        printf("Error opening Tables.csv.\n");
        return 0;
    }

    char line[100];
    char updatedTables[1200] = "";
    int assigned = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "TableNumber") != NULL) { 
            strcat(updatedTables, line);
            continue;
        }

        int tableNumber, capacity;
        char diningStatus[15], occupyingCustomerID[10];
        sscanf(line, "%d,%d,%[^,],%s", &tableNumber, &capacity, diningStatus, occupyingCustomerID);

        if (strcmp(diningStatus, "Not Dining") == 0 && capacity >= totalPersons && !assigned) {
            strcpy(diningStatus, "Dining");
            strcpy(occupyingCustomerID, id);
            assigned = 1;
        }

        char updatedLine[100];
        snprintf(updatedLine, sizeof(updatedLine), "%d,%d,%s,%s\n", tableNumber, capacity, diningStatus, occupyingCustomerID);
        strcat(updatedTables, updatedLine);
    }

    freopen("Tables.csv", "w", file);
    fputs(updatedTables, file);
    fclose(file);

    return assigned;
}

void handleNewCustomer(char id[], char name[], char mobile[], int totalPersons) {
    if (assignTableToCustomer(id, totalPersons)) {
        printf("Customer %s assigned to a table.\n", id);
    } else {
        addToQueue(id, name, mobile, totalPersons);
    }
}
