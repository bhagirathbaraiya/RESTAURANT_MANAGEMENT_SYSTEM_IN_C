#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Table {
    int tableNumber;
    int capacity;
    char diningStatus[15]; 
    char customerID[10]; 
    struct Table *next;
} Table;

Table *tableHead = NULL;

void initializeTables() {
    FILE *file = fopen("Tables.csv", "w");
    if (!file) {
        printf("Error creating Tables.csv file.\n");
        return;
    }
    fprintf(file, "TableNumber,Capacity,DiningStatus,CustomerID\n");

    Table *tail = NULL;

    for (int i = 1; i <= 12; i++) {
        int capacity = (i <= 4) ? 4 : (i <= 8) ? 8 : 10;
        fprintf(file, "%d,%d,Not Dining,-\n", i, capacity);

        Table *newTable = (Table *)malloc(sizeof(Table));
        if (!newTable) {
            printf("Memory allocation failed.\n");
            fclose(file);
            return;
        }

        newTable->tableNumber = i;
        newTable->capacity = capacity;
        strcpy(newTable->diningStatus, "Not Dining");
        strcpy(newTable->customerID, "-");
        newTable->next = NULL;

        if (!tableHead) {
            tableHead = newTable;
            tail = newTable;
        } else {
            tail->next = newTable;
            tail = newTable;
        }
    }

    fclose(file);
    printf("Tables initialized successfully.\n");
}

void loadTables() {
    // Clear existing list to avoid duplicates
    Table *temp;
    while (tableHead) {
        temp = tableHead;
        tableHead = tableHead->next;
        free(temp);
    }

    FILE *file = fopen("Tables.csv", "r");
    if (!file) {
        printf("No Tables.csv file found. Initializing new tables.\n");
        initializeTables();
        return;
    }

    char line[100];
    fgets(line, sizeof(line), file); // Skip header

    Table *tail = NULL;

    while (fgets(line, sizeof(line), file)) {
        Table *newTable = (Table *)malloc(sizeof(Table));
        if (!newTable) {
            printf("Memory allocation failed.\n");
            fclose(file);
            return;
        }

        if (sscanf(line, "%d,%d,%14[^,],%9s", &newTable->tableNumber, &newTable->capacity, 
                   newTable->diningStatus, newTable->customerID) == 4) {
            newTable->next = NULL;

            if (!tableHead) {
                tableHead = newTable;
                tail = newTable;
            } else {
                tail->next = newTable;
                tail = newTable;
            }
        } else {
            free(newTable);
            printf("Error reading line: %s", line);
        }
    }

    fclose(file);
}

void displayTables() {
    if (!tableHead) {
        printf("No tables available.\n");
        return;
    }

    printf("=====================================================\n");
    printf("| %-12s | %-8s | %-12s | %-10s |\n", 
           "Table Number", "Capacity", "Dining Status", "Customer ID");
    printf("=====================================================\n");

    Table *current = tableHead;
    while (current) {
        printf("| %-12d | %-8d | %-12s | %-10s |\n", 
               current->tableNumber, current->capacity, 
               current->diningStatus, current->customerID);
        current = current->next;
    }

    printf("=====================================================\n");
}

void updateTableStatus(int tableNumber, const char *diningStatus, const char *customerID) {
    Table *current = tableHead;
    while (current) {
        if (current->tableNumber == tableNumber) {
            strcpy(current->diningStatus, diningStatus);
            strcpy(current->customerID, customerID);

            FILE *file = fopen("Tables.csv", "w");
            if (!file) {
                printf("Error opening Tables.csv for updating.\n");
                return;
            }

            fprintf(file, "TableNumber,Capacity,DiningStatus,CustomerID\n");

            Table *temp = tableHead;
            while (temp) {
                fprintf(file, "%d,%d,%s,%s\n", temp->tableNumber, temp->capacity, 
                        temp->diningStatus, temp->customerID);
                temp = temp->next;
            }

            fclose(file);
            printf("Table %d updated successfully.\n", tableNumber);
            return;
        }
        current = current->next;
    }
    printf("Table not found.\n");
}

void autoAssignTable(Customer *customer) {
    Table *current = tableHead;

    // Check for an available table
    while (current) {
        if (strcmp(current->diningStatus, "Not Dining") == 0 && current->capacity == customer->totalPersons) {
            // Assign the table to the customer
            strcpy(current->diningStatus, "Dining");
            strcpy(current->customerID, customer->id);
            strcpy(customer->diningStatus, "Dining");
            updateTableStatus(current->tableNumber, current->diningStatus, current->customerID);

            printf("Table %d assigned to Customer ID: %s\n", current->tableNumber, customer->id);
            return; // Exit after assigning
        }
        current = current->next;
    }

    // If no table is available, add the customer to the waiting list
    printf("No table available for Customer ID: %s. Adding to waiting list.\n", customer->id);
    strcpy(customer->diningStatus, "Waiting");

    // Open or create the Waiting.csv file in append mode
    FILE *file = fopen("Waiting.csv", "a");
    if (!file) {
        printf("Error creating or opening Waiting.csv file.\n");
        return;
    }

    // Write the customer's details to the waiting file
    fprintf(file, "%s,%s,%s,%d,%s\n",
            customer->id,
            customer->name,
            customer->mobile,
            customer->totalPersons,
            customer->diningStatus);

    fclose(file); // Close the file
    printf("Customer ID: %s added to the waiting list.\n", customer->id);
}



int TableAvailable(int requiredCapacity) {
    Table *current = tableHead;

    // Traverse the linked list
    while (current) {
        // Check if the table is unoccupied and matches the required capacity
        if (strcmp(current->diningStatus, "Not Dining") == 0 && current->capacity >= requiredCapacity) {
            return 1; // Table found
        }
        current = current->next;
    }

    return 0; // No suitable table found
}
