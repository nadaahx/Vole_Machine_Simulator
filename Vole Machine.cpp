#include "Vole Machine.h"
#include <bits/stdc++.h>
using namespace std;


// Register Class
Register::Register() = default;

void Register::setRegister(int n , string m) {
    if(n>=0 && n<=15) {
        R[n] = m;
    }
}

void Register::clear(int x) {
    R[x]="";
}
// End of Register Class


// Memory Class
Memory::Memory() = default;

void Memory::setmemo(int n, string c) {
    if(n>=0 && n<=255) {
        m[n] = c;
    }
}

void Memory::clear() {
    m.clear();
}

void Memory::clearcell(int x) {
    m[x]="";
}
// End of Memory Class


// Instructions Class
void Instructions:: readFromFile() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string currentPathStr = currentPath.string();
    std::string filePath = currentPathStr + "/input.txt";

    ifstream inputFile(filePath);
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            instructions.push_back(line);
        }
        inputFile.close();
    } else {
        cerr << "Unable to open file: " << "input.txt" << endl;
        exit(1);
    }
}

void Instructions:: decode() {
    for (string& instruction : instructions) {

        istringstream iss(instruction);
        ostringstream decoded;


        string hexValue;
        while (iss >> hexValue) {

            if (hexValue.substr(0, 2) == "0x") {
                hexValue = hexValue.substr(2);
            }


            decoded << hexValue;
        }


        instruction = decoded.str();
    }
}


// End of Instructions Class




// Machine Class
Machine ::Machine() {

    input.readFromFile();
    input.decode();
    for (int i = 0; i < 256; i++) {
        if (i > input.instructions.size()-1){
            break;
        }
        if (input.instructions[i].size() < 4 ){
            continue;
        }
        string tmp = input.instructions[i].substr(0, 2);
        memory.m.emplace_back(tmp);
        tmp = input.instructions[i].substr(2, 2);

        memory.m.emplace_back(tmp);
        if (memory.m.size() > 256){
            cerr << "Memory is full" << endl;
            exit(1);
        }


    }
    for (int i = 0; i < 256; ++i) {
        if (memory.m.size() > 256){
            break;
        }
        memory.m.emplace_back("00");
    }
    for (int i = 0; i < 16; ++i) {
        if (registers.R.size() > 16){
            break;
        }
        registers.R.emplace_back("00");
    }

}


string Machine:: Get_Next_Instruction(){


    string the_instruction = memory.m[Program_Counter]+memory.m[Program_Counter+1];
    Program_Counter+=2;
    return  the_instruction;


}



// converting Hexa to Int
int hexCharToDecimal(char hexChar) {
    // Convert hex digit to decimal
    if (isdigit(hexChar)) {
        return hexChar - '0';
    } else {
        return toupper(hexChar) - 'A' + 10;
    }
}

int hexToDecimal( string hex) {

    return hexCharToDecimal(hex[0]) * 16 + hexCharToDecimal(hex[1]);
}


// converting Hexa to Bin
string hexToBinary(const string& hexString) {


    // Convert hexadecimal string to decimal
    stringstream hexStream(hexString);
    unsigned int decimal;
    hexStream >> hex >> decimal;

    // Convert decimal to binary string
    bitset<8> binary(decimal);

    return binary.to_string();
}

// converting Bin to Hexa
string binaryToHex(const string& binaryString) {


    // Convert binary string to decimal
    bitset<8> binary(binaryString);
    unsigned long decimal = binary.to_ulong();

    // Convert decimal to hexadecimal string
    stringstream hexStream;
    hexStream << hex << decimal;

    // Pad with leading zero if needed
    string hexString = hexStream.str();
    if (hexString.length() == 1) {
        hexString = "0" + hexString;
    }

    return hexString;
}





