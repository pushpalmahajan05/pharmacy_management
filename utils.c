#include "include.h"
#include "trie.h"

bool Is_Supplier_Present_In_List(PtrNode *head, Supplier *s)
{
    bool found = false;
    while (head && !found) {
        if (head->ref == s) {
            found = true;
        }
        head = head->next;
    }
    return found;
}

bool Is_Medication_Present_In_List(ListNode *head, uint64_t index)
{
    bool found = false;
    while (head && !found) {
        if (head->val == index) {
            found = true;
        }
        head = head->next;
    }
    return found;
}

void Insert_Supplier_Ref_In_List(Medication *medicine, Supplier *s)
{
    PtrNode *new = malloc(sizeof(PtrNode));
    new->ref = (void *)s;
    new->next = medicine->supplier_list;
    medicine->supplier_list = new;
}

void Insert_Medication_Ref_In_List(Supplier *s, int index)
{
    ListNode *new = (ListNode *)malloc(sizeof(ListNode));
    new->val = index;
    new->next = s->idx_list;
    s->idx_list = new;
    s->num_unique_med += 1;
}

void Delete_Medication_Ref_In_List(Supplier *sup, uint64_t index)
{
    ListNode *temp;
    ListNode dummy;
    dummy.next = sup->idx_list;
    ListNode *head = &dummy;
    bool found = false;
    while (head && (head->next) && !found) {
        if ((head->next)->val == index) {
            temp = head->next;
            head->next = temp->next;
            free(temp);
            found = true;
            sup->num_unique_med -= 1;
        }
        head = head->next;
    }
    sup->num_unique_med -= found;
    sup->idx_list = dummy.next;
}

void Delete_Supplier_Ref_In_List(Medication *medicine, Supplier *supplier)
{
    PtrNode *temp;
    PtrNode dummy;
    dummy.next = medicine->supplier_list;
    PtrNode *head = &dummy;
    while (head && (head->next)) {
        if ((head->next)->ref == supplier) {
            temp = head->next;
            head->next = temp->next;
            free(temp);
        }
        head = head->next;
    }
    medicine->supplier_list = dummy.next;
}

void Associate_Supplier_With_Medication(Medication *med, Supplier *sup, int index)
{
    Insert_Supplier_Ref_In_List(med, sup);
    Insert_Medication_Ref_In_List(sup, index);
}

void Dissociate_Supplier_Medication(Inventory *inventory, Supplier *s)
{
    ListNode *head = s->idx_list;
    while (head) {
        int64_t index = head->val;
        Delete_Supplier_Ref_In_List(inventory->shelf[index], s);
        head = head->next;
    }
}

void Dissociate_Medication_Supplier(int index, Medication *medicine)
{
    PtrNode *head = medicine->supplier_list;
    while (head) {
        Supplier *s = (Supplier *)(head->ref);
        Delete_Medication_Ref_In_List(s, index);
        head = head->next;
    }
}

void Find_Medication_By_Supplier(Inventory *inventory)
{
    char s_id[16];
    inputSupplierID(s_id);
    Supplier *PP = (Supplier *)search_in_trie(Supplier_ID2PrevPtr, s_id);
    if (PP) {
        Medication **shelf = inventory->shelf;
        Supplier *supplier = PP->next;
        ListNode *head = supplier->idx_list;
        while (head) {
            Medication *medicine = shelf[head->val];
            printf("Medication Name : %s\nMedication ID : %s\nQuantity : %d", medicine->medication_name, medicine->medication_id, medicine->total_quantity);
            head = head->next;
        }
    }
}

void Find_Supplier_By_Medicaiton(Inventory *inventory)
{
    char med_id[16];
    input_Medication_ID(med_id);
    uint64_t index = (uint64_t) search_in_trie(Medication_ID2Index, med_id);
    if (index) {
        PtrNode *head = inventory->shelf[index]->supplier_list;
        while (head) {
            Supplier *supplier = (Supplier *) head->ref;
            printf("\nSupplier Name  : %s\nSupplier ID    : %s\nSupplier Cotact:%ld\n", supplier->supp_name, supplier->supp_id, supplier->contact);
            head = head->next;
        }
    }
}

Inventory *create_inventory(uint32_t size)
{
    if (size <= 1) size = 2;
    Inventory *new = malloc(sizeof(Inventory));
    new->shelf = calloc(size, sizeof(Medication *));
    new->shelf[0] = NULL;
new->total_batches = 0;
    new->total_batches = 0;
    new->total_medications = 0;
    new->capacity = size;
    new->index = 1;
    new->free_list = NULL;
    return new;
}

