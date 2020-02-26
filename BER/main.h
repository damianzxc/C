//
// Created by damian on 30.01.2020.
//

#ifndef BER_MAIN_H
#define BER_MAIN_H

void    openLog    (void);
void    saveLog    (char* pMsg);
void    closeLog   (void);

void createFile(const char *pName, const int32_t count, const char value);

FILE *getFile(char *filename);

void calculateBer(FILE *pFile, FILE *pFile1, uint32_t fileSize);

uint32_t getSize(char *fileName);

uint8_t hammingDistance(uint8_t byteA, uint8_t byteB);

void displayResults();

void modifyFile(char *name, char value);

void prepareFiles();

void processFiles(char *const *argv);

#endif //BER_MAIN_H
