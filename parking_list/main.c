#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "main.h"

typedef struct List {
    char * plates;
    time_t entry;
    int price;
    struct List * pNext;
} List;

static FILE *log = NULL;


//Program zamist godzin używa sekund (żeby dało się pokazać funkcjonalność)
int main() {
    openLog();
    List * level0 = init("Poziom 0");
    List * level1 = init("Poziom 1");

    addCar(level0 ,"Fiat 500", 5);
    addCar(level1, "Mazda 323", 4);
    sleep(2);
    addCar(level0, "BMW E46", 5);
    addCar(level1, "Mustang", 5);
    sleep(2);
    addCar(level1, "Ford", 5);
    displayList(level0);
    displayList(level1);
    removeCar(level1, "Mazda 323");
    displayList(level0);
    deinit(level0);
    deinit(level1);
    closeLog();
    return 0;
}

void closeLog() {
    if (0 != fclose(log)) perror("fclose file failed");
}

void saveLog(char *msg) {
    //Write the log header
    time_t now = time(NULL);
    size_t headerSize = snprintf(NULL, 0, asctime(localtime(&now)));

    char *pLogHeader = malloc(headerSize);
    headerSize = snprintf(pLogHeader, headerSize, asctime(localtime(&now)));
    size_t s = fwrite(pLogHeader, sizeof(char), headerSize - 1u, log);
    if (s != headerSize - 1u) perror("fwrite failed");
    s = fwrite(" ", sizeof(char), 1u, log);
    if (s != 1u) perror("fwrite failed");
    free(pLogHeader);

    //Write user pMsg
    s = fwrite(msg, sizeof(char), strlen(msg), log);
    s += fwrite("\n", sizeof(char), 1u, log);
    if (s != strlen(msg) + 1u) perror("fwrite failed");
    if (0 != fflush(log)) perror("fflush failed");
}

void openLog() {
    log = fopen("log.txt", "a+");
    if(NULL == log) perror("fopen filed to open log");
}



void removeCar(List *pList, char *plates) {
    char * parkingName = pList->plates;
    bool found = false;
    List *pBefore = NULL;
    List *pRemove = pList;
    List *pAfter = pList->pNext;
    while(NULL != pRemove && !found) {
        if(0 != strstr(pRemove->plates, plates)) {
            found = true;
        }
        else {
            if(NULL != pRemove->pNext) {
                pBefore = pRemove;
                pAfter = pRemove->pNext->pNext;
            }
            else {
                pAfter = NULL;
            }
            pRemove = pRemove->pNext;
        }
    }
    if(found) {
        if(NULL == pAfter) {
            pBefore->pNext = NULL;
        }
        else {
            pBefore->pNext = pAfter;
        }
        char * msg = concat("Wyjazd z parkingu: ", pRemove->plates);
        saveLog(msg);
        free(msg);
        displayExit(parkingName, pRemove);
        free(pRemove->plates);
        free(pRemove);
    }
}

void displayList(List *pList) {
    printf("\nWszystkich samochodów na parkingu %s jest: %d\n",
            pList->plates, getNumberOfCars(pList));
    saveLog("Wyświetlanie listy samochodów");
    while(pList->pNext != NULL) {
        pList = pList->pNext;
        char * entryTime = asctime(localtime(&(pList->entry)));
        strtok(entryTime, "\n");
        int parkingTime = getParkingTime(pList);
        printf("%s, data wjazdu: %s, czas: %d godziny\n",
                pList->plates, entryTime, parkingTime);
    }
}

int getParkingTime(const List *pElement) {
    int parkingTime = time(0) - pElement->entry;
    return parkingTime;
}

void addCar(List *pList, char * plates, int price) {
    char * parking = pList->plates;
    while(pList->pNext != NULL) {
        pList = pList->pNext;
    }
    List *pNew = malloc(sizeof(*pNew));
    pNew->plates = malloc(strlen(plates) + 1U);
    strcpy(pNew->plates, plates);
    time(&(pNew->entry));
    pNew->price = price;
    pList->pNext = pNew;
    pNew->pNext = NULL;
    char * msg = concat("Wjazd samochodu: ", pNew->plates);
    saveLog(msg);
    free(msg);
    displayEntry(parking, pNew);
}

void displayEntry(char *parkingName, List *pElement) {
    printf("Wjazd auta na parking: %s -> %s, cena: %d zł za godzinę\n",
            parkingName,pElement->plates, pElement->price);
}

void displayExit(char * parkingName, List *pElement) {
    printf("\nWyjazd auta z parkingu: %s\n", parkingName);
    int parkingTime = getParkingTime(pElement);
    printf("Czas postoju: %d godziny, kwota do zapłaty: %d zł.\n", parkingTime, parkingTime * pElement->price);
}

int getNumberOfCars(List *pList) {
    int size = 0;
    while(pList->pNext != NULL) {
        pList = pList->pNext;
        size++;
    }
    return size;
}

struct List *init(char * name) {
    struct List * root = malloc(sizeof(*root));
    root->plates = malloc(strlen(name) + 1U);
    strcpy(root->plates, name);
    time(&(root->entry));
    root->price = 0;
    root->pNext = NULL;
    char * msg = concat("Utworzono parking: ", name);
    saveLog(msg);
    free(msg);
    printf("Utworzono parking: %s\n\n", name);
    return root;
}

char *concat(const char *s1, const char *s2) {
    char * result = malloc(strlen(s1) + strlen(s2) +1U);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void deinit(List *pList) {
    for(int i = 0; i <= getNumberOfCars(pList); i++) {
        List *pLast = NULL;
        List *pOneBefore = pList;
        while(NULL != pOneBefore->pNext->pNext) {
            pOneBefore = pOneBefore->pNext;
        }
        pLast = pOneBefore->pNext;
        free(pLast->plates);
        free(pLast);
        pOneBefore->pNext = NULL;
    }
    char * msg = concat("Kasowanie parkingu: ", pList->plates);
    saveLog(msg);
    free(msg);
    printf("\nKasowanie parkingu: %s", pList->plates);
    free(pList->plates);
    free(pList);
}

