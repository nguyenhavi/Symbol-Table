#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable{
public:
    SymbolTable(){}
    void run (string fileName);
};

struct ArrayData{
    string name;
    int level;
    string type;
};
ArrayData* arr = new ArrayData[20];

class Node{
public:
    string indentifier;
    string varType;
    int levelBlock;
    bool state;
    Node* left;
    Node* right;
    Node* parent;
    Node(string indentifier, string varType, int levelBlock, bool state){
        this->indentifier = indentifier;
        this->varType = varType;
        this->levelBlock = levelBlock;
        this->state = state;
        this->left = NULL;
        this->right = NULL;
        this->parent = NULL;
    }
};

class SplayTree{
private:
    Node* root;
protected:
    void splay(Node*, int&);
    void preOrderRec(Node*);
    void rotateLeft(Node* );
    void rotateRight(Node* );
    void deleteNodeHelp(Node*, string, int);
    void deleteTreeRec(Node*&);
    void split(Node*&,Node*&,Node*&);
    Node* join(Node*, Node*);
    Node* findRec(Node*, string, int, int&);
    Node* maxLeftTree(Node* node);
public:
    SplayTree(){
        this->root = NULL;
    }
    ~SplayTree(){
        deleteTree();
    }
    bool insert(string, string, bool, int, int&, int&);
    void del(string, int);
    int find(string, int, int&);
    string findVarType(string, int, int&, int&);
    void preOrder();
    void deleteTree();
    int getSize();
};

void SplayTree::preOrderRec(Node* node){
    if (node != NULL){
        if(node == this->root){
            cout << node->indentifier << "//" << node->levelBlock;
        }
        else{
            cout << " " << node->indentifier << "//" << node->levelBlock;
        }
        preOrderRec(node->left);
        preOrderRec(node->right);
    }
    return;
}

void SplayTree::preOrder(){
    preOrderRec(root);
}

void SplayTree::rotateRight(Node* node){
    Node* y = node->left;
    node->left = y->right;
    if (y->right != NULL){
        y->right->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == NULL){
        this->root = y;
    }
    else if (node == node->parent->right){
        node->parent->right =y;
    }
    else {
        node->parent->left = y;
    }
    y->right = node;
    node->parent = y;
}

void SplayTree::rotateLeft(Node* node){
    Node* y = node->right;
    node->right = y->left;
    if (y->left != NULL){
        y->left->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == NULL){
        //use for splay zig and splay zag
        this->root = y;
    }
    else if (node == node->parent->left){
        node->parent->left = y;
    }
    else {node->parent->right = y;}
    y->left = node;
    node->parent = y;
}

void SplayTree::splay(Node* node, int& num_splay){
    if(node != this->root){num_splay = num_splay + 1;}
    while (node != this->root){
        if (node->parent->parent == NULL){  
            // Node need to splay dont have grand parent
            if (node == node->parent->left){
                rotateRight(node->parent);      //splay Zig
            }
            else {
                rotateLeft(node->parent);       //splay Zag
            }
        }
        else if (node == node->parent->left && node->parent == node->parent->parent->left){
            // splay Zig-Zig
            rotateRight(node->parent->parent);
            rotateRight(node->parent);
        }
        else if (node == node->parent->right && node->parent == node->parent->parent->right){
            //splay Zag-Zag
            rotateLeft(node->parent->parent);
            rotateLeft(node->parent);
        }
        else if (node == node->parent->right && node->parent == node->parent->parent->left){
            // splay Zig-Ztag
            rotateLeft(node->parent);
            rotateRight(node->parent);
        }
        else {
            // splay Zag-Zig
            rotateRight(node->parent);
            rotateLeft(node->parent);
        }
    }
}

void SplayTree::deleteNodeHelp(Node* node, string name, int level){
    Node* x = NULL;
    Node* t = NULL;
    Node* s = NULL;
    while(node!= NULL){
        if(node->levelBlock == level){
            if(node->indentifier == name){
                x = node;
                break;
            }
            else if(node->indentifier < name){
                node = node->right;
            }
            else if (node->indentifier > name){
                node = node->left;
            }
        }
        else if(node->levelBlock < level){
            node = node->right;
        }
        else if (node->levelBlock > level){
            node = node->left;
        }
    }

    split(x, s, t);
    if(s->left){
        s->left->parent = NULL;
    }
    root = join(s->left, t);
    delete(s);
    s = NULL;
}

void SplayTree::split(Node* &x, Node* &s, Node* &t){
    int b = 0;
    splay(x,b);
    if (x->right == NULL){
        t = NULL;
    }
    else{
        t = x->right;
        t->parent = NULL;
    }
    s = x;
    s->right = NULL;
    x = NULL;
}

Node* SplayTree::join(Node* s, Node* t){
    if (!s){
        return t;
    }

    if(!t){
        return s;
    }
    Node* x = maxLeftTree(s);
    int b = 0;
    splay(x,b);
    x->right = t;
    t->parent = x;
    return x;
}

Node* SplayTree::maxLeftTree(Node* node){
    while(node->right != NULL){
        node = node->right;
    }
    return node;
}

