#include <stdio.h>
#include <stdint.h>
#include <stab.h>

void bubbleSort(int *tab, uint8_t n);

void printTab(int *tab, uint8_t n);

void selectionSort(int *tab, int n);

void insertionSort(int *tab, int n);

int main() {

    int tab[] = {1,4,5,7,25,76,11,15,87,31};
    int n = sizeof(tab) / sizeof(tab[0]);
    printf("długość tablicy: %d\n", n);
    //bubbleSort(tab, n);
    //selectionSort(tab, n);
    insertionSort(tab, n);
    printTab(tab, n);
    return 0;
}

void insertionSort(int *tab, int n) {
    int pom;
    uint8_t j;
    for(uint8_t i=1; i<n; i++) {
        pom = tab[i];
        for(j=i-1; (j>=0) && (tab[j]>pom); j--) {
            tab[j+1] = tab[j];
        }
        tab[j+1] = pom;
    }
}

void selectionSort(int *tab, int n) {
    for(uint8_t i=n-1; i>0; i--) {
        printTab(tab, n);
        uint8_t max = 0;
        for(uint8_t j=1; j<=i; j++) {
            if (tab[max] < tab[j]) {
                max = j;
            }
            uint8_t swap = tab[i];
            tab[i] = tab[max];
            tab[max] = swap;
        }
    }
}

void printTab(int *tab, uint8_t n) {
    for(uint8_t i=0; i<n; i++) {
        printf("%d, ", tab[i]);
    }
    printf("\n");
}

void bubbleSort(int *tab, uint8_t n) {
    int pom;
    for(uint8_t i=n-1; i>0; i--) {
        for(uint8_t j=0; j<i; j++) {
            if(tab[j] > tab[j+1]) {
                pom = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = pom;
            }
        }
    }
}
