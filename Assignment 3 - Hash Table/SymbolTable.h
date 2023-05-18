#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};

enum STATUS_TYPE{NIL, NON_EMPTY, DELETED};
class OpenHash{
private:
    int (*hashFunction)(long long, int, int, int, int);
    STATUS_TYPE* status;
    int* level;
    string* identifier;
    string* type;
    int size;
public:
    OpenHash(int size, int (*hashFunction)(long long, int, int, int, int)){
        this->size = size;
        this->hashFunction = hashFunction;
        this->level = new int[size];
        this->identifier = new string[size];
        this->type = new string[size];
        this->status = new STATUS_TYPE[size];
        for(int i = 0; i < size; i++){
            this->level[i] = -1;
            this->status[i] = NIL;
        }
    }
    int insert(string, int, string, int, int);
    bool checkRedeclared(string, int);
    string print();
    int getSize();
    bool checkUndeclared(string);
    void DeleteScope(int);
    int lookup(string, int, int);
    string takeVarType(string, int, int, int&);
    string takeTypeFromArr(string*&, int, int&, int, int, bool&);
    string lookUpUseForFunction(string, int, int, int&);
    void loadTypeFuncToHash(string, string, int, int);
    int LayKieuHamCuaInput(string*&, string, string, string, string, int, int, int, int&);
    int XuLyChoCALL(string*&, string, string, int, int, int, int&);
};

long long encode(string name, int level){
    string strAns = to_string(level);
    for(unsigned int i = 0; i < name.size(); i++){
        strAns += to_string(name[i]-48);
    }
    long long ans = stoll(strAns);
    return ans;
}

int OpenHash::lookup(string name, int constant1, int constant2){
    int maxLevel = 0;
    for(int i = 0;  i < size; i++){
        if(identifier[i] == name && level[i] > maxLevel && status[i] == NON_EMPTY){maxLevel = level[i];}
    }
    long long code = encode(name, maxLevel);
    int i = 0;
    while(i < size){
        int j = hashFunction(code, constant1, constant2, size, i);
        if(identifier[j] == name){
            return j;
        }
        else if (status[j] == NIL){
            return -1;
        }
        else{
            i = i + 1;
        }
    }
    return -1;
}

void OpenHash::DeleteScope(int levelInput){
    for(int i = 0; i < size; i++){
        if(level[i] == levelInput){
            status[i] = DELETED;
            identifier[i] = "SpiderMan";
            type[i] = "none";
            level[i] = -99; 
        }
    }
    return;
}

int OpenHash::insert(string name, int levelinput, string typeInput, int constant1, int constant2){
    int i = 0;
    long long key = encode(name, levelinput);
    while(i < size){
        int j = hashFunction(key, constant1, constant2, size, i);
        if(status[j] == NIL || status[j] == DELETED){
            identifier[j] = name;
            level[j] = levelinput;
            type[j] = typeInput;
            status[j] = NON_EMPTY;
            return i;
        }
        else{
            i = i + 1;
        }
    }
    return -1;
}

bool OpenHash::checkRedeclared(string name, int levelinput){
    for(int i = 0 ; i < size; i++){
        if(status[i] == NON_EMPTY){
            if(identifier[i] == name && level[i] == levelinput){
                return 1;
            }
        }
    }
    return 0;
}

string OpenHash::print(){
    string ans = "";
    for(int i = 0; i < size ; i++){
        if(status[i] == NON_EMPTY){
            ans += to_string(i);
            ans += ' ';
            ans += identifier[i];
            ans += "//";
            ans += to_string(level[i]);
            ans += ';';
        }
    }
    if(ans.size() == 0){return ans;}
    ans.pop_back();
    return ans;
}

int linearHashing(long long key, int constant, int drop, int size, int i){
    return ((key % size) + constant*i) % size;
}

int quadraticHashing(long long key, int constant1, int constant2, int size, int i){
    return ((key % size) + constant1*i + constant2*i*i) % size;
}

int doubleHashing(long long key, int constant, int drop, int size, int i){
    int h1 = key % size;
    int h2 = 1 + (key % (size-2));
    return (h1 + constant*i*h2) % size;
}

string getMethod(string input){
    string ans = "";
    for(int i = 0; input[i] != ' '; i++){ans += input[i];}
    return ans;
}

