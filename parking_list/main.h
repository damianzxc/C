//
// Created by damian on 02.02.2020.
//

#ifndef PARKING_LIST_MAIN_H
#define PARKING_LIST_MAIN_H
typedef struct List List;

void deinit(List *pList);

List * init();

int getNumberOfCars(struct List *pList);

void addCar(struct List *pList, char * string, int price);

void displayEntry(char * parkingName, List *pList);

void displayList(List *pList);

void removeCar(List *pList, char *plates);

void displayExit(char *parkingName, List *pElement);

int getParkingTime(const List *pElement);

void openLog();

void saveLog(char *string);

void closeLog();

char *concat(const char *string,const char *name);

#endif //PARKING_LIST_MAIN_H
