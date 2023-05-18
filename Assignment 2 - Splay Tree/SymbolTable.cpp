#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    SplayTree IronMan;
    int countArr = 0;
    ifstream instructionCurrent;
    instructionCurrent.open(filename);
    int countBegin = 0;
    int countEnd = 0;
    while (!instructionCurrent.eof()){
        string inputInstruction = "";
        getline(instructionCurrent, inputInstruction);
        if (inputInstruction[0] == 'I' && inputInstruction[3] == 'E' && countSpace(inputInstruction) == 3){
            int num_splay = 0;
            int num_comp = 0;
            string name = takeVarName(inputInstruction);
            bool status = takeState(inputInstruction);
            arr[countArr].name = name;
            int level = -1;
            if (status == 1){level = 0;}
            else{level = countBegin-countEnd;}
            arr[countArr].level = level;
            string type = takeVarType(inputInstruction);
            arr[countArr].type = type;
            countArr++;
            if (!IronMan.insert(name, type, status, level, num_comp, num_splay)){delete[] arr; throw Redeclared(inputInstruction);}
            if (level != 0 && type[0] == 'f'){delete[] arr; throw InvalidDeclaration(inputInstruction);}
            cout << num_comp << " " << num_splay << endl;
        }
        else if (inputInstruction[0] == 'A' && inputInstruction[3] == 'I'){
           int num_comp = 0;
           int num_splay = 0;
           string name = takeVarName(inputInstruction);
           string value = takeVarValue(inputInstruction);

           if(checkString(value) || checkNumber(value)){
                int levelName = level(arr, name, countArr);
                if(levelName == -1){delete[] arr; throw Undeclared(inputInstruction);}
                string type1 = IronMan.findVarType(name, levelName, num_comp, num_splay);
                if(checkNumber(value) && type1 == "n"){}
                else if(checkString(value) && type1 == "s"){}
                else{delete[] arr; throw TypeMismatch(inputInstruction);}
                cout << num_comp << " " << num_splay << endl;
           }

            bool MayPhaiLaHamKo = (value.find('(') > 0 && value.find('(') < value.size()) && (value.find(')') > 0 && value.find(')') < value.size());
            if(MayPhaiLaHamKo == false){
                int levelName = level(arr, name, countArr);
                int levelValue = level(arr, value, countArr);
                if(levelValue == -1){delete[] arr; throw Undeclared(inputInstruction);}
                string type2 = IronMan.findVarType(value, levelValue, num_comp, num_splay);
                if(levelName == -1){delete[] arr; throw Undeclared(inputInstruction);}
                string type1 = IronMan.findVarType(name, levelName, num_comp, num_splay);
                if(type1 != type2){delete[] arr; throw TypeMismatch(inputInstruction);}
                cout << num_comp << " " << num_splay << endl;
            } 

            if(MayPhaiLaHamKo == true){
                bool MayCoThamSoKhong = HavePara(value);
                if(MayCoThamSoKhong == false){
                    string nameFunc = takeFuncName(value);
                    string type2 = IronMan.findVarType(nameFunc, 0, num_comp, num_splay);
                    if(type2 == "none"){delete[] arr; throw Undeclared(inputInstruction);}
                    int levelName = level(arr, name, countArr);
                    if(levelName == -1){delete[] arr; throw Undeclared(inputInstruction);}
                    string type1 = IronMan.findVarType(name, levelName, num_comp, num_splay);
                    if(type1[0] != type2[1]){delete[] arr; throw TypeMismatch(inputInstruction);}
                    cout << num_comp << " " << num_splay << endl;
                }
                //Thi ra may co tham so
                else{
                    //ASSIGN x sum(1,1)
                    int support = 0;
                    int pond = countPond(value);
                    string typeFunc = "f";
                    for(int i = 0; value[i] != '('; i++){
                        support = i;
                    }
                    string* Array = new string[pond+1];
                    for(int i = 0; i < pond + 1; i++){
                        support = support + 1;  
                        for(int j = support + 1; value[j] != ',' && value[j] != ')'; j++){
                            Array[i] += value[j];    
                            support = j;
                        }
                    }
                    string nameFunc = takeFuncName(value);
                    string type2 = IronMan.findVarType(nameFunc, 0, num_comp, num_splay);
                    if(type2 == "none"){delete[] arr; delete[] Array; throw Undeclared(inputInstruction);}
                    for(int i = 0; i <= pond; i++){
                        if((Array[i][0] >= '0' && Array[i][0] <= '9') || Array[i][0] == 39){
                            //may la hang va chuoi
                            if(checkNumber(Array[i])){typeFunc += "n";}
                            if(checkString(Array[i])){typeFunc += "s";}
                        }
                        else{
                            int levelVar = level(arr, Array[i], countArr);
                            if(levelVar == -1){delete[] arr;delete[] Array; throw Undeclared(inputInstruction);}
                            string typeV = IronMan.findVarType(Array[i], levelVar, num_comp, num_splay);
                            typeFunc += typeV;
                            //may la bien
                        }
                    }
                    int countSame = 0;
                    for(unsigned int i = 0; i < typeFunc.size(); i++){
                        if(typeFunc[i] != type2[i]){countSame++;}
                    }
                    if(countSame != 0){delete[] arr;delete[] Array; throw TypeMismatch(inputInstruction);}
                    int levelName = level(arr, name, countArr);
                    if(levelName == -1){delete[] arr;delete[] Array; throw Undeclared(inputInstruction);}
                    string type1 = IronMan.findVarType(name, levelName, num_comp, num_splay);
                    if(type1[0] == type2[type2.size()-1]){cout << num_comp << " " << num_splay << endl;}
                    else{delete[] arr; delete[] Array;throw TypeMismatch(inputInstruction);}
                    delete[] Array;
                    //Dem dau phay
                    //so tham so bang so dau phay + 1
                    //Tao mang chuoi roi cho vong lap
                }
            }
        }
        else if (inputInstruction[0] == 'L' && inputInstruction[2] == 'O' && countSpace(inputInstruction) == 1){
            string varName = takeNameLookup(inputInstruction);
            int level = -1;
            for(int i = 0; i < countArr; i++){
                if(arr[i].name == varName){level = arr[i].level;}
            }
            if (level == -1){delete[] arr; throw Undeclared(inputInstruction);}
            int a = 0;
            cout << IronMan.find(varName,level, a) << endl; 
        }
        else if (inputInstruction == "BEGIN"){
            countBegin++;
        }
        else if (inputInstruction == "END"){
            countEnd++;
            if (countBegin < countEnd){
                delete[] arr;
                throw UnknownBlock();}
            int index = 0;
            while(1){
                if(arr[index].level == countBegin-countEnd+1){
                    IronMan.del(arr[index].name, countBegin-countEnd+1);
                }
                index = index + 1;
                if(index >= countArr){break;}
            }
            //Xoa mang
            int countVar = 0;
            while (countVar < countArr){
                if(arr[countVar].level == countBegin-countEnd+1){
                    delArr(arr, countArr,countVar);
                    countVar = 0;
                }
                countVar = countVar + 1;
            }
        }
        else if (inputInstruction == "PRINT"){
            if(countArr > 0){
                IronMan.preOrder();
                cout << endl;
            }
        }
        else{
            throw InvalidInstruction(inputInstruction);
        }
    }
    if (countEnd < countBegin){delete[] arr; throw UnclosedBlock(countBegin-countEnd);}
    instructionCurrent.close();
    delete[] arr;
    IronMan.deleteTree();
    return;
}