int getSizeTable(string input){
    string size = "";
    int support = 0;
    for(int i = 0; input[i] != ' '; i++){support = i;}
    for(int i = support + 2; input[i] != ' '; i++){size += input[i];}
    int ans = stoi(size);
    return ans;
}

int getConstant1ForLinearAndDouble(string input){
    string constant1 = "";
    int support = 0;
    for(int i = 0; input[i] != ' '; i++){support = i;}
    for(int i = support + 2; input[i] != ' '; i++){support = i;}
    for(unsigned int i = support + 2;  i < input.size(); i++){constant1 += input[i];}
    int ans = stoi(constant1);
    return ans;
}

int getConstant1ForQUADRATIC(string input){
    string constant1 = "";
    int support = 0;
    for(int i = 0; input[i] != ' '; i++){support = i;}
    for(int i = support + 2; input[i] != ' '; i++){support = i;}
    for(int i = support + 2;  input[i] != ' '; i++){constant1 += input[i];}
    int ans = stoi(constant1);
    return ans;
}

int getConstant2(string input){
    string constant2 = "";
    int support = 0;
    for(int i = 0; input[i] != ' '; i++){support = i;}
    for(int i = support + 2; input[i] != ' '; i++){support = i;}
    for(int i = support + 2; input[i] != ' '; i++){support = i;}
    for(unsigned int i = support + 2; i < input.size(); i++){constant2 += input[i];}
    int ans = stoi(constant2);
    return ans;
}

int countSpace(string input){
    int ans = 0;
    for(unsigned int i = 0; i < input.size(); i++){
        if(input[i] == ' '){ans = ans + 1;}
    }
    return ans;
}

//Use for LOOKUP and INSERT, ASSIGN
string getIdentifierForVar(string input){
    string ans = "";
    int support = 0;
    for(int i = 0; input[i] != ' '; i++){support = i;}
    for(unsigned int i = support + 2; i < input.size(); i++){ans += input[i];}
    return ans;
}

string getIdentifierForFunc(string input){
    string ans = "";
    int support = 0;
    for(int i = 0; input[i] != ' '; i++){support = i;}
    for(int i = support + 2; input[i] != ' '; i++){ans += input[i];}
    return ans;
}

int NumPara(string input){
    string ans = "";
    unsigned int support = 0;
    for(int i = 0; input[i] != ' '; i++){support = i;}
    for(unsigned int i = support + 2; input[i] != ' ' && i < input.size(); i++){support = i;}
    if(support == input.size()-1){return -1;}
    for(unsigned int i = support + 2; i < input.size(); i++){ans += input[i];}
    int num = stoi(ans);
    return num;
}

string getType(string input){
    int num = NumPara(input);
    if(num == -1){return "u";}
    else{
        string ans = "f";
        for(int i = 0; i < num; i++){
            ans += "u";
        }
        ans += "u";
        return ans;
    }
    return "";
}

//Use for CALL function
string getIdentifierCALL(string input){
    string ans = "";
    int support = 0;
    for(int i = 0; input[i] != ' '; i++){support = i;}
    for(int i = support + 2; input[i] != '('; i++){ans += input[i];}
    return ans;
}

bool OpenHash::checkUndeclared(string input){
    for(int i = 0; i < size; i++){
        if(identifier[i] == input){return 0;}
    }
    return 1;
}

string OpenHash::takeVarType(string name, int constant1, int constant2, int& ans){
    string j = lookUpUseForFunction(name, constant1, constant2, ans);
    return j;
}

int countPond(string input){
    int ans = 0;
    for(unsigned int i = 0; i < input.size(); i++){
        if(input[i] == ','){ans = ans + 1;}
    }
    return ans;
}

bool checkNumber(string input){
    for (unsigned int i = 0; i < input.size(); i++){
        if (input[i] < '0' || input[i] > '9'){return 0;}
    }
    if(input.size() == 0){return 0;}
    return 1;
}

bool checkString(string input){
    bool a = (input[0] == 39) && (input[input.size()-1] == 39);
    bool b = 0;
    for (unsigned int i = 1 ; i < input.size()-1; i++){
        if ((input[i] >= '0' && input[i] <= '9') || (input[i] >= 'A' && input[i] <= 'Z') || (input[i] >='a' && input[i] <= 'z') || input[i] == ' '){
            b = 1;
        }
        else {
            b = 0;
            break;
        }
    }
    return a && b;
}