void Machine::Run_Instruction() {
    while (true) {
        string instruction = Get_Next_Instruction();

        // LOAD the register R with the bit pattern found in the memory cell whose address is XY
        if (instruction[0] == '1') {
            int store_in_R = hexCharToDecimal(instruction[1]);

            int pattern_in_M = hexToDecimal(instruction.substr(2,2));
            registers.R[store_in_R] = memory.m[pattern_in_M];


        }

        //LOAD the register R with the bit pattern XY.
        if (instruction[0] == '2') {
            int store_in_R = hexCharToDecimal(instruction[1]);
            registers.R[store_in_R] = instruction.substr(2,2);


        }


        //STORE the bit pattern found in register R in the memory cell whose address is XY
        if (instruction[0] == '3') {

            int store_that_R = hexCharToDecimal(instruction[1]);

            //STORE to location 00, which is a memory mapping for the screen.
            if ( instruction.substr(2,2) == "00" ){
                cout << registers.R[store_that_R] << '\n';

            }
            else {
                int pattern_in_M = hexToDecimal(instruction.substr(2,2));
                memory.m[pattern_in_M] = registers.R[store_that_R];
            }


        }


        //MOVE the bit pattern found in register R to register S
        if (instruction[0] == '4') {
            int  ss , tt;
            if(instruction[2]>='0' && instruction[2]<='9'){
                ss = instruction[2]-'0';
            }
            else {
                if (instruction[2]=='A'){
                    ss = 10;
                }
                if (instruction[2]=='B'){
                    ss = 11;
                }
                if (instruction[2]=='C'){
                    ss = 12;
                }
                if (instruction[2]=='D'){
                    ss = 13;
                }
                if (instruction[2]=='E'){
                    ss = 14;
                }
                if (instruction[2]=='F'){
                    ss = 15;
                }
            }

            if(instruction[3]>='0' && instruction[3]<='9'){
                tt = instruction[3]-'0';
            }
            else {
                if (instruction[3]=='A'){
                    tt = 10;
                }
                if (instruction[3]=='B'){
                    tt = 11;
                }
                if (instruction[3]=='C'){
                    tt = 12;
                }
                if (instruction[3]=='D'){
                    tt = 13;
                }
                if (instruction[3]=='E'){
                    tt = 14;
                }
                if (instruction[3]=='F'){
                    tt = 15;
                }
            }

            registers.R[tt]=registers.R[ss];

        }

        //ADD the bit patterns in registers S and T as though they were two’s complement representations
        if (instruction[0] == '5') {
            string res="";
            int carry=0;
            int rr , ss , tt;
            if(instruction[1]>='0' && instruction[1]<='9'){
                rr = instruction[1]-'0';
            }
            else {
                if (instruction[1]=='A'){
                    rr = 10;
                }
                if (instruction[1]=='B'){
                    rr = 11;
                }
                if (instruction[1]=='C'){
                    rr = 12;
                }
                if (instruction[1]=='D'){
                    rr = 13;
                }
                if (instruction[1]=='E'){
                    rr = 14;
                }
                if (instruction[1]=='F'){
                    rr = 15;
                }
            }

            if(instruction[2]>='0' && instruction[2]<='9'){
                ss = instruction[2]-'0';
            }
            else {
                if (instruction[2]=='A'){
                    ss = 10;
                }
                if (instruction[2]=='B'){
                    ss = 11;
                }
                if (instruction[2]=='C'){
                    ss = 12;
                }
                if (instruction[2]=='D'){
                    ss = 13;
                }
                if (instruction[2]=='E'){
                    ss = 14;
                }
                if (instruction[2]=='F'){
                    ss = 15;
                }
            }

            if(instruction[3]>='0' && instruction[3]<='9'){
                tt = instruction[3]-'0';
            }
            else {
                if (instruction[3]=='A'){
                    tt = 10;
                }
                if (instruction[3]=='B'){
                    tt = 11;
                }
                if (instruction[3]=='C'){
                    tt = 12;
                }
                if (instruction[3]=='D'){
                    tt = 13;
                }
                if (instruction[3]=='E'){
                    tt = 14;
                }
                if (instruction[3]=='F'){
                    tt = 15;
                }
            }


            string s = hexToBinary(registers.R[ss]);
            string t = hexToBinary(registers.R[tt]);


            char sign1=s[0] , sign2=t[0];

            for (int i=7 ; i>=0 ; i--){
                if (s[i]=='1' && t[i]=='1' && carry==0){
                    res = res + "0";
                    carry=1;
                }
                else if (s[i]=='1' && t[i]=='1' && carry==1){
                    res = res + "1";
                    carry=1;
                }
                else if (((s[i]=='0' && t[i]=='1') || (s[i]=='1' && t[i]=='0')) && carry==0){
                    res = res + "1";
                    carry=0;
                }
                else if (((s[i]=='0' && t[i]=='1') || (s[i]=='1' && t[i]=='0')) && carry==1){
                    res = res + "0";
                    carry=1;
                }

                else if (s[i]=='0' && t[i]=='0' && carry==0){
                    res = res + "0";
                    carry=0;
                }
                else if (s[i]=='0' && t[i]=='0' && carry==1){
                    res = res + "1";

                    carry=0;
                }
            }

            reverse(res.begin() , res.end());
            string final = binaryToHex(res);
            transform(final.begin(), final.end(), final.begin(), ::toupper);

            registers.R[rr]=final;


        }

        // ADD the bit patterns in registers S and T as though they represented values in floating-point
        if (instruction[0] == '6') {
            int rr , ss , tt;
            if(instruction[1]>='0' && instruction[1]<='9'){
                rr = instruction[1]-'0';
            }
            else {
                if (instruction[1]=='A'){
                    rr = 10;
                }
                if (instruction[1]=='B'){
                    rr = 11;
                }
                if (instruction[1]=='C'){
                    rr = 12;
                }
                if (instruction[1]=='D'){
                    rr = 13;
                }
                if (instruction[1]=='E'){
                    rr = 14;
                }
                if (instruction[1]=='F'){
                    rr = 15;
                }
            }

            if(instruction[2]>='0' && instruction[2]<='9'){
                ss = instruction[2]-'0';
            }
            else {
                if (instruction[2]=='A'){
                    ss = 10;
                }
                if (instruction[2]=='B'){
                    ss = 11;
                }
                if (instruction[2]=='C'){
                    ss = 12;
                }
                if (instruction[2]=='D'){
                    ss = 13;
                }
                if (instruction[2]=='E'){
                    ss = 14;
                }
                if (instruction[2]=='F'){
                    ss = 15;
                }
            }

            if(instruction[3]>='0' && instruction[3]<='9'){
                tt = instruction[3]-'0';
            }
            else {
                if (instruction[3]=='A'){
                    tt = 10;
                }
                if (instruction[3]=='B'){
                    tt = 11;
                }
                if (instruction[3]=='C'){
                    tt = 12;
                }
                if (instruction[3]=='D'){
                    tt = 13;
                }
                if (instruction[3]=='E'){
                    tt = 14;
                }
                if (instruction[3]=='F'){
                    tt = 15;
                }
            }

            string s = hexToBinary(registers.R[ss]);
            string t = hexToBinary(registers.R[tt]);
            string resmnts="";
            int carry=0;
            char sign1=s[0] , sign2=t[0] , ressign;
            string expo1 = s.substr(1 , 3);
            string expo2 = t.substr(1 , 3);
            string resexpo;

            string mnts1 = s.substr(4 , 4);
            string mnts2 = t.substr(4 , 4);


            map<string , int>mp;
            mp["111"]=3;
            mp["110"]=2;
            mp["101"]=1;
            mp["100"]=0;
            mp["011"]=-1;
            mp["010"]=-2;
            mp["001"]=-3;
            mp["000"]=-4;

            bool sub=false;
            bool first=true;


            if (mp[expo1]>mp[expo2]){
                int s = mp[expo1] - mp[expo2];
                for (int i=0 ; i<s ; i++){
                    mnts2 = "0" + mnts2;
                    mnts2.pop_back();

                }
            }
            else if (mp[expo2]>mp[expo1]){
                int s = mp[expo2] - mp[expo1];
                for (int i=0 ; i<s ; i++){
                    mnts1 = "0" + mnts1;
                    mnts1.pop_back();
                }
            }

            if(sign1==sign2){
                ressign=sign1;

            }
            else{
                first = true;
                sub = true;
                for(int i=0 ; i<4 ; i++){
                    if(mnts1[i]==mnts2[i]){
                        continue;
                    }
                    else if(mnts1[i]=='1' && mnts2[i]=='0'){
                        first = true;
                        break;
                    }
                    else {
                        first = false;
                        break;
                    }
                }

                if (first== true){
                    ressign = sign1;
                }
                else{
                    ressign = sign2;
                }

            }

            if(mp[expo1]>mp[expo2]){
                resexpo = expo1;
            }
            else {
                resexpo = expo2;
            }


            if(sub==false){
                for (int i=3 ; i>=0 ; i--){
                    if (mnts1[i]=='1' && mnts2[i]=='1' && carry==0){
                        resmnts+="0";
                        carry=1;
                    }
                    else if (mnts1[i]=='1' && mnts2[i]=='1' && carry==1){
                        resmnts+="1";
                        carry=1;
                    }
                    else if (((mnts1[i]=='0' && mnts2[i]=='1') || (mnts1[i]=='1' && mnts2[i]=='0')) && carry==0){
                        resmnts+="1";
                        carry=0;
                    }
                    else if (((mnts1[i]=='0' && mnts2[i]=='1') || (mnts1[i]=='1' && mnts2[i]=='0')) && carry==1){
                        resmnts+="0";
                        carry=1;
                    }

                    else if (mnts1[i]=='0' && mnts2[i]=='0' && carry==0){
                        resmnts+="0";
                        carry=0;
                    }
                    else if (mnts1[i]=='0' && mnts2[i]=='0' && carry==1){
                        resmnts+="1";
                        carry=0;
                    }
                }
                if(carry==1){
                    resmnts+="1";
                    string z;
                    for (auto it = mp.begin(); it != mp.end(); ++it)
                        if (it->second == (mp[resexpo]+1)){
                            resexpo = it->first;
                            break;
                        }
                }
                reverse(resmnts.begin() , resmnts.end());
                if (carry==1){
                    resmnts.pop_back();
                }
            }

            else{
                if(first==true){
                    for (int i=3 ; i>=0 ; i--){
                        if (mnts1[i]=='1' && mnts2[i]=='1' && carry==0){
                            resmnts+="0";
                        }
                        if (mnts1[i]=='0' && mnts2[i]=='0' && carry==0){
                            resmnts+="0";
                        }
                        if (mnts1[i]=='1' && mnts2[i]=='0' && carry==0){
                            resmnts+="1";
                        }

                        if (mnts1[i]=='0' && mnts2[i]=='1' && carry==0){
                            resmnts+="1";
                            carry=1;
                        }
                        if (mnts1[i]=='0' && mnts2[i]=='1' && carry==1){
                            resmnts+="0";
                        }
                        if (mnts1[i]=='1' && mnts2[i]=='0' && carry==1){
                            resmnts+="0";
                            carry=0;
                        }
                        if (mnts1[i]=='0' && mnts2[i]=='0' && carry==1){
                            resmnts+="1";
                        }
                        if (mnts1[i]=='1' && mnts2[i]=='1' && carry==1){
                            resmnts+="1";
                        }
                    }

                }

                else{
                    for (int i=3 ; i>=0 ; i--){
                        if (mnts2[i]=='1' && mnts1[i]=='1' && carry==0){
                            resmnts+="0";
                        }
                        if (mnts2[i]=='0' && mnts1[i]=='0' && carry==0){
                            resmnts+="0";
                        }
                        if (mnts2[i]=='1' && mnts1[i]=='0' && carry==0){
                            resmnts+="1";
                        }

                        if (mnts2[i]=='0' && mnts1[i]=='1' && carry==0){
                            resmnts+="1";
                            carry=1;
                        }
                        if (mnts2[i]=='0' && mnts1[i]=='1' && carry==1){
                            resmnts+="0";
                        }
                        if (mnts2[i]=='1' && mnts1[i]=='0' && carry==1){
                            resmnts+="0";
                            carry=0;
                        }
                        if (mnts2[i]=='0' && mnts1[i]=='0' && carry==1){
                            resmnts+="1";
                        }
                        if (mnts2[i]=='1' && mnts1[i]=='1' && carry==1){
                            resmnts+="1";
                        }
                    }
                }

                reverse(resmnts.begin() , resmnts.end());
            }

            while(resmnts[0]=='0'){
                for (auto it = mp.begin(); it != mp.end(); ++it)
                    if (it->second == (mp[resexpo]-1)){
                        resexpo = it->first;
                        break;
                    }
                resmnts.erase(0, 1);
                resmnts+="0";
            }
            string final = binaryToHex((ressign+resexpo+resmnts));
            transform(final.begin(), final.end(), final.begin(), ::toupper);

            registers.R[rr]=final;

        }

        // JUMP to the instruction located in the memory cell at address XY if the bit pattern in register R
        //is equal to the bit pattern in register number 0
        if (instruction[0] == 'B') {
            int rr;
            if(instruction[1]>='0' && instruction[1]<='9'){
                rr = instruction[1]-'0';
            }
            else {
                if (instruction[1]=='A'){
                    rr = 10;
                }
                if (instruction[1]=='B'){
                    rr = 11;
                }
                if (instruction[1]=='C'){
                    rr = 12;
                }
                if (instruction[1]=='D'){
                    rr = 13;
                }
                if (instruction[1]=='E'){
                    rr = 14;
                }
                if (instruction[1]=='F'){
                    rr = 15;
                }
            }


            if ( registers.R[rr]==registers.R[0]){

                Program_Counter = hexToDecimal(instruction.substr(2,2));
            }
            else {
                continue;
            }

        }

        //HALT execution
        if (instruction[0] == 'C') {
            break;

        }

    }


}

