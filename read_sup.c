#include "./include.h"

uint64_t contact_strtoi(char *s)
{
    uint64_t number = 0;
    for (int i = 0; i < 10; ++i) {
        number *= 10;
        number += s[i] - '0';
    }
    return number;
}

int Get_Supplier_List_From_File(SupplierList *supplierList)
{
    FILE *file = fopen("supplier_data.txt", "r");

    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }

    char supp_name[64];
    char supp_id[16];
    char contact_str[11];
    uint64_t contact;
    float turnover;

    while (fscanf(file, "%63s %15s %10s %f", supp_name, supp_id, contact_str, &turnover) == 4) {
        contact = contact_strtoi(contact_str);
        Add_Supplier(supplierList,
                     supp_name,
                     supp_id,
                     turnover,
                     contact);
    }

    fclose(file);
    return 1; 
}