void free_list(ListNode *head)
{
    ListNode *toFree;
    while (head) {
        toFree = head;
        head = head->next;
        free(toFree);
    }
}

void free_ptr_list(PtrNode *head)
{
    PtrNode *toFree;
    while (head) {
        toFree = head;
        head = head->next;
        free(toFree);
    }
}

void free_inventory(Inventory *inventory)
{
    if (inventory) {
        Medication **shelf = inventory->shelf;
        int size = inventory->index;
        for (int i = 1; i < size; ++i) {
            if (shelf[i]) {
                delete_list_batch(inventory, (shelf[i]->next_batch)->next);
                free_ptr_list(shelf[i]->supplier_list);
                free(shelf[i]);
            }
        }
        free_list(inventory->free_list);
        free(shelf);
        free(inventory);
    }
}

void print_sup(Supplier *head)
{
    while (head) {
        printf("%s %s %ld %f\n", head->supp_name, head->supp_id, head->contact, head->turnover);
        head = head->next;
    }
    printf("\n");
}

void print_batch_medication(Batch *head)
{
    while (head) {
        printf("\t%s %d %d %f\n", head->batch_number, head->batch_quantity, head->expiration_date, head->purchase_price);
        head = head->next;
    }
    printf("\n");
}

void print_med_inventory(Inventory *inventory)
{
    Medication **shelf = inventory->shelf;
    int size = inventory->index;
    for (int i = 1; i < size; ++i) {
        if (shelf[i]) {
            Medication *medicine = shelf[i];
            printf("%s : %s : %f : %d : %d : %d\n",
                   medicine->medication_id,
                   medicine->medication_name,
                   medicine->price_per_unit,
                   medicine->reorder_level,
                   medicine->total_quantity,
                   medicine->total_sales);
            print_batch_medication((medicine->next_batch->next));
        }
    }
    printf("\n");
}

void free_sup_smol(Supplier *head)
{
    Supplier *toFree;
    while (head) {
        toFree = head;
        head = head->next;
        free_list(toFree->idx_list);
        free(toFree);
    }
}

void Add_Medication(Inventory *inventory,
                    char *m_id,
                    char *m_name,
                    int32_t quantity_in_stock,
                    real price_per_unit,
                    uint32_t reorder_level,
                    int32_t total_sales
                    ) 
{
    Medication *new = (Medication*)calloc(1, sizeof(Medication) + sizeof(Batch));
    new->next_batch = (Batch *)((uint8_t *)new + sizeof(Medication));
    new->tail = new->next_batch;
    strcpy(new->medication_id, m_id);
    strcpy(new->medication_name, m_name);
    new->total_quantity = quantity_in_stock;
    new->price_per_unit = price_per_unit;
    new->reorder_level = reorder_level;
    new->total_sales = total_sales;
    inventory->total_medications += 1;
    int32_t index = -1;

    if (inventory->free_list) {
        ListNode *temp = inventory->free_list;
        index = temp->val;
        (inventory->free_list) = (inventory->free_list)->next;
        free(temp);
        temp = NULL;
    }
    else if (inventory->index == inventory->capacity) {
        inventory->capacity <<= 1;
        inventory->shelf = (Medication **)realloc(inventory->shelf, (inventory->capacity) * sizeof(Medication *));
        index = inventory->index;
        inventory->index += 1;
    }
    else {
        index = inventory->index;
        inventory->index += 1;
    }

    insert_in_trie(Medication_ID2Index, m_id, (void *)(uint64_t)index);
    insert_in_trie(Medication_Name2Index, m_name, (void *)(uint64_t)index);
    inventory->shelf[index] = new;
}

void Add_Supplier(SupplierList *supplierList,
                 char *new_name,
                 char *new_id,
                 real new_turnover,
                 uint64_t new_contact)
{
    Supplier *ListHead = supplierList->next;
    Supplier *new = (Supplier *)malloc(sizeof(Supplier));
    memset(new, 0, sizeof(Supplier));
    strcpy(new->supp_name, new_name);
    strcpy(new->supp_id, new_id);
    new->contact = new_contact;
    new->turnover = new_turnover;
    new->next = ListHead;

    if (ListHead) {
        delete_from_trie(Supplier_ID2PrevPtr, ListHead->supp_id);
        insert_in_trie(Supplier_ID2PrevPtr, ListHead->supp_id, new);
    }
    insert_in_trie(Supplier_ID2PrevPtr, new->supp_id, supplierList);
    supplierList->next = new;
}

