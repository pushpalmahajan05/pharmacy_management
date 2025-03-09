#include "include.h"

void input_Medication_ID(char* medication_id) {
    int i = 0, ch;
    int size = 17;
    printf("Enter Medication ID: ");
    while (i < size - 1 && (ch = fgetc(stdin)) != '\n' && ch != EOF) {
        medication_id[i++] = ch;
    }
    medication_id[i] = '\0';
    while (ch != '\n' && ch != EOF) {
        ch = fgetc(stdin);
    }
}

void input_Medication_Name(char* medication_name) {
    int i = 0, ch;
    int size = 72;
    printf("Enter Medication Name: ");
    while (i < size - 1 && (ch = fgetc(stdin)) != '\n' && ch != EOF) {
        medication_name[i++] = ch;
    }
    medication_name[i] = '\0';

    while (ch != '\n' && ch != EOF) {
        ch = fgetc(stdin);
    }
}

void input_Total_Quantity(int* total_quantity) {
    printf("Enter Total Quantity: ");
    scanf("%d", total_quantity);
    while (getchar() != '\n'); 
}

void input_Price_Per_Unit(float* price_per_unit) {
    printf("Enter Price Per Unit: ");
    scanf("%f", price_per_unit);
    while (getchar() != '\n'); 
}

void input_Reorder_Level(int* reorder_level) {
    printf("Enter Reorder Level: ");
    scanf("%d", reorder_level);
    while (getchar() != '\n');
}

void input_Total_Sales(int* total_sales) {
    printf("Enter Total Sales: ");
    scanf("%d", total_sales);
    while (getchar() != '\n'); 
}

void inputSupplierName(char *supplier_name) {
    printf("Enter Supplier Name: ");
    fgets(supplier_name, 64, stdin);
    supplier_name[strcspn(supplier_name, "\n")] = '\0';
}

void inputSupplierID(char *supplier_id) {
    printf("Enter Supplier ID: ");
    fgets(supplier_id, 16, stdin);
    supplier_id[strcspn(supplier_id, "\n")] = '\0';
}

void inputSupplierContact(uint64_t *contact) {
    printf("Enter Contact Number: ");
    scanf("%lu", contact);
}

void inputSupplierTurnover(float *turnover) {
    printf("Enter Turnover: ");
    scanf("%f", turnover);
    while (getchar() != '\n');
}

void inputSupplierUniqueMedCount(int *num_unique_med) {
    printf("Enter Number of Unique Medications: ");
    scanf("%d", num_unique_med);
    while (getchar() != '\n');
}

void print_medication(Medication *medicine)
{
    printf("Name : %s\nID : %s\n Quantity : %d\n Price Per Unit : %f \n Total Sales : %d\n", medicine->medication_name, medicine->medication_id, medicine->total_quantity, medicine->price_per_unit, medicine->total_sales);
}

void print_supplier(Supplier *supplier)
{
    printf("Name : %s\nID : %s\n Contact : %ld\n Unique Medications : %d \n", supplier->supp_name, supplier->supp_id, supplier->contact, supplier->num_unique_med);
}

void print_inventory(Inventory *inventory)
{
    Medication **shelf = inventory->shelf;
    int size = inventory->index;
    for (int64_t i = 0; i < size; ++i) {
        if (shelf[i]) {
            print_medication(shelf[i]);
        }
    }
}

/* wrappers */