bool SplayTree::insert(string indentifier, string varType, bool state, int levelBlock, int& num_comp, int& num_splay){
    Node* node = new Node(indentifier, varType, levelBlock, state);
    Node* y = NULL;
    Node* x = this->root;
    while(x != NULL){
        y = x;
        if (node->levelBlock < x->levelBlock){
            num_comp++;
            x = x->left;
        }
        else if(node->levelBlock > x->levelBlock){
            num_comp++;
            x = x->right;
        }
        else if(node->levelBlock == x->levelBlock){
            if(node->indentifier < x->indentifier){
                num_comp++;
                x = x->left;
            }
            else if(node->indentifier > x->indentifier){
                num_comp++;
                x = x->right;
            }
            else {return 0;}
        }
    }
    node->parent = y;
    if (y == NULL){
        root = node;
    }
    else if(node->levelBlock < y->levelBlock){
        y->left = node;
    }
    else if (node->levelBlock > y->levelBlock){
        y->right = node;
    }
    else{
        if(node->indentifier < y->indentifier){
            y->left = node;
        }
        else if (node->indentifier > y->indentifier){
            y->right = node;
        }
        else{return 0;}
    }
    splay(node, num_splay);
    return 1;
}

Node* SplayTree::findRec(Node* node, string name, int level, int& num_comp){
    if (node == NULL){return NULL;}
    if(level == -1){return NULL;}
    while(node != NULL){
        if(node->levelBlock == level){
            num_comp = num_comp + 1;
            if(node->indentifier == name){
                return node;
            }
            else if (node->indentifier < name){
                node = node->right;
            }
            else if(node->indentifier > name){
                node = node->left;
            }
        }
        else if (node->levelBlock < level){
            num_comp = num_comp + 1;
            node = node->right;
        }   
        else if(node->levelBlock > level){
            num_comp = num_comp + 1;
            node = node->left;
        }
    }
    return node;
}

int SplayTree::find(string indentifier, int countBegin, int& num_comp){
    Node* a = findRec(root, indentifier, countBegin, num_comp);
    if(a == NULL){return -1;}
    int b =0;
    splay(a,b);
    return a->levelBlock;
}

string SplayTree::findVarType(string indentifier, int level, int& num_comp, int& num_splay){
    Node* a = findRec(root, indentifier, level, num_comp);
    if(a == NULL){return "none";}
    splay(a, num_splay);
    string ans = a->varType;
    return ans;
}

void SplayTree::del(string name, int level){
    deleteNodeHelp(this->root, name, level);
}

string takeVarName(string input){
    string ans = "";
    for(int i = 7; input[i] != ' '; i++){
        ans += input[i];
    }
    return ans;
}

string takeNameLookup(string input){
    string ans = "";
    for(unsigned int i = 7; i < input.size(); i++){
        ans += input[i];
    }
    return ans;
}

string takeVarType(string input){
    string ans = "";
    int support = 0;
    for (int i = 7; input[i] != ' '; i++){
        support = i;
    }
    for (int i = support + 2; input[i] != ' '; i++){
        ans += input[i];  //(number,number)->number
    }
    if (ans == "number"){return "n";}
    else if(ans == "string"){return "s";}
    if(ans[0] == '(' && ans[1] == ')'){
        if(ans[ans.size()-1] == 'r'){return "fn";}
        return "fs";
    }
    string reallyAns ="f";    
    if(ans[1] == 'n'){reallyAns += 'n';}
    else {reallyAns += 's';}
    for(int i = 1; ans[i] != ')'; i++){
        if(ans[i] == ','){
            reallyAns += ans[i+1];
        }
    }
    if(ans[ans.size()-1] == 'r'){reallyAns += 'n';}
    else{reallyAns += 's';}
    return reallyAns;
}

bool takeState(string input){
    if (input[input.size()-2] == 's'){return 0;}
    return 1;
}

string takeVarValue(string input){
    string ans = "";
    int support = 0;
    for (int i = 7; input[i] != ' '; i++){
        support = i;
    }
    for (unsigned int i = support + 2; i < input.size(); i++){
        ans += input[i];
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

int level(ArrayData* a, string name,int countArr){
    int level = -1;
    for(int i = 0; i < countArr; i++){
        if(a[i].name == name){level = a[i].level;}
    }
    return level;
}

bool HavePara(string value){
    if (value[value.size()-1] == ')' && value[value.size()-2] == '('){return 0;}
    return 1;
}

string takeFuncName(string value){
    string ans = "";
    for(int i = 0; value[i] != '('; i++){
        ans += value[i];
    }
    return ans;
}

int countPond(string value){
    int ans = 0;
    for(unsigned int i = 0; i < value.size(); i++){
        if(value[i] == ','){ans = ans + 1;}
    }
    return ans;
}

void delArr(ArrayData* &arr, int& countArr, int pos){
    for(int i = pos; i < countArr; i++){
        arr[i].name = arr[i+1].name;
        arr[i].level = arr[i+1].level;
    }
    countArr--;
    return;
}

void SplayTree::deleteTreeRec(Node*& node){
    if(node != NULL){
        deleteTreeRec(node->left);
        deleteTreeRec(node->right);
        delete(node);
        node = NULL;
    }
}

void SplayTree::deleteTree(){
    deleteTreeRec(this->root);
}

int countSpace(string input){
    int ans = 0;
    for(unsigned int i = 0; i < input.size(); i++){
        if(input[i] == ' '){ans++;}
    }
    return ans;
}
#endif