#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Customer {
    char id[4];
    char name[40];
    char mobile[15];
    int totalPersons;
    char diningStatus[10];
    struct Customer *next;
} Customer;

Customer *head = NULL;

void loadCustomers() {
    FILE *file = fopen("Customers.csv", "r");
    if (!file) return;

    char line[100];
    Customer *tail = NULL;
    while (fgets(line, sizeof(line), file)) {
        Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]", newCustomer->id, newCustomer->name,
               newCustomer->mobile, &newCustomer->totalPersons, newCustomer->diningStatus);
        newCustomer->next = NULL;

        if (!head) {
            head = newCustomer;
            tail = newCustomer;
            int temp = TableAvailable(newCustomer->totalPersons);
            if (temp == 1){
                autoAssignTable(newCustomer);
            }else{
                addToQueue(newCustomer->id, newCustomer->name, newCustomer->mobile, newCustomer->mobile);
            }
        } else {
            tail->next = newCustomer;
            tail = newCustomer;
            int temp = TableAvailable(newCustomer->totalPersons);
            if (temp == 1){
                autoAssignTable(newCustomer);
            }else{
                addToQueue(newCustomer->id, newCustomer->name, newCustomer->mobile, newCustomer->mobile);
            }
        }
    }
    fclose(file);
}

void saveCustomers() {
    FILE *file = fopen("Customers.csv", "w");
    if (!file) {
        printf("Error saving data.\n");
        return;
    }

    Customer *current = head;
    while (current) {
        fprintf(file, "%s,%s,%s,%d,%s\n", current->id, current->name, current->mobile, current->totalPersons, current->diningStatus);
        current = current->next;
    }

    fclose(file);
}

void saveCustomerRealTime(Customer *customer, const char *operation) {
    FILE *file = NULL;
    if (strcmp(operation, "add") == 0) {
        file = fopen("Customers.csv", "a");
        if (!file) {
            printf("Error saving customer data.\n");
            return;
        }
        fprintf(file, "%s,%s,%s,%d,%s\n", customer->id, customer->name, customer->mobile, customer->totalPersons, customer->diningStatus);
        fclose(file);
    } else if (strcmp(operation, "update") == 0 || strcmp(operation, "delete") == 0) {
        saveCustomers();
    }
}

char* generateCID() {
    static int lastID = 0;
    if (head) {
        Customer *current = head;
        while (current) {
            int currentID = atoi(current->id);
            if (currentID > lastID) lastID = currentID;
            current = current->next;
        }
    }
    static char newID[4];
    snprintf(newID, 4, "%03d", lastID + 1);
    return newID;
}

void addCustomer() {
    Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
    strcpy(newCustomer->id, generateCID());
    printf("Enter customer name (max 40 characters): ");
    getchar();
    fgets(newCustomer->name, 40, stdin);
    newCustomer->name[strcspn(newCustomer->name, "\n")] = 0; 
    printf("Enter mobile number: ");
    scanf("%s", newCustomer->mobile);
    printf("Enter total persons: ");
    scanf("%d", &newCustomer->totalPersons);
    strcpy(newCustomer->diningStatus, "Dining");

    newCustomer->next = NULL;

    if (!head) {
        head = newCustomer;
    } else {
        Customer *current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newCustomer;
    }
    saveCustomerRealTime(newCustomer, "add");
    int temp = TableAvailable(newCustomer->totalPersons);
    if(temp==1){
        autoAssignTable(newCustomer);
    }else{
        addToQueue(newCustomer->id,newCustomer->name,newCustomer->mobile,newCustomer->mobile);
    }
    printf("Customer added successfully with ID: %s\n", newCustomer->id);
}

void editCustomer() {
    char id[4];
    printf("Enter Customer ID to edit: ");
    scanf("%s", id);

    Customer *current = head;
    while (current) {
        if (strcmp(current->id, id) == 0) {
            int choice;
            printf("Editing Customer ID: %s\n", id);
            do {
                printf("\nSelect field to edit:\n");
                printf("1. Name\n");
                printf("2. Mobile Number\n");
                printf("3. Total Persons\n");
                printf("4. Edit All\n");
                printf("5. Cancel\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        printf("Enter new name [old: %s]: ", current->name);
                        getchar();
                        fgets(current->name, 40, stdin);
                        current->name[strcspn(current->name, "\n")] = 0;
                        printf("Name updated successfully.\n");
                        break;
                    case 2:
                        printf("Enter new mobile number [old: %s]: ", current->mobile);
                        scanf("%s", current->mobile);
                        printf("Mobile number updated successfully.\n");
                        break;
                    case 3:
                        printf("Enter new total persons [old: %d]: ", current->totalPersons);
                        scanf("%d", &current->totalPersons);
                        printf("Total persons updated successfully.\n");
                        break;
                    case 4:
                        printf("Enter new name [old: %s]: ", current->name);
                        getchar();
                        fgets(current->name, 40, stdin);
                        current->name[strcspn(current->name, "\n")] = 0;
                        printf("Enter new mobile number [old: %s]: ", current->mobile);
                        scanf("%s", current->mobile);
                        printf("Enter new total persons [old: %d]: ", current->totalPersons);
                        scanf("%d", &current->totalPersons);
                        printf("All fields updated successfully.\n");
                        break;
                    case 5:
                        printf("Edit operation canceled.\n");
                        return;
                    default:
                        printf("Invalid choice. Try again.\n");
                }
            } while (choice != 5);
            saveCustomers();
            return;
        }
        current = current->next;
    }

    printf("Customer ID not found.\n");
}

void searchCustomer() {
    char id[4];
    printf("Enter Customer ID to search: ");
    scanf("%s", id);

    Customer *current = head;
    while (current) {
        if (strcmp(current->id, id) == 0) {
            printf("Customer Found: ID=%s, Name=%s, Mobile=%s, Persons=%d, Status=%s\n",
                   current->id, current->name, current->mobile, current->totalPersons, current->diningStatus);
            return;
        }
        current = current->next;
    }

    printf("Customer ID not found.\n");
}

void displayCustomers() {
    if (!head) {
        printf("No customers to display.\n");
        return;
    }

    printf("===========================================================================================\n");
    printf("| %-5s | %-40s | %-12s | %-8s | %-10s |\n", 
           "ID", "Name", "Mobile", "Persons", "Status");
    printf("===========================================================================================\n");

    Customer *current = head;
    while (current) {
        printf("| %-5s | %-40s | %-12s | %-8d | %-5s |\n", 
               current->id, current->name, current->mobile, 
               current->totalPersons, current->diningStatus);
        current = current->next;
    }

    printf("===========================================================================================\n");
}

