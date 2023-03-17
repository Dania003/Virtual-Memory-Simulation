#include "hashing.h"


Process::Process(unsigned int pid = 0) {   //constructor can be called w/o argument (pid defult is 0)
    PID = pid;
    page_address = 0;
    wasOccupied = 0;
}

HashTable::HashTable(int n, int p) : N(n), P(p), m(N/P), memory(nullptr), indexOccupied(nullptr) {
    memory = new int[N];
    for (int i = 0; i < N; i++) {
        memory[i] = 0;      //initially all values are 0
    }

    indexOccupied = new bool[m];
    for (int i = 0; i < m; i++) {
        indexOccupied[i] = 0;       //initially all pages are free
    }
}
HashTable::~HashTable() {
    delete [] memory;
    delete [] indexOccupied;
}

int HashTable::firstFreePage() {
    for (int i = 0; i < m; i++) {
        if (indexOccupied[i] == 0) {
            return i;
        }       
    }
    return -1;
}

int HashTable::primaryHash(unsigned int PID) {
    int result{0};

    result = (PID % m);
    return result;
}

void HashTable::freePage(Process process) {
    int address = process.page_address;     //find the page address allocated to process

    //if(address != 0) {      
    address = address/P;     //if the address is not 0, address in bool array equals address/p
    //}                           //the only other case would be if address is 0, in this case address in bool array would also be 0
                                //this is done to avoid dividing by 0
    
    indexOccupied[address] = 0; //free that space in bool array
}

ChainingTable::ChainingTable(int n, int p) : HashTable(n, p){
    table = new std::vector<Process>[m];  //allocate memory for an array of m vectors
    size = 0;
}

ChainingTable::~ChainingTable() {
    delete[] table;
}


void ChainingTable::insertProcess(unsigned int PID) {
    int pageIndex = firstFreePage();
    int index = primaryHash(PID);

    bool found = false;
    for(int i=0; i<table[index].size(); i++) {
        if(table[index][i].PID == PID) {
            found = true;
            break;
        }
    }
    if(!(found) && (pageIndex>=0) && (size < m) ) {        //if there is an empty page in memory and space in hashtable

        Process newProcess(PID);
        newProcess.page_address = (pageIndex*P);
    
        table[index].push_back(newProcess);   //push process with corresponding PID to the end of the array

        size++;
        indexOccupied[pageIndex] = 1;       //this page is now occupied

        std::cout<<"success" << std::endl;
        return;
    } 
    else {
        std::cout<<"failure" << std::endl;
    }
    
}

void ChainingTable::deleteProcess(unsigned int PID) {
    int index = primaryHash(PID);
    for (int i = 0; i < table[index].size(); i++) {
        if(table[index][i].PID == PID) {
            freePage(table[index][i]);      //free space in bool array
            table[index][i].PID = 0;        //delete process by setting its PID to 0
            //table[index].erase(table[index].begin() + i);   //delete the process
            std::cout<<"success" <<std::endl;
            size--;
            return;
        }
    }
    std::cout<<"failure"<<std::endl;
}

void ChainingTable::searchTable(unsigned int PID){
    int index = primaryHash(PID);

    for (int i = 0; i < table[index].size(); i++) {
        Process current = table[index][i];

        if(current.PID == PID) {
            std::cout<<"found " << PID <<" in " << index <<std::endl;
            return;
        }
    }
    std::cout<<"not found"<<std::endl;
}

void ChainingTable::writeToMemory(unsigned int PID, int ADDR, int x) {
    int index = primaryHash(PID);
    int memAddr{0};
    int inputIndex{0};

    for (int i = 0; i < table[index].size(); i++) {
        Process current = table[index][i];

        if(current.PID == PID) {        //finding the process with given PID
            memAddr = current.page_address;     //finding page address of current
            if( (ADDR>=0)  && (ADDR<P) ) {  //checking offset is within page range
                inputIndex = memAddr + ADDR;    //finding index where to input x
                memory[inputIndex] = x;
                std::cout<< "success" <<std::endl;
                return;
            } 

        }
    }
    std::cout<< "failure" <<std::endl;    
}

void ChainingTable::readFromMemory(unsigned int PID, int ADDR) {
    int index = primaryHash(PID);
    int memAddr{0};
    int readIndex{0};

    for (int i = 0; i < table[index].size(); i++) {
        Process current = table[index][i];

        if(current.PID == PID) {        //finding the process with given PID
            memAddr = current.page_address;     //finding page address of current
            if( (ADDR>=0)  && (ADDR<P) ) {
                readIndex = memAddr + ADDR;
                std::cout<< ADDR << " " << memory[readIndex] <<std::endl;
                return;
            }
        }
    }
    std::cout<< "failure" <<std::endl;
}


