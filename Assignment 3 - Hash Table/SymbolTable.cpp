#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    ifstream readFile(filename);
    string input = "";
    getline(readFile, input);
    if(getMethod(input) != "LINEAR" && getMethod(input) != "QUADRATIC" && getMethod(input) != "DOUBLE"){throw InvalidInstruction(input);}
    if(countSpace(input) == 2 || countSpace(input) == 3){}
    else{throw InvalidInstruction(input);}
    int size = getSizeTable(input);
    int constant1 = 0;
    if(getMethod(input) == "LINEAR" || getMethod(input) == "DOUBLE"){constant1 = getConstant1ForLinearAndDouble(input);}
    else{constant1 = getConstant1ForQUADRATIC(input);}
    int countBegin = 0;
    int countEnd = 0;
    //Hash Table with linear probing
    if(getMethod(input) == "LINEAR" && countSpace(input) == 2){
        OpenHash* Avenger = new OpenHash(size, linearHashing);
        while(!readFile.eof()){
            getline(readFile, input);
            if(getMethod(input) == "LINEAR"){continue;}
            if(getMethod(input) == "INSERT"){
                string name = "";
                if(countSpace(input) == 2){name = getIdentifierForFunc(input);}
                else{name = getIdentifierForVar(input);}
                if(tenbien(name) == 0){throw InvalidInstruction(input);}
                int level = countBegin - countEnd;
                string type = getType(input);
                if(Avenger->checkRedeclared(name, level)){throw Redeclared(name);}
                if(type[0] == 'f' && level != 0){throw InvalidDeclaration(input);}
                int ans = Avenger->insert(name, level, type, constant1, 0);
                if(ans == -1){throw Overflow(input);}
                cout << ans << endl;
            }
            else if (getMethod(input) == "ASSIGN" && countSpace(input) == 2){
                int ans = 0;
                // input = ASSIGN X Y
                string name = getIdentifierForFunc(input);     //name = X
                string typeName = Avenger->takeVarType(name, constant1, 0, ans); //typeName = u
              //  if(typeName == "Undeclared"){throw Undeclared(name);}      
                string value = valueOfAssign(input);            //value = y
                if(checkNumber(value)){
                    if(typeName == "Undeclared"){throw Undeclared(name);}
                    if(typeName == "n"){cout << ans << endl;}
                    else if(typeName == "u"){
                        Avenger->loadTypeFuncToHash(name, "n", constant1, 0);
                        cout << ans << endl;
                    }
                    else{throw TypeMismatch(input);}
                }
                else if(checkString(value)){
                    if(typeName == "Undeclared"){throw Undeclared(name);}
                    if(typeName == "s"){cout << ans << endl;}
                    else if(typeName == "u"){
                        Avenger->loadTypeFuncToHash(name, "s", constant1, 0);
                        cout << ans << endl;
                    }
                    else{throw TypeMismatch(input);}
                }
                else if(countHook(value) == 0){
                    if(tenbien(value) == 0){throw InvalidInstruction(input);}
                    string typeValue = Avenger->takeVarType(value, constant1, 0, ans);
                    if(typeValue == "Undeclared"){throw Undeclared(value);}
                    if(typeName == "Undeclared"){throw Undeclared(name);}
                    if(typeValue == "u" && typeName == "u"){throw TypeCannotBeInferred(input);}
                    else if ((typeValue == "n" || typeValue == "s") && typeName == "u"){
                        Avenger->loadTypeFuncToHash(name, typeValue, constant1, 0);
                        cout << ans << endl;
                    }
                    else if (typeValue == "u" && (typeName == "n" || typeName == "s")){
                        Avenger->loadTypeFuncToHash(value, typeName, constant1, 0);
                    }
                    else if (typeValue == typeName){cout << ans << endl;}
                    else{throw InvalidInstruction(input);}
                }
                else if (countHook(value) == 2){    // value = foo(x,y)
                    string nameFunc = funcName(value);      //nameFunc = foo
                    string typeFunc = Avenger->takeVarType(nameFunc, constant1, 0, ans);
                    if(typeFunc == "Undeclared"){throw Undeclared(nameFunc);}
                    string supportToLoad = "";
                    if(DoyouHavePara(value) == 0){
                        if(typeName == "Undeclared"){throw Undeclared(name);}
                        if(typeFunc[1] == 'u' && typeName == "u"){throw TypeCannotBeInferred(input);}
                        else if((typeFunc[1] == 'n' || typeFunc[1] == 's') && typeName == "u"){
                            supportToLoad += typeFunc[1];
                            Avenger->loadTypeFuncToHash(name, supportToLoad, constant1, 0);
                            cout << ans << endl;
                        }
                        else if(typeFunc[1] == 'u' && (typeName == "n" || typeName == "s")){
                            supportToLoad = "f";
                            supportToLoad += typeName;
                            Avenger->loadTypeFuncToHash(nameFunc, supportToLoad, constant1, 0);
                            cout << ans << endl;
                        }
                        else if (typeFunc[1] == typeName[0]){cout << ans << endl;}
                        else{throw TypeMismatch(input);}
                    }
                    else {        //DoyouHavePara = 1
                        unsigned int NumberOfPara = countPond(value) + 1;
                        string* ArrOfInput = new string[NumberOfPara];
                        loadValueToArr(ArrOfInput, value, NumberOfPara);
                        for(unsigned int i = 0; i < NumberOfPara; i++){
                            if(checkString(ArrOfInput[i]) || checkNumber(ArrOfInput[i])){continue;}
                            else{
                                if(tenbien(ArrOfInput[i]) == 0){throw InvalidInstruction(input);}
                                else{continue;}
                            }
                        }
                        if(NumberOfPara != typeFunc.size()-2){throw TypeMismatch(input);}
                        int a = Avenger->LayKieuHamCuaInput(ArrOfInput, typeName, nameFunc, typeFunc, name, constant1, 0, NumberOfPara, ans);
                        string b = ArrOfInput[0];
                        delete[] ArrOfInput;
                        if(a == 0){throw Undeclared(b);}
                        if(a == 1){throw TypeMismatch(input);}
                        if(a == 2){throw TypeCannotBeInferred(input);}
                        if(a == 3){cout << ans << endl;}
                    }
                }
                else{throw InvalidInstruction(input);}
            }
            else if (getMethod(input) == "CALL" && countHook(input) == 2 && countSpace(input) == 1){
                int ans = 0;
                string value = getIdentifierForVar(input);
                if(countHook(value) == 0){throw InvalidInstruction(input);}
                string nameFunc = funcName(value);      //nameFunc = foo
                string typeFunc = Avenger->takeVarType(nameFunc, constant1, 0, ans);
                if(typeFunc == "Undeclared"){throw Undeclared(nameFunc);}
                if(DoyouHavePara(value) == 0){
                    if(typeFunc[1] == 'u'){Avenger->loadTypeFuncToHash(nameFunc, "fu", constant1, 0); cout << ans << endl;}
                    if(typeFunc[1] != 'v'){throw TypeMismatch(input);}
                }
                if(DoyouHavePara(value)){        //DoyouHavePara = 1
                    unsigned int NumberOfPara = countPond(value) + 1;
                    string* ArrOfInput = new string[NumberOfPara];
                    loadValueToArr(ArrOfInput, value, NumberOfPara);
                        for(unsigned int i = 0; i < NumberOfPara; i++){
                            if(checkString(ArrOfInput[i]) || checkNumber(ArrOfInput[i])){continue;}
                            else{
                                if(tenbien(ArrOfInput[i]) == 0){throw InvalidInstruction(input);}
                                else{continue;}
                            }
                        }
                    if(NumberOfPara != typeFunc.size()-2){throw TypeMismatch(input);}
                    int a = Avenger->XuLyChoCALL(ArrOfInput, nameFunc, typeFunc, constant1, 0, NumberOfPara, ans);
                    string b = ArrOfInput[0];
                    delete[] ArrOfInput;
                    if(a == 0){throw Undeclared(b);}
                    if(a == 1){throw TypeMismatch(input);}
                    if(a == 2){throw TypeCannotBeInferred(input);}
                    if(a == 3){cout << ans << endl;}
                }
            }
            else if(input == "BEGIN"){
                countBegin = countBegin + 1;
            }
            else if(input == "END"){
                countEnd = countEnd + 1;
                if(countEnd > countBegin){throw UnknownBlock();}
                Avenger->DeleteScope(countBegin - countEnd + 1);
            }
            else if(getMethod(input) == "LOOKUP" && countSpace(input) == 1){
                string name = getIdentifierForVar(input);
                if(tenbien(name) == 0){throw InvalidInstruction(input);}
                if(Avenger->lookup(name, constant1, 0) == -1){throw Undeclared(name);}
                cout << Avenger->lookup(name, constant1, 0) << endl;
            }
            else if(input == "PRINT"){
                string ans = Avenger->print();
                if(ans.size() == 0){continue;}
                else{cout << ans << endl;}
            }
            else{
                throw InvalidInstruction(input);
            }
        }
        readFile.close();
        if (countBegin != countEnd){throw UnclosedBlock(countBegin - countEnd);}
    }

    // Hash Table with quadratic probing
    else if(getMethod(input) == "QUADRATIC" && countSpace(input) == 3){
        int constant2 = getConstant2(input);
        OpenHash* Avenger = new OpenHash(size, quadraticHashing);
        while(!readFile.eof()){
            getline(readFile, input);
            if(getMethod(input) == "QUADRATIC"){continue;}
            if(getMethod(input) == "INSERT"){
                string name = "";
                if(countSpace(input) == 2){name = getIdentifierForFunc(input);}
                else{name = getIdentifierForVar(input);}
                if(tenbien(name) == 0){throw InvalidInstruction(input);}
                int level = countBegin - countEnd;
                string type = getType(input);
                if(Avenger->checkRedeclared(name, level)){throw Redeclared(name);}
                if(type[0] == 'f' && level != 0){throw InvalidDeclaration(input);}
                int ans = Avenger->insert(name, level, type, constant1, constant2);
                if(ans == -1){throw Overflow(input);}
                cout << ans << endl;
            }
            else if (getMethod(input) == "ASSIGN" && countSpace(input) == 2){
                int ans = 0;
                // input = ASSIGN X Y
                string name = getIdentifierForFunc(input);     //name = X
                string typeName = Avenger->takeVarType(name, constant1, constant2, ans); //typeName = u
              //  if(typeName == "Undeclared"){throw Undeclared(name);}      
                string value = valueOfAssign(input);            //value = y
                if(checkNumber(value)){
                    if(typeName == "Undeclared"){throw Undeclared(name);}
                    if(typeName == "n"){cout << ans << endl;}
                    else if(typeName == "u"){
                        Avenger->loadTypeFuncToHash(name, "n", constant1, constant2);
                        cout << ans << endl;
                    }
                    else{throw TypeMismatch(input);}
                }
                else if(checkString(value)){
                    if(typeName == "Undeclared"){throw Undeclared(name);}
                    if(typeName == "s"){cout << ans << endl;}
                    else if(typeName == "u"){
                        Avenger->loadTypeFuncToHash(name, "s", constant1, constant2);
                        cout << ans << endl;
                    }
                    else{throw TypeMismatch(input);}
                }
                else if(countHook(value) == 0){
                    if(tenbien(value) == 0){throw InvalidInstruction(input);}
                    string typeValue = Avenger->takeVarType(value, constant1, constant2, ans);
                    if(typeValue == "Undeclared"){throw Undeclared(value);}
                    if(typeName == "Undeclared"){throw Undeclared(name);}
                    if(typeValue == "u" && typeName == "u"){throw TypeCannotBeInferred(input);}
                    else if ((typeValue == "n" || typeValue == "s") && typeName == "u"){
                        Avenger->loadTypeFuncToHash(name, typeValue, constant1, constant2);
                        cout << ans << endl;
                    }
                    else if (typeValue == "u" && (typeName == "n" || typeName == "s")){
                        Avenger->loadTypeFuncToHash(value, typeName, constant1, constant2);
                    }
                    else if (typeValue == typeName){cout << ans << endl;}
                    else{throw InvalidInstruction(input);}
                }
                else if (countHook(value) == 2){    // value = foo(x,y)
                    string nameFunc = funcName(value);      //nameFunc = foo
                    string typeFunc = Avenger->takeVarType(nameFunc, constant1, constant2, ans);
                    if(typeFunc == "Undeclared"){throw Undeclared(nameFunc);}
                    string supportToLoad = "";
                    if(DoyouHavePara(value) == 0){
                        if(typeName == "Undeclared"){throw Undeclared(name);}
                        if(typeFunc[1] == 'u' && typeName == "u"){throw TypeCannotBeInferred(input);}
                        else if((typeFunc[1] == 'n' || typeFunc[1] == 's') && typeName == "u"){
                            supportToLoad += typeFunc[1];
                            Avenger->loadTypeFuncToHash(name, supportToLoad, constant1, constant2);
                            cout << ans << endl;
                        }
                        else if(typeFunc[1] == 'u' && (typeName == "n" || typeName == "s")){
                            supportToLoad = "f";
                            supportToLoad += typeName;
                            Avenger->loadTypeFuncToHash(nameFunc, supportToLoad, constant1, constant2);
                            cout << ans << endl;
                        }
                        else if (typeFunc[1] == typeName[0]){cout << ans << endl;}
                        else{throw TypeMismatch(input);}
                    }
                    else {        //DoyouHavePara = 1
                        unsigned int NumberOfPara = countPond(value) + 1;
                        string* ArrOfInput = new string[NumberOfPara];
                        loadValueToArr(ArrOfInput, value, NumberOfPara);
                        for(unsigned int i = 0; i < NumberOfPara; i++){
                            if(checkString(ArrOfInput[i]) && checkNumber(ArrOfInput[i])){continue;}
                            else{
                                if(tenbien(ArrOfInput[i]) == 0){throw InvalidInstruction(input);}
                                else{continue;}
                            }
                        }
                        if(NumberOfPara != typeFunc.size()-2){throw TypeMismatch(input);}
                        int a = Avenger->LayKieuHamCuaInput(ArrOfInput, typeName, nameFunc, typeFunc, name, constant1, constant2, NumberOfPara, ans);
                        string b = ArrOfInput[0];
                        delete[] ArrOfInput;
                        if(a == 0){throw Undeclared(ArrOfInput[0]);}
                        if(a == 1){throw TypeMismatch(input);}
                        if(a == 2){throw TypeCannotBeInferred(input);}
                        if(a == 3){cout << ans << endl;}
                    }
                }
                else{throw InvalidInstruction(input);}
            }
            else if (getMethod(input) == "CALL" && countHook(input) == 2 && countSpace(input) == 1){
                int ans = 0;
                string value = getIdentifierForVar(input);
                if(countHook(value) == 0){throw InvalidInstruction(input);}
                string nameFunc = funcName(value);      //nameFunc = foo
                string typeFunc = Avenger->takeVarType(nameFunc, constant1, constant2, ans);
                if(typeFunc == "Undeclared"){throw Undeclared(nameFunc);}
                if(DoyouHavePara(value) == 0){
                    if(typeFunc[1] == 'u'){Avenger->loadTypeFuncToHash(nameFunc, "fu", constant1, constant2); cout << ans << endl;}
                    if(typeFunc[1] != 'v'){throw TypeMismatch(input);}
                }
                if(DoyouHavePara(value)){        //DoyouHavePara = 1
                    unsigned int NumberOfPara = countPond(value) + 1;
                    string* ArrOfInput = new string[NumberOfPara];
                    loadValueToArr(ArrOfInput, value, NumberOfPara);
                    for(unsigned int i = 0; i < NumberOfPara; i++){
                        if(checkString(ArrOfInput[i]) || checkNumber(ArrOfInput[i])){continue;}
                        else{
                            if(tenbien(ArrOfInput[i]) == 0){throw InvalidInstruction(input);}
                            else{continue;}
                        }
                    }
                    if(NumberOfPara != typeFunc.size()-2){throw TypeMismatch(input);}
                    int a = Avenger->XuLyChoCALL(ArrOfInput, nameFunc, typeFunc, constant1, constant2, NumberOfPara, ans);
                    string b = ArrOfInput[0];
                    delete[] ArrOfInput;
                    if(a == 0){throw Undeclared(b);}
                    if(a == 1){throw TypeMismatch(input);}
                    if(a == 2){throw TypeCannotBeInferred(input);}
                    if(a == 3){cout << Avenger->takeVarType(nameFunc, constant1, constant2, ans) << endl;}
                }
            }
            else if(input == "BEGIN"){
                countBegin = countBegin + 1;
            }
            else if(input == "END"){
                countEnd = countEnd + 1;
                if(countEnd > countBegin){throw UnknownBlock();}
                Avenger->DeleteScope(countBegin - countEnd + 1);
            }
            else if(getMethod(input) == "LOOKUP" && countSpace(input) == 1){
                string name = getIdentifierForVar(input);
                if(tenbien(name) == 0){throw InvalidInstruction(input);}
                int ans = Avenger->lookup(name, constant1, constant2);
                if(ans == -1){throw Undeclared(name);}
                cout << ans << endl;
            }
            else if(input == "PRINT"){
                string ans = Avenger->print();
                if(ans.size() == 0){continue;}
                else{cout << ans << endl;}
            }
            else{
                throw InvalidInstruction(input);
            }
        }
        readFile.close();
        if (countBegin != countEnd){throw UnclosedBlock(countBegin - countEnd);}
    }
    
    // Hash Table with double hashing
    else if(getMethod(input) == "DOUBLE" && countSpace(input) == 2){
        OpenHash* Avenger = new OpenHash(size, doubleHashing);
        while(!readFile.eof()){
            getline(readFile, input);
            if(getMethod(input) == "INSERT"){
                string name = "";
                if(countSpace(input) == 2){name = getIdentifierForFunc(input);}
                else{name = getIdentifierForVar(input);}
                if(tenbien(name) == 0){throw InvalidInstruction(input);}
                int level = countBegin - countEnd;
                string type = getType(input);
                if(Avenger->checkRedeclared(name, level)){throw Redeclared(name);}
                if(type[0] == 'f' && level != 0){throw InvalidDeclaration(input);}
                int ans = Avenger->insert(name, level, type, constant1, 0);
                if(ans == -1){throw Overflow(input);}
                cout << ans << endl;
            }
            else if (getMethod(input) == "ASSIGN" && countSpace(input) == 2){
                int ans = 0;
                // input = ASSIGN X Y
                string name = getIdentifierForFunc(input);     //name = X
                string typeName = Avenger->takeVarType(name, constant1, 0, ans); //typeName = u
              //  if(typeName == "Undeclared"){throw Undeclared(name);}      
                string value = valueOfAssign(input);            //value = y
                if(checkNumber(value)){
                    if(typeName == "Undeclared"){throw Undeclared(name);}
                    if(typeName == "n"){cout << ans << endl;}
                    else if(typeName == "u"){
                        Avenger->loadTypeFuncToHash(name, "n", constant1, 0);
                        cout << ans << endl;
                    }
                    else{throw TypeMismatch(input);}
                }
                else if(checkString(value)){
                    if(typeName == "Undeclared"){throw Undeclared(name);}
                    if(typeName == "s"){cout << ans << endl;}
                    else if(typeName == "u"){
                        Avenger->loadTypeFuncToHash(name, "s", constant1, 0);
                        cout << ans << endl;
                    }
                    else{throw TypeMismatch(input);}
                }
                else if(countHook(value) == 0){
                    if(tenbien(value) == 0){throw InvalidInstruction(input);}
                    string typeValue = Avenger->takeVarType(value, constant1, 0, ans);
                    if(typeValue == "Undeclared"){throw Undeclared(value);}
                    if(typeName == "Undeclared"){throw Undeclared(name);}
                    if(typeValue == "u" && typeName == "u"){throw TypeCannotBeInferred(input);}
                    else if ((typeValue == "n" || typeValue == "s") && typeName == "u"){
                        Avenger->loadTypeFuncToHash(name, typeValue, constant1, 0);
                        cout << ans << endl;
                    }
                    else if (typeValue == "u" && (typeName == "n" || typeName == "s")){
                        Avenger->loadTypeFuncToHash(value, typeName, constant1, 0);
                    }
                    else if (typeValue == typeName){cout << ans << endl;}
                    else{throw InvalidInstruction(input);}
                }
                else if (countHook(value) == 2){    // value = foo(x,y)
                    string nameFunc = funcName(value);      //nameFunc = foo
                    string typeFunc = Avenger->takeVarType(nameFunc, constant1, 0, ans);
                    if(typeFunc == "Undeclared"){throw Undeclared(nameFunc);}
                    string supportToLoad = "";
                    if(DoyouHavePara(value) == 0){
                        if(typeName == "Undeclared"){throw Undeclared(name);}
                        if(typeFunc[1] == 'u' && typeName == "u"){throw TypeCannotBeInferred(input);}
                        else if((typeFunc[1] == 'n' || typeFunc[1] == 's') && typeName == "u"){
                            supportToLoad += typeFunc[1];
                            Avenger->loadTypeFuncToHash(name, supportToLoad, constant1, 0);
                            cout << ans << endl;
                        }
                        else if(typeFunc[1] == 'u' && (typeName == "n" || typeName == "s")){
                            supportToLoad = "f";
                            supportToLoad += typeName;
                            Avenger->loadTypeFuncToHash(nameFunc, supportToLoad, constant1, 0);
                            cout << ans << endl;
                        }
                        else if (typeFunc[1] == typeName[0]){cout << ans << endl;}
                        else{throw TypeMismatch(input);}
                    }
                    else {        //DoyouHavePara = 1
                        unsigned int NumberOfPara = countPond(value) + 1;
                        string* ArrOfInput = new string[NumberOfPara];
                        loadValueToArr(ArrOfInput, value, NumberOfPara);
                        for(unsigned int i = 0; i < NumberOfPara; i++){
                            if(checkString(ArrOfInput[i]) || checkNumber(ArrOfInput[i])){continue;}
                            else{
                                if(tenbien(ArrOfInput[i]) == 0){throw InvalidInstruction(input);}
                                else{continue;}
                            }
                        }
                        if(NumberOfPara != typeFunc.size()-2){throw TypeMismatch(input);}
                        int a = Avenger->LayKieuHamCuaInput(ArrOfInput, typeName, nameFunc, typeFunc, name, constant1, 0, NumberOfPara, ans);
                        string b = ArrOfInput[0];
                        delete[] ArrOfInput;
                        if(a == 0){throw Undeclared(ArrOfInput[0]);}
                        if(a == 1){throw TypeMismatch(input);}
                        if(a == 2){throw TypeCannotBeInferred(input);}
                        if(a == 3){cout << ans << endl;}
                    }
                }
                else{throw InvalidInstruction(input);}
            }
            else if (getMethod(input) == "CALL" && countHook(input) == 2 && countSpace(input) == 1){
                int ans = 0;
                string value = getIdentifierForVar(input);
                if(countHook(value) == 0){throw InvalidInstruction(input);}
                string nameFunc = funcName(value);      //nameFunc = foo
                string typeFunc = Avenger->takeVarType(nameFunc, constant1, 0, ans);
                if(typeFunc == "Undeclared"){throw Undeclared(nameFunc);}
                if(DoyouHavePara(value) == 0){
                    if(typeFunc[1] == 'u'){Avenger->loadTypeFuncToHash(nameFunc, "fu", constant1, 0); cout << ans << endl;}
                    if(typeFunc[1] != 'v'){throw TypeMismatch(input);}
                }
                if(DoyouHavePara(value)){        //DoyouHavePara = 1
                    unsigned int NumberOfPara = countPond(value) + 1;
                    string* ArrOfInput = new string[NumberOfPara];
                    loadValueToArr(ArrOfInput, value, NumberOfPara);
                        for(unsigned int i = 0; i < NumberOfPara; i++){
                            if(checkString(ArrOfInput[i]) || checkNumber(ArrOfInput[i])){continue;}
                            else{
                                if(tenbien(ArrOfInput[i]) == 0){throw InvalidInstruction(input);}
                                else{continue;}
                            }
                        }
                    if(NumberOfPara != typeFunc.size()-2){throw TypeMismatch(input);}
                    int a = Avenger->XuLyChoCALL(ArrOfInput, nameFunc, typeFunc, constant1, 0, NumberOfPara, ans);
                    string b = ArrOfInput[0];
                    delete[] ArrOfInput;
                    if(a == 0){throw Undeclared(ArrOfInput[0]);}
                    if(a == 1){throw TypeMismatch(input);}
                    if(a == 2){throw TypeCannotBeInferred(input);}
                    if(a == 3){cout << Avenger->takeVarType(nameFunc, constant1, 0, ans) << endl;}
                }
            }
            else if(input == "BEGIN"){
                countBegin = countBegin + 1;
            }
            else if(input == "END"){
                countEnd = countEnd + 1;
                if(countEnd > countBegin){throw UnknownBlock();}
                Avenger->DeleteScope(countBegin - countEnd + 1);
            }
            else if(getMethod(input) == "LOOKUP" && countSpace(input) == 1){
                string name = getIdentifierForVar(input);
                if(tenbien(name) == 0){throw InvalidInstruction(input);}
                if(Avenger->lookup(name, constant1, 0) == -1){throw Undeclared(name);}
                cout << Avenger->lookup(name, constant1, 0) << endl;
            }
            else if(input == "PRINT"){
                string ans = Avenger->print();
                if(ans.size() == 0){continue;}
                else{cout << ans << endl;}
            }
            else{
                throw InvalidInstruction(input);
            }
        }
        readFile.close();
        if (countBegin != countEnd){throw UnclosedBlock(countBegin - countEnd);}
    }
    else{throw InvalidInstruction(input);}
    return;
}