void Add_Batch_To_Medication(Inventory *inventory,
                             int med_index,
                             int batch_quantity,
                             int expiration_date,
                             char *batch_number,
                             real price,
                             char *supplier_id)
{
    Medication *medicine = inventory->shelf[med_index];
    inventory->total_batches += 1;
    Batch *new = (Batch *) malloc(sizeof(Batch));

    new->next = NULL;
    new->purchase_price = price;
    new->expiration_date = expiration_date;
    strcpy(new->batch_number, batch_number);
    new->index = med_index;
    new->batch_quantity = batch_quantity;
    strcpy(new->supplier_id, supplier_id); 
    medicine->total_quantity += batch_quantity;

    if (medicine->tail == medicine->next_batch) {
        (medicine->next_batch)->next = new;
        medicine->tail = new;
        insert_in_trie(Batch_ID2PrevPtr, batch_number, medicine->next_batch);
    }
    else if ((medicine->tail)->expiration_date <= expiration_date) {
        (medicine->tail)->next = new;
        medicine->tail = new;
        insert_in_trie(Batch_ID2PrevPtr, batch_number, medicine->tail);
        medicine->tail = new;
    }
    else {
        Batch *PP = (medicine->next_batch);
        while (PP->next != NULL && ((PP->next)->expiration_date < expiration_date)) {
            PP = PP->next;
        }
        delete_from_trie(Batch_ID2PrevPtr, (PP->next)->batch_number);
        insert_in_trie(Batch_ID2PrevPtr, (PP->next)->batch_number, new);
        insert_in_trie(Batch_ID2PrevPtr, batch_number, PP);
        new->next = PP->next;
        PP->next = new;
    }
    Supplier *PP = (Supplier *) search_in_trie(Supplier_ID2PrevPtr, supplier_id);
    Supplier *found = PP->next;
    if (!Is_Supplier_Present_In_List(medicine->supplier_list, found)) {
        Associate_Supplier_With_Medication(medicine, found, med_index);
    }
    found->turnover += price * batch_quantity;
}

void add_new_batch(Inventory *inventory)
{
    char med_id[16];
    int64_t index;
    input_Medication_ID(med_id);
    void *found = search_in_trie(Medication_ID2Index, med_id);
    if (!found) {
        printf("No such medication id exists\n!!!ADD MEDICATION FIRST!!!\n... exiting");
        return;
    }
    index = (int64_t) found;
    char buffer[24];
    printf("Enter the batch number(ID) (max_characters = 16) : ");
    scanf("%s", buffer);
    Batch *preptr = search_in_trie(Batch_ID2PrevPtr, buffer);
    if (preptr) {
        printf("Batch with given batch ID exists already ... exiting\n");
    }
    else {
        /*Medication *medicine = inventory->shelf[dex];*/
        real price;
        int quantity;
        printf("Enter the following details\n");

        printf("Buying price : ");
        scanf("%f", &price);
        if (price <= 0) {
            printf("Invalid price ... exiting\n");
            return;
        }

        printf("Quantity of medications : ");
        scanf("%d", &quantity);
        if (quantity == 0) {
            printf("Can't add batches without medications ... exiting\n");
            return;
        }

        int date;
        printf("Expiration date (YYYYMMDD) : ");
        scanf("%d", &date);
        getchar();
        char s_id[16];
        inputSupplierID(s_id);
        found = search_in_trie(Supplier_ID2PrevPtr, s_id);
        if (!found) {
            printf("No Supplier with this ID exists\n!!!ADD SUPPLIER FIRST!!!... exiting\n");
            return;
        }

        Add_Batch_To_Medication(inventory,
                                index,
                                quantity,
                                date,
                                buffer,
                                price,
                                s_id);
        printf("Batch added successfully\n");
    }
}

void Update_Medication(Inventory *inventory)
{
    char m_id[17];
    input_Medication_ID(m_id);
    uint64_t index = (uint64_t) search_in_trie(Medication_ID2Index, m_id);
    if (index) {
        int option;
        Medication *medicine = inventory->shelf[index];
        printf("To Change:\n\t1. Price Per Unit\n\t2. Quantity In Stock\n");
        scanf("%d", &option);
        switch (option) {
            case 1:
                {
                    real new_price;
                    printf("Enter new price : ");
                    scanf("%f", &new_price);
                    if (new_price > 0) {
                        medicine->price_per_unit = new_price;
                        printf("Price Per Unit has been updated successfully\n");
                    }
                    break;
                }
            case 2:
                {
                    getchar();
                    add_new_batch(inventory);
                    break;
                }
        }
    }
}

