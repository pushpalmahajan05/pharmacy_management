#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "./trie.h"

#ifndef INCLUDE_H
#define INCLUDE_H

typedef float real;

typedef struct Batch {
    struct Batch* next;
    int index;
    int batch_quantity;
    int expiration_date;
    char batch_number[16];
    char supplier_id[16];
    real purchase_price;
} Batch;

typedef struct BatchRef {
    struct BatchRef *next;
    struct Batch *ref;
    int64_t exp_date; 
} BatchRef;

typedef struct Holder
{
    struct Holder *next;
    struct BatchRef *Batchlist;
} Holder;

typedef struct PtrNode {
    void *ref;
    struct PtrNode *next;
} PtrNode;

typedef struct Medication {
    char medication_id[16];
    char medication_name[64];
    int total_quantity;
    real price_per_unit;
    int reorder_level;
    int total_sales;
    Batch *tail;
    Batch* next_batch;
    PtrNode *supplier_list;
} Medication;

typedef struct ListNode {
    struct ListNode *next;
    uint64_t val;
} ListNode;

typedef struct Inventory {
    Medication **shelf;
    uint32_t total_batches;
    uint32_t total_medications;
    uint64_t capacity;
    uint64_t index;
    ListNode *free_list;
} Inventory;

typedef struct ID_Node {
    struct ID_Node *next;
    char med_id[16];
} ID_Node;

typedef struct Supplier {
    struct Supplier *next;
    char supp_name[64];
    char supp_id[16];
    uint64_t contact;
    real turnover;
    int num_unique_med;
    ListNode *idx_list;
} Supplier;

typedef struct SupplierList {
    Supplier *next;
} SupplierList;

extern Supplier *supplier_list;

/* functions */
Inventory *create_inventory(uint32_t size);

// rough
void print_inventory(Inventory *inventory);
void print_medication(Medication *medicine);
void print_supplier(Supplier *supplier);
void add_new_batch(Inventory *inventory);
void Update_Medication(Inventory *inventory);
int days_since_2000(int date);

// io core
void input_Medication_ID(char* medication_id);
void input_Medication_Name(char* medication_name);
void input_Total_Quantity(int* total_quantity);
void input_Price_Per_Unit(float* price_per_unit);
void input_Reorder_Level(int* reorder_level);
void input_Total_Sales(int* total_sales);

void inputSupplierID(char *supplier_id);
void inputSupplierContact(uint64_t *contact);
void inputSupplierTurnover(float *turnover);
void inputSupplierUniqueMedCount(int *num_unique_med);
void inputSupplierName(char *supplier_name);

// io wrappers
void search_medication_by_name_wrapper(Inventory *inventory);
void Add_Medication_Wrapper(Inventory *inventory);

void search_medication_by_id_wrapper(Inventory *inventory);

void delete_list_batch(Inventory *inventory, Batch *head);
void delete_medication_id(Inventory *inventory);
void free_list(ListNode *head);
void free_inventory(Inventory *inventory);


int Get_Supplier_List_From_File(SupplierList *List);
int Get_Medications_From_File(Inventory *inventory);
int Get_Batches_From_File(Inventory *inventory);

void Add_Supplier(SupplierList *supplierList,
                 char *new_name,
                 char *new_id,
                 real new_turnover,
                 uint64_t new_contact);

void Add_Medication(Inventory *inventory,
                    char *m_id,
                    char *m_name,
                    int32_t quantity_in_stock,
                    real price_per_unit,
                    uint32_t reorder_level,
                    int32_t total_sales
                    );

void Add_Batch_To_Medication(Inventory *inventory,
                             int med_index,
                             int batch_quantity,
                             int expiration_date,
                             char *batch_number,
                             real price,
                             char *supplier_id);

bool Is_Supplier_Present_In_List(PtrNode *head, Supplier *s);
bool Is_Medication_Present_In_List(ListNode *head, uint64_t index);
void Insert_Supplier_Ref_In_List(Medication *medicine, Supplier *s);
void Insert_Medication_Ref_In_List(Supplier *s, int index);
void Delete_Medication_Ref_In_List(Supplier *sup, uint64_t index);
void Delete_Supplier_Ref_In_List(Medication *medicine, Supplier *supplier);
void Associate_Supplier_With_Medication(Medication *med, Supplier *sup, int index);
void Dissociate_Supplier_Medication(Inventory *inventory, Supplier *s);
void Dissociate_Medication_Supplier(int index, Medication *medicine);
void Find_Medication_By_Supplier(Inventory *inventory);
void Find_Supplier_By_Medicaiton(Inventory *inventory);
void free_ptr_list(PtrNode *head);
void add_new_batch(Inventory *inventory);
void delete_batch(Inventory *inventory);
void Delete_Supplier(Inventory *inventory);
void Update_Supplier();

void Print_Top_Ten_All_Rounder_Suppliers(SupplierList *list);
void Print_Top_Ten_Turnover_Suppliers(SupplierList *list);
void Search_Supplier_By_ID();
void check_expiration(Inventory *inventory);
void stock_alerts(Inventory *inventory);
void *Make_temp(Inventory *inventory);
BatchRef *merge_batch_ref(BatchRef *l1, BatchRef *l2);
void sort_by_expiry(Inventory *inventory);
void sell_medication(Inventory *inventory);
void search_medication_by_id_wrapper(Inventory *inventory);
void search_medication_by_name_wrapper(Inventory *inventory);
void search_medication_by_id(Inventory *inventory, char *id);
void search_medication_by_name(Inventory *inventory, char *m_name);

void save_medications_to_file(Medication **medications, int size, const char *filename);
int Save_Batches_To_File(Inventory *inventory);
int Save_Supplier_List_To_File(SupplierList *supplierList);
void search_medication_by_supplier_id(Inventory *inventory);

#endif
