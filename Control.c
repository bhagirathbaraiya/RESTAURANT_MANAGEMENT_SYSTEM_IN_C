#include "Customer.c"
#include "Queue.c"
#include "Table.c"

void StartProject(){
    for(int i=0;i<1;i++){
        initializeTables();
        loadTables();
        loadCustomers();
        loadWaitingQueue();
    }
}