
#include <bits/stdc++.h>
using namespace std;
#ifndef VOLE_MACHINE_H
#define VOLE_MACHINE_H




class Register {
private:
    vector<string>R;
public:
    Register();
    void setRegister(int n , string m);
    void clear(int x);
    friend class Machine;
};



class Memory {
private:
    vector<string> m ;
public:
    Memory();
    void setmemo(int n , string c);
    void clearcell(int x);
    void clear();
    friend class Machine;
};



class Instructions {
private:
    vector<string> instructions;

public:

    // Read instructions from a file and store in the vector
    void readFromFile();

    // Decode instructions in the vector
    void decode();


    friend class Machine;
};




class Machine {
private:
    Register registers;
    Memory memory;
    Instructions input;
    int Program_Counter = 0;
public:
    Machine();
    string Get_Next_Instruction();
    void Run_Instruction();
    void Display_Memory();
    void Display_Registers();

};









#endif //VOLE_MACHINE_H
