#include "./include.h"

TrieNode *Medication_ID2Index;
TrieNode *Medication_Name2Index;
TrieNode *Batch_ID2PrevPtr;
TrieNode *Supplier_ID2PrevPtr;

int main()
{
    Medication_ID2Index = create_trie_node();
    Batch_ID2PrevPtr = create_trie_node();
    Medication_Name2Index = create_trie_node();
    Supplier_ID2PrevPtr = create_trie_node();
    Inventory *newInventory = create_inventory(3);
    SupplierList List;
    List.next = NULL;
    (void) newInventory;
    Get_Supplier_List_From_File(&List);
    Get_Medications_From_File(newInventory);
    Get_Batches_From_File(newInventory);
    int option;
    bool quit = false;
    while (!quit) {
        printf("\nEnter the option.\n");
        printf("1. Add Medication\n");
        printf("2. Update Medication\n");
        printf("3. Delete Medication by ID\n");
        printf("4. Delete Batch by ID\n");
        printf("5. Search Medication by ID\n");
        printf("6. Search Medication by Name\n");
        printf("7. Search Medication by supplier ID\n");
        printf("8. Stock Alerts\n");
        printf("9. Check Expiration date\n");
        printf("10. Sort by Expiration Date\n");
        printf("11. Add Suppliers\n");
        printf("12. Update Suppliers\n");
        printf("13. Delete Suppliers\n");
        printf("14. Search Suppliers\n");
        printf("15. Print top 10 All rounder suppliers\n");
        printf("16. Print top 10 Suppliers with largest turnover\n");
        printf("17. Add Batch\n");
        printf("18. Sell Medication\n");
        printf("19. Exit\n");
        scanf("%d", &option);
        getchar();
        switch (option) {
            case 1:
                Add_Medication_Wrapper(newInventory);
                break;
            case 2:
                Update_Medication(newInventory);
                break;
            case 3:
                delete_medication_id(newInventory);
                break;
            case 4:
                delete_batch(newInventory);
                break;
            case 5:
                search_medication_by_id_wrapper(newInventory);
                break;
            case 6:
                search_medication_by_name_wrapper(newInventory);
                break;
            case 7:
                search_medication_by_supplier_id(newInventory);
                break;
            case 8:
                stock_alerts(newInventory);
                break;
            case 9:
                check_expiration(newInventory);
                break;
            case 10:
                sort_by_expiry(newInventory);
                break;
            case 11:
                char supplier_id[16];
                uint64_t contact;
                real turnover;
                char supplier_name[64];
                inputSupplierID(supplier_id);
                inputSupplierContact(&contact);
                inputSupplierTurnover(&turnover);
                inputSupplierName(supplier_name);
                Add_Supplier(&List,
                             supplier_name,
                             supplier_id,
                             turnover,
                             contact);
                break;
            case 12:
                Update_Supplier();
                break;
            case 13:
                Delete_Supplier(newInventory);
                break;
            case 14:
                Search_Supplier_By_ID();
                break;
            case 15:
                Print_Top_Ten_All_Rounder_Suppliers(&List);
                break;
            case 16:
                Print_Top_Ten_Turnover_Suppliers(&List);
                break;
            case 17:
                add_new_batch(newInventory);
                break;
            case 18:
                sell_medication(newInventory);
                break;
            case 19:
                quit = 1;
                break;
            default:
                printf("Enter a valid option\n");
        }
    }
    save_medications_to_file(newInventory->shelf, newInventory->index, "medications.txt");
    Save_Supplier_List_To_File(&List);
    Save_Batches_To_File(newInventory);
    free_inventory(newInventory);
    free_trie(Medication_ID2Index);
    free_trie(Medication_Name2Index);
    free_trie(Batch_ID2PrevPtr);
    return EXIT_SUCCESS;
}