void delete_batch(Inventory *inventory)
{
    char b_id[16];
    printf("Enter Batch ID\n");
    scanf("%s", b_id);
    Batch *PP = (Batch *) search_in_trie(Batch_ID2PrevPtr, b_id);
    if (PP) {
        Batch *temp = PP->next;
        PP->next = temp->next;
        inventory->shelf[temp->index]->total_quantity -= temp->batch_quantity;
        if (PP->next != NULL) {
            delete_from_trie(Batch_ID2PrevPtr, (PP->next)->batch_number);
            insert_in_trie(Batch_ID2PrevPtr ,((PP->next)->batch_number), PP);
        }
        free(temp);
        printf("Batch deleted successfully\n");
        inventory->total_batches -= 1;
    }
    else {
        printf("Batch ID does not exist\n");
    }
}

void delete_list_batch(Inventory *inventory, Batch *head)
{
    Batch *batch_to_free;
    while (head) 
    {
        delete_from_trie(Batch_ID2PrevPtr, head->batch_number);
        batch_to_free = head;
        head = head->next;
        free(batch_to_free);
        inventory->total_batches -= 1;
    }
}

void delete_medication_id(Inventory *inventory)
{
    char id[17];
    input_Medication_ID(id);
    uint64_t index = (uint64_t)search_in_trie(Medication_ID2Index, id);
    if (index > 0) {
        Dissociate_Medication_Supplier(index, inventory->shelf[index]);
        delete_list_batch(inventory, ((inventory->shelf[index])->next_batch)->next);
        free_ptr_list((inventory->shelf[index])->supplier_list);
        delete_from_trie(Medication_ID2Index, id);
        delete_from_trie(Medication_Name2Index, id);

        ListNode *new_node = (ListNode*)malloc(sizeof(ListNode));
        new_node->val = index;
        new_node->next = inventory->free_list;
        inventory->free_list = new_node;
        free(inventory->shelf[index]);
        inventory->shelf[index] = NULL;
        printf("All records of %s have been removed successfully\n", id);
        inventory->total_medications -= 1;
    }
    else {
        printf("Medication ID does not exist\n");
    }
}

void Delete_Supplier(Inventory *inventory)
{
    char supp_id[16];
    inputSupplierID(supp_id);
    void *searchResult = search_in_trie(Supplier_ID2PrevPtr, supp_id);
    if (searchResult) {
        Supplier *PP = (Supplier *) searchResult;
        Supplier *temp = PP->next;
        PP->next = temp->next;
        Dissociate_Supplier_Medication(inventory, temp);
        if (temp->next) {
            delete_from_trie(Supplier_ID2PrevPtr, (temp->next)->supp_id);
            insert_in_trie(Supplier_ID2PrevPtr, (temp->next)->supp_id, PP);
        }
        free(temp);
        printf("Supplier deleted successfully\n");
    }
    else {
        printf("Supplier with ID : %s does not exist ... exiting\n", supp_id);
    }
}

void Update_Supplier()
{
    char supp_id[16];
    inputSupplierID(supp_id);
    void *searchResult = search_in_trie(Supplier_ID2PrevPtr, supp_id);
    if (searchResult) {
        Supplier *PP = (Supplier *) searchResult;
        Supplier *supp = PP->next; 
        int option;
        print_supplier(supp);
        printf("\nWhat to change?\n\t1. ID\n\t2. Name\n\t3. Contact No.\n\t4. Turnover\n[1/2/3] : ");
        scanf("%d", &option);
        getchar();
        switch (option) {
            case 1:
                {
                    char new_id[16];
                    inputSupplierID(new_id);
                    void *isAlreadyPresent = search_in_trie(Supplier_ID2PrevPtr, new_id);
                    if (!isAlreadyPresent) {
                        delete_from_trie(Supplier_ID2PrevPtr, supp_id);
                        insert_in_trie(Supplier_ID2PrevPtr, new_id, PP);
                        strcpy(supp->supp_id, new_id);
                        printf("ID updated successfully\n");
                    }
                    else {
                        printf("Supplier with ID: %s, already exists ... exiting\n", new_id);
                    }
                    break;
                }
            case 2:
                {
                    char new_name[64];
                    inputSupplierName(new_name);
                    strcpy(supp->supp_name, new_name);
                    printf("Supplier name updated successfully\n");
                    break;
                }
            case 3:
                {
                    uint64_t number;
                    inputSupplierContact(&number);
                    supp->contact = number;
                    printf("Supplier contact updated successfully\n");
                    break;
                }
            case 4:
                {
                    real new_turnover;
                    inputSupplierTurnover(&new_turnover);
                    if (new_turnover >= 0.0) {
                        supp->turnover = new_turnover;
                    }
                    else {
                        printf("Invalid turnover ... exiting\n");
                    }
                    break;
                }
            default:
                printf("Invalid option ... exiting\n");
        }
    }
    else {
        printf("Supplier ID does not exist ... exiting\n");
    }
}


