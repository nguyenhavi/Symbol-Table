#include "SymbolTable.h"


void SymbolTable::run(string filename)
{
   ifstream ifs(filename);
   string line;
   int indexAssign = 0;
   int indexInsert = 0;
   int countBegin = 0;
   int countEnd = 0;

   while (!ifs.eof()){
      getline(ifs, line);
      //INSERT
      if (line[0] == 'I' && countSpace(line) == 2){
         MyPower[indexInsert].name = Var_Name(line);
         MyPower[indexInsert].VarType = Var_Type(line);
         MyPower[indexInsert].levelBlock = countBegin - countEnd;
         if(checkRedeclared(MyPower, Var_Name(line), indexInsert)){cout << "success"<<endl;}
         else {
            throw Redeclared(line);
         }
         indexInsert++;
      }
      //ASSIGN
      else if (line[0] == 'A'){
         int levelAssign = countBegin-countEnd;
         MyPower[indexAssign].nameAssign = Var_Name_Assign(line);
         MyPower[indexAssign].Value = Var_Value(line);
         indexAssign++;
         if (checkUndeclared(MyPower, Var_Name_Assign(line),Var_Value(line), indexInsert, levelAssign) && TheLevelOfVariable(MyPower, Var_Name_Assign(line), indexInsert) <= levelAssign){
            if (checkTypeMiss(MyPower, Var_Value(line), Var_Name_Assign(line), indexInsert)){
               cout << "success"<<endl;
            }
            else {
               throw TypeMismatch(line);
            }
         }
         else {
            throw Undeclared(line);
         }
      }
      //BEGIN
      else if (line == "BEGIN"){
         countBegin += 1;
      }
      //END
      else if (line == "END"){
         int countDelete = 0;
         for (int i = 0; i < indexInsert; i++){
            if (MyPower[i].levelBlock == countBegin - countEnd){countDelete++;}            
         }
         if (countDelete >0){
         indexInsert -= countDelete;
         }
         countEnd += 1;
         if (countEnd > countBegin){throw UnknownBlock();}
      }
      //LOOKUP
      else if (line[0] == 'L' && countSpace(line) == 1){
         string VarLookup = VarNameLookUp(line);
         int levelLookup = countBegin - countEnd;
         if (checkUndeclaredLookup(MyPower, VarLookup, indexInsert) && levelLookup >= TheLevelOfVariable(MyPower, VarLookup, indexInsert)){
            cout << TheLevelOfVariable(MyPower, VarLookup, indexInsert) << endl;
         }
         else {
            throw Undeclared(line);
         }
      }
      //PRINT
      else if (line == "PRINT"){
         int levelOfPrint = countBegin-countEnd;
         int countVarCanCouts = 0;
         string ans ="";
            if (indexInsert == 1){
                  cout << MyPower[0].name <<"//"<< MyPower[0].levelBlock<< endl;
               }
            else if (indexInsert != 0){
            for (int i = 0; i < indexInsert-1; i++){
                  if(levelOfPrint >= MyPower[i].levelBlock && findMaxScope(MyPower, MyPower[i].name, indexInsert)==MyPower[i].levelBlock){
                  //cout << MyPower[i].name << "//" << MyPower[i].levelBlock <<"ABC";
                  ans += MyPower[i].name;
                  ans += "//";
                  ans += to_string(MyPower[i].levelBlock);
                  ans += ' ';
                  countVarCanCouts += 1;
               }
            }   
            //if (countVarCanCouts != 0){
               if (levelOfPrint >= MyPower[indexInsert-1].levelBlock){
                  //cout << MyPower[indexInsert-1].name <<"//"<< MyPower[indexInsert-1].levelBlock << endl;
                     ans += MyPower[indexInsert-1].name;
                     ans += "//";
                     ans += to_string(MyPower[indexInsert-1].levelBlock);
                     cout << ans << endl;
                  }
               else {
                  ans.pop_back();
                  cout << ans;
                  cout << endl;
               }
            
         }
      }
      //RPRINT
      else if (line == "RPRINT"){
         int levelOfRprint = countBegin - countEnd;
         int countVarCanCout = 0;
         string ans ="";
            if (indexInsert == 1){
               cout << MyPower[0].name <<"//"<< MyPower[0].levelBlock<< endl;
               }
            
            else if (indexInsert != 0){
            for (int i = indexInsert - 1; i > 0; i--){
                  if (levelOfRprint >= MyPower[i].levelBlock && findMaxScope(MyPower, MyPower[i].name, indexInsert)==MyPower[i].levelBlock){
                  //cout << MyPower[i].name << "//" << MyPower[i].levelBlock <<" ";
                  ans += MyPower[i].name;
                  ans += "//";
                  ans += to_string(MyPower[i].levelBlock);
                  ans += ' ';
                  countVarCanCout += 1;
               }
            }
            //if (countVarCanCout != 0){
               if (levelOfRprint >= MyPower[0].levelBlock && findMaxScope(MyPower, MyPower[0].name, indexInsert)==MyPower[0].levelBlock){
                  //cout << MyPower[0].name<< "//" << MyPower[0].levelBlock << endl;
                  ans += MyPower[0].name;
                  ans += "//";
                  ans += to_string(MyPower[0].levelBlock);
                  cout << ans << endl;
               }
               else {
                  ans.pop_back();
                  cout << ans;
                  cout << endl;
               }
            }         
      }
      else {
         throw InvalidInstruction(line);
      }
   }
   if (countBegin > countEnd){
      throw UnclosedBlock(countBegin-countEnd);
   }
   else if (countEnd > countBegin){
      throw UnknownBlock();
   }
   ifs.close();
   return;
}