bool isFunctionHaveUnkownType(string input){
    int count = 0;
    for(unsigned int i = 0; i < input.size(); i++){
        if(input[i] == 'u'){count = count + 1;}
    }
    if(count == 0){return 0;}
    return 1;
}

void loadValueToArr(string* &arr, string input, int numberOfPara){
    //input = foo(3,'abc', a)
    int support = 0;
    for(int i = 0; input[i] != '('; i++){support = i;}
    support = support + 2;
    int i = 0;
    while(i < numberOfPara){
        while(input[support] != ',' && input[support] != ')'){
            arr[i] += input[support];
            support = support + 1;
        }
        i = i + 1;
        support = support + 1;
    }
    return;
}

string OpenHash::takeTypeFromArr(string* &arr, int countPond, int& ans, int constant1, int constant2, bool& flag){
    string answer = "f";
    for(int i = 0; i < countPond; i++){
        if(checkNumber(arr[i])){answer += "n";}
        else if (checkString(arr[i])){answer += "s";}
        else{
            //tham so
            string subtype = lookUpUseForFunction(arr[i], constant1, constant2, ans);
            if(subtype == "Undeclared"){flag = 0; return arr[i];}
            answer += subtype;
        }
    }
    flag = 1;
    answer += "v";
    return answer;
}

string OpenHash::lookUpUseForFunction(string name, int constant1, int constant2, int& ans){
    int maxLevel = 0;
    for(int i = 0;  i < size; i++){
        if(identifier[i] == name && level[i] > maxLevel){maxLevel = level[i];}
    }
    long long code = encode(name, maxLevel);
    int i = 0;
    while(i < size){
        int j = hashFunction(code, constant1, constant2, size, i);
        if(identifier[j] == name){
            ans += i;
            return type[j];
        }
        else if (status[j] == NIL){
            return "Undeclared";
        }
        else{
            i = i + 1;
        }
    }
    return "Undeclared";
}

bool DoyouHavePara(string input){
    int support = 0;
    for(int i = 0; input[i] != '('; i++){support = i;}
    if(input[support + 1] == '(' && input[support+2] == ')'){return 0;}
    return 1;
}

string takePara(string input){
    int support = 0;
    string ans = 0;
    for(int i = 0; input[i] != '('; i++){support = i;}
    for(int i = support + 2; input[i] != ')'; i++){ans += input[i];}
    return ans;
}

void OpenHash::loadTypeFuncToHash(string name, string typeFunc, int constant1, int constant2){
    int maxLevel = 0;
    for(int i = 0;  i < size; i++){
        if(identifier[i] == name && level[i] > maxLevel){maxLevel = level[i];}
    }
    long long code = encode(name, maxLevel);
    int i = 0;
    while(i < size){
        int j = hashFunction(code, constant1, constant2, size, i);
        if(identifier[j] == name){
            type[j] = typeFunc;
            return ;
        }
        else if (status[j] == NIL){
            return ;
        }
        else{
            i = i + 1;
        }
    }
    return;
}

string valueOfAssign(string input){
    string ans = "";
    int support = 0;
    for(int i = 0; input[i] != ' '; i++){support = i;}
    for(int i = support + 2; input[i] != ' '; i++){support = i;}
    for(unsigned int i = support + 2; i < input.size(); i++){ans += input[i];}
    return ans;
}

string funcName(string input){
    string ans = "";
    for(int i = 0; input[i] != '('; i++){ans += input[i];}
    return ans;
}

int countHook(string input){
    int ans = 0;
    for(unsigned int i = 0; i < input.size(); i++){
        if(input[i] == '(' || input[i] == ')'){ans = ans + 1;}
    }
    return ans;
}

bool tenbien(string name){
    if(name[0] > 'z' || name[0] < 'a'){return 0;}
    for(unsigned int i = 1; i < name.size(); i++){
        if((name[i] > 'z' || name[i] < 'a') && (name[i] < 'A' || name[i] > 'Z') && name[i] != '_' && (name[i] > '9' || name[i] < '0')){return 0;}
    }
    return 1;
}