void Machine:: Display_Memory(){

    cout<<" Memory | Value | Register | Value"<<'\n';
    cout<<"-----------------------------------"<<'\n';

    for (int i = 0; i < memory.m.size()-1; ++i) {
        if(memory.m[i].length()==2){
            if(i<9)
                cout <<"  "<< (i+1)<< "     |  " << memory.m[i] << "   | ";
            else
                cout <<"  "<< (i+1)<< "    |  " << memory.m[i] << "   | ";

        }
        else {
            if(i<9)
                cout <<"  "<< (i+1)<< "     |  " << "00" << "   | ";
            else
                cout <<"  "<< (i+1)<< "    |  " << "00" << "   | ";
        }

        if (i < registers.R.size()){

            if(registers.R[i].length()==2){
                if(i<9)
                    cout <<"  "<< (i+1)<< "     |  " << registers.R[i] << "   | ";
                else
                    cout <<"  "<< (i+1)<< "    |  " << registers.R[i] << "   | ";

            }
            else {
                if(i<9)
                    cout <<"  "<< (i+1)<< "     |  " << "00" << "   | ";
                else
                    cout <<"  "<< (i+1)<< "    |  " << "00" << "   | ";
            }
        }
        else if(i == registers.R.size()) {
            cout<<"------------------";
        }

        cout<<'\n';
    }



};