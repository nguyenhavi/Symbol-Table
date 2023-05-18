#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};

struct Data{
    string name;
    string VarType;
    string Value;
    string nameAssign;
    int levelBlock;
};

string Var_Name(string line){
    string ans = "";
    for (int i = 7; line[i] != ' '; i++){
        ans += line[i];
    }
    return ans;
}

string Var_Type(string line){
    string ans ="";
    int index = 0;
    for (int i = 7; line[i] != ' '; i++){
        index = i;
    }
    if (line[index + 2] == 'n'){ans = "number";}
    else if (line[index + 2] == 's'){ans = "string";}
    return ans;
}

string Var_Name_Assign(string line){
    string ans ="";
    for (int i = 7; line[i] != ' '; i++){
        ans += line[i];
    }
    return ans;
}

string Var_Value(string line){
    string ans="";
    int index = 0;
    for (int i = 7; line [i] != ' '; i++){
        index = i;
    }
    for (unsigned int i = index +2; i < line.size(); i++){
        ans += line[i];
    }
    return ans;
}

//From 20 -> 57 to take data from input

Data *MyPower = new Data[100]; 

//Throw Redeclared
bool checkRedeclared(Data* arr, string a, int index){
    //Position for the case we need to delete a Variable
    for (int i = 0; i < index; i++){
        if (a == arr[i].name){
            if (arr[i].levelBlock == arr[index].levelBlock){
                return 0;
            }
        }
    }
    return 1;
}

//Throw Undeclared use for LOOKUP and ASSIGN

bool checkUndeclaredLookup (Data* arr, string a, int index){
    for (int i = 0 ; i< index; i++){
        if (arr[i].name == a){return 1;}
    }
    return 0;
}

//Support for TypeMissMatch
bool checkNumberType(string a){
    for (unsigned int i = 0; i < a.size(); i++){
        if (a[i] < '0' || a[i] > '9'){return false;}
    }
    return true;
}

//Support for TypeMissMatch
bool checkStringType(string a){
    bool flag = 0;
    if (a[0] == 39 && a[a.size()-1] == 39){
        for (unsigned int i = 1; i < a.size()-1; i++){
            if ((a[i] >= 'a' && a[i] <= 'z') || (a[i] >= '0' && a[i] <= '9') || (a[i] >= 'A' && a[i] <= 'Z') || a[i] == 32){
                flag = 1;
            }
            else {flag = 0;break;}
        }
    }
    return flag;
}

bool checkUndeclared(Data* arr, string a, string b, int index, int levelAssign){
    bool flag1 = 0;
    bool flag2 = 0;
    bool flag = 0;
    if (checkStringType(b) || checkNumberType(b)){
        for (int i = 0; i < index; i++){
            if (arr[i].name == a){return 1;}
        }
    }
    else {
        for (int i = 0; i < index; i++){
            if (arr[i].name == a && arr[i].levelBlock <= levelAssign){flag1 = 1;break;}
            else {flag1 = 0;}
        }
        for (int i = 0; i < index; i++){
            if (arr[i].name == b && arr[i].levelBlock <= levelAssign){flag2 = 1;break;}
            else {flag2 = 0;}
        }
    }
    if (flag1 == 1 && flag2 == 1){flag = 1;}
    else {flag = 0;}
    return flag ;
}

//support for TypeMissMatch or you can use it to take Var Type
string takeVarType(Data* arr, string name, int index){
    string ans= "";
    for (int i = 0; i < index; i++){
        if (name == arr[i].name){ans = arr[i].VarType;}
    }
    return ans;
}

//Support for checkTypeMiss
bool checkAnotherIndentifier (Data* arr, string a, string b, int indexInsert){
    bool flag = 0;
    if (takeVarType(arr, a, indexInsert) == takeVarType(arr, b, indexInsert)){flag = 1;}
    return flag;
}

//TypeMisMatch
bool checkTypeMiss(Data* arr, string a, string b, int indexInsert){
    if (checkStringType(a) && takeVarType(arr, b, indexInsert) == "string"){
        return true;
    }
    else if (checkNumberType(a) && takeVarType(arr, b, indexInsert) == "number"){
        return true;
    }
    else if (checkAnotherIndentifier(arr, a, b, indexInsert)){
        return true;
    }
    return false;
}

//Support for LOOKUP
string VarNameLookUp(string a){
    string ans ="";
    for (unsigned int i = 7;  i < a.size() ;i++){
        ans += a[i];
    }
    return ans;
}

//Support for checkRedeclared, PRINT , RPRINT
int TheLevelOfVariable(Data* arr, string a, int indexInsert){
    int level = 0;
    for (int i = 0 ; i < indexInsert; i++){
        if (a == arr[i].name){level = arr[i].levelBlock;}
    }
    return level;
}

//Support for PRINT and RPRINT
void DeleteDuplicate(Data* arr, int& indexInsert, int position){
    for (int i = position; i < indexInsert; i++){
        arr[i].name = arr[i+1].name;
        arr[i].VarType = arr[i+1].VarType;
        arr[i].Value = arr[i+1].Value;
        arr[i].nameAssign = arr[i+1].nameAssign;
        arr[i].levelBlock = arr[i+1].levelBlock;
    }
    indexInsert -= 1;
    return;
}

//Support for check if this line is Invalid Instruction
int countSpace (string line){
    int count = 0;
    for (unsigned int i = 0; i < line.size(); i++){
        if (line[i] == ' '){count++;}
    }
    return count;
}

int findMaxScope(Data* arr, string name, int indexInsert){
    int ans = 0;
    for (int i = 0; i < indexInsert; i++){
        if (arr[i].name == name){ans = arr[i].levelBlock;}
    }
    return ans;
}

#endif