void ChainingTable::printChain(unsigned int position) {

    int temp{0};
    bool emptyVector{true};

    //bubble sorting
    for(int i{0}; i<table[position].size(); i++) {
        for(int k{i+1}; k<table[position].size(); k++) {

            if(table[position][k].PID > table[position][i].PID) {
                temp = table[position][k].PID;
                table[position][k].PID = table[position][i].PID;
                table[position][i].PID = temp;
            }
        }
    }

    for (int i = 0; i < table[position].size(); i++) {
        if(table[position][i].PID != 0) {
            std::cout << table[position][i].PID << " ";
            emptyVector = false;
        }
    }
    if(emptyVector) {
        std::cout << "chain is empty";
    }
    std::cout << std::endl;
  
}


DoubleHashing::DoubleHashing(int n, int p) : HashTable(n, p) {
    tableArray = new Process[m];
    sizeArr = 0;
}

DoubleHashing::~DoubleHashing() {
    delete[] tableArray;
}

bool DoubleHashing::checkIndex(int position) {
    bool isEmpty{true};

    if(tableArray[position].PID != 0) {
        isEmpty = false;
    }

    return isEmpty;
}

int DoubleHashing::secondaryHash(unsigned int PID) {
    int h1 = primaryHash(PID);
    int h2 = (PID/m) % m;
    int index{h1};

    if (h2 % 2 == 0) {      //if even add 1
        h2++;
    }
    

    for(int i{0}; i<m; i++) {
        index = (h1 + (i*h2))%m;
        if(checkIndex(index)) {
            return index;
        }
    }
    return index;
}

int DoubleHashing::searchForIndex(unsigned int PID) {
    int h1 = primaryHash(PID);
    int h2 = (PID/m) % m;
    int index{h1};

    if (h2 % 2 == 0) {      //if even add 1
        h2++;
    }

    int i{0};

    while ( (i<m)  && (tableArray[index].wasOccupied=1) ) {
        //keep hashing until empty index is found
        if(tableArray[index].PID == PID){
            return index;
        }
        index = (index + h2) % m;
        //index = (h1 + (i*h2))%m;
        i++;
    }
    // if(index<m) {
    //     return index;        
    // }
    return -1;
}


void DoubleHashing::insertToArray(unsigned int PID) {
    int pageIndex = firstFreePage();    //getting the address of first free page
    int index = secondaryHash(PID);
    int search = searchForIndex(PID);

    bool found = false;     //search if PID exists 
     if(search >= 0 ) {
        found = true;
     }

    if(!found && (pageIndex>=0) && (sizeArr < m) ) {     //there is an empty page in memory and space in hashtable
        //int index = secondaryHash(PID);

        Process newProcess(PID);
        newProcess.page_address = (pageIndex*P);
        newProcess.wasOccupied = 1;     //now flag shows that this space has been occupied

        tableArray[index] = newProcess;
        sizeArr++;
        indexOccupied[pageIndex] = 1;
        std::cout<<"success" << std::endl;
    }
    else {
        std::cout<<"failure" << std::endl;
    }

}

void DoubleHashing::searchArr(unsigned int PID) {
    int index = searchForIndex(PID);

    if((index>=0) && (tableArray[index].PID == PID)) {
        std::cout<<"found " << PID << " in " << index << std::endl;
    }
    else {
        std::cout<<"not found" << std::endl;
    }
}

void DoubleHashing::deleteFromArray(unsigned int PID) {
    int index = searchForIndex(PID);
    bool validIndex{false};

    if(index>=0) {
        validIndex = true;
    } 
    if( (validIndex) && (tableArray[index].PID == PID) ) {
        freePage(tableArray[index]);    //free space in bool array
        tableArray[index].PID = 0;
        sizeArr--;
        std::cout<<"success" << std::endl;
    } 
    else {
        std::cout<<"failure" << std::endl;
    }
}

void DoubleHashing::writeToMemory(unsigned int PID, int ADDR, int x) {
    int index = searchForIndex(PID);
    int memAddr{0};
    int inputIndex{0};
    bool validIndex{false};

    if(index>=0) {
        validIndex = true;
    }


    if( (validIndex) && (tableArray[index].PID == PID) ) {
        memAddr = tableArray[index].page_address;     //finding page address of current

            if( (ADDR>=0)  && (ADDR<P) ) {      //checking offset is within page range
                inputIndex = memAddr + ADDR;    //finding index where to input x
                memory[inputIndex] = x;
                std::cout<< "success" <<std::endl;
                return;
            }    
    }
    std::cout<< "failure" <<std::endl;
}

void DoubleHashing::readFromMemory(unsigned int PID, int ADDR) {
    int index = searchForIndex(PID);
    int memAddr{0};
    int readIndex{0};
    bool validIndex{false};

    if(index>=0) {
        validIndex = true;
    }

    if( (validIndex) && (tableArray[index].PID == PID) ) {
        memAddr = tableArray[index].page_address;     //finding page address of current

            if( (ADDR>=0)  && (ADDR<P) ) {      //checking offset is within page range
                readIndex = memAddr + ADDR;    //finding index where to input x
                std::cout<< ADDR << " " << memory[readIndex] <<std::endl;
                return;
            }    
    }
    std::cout<< "failure" <<std::endl;
}
