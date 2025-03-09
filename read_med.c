#include "./include.h"
#include <ctype.h>

/*
int Get_Medications_From_File(Inventory *inventory)
{
    FILE *file = fopen("medication_data.txt", "r");

    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    char medication_id[16];
    char medication_name[64];
    int total_quantity;
    float price_per_unit;
    int reorder_level;
    int total_sales;

    while (fscanf(file, "%15s %63s %d %f %d %d", medication_id, medication_name, 
                  &total_quantity, &price_per_unit, &reorder_level, &total_sales) == 6) {

        Add_Medication(inventory,
                       medication_id,
                       medication_name,
                       0,
                       price_per_unit,
                       reorder_level,
                       total_sales
                       ); 
    }

    fclose(file);
    return 0;
    
}


int Get_Medications_From_File(Inventory *inventory) {
    FILE *file = fopen("medication_data.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char line[256];  // Buffer to read a line
    char medication_id[16];
    char medication_name[64];
    int total_quantity;
    float price_per_unit;
    int reorder_level;
    int total_sales;

    while (fgets(line, sizeof(line), file)) {  // Read a full line safely
        if (sscanf(line, "%15s %63[^\n] %d %f %d %d", medication_id, medication_name, 
                   &total_quantity, &price_per_unit, &reorder_level, &total_sales) == 6) {

            Add_Medication(inventory,
                           medication_id,
                           medication_name,
                           total_quantity,  // Now correctly passed
                           price_per_unit,
                           reorder_level,
                           total_sales);
        } else {
            printf("Error parsing line: %s\n", line);
        }
    }

    fclose(file);
    return 0;
}
*/

#define MAX_LINE_LENGTH 256

int Get_Medications_From_File(Inventory *inventory) {
    FILE *file = fopen("medication_data.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char medication_id[16];
        char medication_name[64];
        int total_quantity = 0;
        float price_per_unit = 0.0;
        int reorder_level = 0;
        int total_sales = 0;

        // Tokenizing the line
        char *token = strtok(line, " ");  // Medication ID
        if (!token) continue;
        strncpy(medication_id, token, sizeof(medication_id) - 1);
        medication_id[sizeof(medication_id) - 1] = '\0'; 
        
        token = strtok(NULL, " ");  // First word of medication name
        if (!token) continue;
        strcpy(medication_name, token);

        // Continue adding parts of medication name until we reach numbers
        while ((token = strtok(NULL, " ")) && !isdigit(token[0])) {
            strcat(medication_name, " ");
            strcat(medication_name, token);
        }

        // Now parse numerical values
        if (token) total_quantity = atoi(token);
        token = strtok(NULL, " ");
        if (token) price_per_unit = atof(token);
        token = strtok(NULL, " ");
        if (token) reorder_level = atoi(token);
        token = strtok(NULL, " ");
        if (token) total_sales = atoi(token);
        total_quantity = 0;

        Add_Medication(inventory,
                       medication_id,
                       medication_name,
                       total_quantity,
                       price_per_unit,
                       reorder_level,
                       total_sales);
    }

    fclose(file);
    return 0;
}
