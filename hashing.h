#ifndef HASH_MEMORY
#define HASH_MEMORY

#include <iostream>
#include <string>
#include <vector>


class Process {
  public:
    unsigned int PID;
    int page_address;
    bool wasOccupied;
    Process(unsigned int);
};

//parent Hashing class
class HashTable {
  protected:
    int N;    //memory capacity
    int P;    //size of a page
    int m;    //size of the hashtable
    int *memory;
    bool *indexOccupied;
    
  public:
    HashTable(int n, int p);
    int primaryHash(unsigned int PID);
    int firstFreePage();
    void freePage(Process process);
    ~HashTable();
};

//first subclass of HashTable
class ChainingTable : public HashTable {
  private:
    std::vector<Process> *table;   //a pointer to vector
    int size;

  public:
    ChainingTable(int n, int p);
    void insertProcess(unsigned int PID);
    void searchTable(unsigned int PID);
    void deleteProcess(unsigned int PID);
    void printChain(unsigned int position);
    int findChainedProcess(unsigned int PID);
    void writeToMemory(unsigned int PID, int ADDR, int x);
    void readFromMemory(unsigned int PID, int ADDR);
    ~ChainingTable();
};

class DoubleHashing : public HashTable {
  private:
    Process *tableArray;  //pointer to array of proccesses
    int sizeArr;

  public:
    DoubleHashing(int n, int p);
    bool checkIndex(int position);
    int secondaryHash(unsigned int PID);
    void insertToArray(unsigned int PID);
    int searchForIndex(unsigned int PID);
    void searchArr(unsigned int PID);
    void deleteFromArray(unsigned int PID);
    void writeToMemory(unsigned int PID, int ADDR, int x);
    void readFromMemory(unsigned int PID, int ADDR);
    ~DoubleHashing();
};








#endif