void Print_Top_Ten_All_Rounder_Suppliers(SupplierList *list)
{
    Supplier *current = list->next;
    if (current) {
        Supplier temp[10];
        Supplier *space[11];
        Supplier **Top = space + 1;
        memset(temp, 0, sizeof(Supplier) * 10);
        for (int i = 0; i < 10; ++i) {
            Top[i] = temp + i;
        }
        int count = 0;
        while (current) {
            int unique = current->num_unique_med;
            int pos = -1;
            Supplier *T1, *T;
            pos += (unique > Top[0]->num_unique_med);
            pos += (unique > Top[1]->num_unique_med);
            pos += (unique > Top[2]->num_unique_med);
            pos += (unique > Top[3]->num_unique_med);
            pos += (unique > Top[4]->num_unique_med);
            pos += (unique > Top[5]->num_unique_med);
            pos += (unique > Top[6]->num_unique_med);
            pos += (unique > Top[7]->num_unique_med);
            pos += (unique > Top[8]->num_unique_med);
            pos += (unique > Top[9]->num_unique_med);
            T   = Top[pos];
            switch (pos) {
                case 9:
                    T1     = Top[8];
                    Top[8] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 8:
                    T1     = Top[7]; 
                    Top[7] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 7:
                    T1     = Top[6];
                    Top[6] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 6:
                    T1     = Top[5];
                    Top[5] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 5:
                    T1     = Top[4];
                    Top[4] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 4:
                    T1     = Top[3];
                    Top[3] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 3:
                    T1     = Top[2];
                    Top[2] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 2:
                    T1     = Top[1];
                    Top[1] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 1:
                    T1     = Top[0];
                    Top[0] = T;
                    T      = T1;
            }
            Top[pos] = current;
            current = current->next;
            count++;
        }
        if (count < 10) {
            for (int i = count - 1; i >= 0; --i) {
                printf("%s : %s : %d\n", Top[i]->supp_id, Top[i]->supp_name, Top[i]->num_unique_med);
            }
        }
        else {
            for (int i = 9; i >= 0; --i) {
                printf("%s : %s : %d\n", Top[i]->supp_id, Top[i]->supp_name, Top[i]->num_unique_med);
            }
        }
    }
    else {
        printf("No Suppliers are present\n");
    }
}

void Print_Top_Ten_Turnover_Suppliers(SupplierList *list)
{
    Supplier *current = list->next;
    if (current) {
        Supplier temp[10];
        Supplier *space[11];
        Supplier **Top = space + 1;
        memset(temp, 0, sizeof(Supplier) * 10);
        for (int i = 0; i < 10; ++i) {
            Top[i] = temp + i;
        }
        int count = 0;
        while (current) {
            real unique = current->turnover;
            int pos = -1;
            Supplier *T1, *T;
            pos += (unique > Top[0]->turnover);
            pos += (unique > Top[1]->turnover);
            pos += (unique > Top[2]->turnover);
            pos += (unique > Top[3]->turnover);
            pos += (unique > Top[4]->turnover);
            pos += (unique > Top[5]->turnover);
            pos += (unique > Top[6]->turnover);
            pos += (unique > Top[7]->turnover);
            pos += (unique > Top[8]->turnover);
            pos += (unique > Top[9]->turnover);
            T   = Top[pos];
            switch (pos) {
                case 9:
                    T1     = Top[8];
                    Top[8] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 8:
                    T1     = Top[7]; 
                    Top[7] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 7:
                    T1     = Top[6];
                    Top[6] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 6:
                    T1     = Top[5];
                    Top[5] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 5:
                    T1     = Top[4];
                    Top[4] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 4:
                    T1     = Top[3];
                    Top[3] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 3:
                    T1     = Top[2];
                    Top[2] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 2:
                    T1     = Top[1];
                    Top[1] = T;
                    T      = T1;
                    __attribute__ ((fallthrough));
                case 1:
                    T1     = Top[0];
                    Top[0] = T;
                    T      = T1;
            }
            Top[pos] = current;
            current = current->next;
            count++;
        }
        if (count < 10) {
            for (int i = count - 1; i >= 0; --i) {
                printf("%s : %s : %f\n", Top[i]->supp_id, Top[i]->supp_name, Top[i]->turnover);
            }
        }
        else {
            for (int i = 9; i >= 0; --i) {
                printf("%s : %s : %f\n", Top[i]->supp_id, Top[i]->supp_name, Top[i]->turnover);
            }
        }
    }
    else {
        printf("No Suppliers are present\n");
    }
}

