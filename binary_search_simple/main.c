#include <stdio.h>
#include <stdbool.h>

int binary_search(int tab[], int x, int n){
    int beginning = 1;
    int end = n;
    int middle;
    bool found = false;

    do {
        middle = (beginning + end) / 2;
        if(x == tab[middle]){
            found = true;
        }
        if(x < tab[middle]) {
            end = middle - 1;
        }
        if(x > tab[middle]) {
            beginning = middle + 1;
        }
    }
    while(!found && beginning <= end);

    return tab[middle];

};

int main() {
    const int key = 22;
    const int tab[] = {1,4,5,7,9,10,15,18,20,22,24,25,27,29,30,32,34,35};
    const int tab_size = 18;
    int result;
    result = binary_search(tab, key, tab_size);
    printf("Szukana liczba to: %d", result);
    return 0;
}
