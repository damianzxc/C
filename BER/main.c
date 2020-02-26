#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <time.h>
#include <string.h>
#include "main.h"

struct berResult
{
    uint32_t tot; //total number of bits
    uint32_t err; //number of bits in error
    float ber;  //ber calc result
    clock_t t1; //calc start clock
    clock_t t2; //calc stop clock
} BER;

static FILE *pLogFile = NULL;

/*
	Running without arguments creates and modifies for calculating
	To find errors in two same length files, run in terminal: 
	./BER file1.bin file2.bin 
*/

int main(int argc, char *argv[]) {

    openLog();
    saveLog("BER v1.0 Start");
    if (3 != argc) {
        puts("Preparing files...");
        prepareFiles();
        puts("Type BER file1 file2 to start");
    }
    else {
        puts("Processing files...");
        processFiles(argv);
    }
    return 0;
}

void processFiles(char *const *argv) {
    saveLog("Loading files...");
    const uint32_t size = getSize(argv[1]);
    const uint32_t size2 = getSize(argv[2]);
    FILE *file1 = getFile(argv[1]);
    FILE *file2 = getFile(argv[2]);
    if(size > 0 && size == size2 && file1 != NULL && file2 != NULL){
        saveLog("Calculating BER...");
        calculateBer(file1, file2, size);
        displayResults();
    }
    else{
        saveLog("File loading failed");
        puts("Files loading problem");
    }
    closeLog();
    fclose(file1);
    fclose(file2);
}

void prepareFiles() {
    saveLog("Empty args. Creating files");
    // file1
    createFile("file1_A.bin", 100u, 0x55);
    createFile("file1_B.bin", 100u, 0x55);
    //file2
    createFile("file2_A.bin", 100u, 0x55);
    createFile("file2_B.bin", 100u, 0x55);
    modifyFile("file2_B.bin", 0x54);
    //file3
    createFile("file3_A.bin", 400000000u, 0x55);
    createFile("file3_B.bin", 400000000u, 0x50);
}

void modifyFile(char *name, char value) {
    FILE *file = fopen(name, "r+");
    if(NULL == file) perror("fopen file failed");
    for (int i = 0u; i< 10; i++) {
        fwrite(&value, sizeof(char), 1u, file);
    }
    if (0 != fclose(file)) perror("fclose file failed");
    saveLog("File modified");

}

void displayResults() {
    double timeUsed = (double) (BER.t2 - BER.t1) / CLOCKS_PER_SEC;
    printf("Total : %u\n", BER.tot);
    printf("Error : %u\n", BER.err);
    printf("BER   : %.2lf\n", BER.ber);
    printf("Time  : %.2lf s\n", timeUsed);
}

FILE *getFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if(NULL == file) perror("fopen file failed");
    saveLog("File ready");
    return file;
}


void createFile(const char *pName, const int32_t count, const char value) {
    FILE *pFile = NULL;
    uint32_t i = 0;
    size_t s = 0;

    pFile = fopen(pName, "w+");
    if (NULL == pFile) perror("fopen file failed");
    saveLog("Save to file started");
    for (i = 0u; i < count; i++) {
        s = fwrite(&value, sizeof(char), 1u, pFile);
        if (s != 1) perror("fwrite failed");
    }
    if (0 != fclose(pFile)) perror("fclose file failed");
    saveLog("File ready");
}

void openLog(void) {
    pLogFile = fopen("log.txt", "a+");
    if (NULL == pLogFile) perror("fopen file failed");
}

void saveLog(char *pMsg) {
    //Write the log header
    time_t now = time(NULL);
    size_t headerSize = snprintf(NULL, 0, asctime(localtime(&now)));

    char *pLogHeader = malloc(headerSize);
    headerSize = snprintf(pLogHeader, headerSize, asctime(localtime(&now)));
    size_t s = fwrite(pLogHeader, sizeof(char), headerSize - 1u, pLogFile);
    if (s != headerSize - 1u) perror("fwrite failed");
    s = fwrite(" ", sizeof(char), 1u, pLogFile);
    if (s != 1u) perror("fwrite failed");
    free(pLogHeader);

    //Write user pMsg
    s = fwrite(pMsg, sizeof(char), strlen(pMsg), pLogFile);
    s += fwrite("\n", sizeof(char), 1u, pLogFile);
    if (s != strlen(pMsg) + 1u) perror("fwrite failed");
    if (0 != fflush(pLogFile)) perror("fflush failed");
}

void closeLog(void) {
    if (0 != fclose(pLogFile)) perror("fclose file failed");
}

void calculateBer(FILE *file1, FILE *file2, uint32_t fileSize) {
    u_int8_t byteA;
    u_int8_t byteB;
    BER.t1 = clock();
    for(int i = 0U; i < fileSize; i++){
        fread(&byteA, sizeof(byteA), 1, file1);
        fread(&byteB, sizeof(byteB), 1, file2);
        BER.tot += 8U;
        BER.err += hammingDistance(byteA, byteB);
    }
    BER.t2 = clock();
    BER.ber = (float) BER.err / BER.tot;
    saveLog("BER calculating success");
}

uint8_t hammingDistance(uint8_t byteA, uint8_t byteB) {
    uint8_t x = byteA ^ byteB;
    uint8_t setBits = 0;
    while(x > 0){
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}

uint32_t getSize(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) perror("fopen file failed");
    fseek(file, 0L, SEEK_END);
    uint32_t size = ftell(file);
    fclose(file);
    saveLog("File size ready");
    return size;
}