void Search_Supplier_By_ID()
{
    char supp_id[16];
    inputSupplierID(supp_id);
    void *searchResult = search_in_trie(Supplier_ID2PrevPtr, supp_id);
    if (searchResult) {
        Supplier *PP = (Supplier *)searchResult;
        Supplier *found = PP->next;
        print_supplier(found);
    }
    else {
        printf("Supplier ID does not exist\n");
    }
}

void check_expiration(Inventory *inventory)
{
    int date;
    printf("Enter date (YYYYMMDD): ");
    scanf("%d", &date);
    int days = days_since_2000(date);
    uint64_t size = inventory->index;
    uint64_t index = 1LLU;
    Medication **shelf = inventory->shelf;
    Medication *medicine;
    Batch *batch;
    while (index < size)
    {
        medicine = shelf[index];
        if (medicine != NULL)
        {
            batch = (medicine->next_batch)->next;
            if (batch) {
                printf("%s : ", medicine->medication_id);
                int diff = days_since_2000(batch->expiration_date) - days;
                if (diff <= 30)
                {
                    if(diff < 0)
                    {
                        printf("has already expired.\n");
                    }
                    else {
                        printf("is going to expire in %d days\n", diff);
                    }
                }
                else {
                    printf("%d days left\n", diff);
                }
            }
        }
        ++index;
    }
}

void stock_alerts(Inventory *inventory)
{
    uint64_t size = inventory->index;
    uint64_t index = 1LLU;
    Medication **shelf = inventory->shelf;
    int count = 0;
    printf("Medication needed to be reordered :\n");
    while (index < size) {
        Medication *medicine = shelf[index];
        if (medicine != NULL)
        {
            if (medicine->total_quantity <= medicine->reorder_level)
            {
                ++count;
                printf(" %d. %s : %s : %d : %d \n",
                       count,
                       medicine->medication_name,
                       medicine->medication_id,
                       medicine->total_quantity,
                       medicine->reorder_level);
            }
        }
        ++index;
    }
    if (count == 0) {
        printf("None\n");
    }
}

void *Make_temp(Inventory *inventory) {
    size_t size = (inventory->total_batches) * sizeof(BatchRef) + 
                  (inventory->total_medications + 2) * sizeof(Holder);
    uint8_t *space = (uint8_t*)malloc(size);
    if (!space) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    size_t curr_byte_index = 0;
    Holder *head = NULL, *current = NULL;
    BatchRef *batch_ref_curr;
    uint64_t i = 1;
    
    while (i < inventory->index) {
        if (inventory->shelf[i] && inventory->shelf[i]->next_batch && 
            inventory->shelf[i]->next_batch->next) {
            
            Holder *newHolder = (Holder*)(space + curr_byte_index);
            curr_byte_index += sizeof(Holder);
            newHolder->next = NULL;
            newHolder->Batchlist = NULL;

            if (!head) {
                head = newHolder;
            } else {
                current->next = newHolder;
            }
            current = newHolder;

            Batch *batch = inventory->shelf[i]->next_batch->next;
            BatchRef *dummy_head = NULL, *tail = NULL;

            while (batch) {
                batch_ref_curr = (BatchRef *)(space + curr_byte_index);
                curr_byte_index += sizeof(BatchRef);
                batch_ref_curr->ref = batch;
                batch_ref_curr->exp_date = batch->expiration_date;
                batch_ref_curr->next = NULL;

                if (!dummy_head) {
                    dummy_head = batch_ref_curr;
                } else {
                    tail->next = batch_ref_curr;
                }
                tail = batch_ref_curr;

                batch = batch->next;
            }
            current->Batchlist = dummy_head;
        }
        ++i;
    }
    current->next = (Holder *)(space + curr_byte_index);
    curr_byte_index += sizeof(Holder);
    current = current->next;
    current->Batchlist = NULL;
    current->next = (Holder *)(space + curr_byte_index);
    current = current->next;
    current->Batchlist = NULL;
    current->next = NULL;
    return head;
}

