#include "./include.h"
#include "./trie.h"

int Get_Batches_From_File(Inventory *inventory) {
    FILE *file = fopen("batch_data.txt", "r");

    if (!file) {
        printf("Error opening file.\n");
        return 0;
    }

    char medication_id[16];
    int batch_quantity;
    int expiration_date;
    char batch_number[16];
    char supplier_id[16];
    float purchase_price;

    while (fscanf(file,
                  "%15s %d %d %15s %f %15s",
                  medication_id,
                  &batch_quantity, 
                  &expiration_date,
                  batch_number,
                  &purchase_price,
                  supplier_id) == 6) {
        uint64_t med_index = (uint64_t)search_in_trie(Medication_ID2Index, medication_id);
        Add_Batch_To_Medication(inventory,
                                med_index,
                                batch_quantity,
                                expiration_date,
                                batch_number,
                                purchase_price,
                                supplier_id);
    }

    fclose(file);
    return 1;
}

