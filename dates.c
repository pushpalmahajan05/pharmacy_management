#include "./include.h"
#include <time.h>
#include <stdio.h>

int days_since_2000(int date) {
    struct tm given_date = {0};
    struct tm base_date = {0};

    base_date.tm_mday = 1;
    base_date.tm_mon = 0;
    base_date.tm_year = 2000 - 1900;

    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;

    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31) {
        printf("Invalid date input: %d. Ensure it follows YYYYMMDD format and is valid.\n", date);
        return -1;
    }

    given_date.tm_year = year - 1900;
    given_date.tm_mon = month - 1;
    given_date.tm_mday = day;

    time_t t1 = mktime(&base_date);
    time_t t2 = mktime(&given_date);

    if (t1 == -1 || t2 == -1) {
        printf("Error computing time.\n");
        return -1;
    }

    return (int) difftime(t2, t1) / (60 * 60 * 24);
}