void print_temp(Inventory *inventory, Holder *holder)
{
    while (holder) {
        BatchRef *batch = holder->Batchlist;
        while (batch) {
            printf("%s %s\n", (inventory->shelf[(batch->ref)->index])->medication_name, (batch->ref)->batch_number);
            batch = batch->next;
        }
        printf("\n");
        holder = holder->next;
    }
}

BatchRef *merge_batch_ref(BatchRef *l1, BatchRef *l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    BatchRef *result = NULL, *tail = NULL;

    while (l1 && l2) {
        BatchRef *chosen = (l1->exp_date < l2->exp_date) ? l1 : l2;
        if (l1->exp_date < l2->exp_date) {
            l1 = l1->next;
        } else {
            l2 = l2->next;
        }

        if (!result) {
            result = chosen;
        } else {
            tail->next = chosen;
        }
        tail = chosen;
    }

    if (l1) tail->next = l1;
    if (l2) tail->next = l2;

    return result;
}

void sort_by_expiry(Inventory *inventory) {
    Holder *auxiliary = Make_temp(inventory);
    /*print_temp(inventory, auxiliary);*/
    if (!auxiliary) return;

    BatchRef *temp;
    Holder *toBeNull = auxiliary->next;

    while (toBeNull->Batchlist) {
        Holder *writeHead = auxiliary;
        Holder *readHead = auxiliary;
        while (readHead->Batchlist || (readHead->next)->Batchlist) {
            temp = merge_batch_ref(readHead->Batchlist, (readHead->next)->Batchlist);
            readHead->Batchlist = NULL;
            readHead->next->Batchlist = NULL;
            writeHead->Batchlist = temp;
            readHead = readHead->next->next;
            writeHead = writeHead->next;
        }
    }
    temp = auxiliary->Batchlist;
    printf("==========Sorted===========\n\n");
    while (temp) {
        int temp_date = temp->exp_date;
        printf("Medication Name: %s\n", inventory->shelf[temp->ref->index]->medication_name);
        printf("Medication ID: %s\n", inventory->shelf[temp->ref->index]->medication_id);
        printf("Batch Number: %s\n", temp->ref->batch_number);
        printf("Expiry Date: %02d/%02d/%d\n\n", 
               temp_date % 100, (temp_date / 100) % 100, temp_date / 10000);
        temp = temp->next;
    }

    free(auxiliary);
}

void sell_medication(Inventory *inventory)
{
    char buffer[16];
    input_Medication_ID(buffer);
    uint64_t index = (uint64_t) search_in_trie(Medication_ID2Index, buffer);
    if (index > 0) {
        Medication *medicine = inventory->shelf[index];
        if (!medicine) {
            printf("Medicine does not exist\n");
            return;
        }
        int32_t toSell;
        printf("Enter quantity to sell : ");
        scanf("%d", &toSell);
        if (toSell <= 0) {
            printf("Invalid quantity\n");
            return;
        }
        if (toSell > medicine->total_quantity) {
            printf("\nQuantity of medication in stock : %d which is greater than required : %d ... exiting\n", medicine->total_quantity, toSell);
            return;
        }
        Batch *dummy = medicine->next_batch;
        int32_t acquired = 0;
        do {
            acquired += (dummy->next)->batch_quantity;
            dummy = dummy->next;
        } while (toSell > acquired);

        int freed_batches = 0;
        Batch *to_free;
        Batch *temp = (medicine->next_batch)->next;

        if (acquired == toSell) {
            dummy = dummy->next;
        }
        else {
            dummy->batch_quantity = acquired - toSell;
        }

        (medicine->next_batch)->next = dummy;
        if (dummy != NULL) {
            delete_from_trie(Batch_ID2PrevPtr, dummy->batch_number);
            insert_in_trie(Batch_ID2PrevPtr, dummy->batch_number, (void *)(medicine->next_batch));
        }
        while (temp != dummy) {
            delete_from_trie(Batch_ID2PrevPtr, temp->batch_number);
            to_free = temp;
            temp = temp->next;
            free(to_free);
            ++freed_batches;
        }

        printf("Medication: %s : %s\n", medicine->medication_name, medicine->medication_id);
        printf("\tQuantity in stock     : %d\n", medicine->total_quantity);
        printf("\tUnits sold just now   : %d\n", toSell);
        medicine->total_quantity -= toSell;
        printf("\tQuantity left in stock: %d\n", medicine->total_quantity);
        medicine->total_sales += (real)toSell * (medicine->price_per_unit);
        inventory->total_batches -= freed_batches;
    }
    else {
        printf("Medicine does not exist\n");
    }
}

