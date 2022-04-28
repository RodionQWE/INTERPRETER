
#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include <map>

using namespace std;

extern map<string, int> table;
extern map<string, int> labels;
extern map<string, vector<int> > ArrayTable;

enum LEX_TYPE
{
        NUMBER,
        OPER,
        VAR, ARRAY
};


enum OPERATOR
{
        LBRACKET, RBRACKET,
        PLUS, MINUS,
        MULTIPLY,
        ASSIGN, EQ, NEQ,
        LEQ, LT,
        GEQ, GT,
        OR, AND,
        BITOR,
        COLON, GOTO, IF, ELSE, ENDIF, WHILE, ENDWHILE,
        LQBRACKET, RQBRACKET, DEREF, ARR
};

class Lexem
{
public:
        LEX_TYPE type;
        Lexem(){}
        int getLexType();
        virtual int getValue() = 0;
        virtual void print() = 0;
};
class Var: public Lexem
{
        string name;
public:
//      Variable (char nName);
        Var(string nName);
        int getValue();
        //void setValue (int value);
        string getName(){return name;}
        void print(){cout << name;}
        int getType(){return type;}

};

class Number: public Lexem
{
        int value ;
public:
        Number (int number);
        int getValue ();
        void print();
};


class Oper: public Lexem
{
        OPERATOR opertype;
        int nStr;
public:
        Oper(string oper);
        int getPriority();
        int getType();
        int getValue(){return 0;}
        int getValue (int left, int right);
        void getEq (string name, int val);
        void getArray (string name, int index, int val);
//      void getEq(string, int);
//      int getValue (const string&, int);
        void print();
        void setRow(int number);
        int getRow();
};

class Array: public Lexem
{
        string name;
        int index;
public:

        Array(string nName, int nIndex);
        int getValue();
        int setValue(int num);
        string getName(){return name;}
        int getIndex(){return index;}
        void mall();
        void print(){cout << name;}
};

vector <Lexem *> getNum(int *i, string codeline, int *flag_n, int *j, vector <Lexem *> infix);

vector <Lexem *> getOper(vector <Lexem *> infix, int *i, string codeline, int *flag_op);

vector <Lexem *> getVar(vector <Lexem *> infix, int *i, string codeline);

vector <Lexem* > parseLexem(string codeline);

void initLabels(vector <Lexem *> infix , int row);

void initJumps(vector < vector<Lexem* >> infixLines);

stack <Oper *> pushVar(stack <Oper *> opstack, Var* lexemvar);

vector<Lexem *> buildPostfix(vector<Lexem *> infix);

stack<Lexem *> makeArr(stack<Lexem *> computationStack);

stack<Lexem *> notA(stack<Lexem *> computationStack, vector<Lexem *> poliz, int i, int right);

int getG(stack<Lexem *> computationStack, vector <Lexem *> poliz, int i, int right, int row);

int evaluatePostfix(vector<Lexem *> poliz, int row);

