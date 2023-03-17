#include "hashing.h"
#include <iostream>



int main() {

    
    std::string cmd;
    std::string hashingType;
    int N{0};
    int P{0};
    unsigned int pid{0};
    int m{0};
    int addr{0};
    int x{0};

    std::cin>>hashingType; 

    if(hashingType == "ORDERED") {
        ChainingTable* newTable{nullptr};
        while(std::cin >> cmd) {
            if(cmd == "M") {
                std::cin >> N;
                std::cin >> P;

                newTable = new ChainingTable(N, P);
                std::cout<<"success" << std::endl;
            }
            else if(cmd == "INSERT") {
                std::cin >> pid;
                newTable->insertProcess(pid);
            }
            else if(cmd == "SEARCH") {
                std::cin >> pid;
                newTable->searchTable(pid);
            }
            else if(cmd == "WRITE") {
                std::cin >> pid;
                std::cin >> addr;
                std::cin >> x;
                newTable->writeToMemory(pid, addr, x);
            }
            else if(cmd == "READ") {
                std::cin >> pid;
                std::cin >> addr;
                newTable->readFromMemory(pid, addr);
            }
            else if(cmd == "DELETE") {
                std::cin >> pid;
                newTable->deleteProcess(pid);
            }
            else if(cmd == "PRINT") {
                std::cin >> m;
                newTable->printChain(m);
            }
            else if(cmd == "END") {
                delete newTable;
                return 0;
            }
        }
    }
    else if(hashingType == "OPEN") {
        DoubleHashing* newTable{nullptr};

        while(std::cin >> cmd) {
            if(cmd == "M") {
                std::cin >> N;
                std::cin >> P;

                newTable = new DoubleHashing(N, P);
                std::cout<<"success" << std::endl;
            }
            else if(cmd == "INSERT") {
                std::cin >> pid;
                newTable->insertToArray(pid);
            }
            else if(cmd == "SEARCH") {
                std::cin >> pid;
                newTable->searchArr(pid);
            }
             else if(cmd == "WRITE") {
                std::cin >> pid;
                std::cin >> addr;
                std::cin >> x;
                newTable->writeToMemory(pid, addr, x);
            }
            else if(cmd == "READ") {
                std::cin >> pid;
                std::cin >> addr;
                newTable->readFromMemory(pid, addr);
            }
            else if(cmd == "DELETE") {
                std::cin >> pid;
                newTable->deleteFromArray(pid);
            }
            else if(cmd == "END") {
                delete newTable;
                return 0;
            }
        }
    }






}