void Add_Medication_Wrapper(Inventory *inventory)
{
    int64_t index;
    char m_id[16];
    char m_name[64];
    int32_t quantity_in_stock = 0;
    real price_per_unit;
    int32_t reorder_level;
    int32_t total_sales;

    input_Medication_ID(m_id);
    index = (int64_t) search_in_trie(Medication_ID2Index, m_id);
    if (index) {
        printf("Medication exists already ... quiting\n");
        return;
    }
    input_Medication_Name(m_name);
    index = (int64_t) search_in_trie(Medication_Name2Index, m_name);
    if (index) {
        printf("Medication exists already ... quiting\n");
        return;
    }
    /*input_Total_Quantity(&quantity_in_stock);*/
    input_Price_Per_Unit(&price_per_unit);
    input_Reorder_Level(&reorder_level);
    input_Total_Sales(&total_sales);

    Add_Medication(inventory,
                        m_id,
                        m_name,
                        quantity_in_stock,
                        price_per_unit,
                        reorder_level,
                        total_sales
                        );
    printf("Medication Added Successfully\n");
}

void save_medications_to_file(Medication **medications, int size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < size; i++) {
        if (medications[i] != NULL)
        {
            fprintf(file, "%s %s %d %.2f %d %d\n",
                    medications[i]->medication_id,
                    medications[i]->medication_name,
                    medications[i]->total_quantity,
                    medications[i]->price_per_unit,
                    medications[i]->reorder_level,
                    medications[i]->total_sales);
        }
    }

    fclose(file);
}

int Save_Batches_To_File(Inventory *inventory) {
    FILE *file = fopen("batch.txt", "w");
    if (!file) {
        perror("Error opening file for writing");
        return 0;
    }

    for (uint64_t i = 0; i < inventory->index; i++) {
        Medication *med = inventory->shelf[i];
        if (med) {
            Batch *batch = med->next_batch->next;  
            while (batch) {
                fprintf(file, "%s %d %d %s %.2f %s\n",
                        med->medication_id,
                        batch->batch_quantity,
                        batch->expiration_date,
                        batch->batch_number,
                        batch->purchase_price,
                        batch->supplier_id);
                batch = batch->next;
            }
        }
    }

    fclose(file);
    return 1;
}

int Save_Supplier_List_To_File(SupplierList *supplierList) {
    FILE *file = fopen("suppliers.txt", "w");
    if (!file) {
        perror("Error opening file for writing");
        return 0;
    }

    Supplier *current = supplierList->next;
    while (current) {
        fprintf(file, "%s %s %010lu %.2f\n",
                current->supp_name,
                current->supp_id,
                current->contact,
                current->turnover);

        current = current->next;
    }

    fclose(file);
    return 1;
}

void search_medication_by_id(Inventory *inventory, char *id)
{
    uint64_t index = (uint64_t)search_in_trie(Medication_ID2Index, id);
    if(index == 0 || inventory->shelf[index] == NULL)
    {
        printf("Medication ID does not exist.\n");
    }
    else {
        print_medication(inventory->shelf[index]);
    }
}

void search_medication_by_name(Inventory *inventory, char *m_name)
{
    uint64_t index = (uint64_t) search_in_trie(Medication_Name2Index, m_name);
    if(index == 0 || inventory->shelf[index] == NULL)
    {
        printf("No such Medication exists.\n");
    }
    else {
        print_medication(inventory->shelf[index]);
    }
}


void search_medication_by_id_wrapper(Inventory *inventory)
{
    char m_id[24];
    input_Medication_ID(m_id);
    search_medication_by_id(inventory, m_id);
}

void search_medication_by_name_wrapper(Inventory *inventory)
{
    char m_name[72];
    input_Medication_Name(m_name);
    search_medication_by_name(inventory, m_name);
}

void search_medication_by_supplier_id(Inventory *inventory)
{
    char s_id[64];
    inputSupplierID(s_id);
    void *found = (void *)search_in_trie(Supplier_ID2PrevPtr, s_id);
    if (found) {
        Medication **shelf = inventory->shelf;
        Medication *medicine;
        Supplier *supplier = ((Supplier *)found)->next;
        ListNode *head = supplier->idx_list;
        while (head) {
            medicine = shelf[head->val];
            printf("Name: %s\nID: %s", medicine->medication_name, medicine->medication_id);
            head = head->next;
        }
    }
    else {
        printf("Supplier ID does not exist\n");
    }
}