int OpenHash::LayKieuHamCuaInput(string*& arr, string typeName, string nameFunc, string typeFunc, string name, int constant1, int constant2, int NumberOfPara, int& ans){
    // 0 la undeclared
    // 1 la typemismatch
    // 2 la typecannotbeinferred
    // 3 la binh thuong
    // 4 la invalid
    for(int i = 0; i < NumberOfPara; i++){
        if(checkNumber(arr[i]) || checkString(arr[i])){continue;}
        else {
            if(tenbien(arr[i]) == 0){return 4;}
            else{continue;}
        }
    }
    string KieuHamInput = "f";
    for(int i = 0; i < NumberOfPara; i++){
        if(checkNumber(arr[i])){KieuHamInput += "n";}
        else if (checkString(arr[i])){KieuHamInput += "s";}
        else{
            string support = takeVarType(arr[i], constant1, constant2, ans);
            if(support == "Undeclared"){arr[0] = arr[i];return 0;}
            if(support != "u" && support != "s" && support != "n"){return 1;}
            KieuHamInput += support;
        }
    }
    if(typeName == "Undeclared"){arr[0] = name;return 0;}
    KieuHamInput += typeName;
    bool a = isFunctionHaveUnkownType(typeFunc);
    bool b = isFunctionHaveUnkownType(KieuHamInput);
    if(a && b){return 2;}
    if(KieuHamInput.size() != typeFunc.size()){return 1;}

    if(a == 1){loadTypeFuncToHash(nameFunc, KieuHamInput, constant1, constant2); return 3;}
    if(b == 1){
        for(unsigned int i = 1; i < KieuHamInput.size()-1; i++){
            if(KieuHamInput[i] == 'u'){
                if(checkNumber(arr[i-1]) && checkString(arr[i-1])){continue;}
                else{
                    string support = "";
                    support += typeFunc[i];
                    loadTypeFuncToHash(arr[i-1], support, constant1, constant2);
                }
            }
            else {
                if(KieuHamInput[i] != typeFunc[i]){return 1;}
            }
        }
        if(typeName == "u"){
            string support = "";
            support += typeFunc[typeFunc.size()-1];
            loadTypeFuncToHash(name, support, constant1, constant2);
        }
        return 3;
    }

    if(typeFunc != KieuHamInput){return 1;}
    return 3;
}

int OpenHash::XuLyChoCALL(string*& arr, string nameFunc, string typeFunc, int constant1, int constant2, int numberOfPara, int& ans){
    // 0 la undeclared
    // 1 la typemismatch
    // 2 la typecannotbeinferred
    // 3 la binh thuong
    // 4 la invalid
    for(int i = 0; i < numberOfPara; i++){
        if(checkNumber(arr[i]) || checkString(arr[i])){continue;}
        else {
            if(tenbien(arr[i]) == 0){return 4;}
            else{continue;}
        }
    }
    string KieuHamInput = "f";
    for(int i = 0; i < numberOfPara; i++){
        if(checkNumber(arr[i])){KieuHamInput += "n";}
        else if (checkString(arr[i])){KieuHamInput += "s";}
        else{
            string support = takeVarType(arr[i], constant1, constant2, ans);
            if(support == "Undeclared"){arr[0] = arr[i];return 0;}
            if(support != "u" && support != "s" && support != "n"){return 1;}
            KieuHamInput += support;
        }
    }
    KieuHamInput += "v";
    bool a = isFunctionHaveUnkownType(typeFunc);
    bool b = isFunctionHaveUnkownType(KieuHamInput);
    if(a == 0 && typeFunc[typeFunc.size()-1] != 'v'){return 1;}
    if(a && b){return 2;}
    if(KieuHamInput.size() != typeFunc.size()){return 1;}
    if(a == 1){loadTypeFuncToHash(nameFunc, KieuHamInput, constant1, constant2); return 3;}
    if(b == 1){
        for(unsigned int i = 1; i < KieuHamInput.size()-1; i++){
            if(KieuHamInput[i] == 'u'){
                if(checkNumber(arr[i-1]) && checkString(arr[i-1])){continue;}
                else{
                    string support = "";
                    support += typeFunc[i];
                    loadTypeFuncToHash(arr[i-1], support, constant1, constant2);
                }
            }
            else{
                if(KieuHamInput[i] != typeFunc[i]){return 1;}
            }
        }
        return 3;
    }
    if(typeFunc != KieuHamInput){return 1;}
    return 3;
}


#endif