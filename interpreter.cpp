#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include <map>

using namespace std;

map<string, int> table;
map<string, int> labels;
map<string, vector<int> > ArrayTable;
enum LEX_TYPE
{
	NUMBER,
	OPER,
	VAR, ARRAY
};

class Lexem
{
public:
	LEX_TYPE type;
	Lexem(){}
	int getLexType();
	virtual int getValue() = 0;
	virtual void print() = 0;
	virtual void setRow(int){return;}
	virtual int getRow(){return 0;}
	
};

int Lexem::getLexType()
{
	return type;
}
class Var: public Lexem
{
	string name;
public:
//	Variable (char nName);
	Var(string nName);
	int getValue();
	//void setValue (int value);
	string getName(){return name;}
	void print(){cout << name;}
	int getType(){return type;}

};

Var::Var(string nName)
{
	type = VAR;
	name = nName;
}
int Var::getValue()
{
	return table[name];
}
class Number: public Lexem
{
	int value ;
public:
	Number (int number);
	int getValue ();
	void print();
};

Number::Number(int number)
{
	value = number;
	type = NUMBER;
}

void Number::print()
{
	cout << value << " ";
}

int Number::getValue()
{
	return value;
}


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

int PRIORITY[] = {
        -1, -1,
        9, 9,
        10,
	0, 6, 6,
	7, 7,
	7, 7,
	1, 2,
	3,
	0, 0, 0, 0, 0, 0, 0,
	-1, -1, -1, 9
	
};

string OPERTEXT [] = 
{
	"(" , ")",
	"==",
	":=" ,"!=", 
	"<=", ">=",
	"<<", ">>",
	"or" ,
	"and" ,
	"|" ,
	"^" ,
	"&" ,
	"<" , ">" ,
	"+" , "-" ,
	"*" , "/" , "%", ":", "goto", "if", "else", "endif", "while", "endwhile",
	"[", "]", "array", "deref"
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
//	void getEq(string, int);
//	int getValue (const string&, int);
	void print();
	void setRow(int number);
	int getRow();
};

void Oper::setRow(int number)
{
	nStr = number;
}

int Oper::getRow()
{
	return nStr;
}
int Oper::getType()
{
        return opertype;
}

void Oper::print()
{
        switch (this->getType())
	{
		case PLUS: cout << "+ "; break;
		case MINUS: cout << "- "; break;
		case MULTIPLY: cout << "* "; break;
		case ASSIGN: cout << ":= "; break;
		case EQ: cout << "=="; break;
		case NEQ: cout << "!="; break;
		case LEQ: cout << "<="; break;
		case GEQ: cout << ">="; break;
		case LT: cout << "<"; break;
		case GT: cout << ">"; break;
		case OR: cout << "or"; break;
		case AND: cout << "and"; break;
		case BITOR: cout << "|"; break;
		case COLON: cout << ":"; break;
		case GOTO: cout << "goto"; break;
		case IF: cout << "if"; break;
		case ELSE: cout << "else"; break;
		case ENDIF: cout << "endif"; break;
		case WHILE: cout << "while"; break;
		case ENDWHILE: cout << "endwhile"; break;
		case DEREF: cout << "deref"; break;
		case ARR: cout << "array"; break;
        }
}

int Oper::getValue(int left = 1, int right = 1)
{
	switch (this->getType())
	{
		case PLUS: return left + right;
		case MINUS: return left - right;
		case MULTIPLY: return left * right;
		case EQ: return left == right;
		case NEQ: return !(left == right);
		case LEQ: return left <= right;
		case GEQ: return left >= right;
		case LT: return left < right;
		case GT: return left > right;
		case OR: return left or right;
		case AND: return left and right;
		case BITOR: return left | right;
	}
	return 0;

}

void Oper::getEq(string name, int val)
{
	table[name] = val;
	return ;
}

void Oper::getArray(string name, int index, int val)
{
	ArrayTable[name][index] = val;
}

Oper::Oper(string oper)
{
	type = OPER;
	nStr = 0;
	if (oper == "+")
		opertype = PLUS;
	if (oper == "-")
		opertype = MINUS;
	if (oper == "*")
		opertype = MULTIPLY;
	if (oper == "(")
		opertype = LBRACKET;
	if (oper == ")")
		opertype = RBRACKET;
	if (oper == ":=")
		opertype = ASSIGN;
	if (oper == "==")
		opertype = EQ;
	if (oper == "!=")
		opertype = NEQ;
	if (oper == "<=")
		opertype = LEQ;
	if (oper == ">=")
		opertype = GEQ;
	if (oper == "<")
		opertype = LT;
	if (oper == ">")
		opertype = GT;
	if (oper == "or")
		opertype = OR;
	if (oper == "and")
		opertype = AND;
	if (oper == "|")
		opertype = BITOR;
	if (oper == ":")
		opertype = COLON;
	if (oper == "goto")
		opertype = GOTO;
	if (oper == "if")
		opertype = IF;
	if (oper == "else")
		opertype = ELSE;
	if (oper == "endif")
		opertype = ENDIF;
	if (oper == "while")
		opertype = WHILE;
	if (oper == "endwhile")
		opertype = ENDWHILE;
	if (oper == "[")
		opertype =  LQBRACKET;
	if (oper == "]")
		opertype =  RQBRACKET;
	if (oper == "deref")
		opertype = DEREF;
	if (oper == "array")
		opertype = ARR;
}

int Oper::getPriority()
{
	return PRIORITY[opertype];
}

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

Array::Array(string nName, int nIndex)
{
	name = nName;
	index = nIndex;
	type = ARRAY;

}
void Array::mall()

{
	ArrayTable[name] = vector<int>(index + 1);
	ArrayTable[name][index] = 0;
}

int Array::getValue()
{
	return ArrayTable[name][index];
}

vector <Lexem* > parseLexem(string codeline)
{
	vector <Lexem* > infix;
	int number = 0;
	int j, flag_n = 0, flag_op = 0;
	string notNum = "";
	for (int i = 0; i < (int)codeline.size(); i++)
	{
		for (j = i; codeline[j] >= '0' && codeline[j] <= '9'; j++)
		{
			number = number * 10 + (codeline[j] - '0');
			flag_n = 1;
		}
		if (flag_n == 1)
		{
			Number *num = new Number(number);
			infix.push_back(num);
			i = j - 1;
			number = 0;
			flag_n = 0;
			continue;
		}
	
		int n = sizeof(OPERTEXT) / sizeof(string);
		for ( int op = 0; op < n ; op ++)
		{
			string subcodeline = codeline.substr(i, OPERTEXT[op].size());
			if(OPERTEXT [op] == subcodeline ) 
			{	
				infix.push_back(new Oper (OPERTEXT[op]));
				i += OPERTEXT[op].size() - 1 ;
				flag_op = 1;
 				break;
			}
		}
		if (flag_op == 1)
		{
			flag_op = 0;
			continue;	
		}
		if (codeline[i] != ' ' && codeline[i] != '\n')
		{	
//		cout << "YES" << endl;
			while ((((int)codeline[i] >= 'a' && codeline[i] <= 'z') || (codeline[i] >= 'A' && codeline[i] <= 'Z')) && i < (int)codeline.size())
			{
				notNum += codeline[i];
				i++;
			}
			if (notNum != "")
			{
				infix.push_back(new Var(notNum));
//				cout << "!" << notNum << "!";
				i--;
			}	
			notNum = "";
			flag_op = 0;
		}

	}
	return infix;


}

void initLabels(vector <Lexem *> infix , int row)
{
	for (int i = 1; i < (int)infix.size(); i++)
	{
		if (infix[i - 1]->getLexType() == VAR && infix[i]->getLexType() == OPER)
		{
//			cout << "YES" << endl;
			Var * lexemvar = (Var*)infix[i - 1];
			Oper * lexemop = (Oper*)infix[i];
			if (lexemop->getType() == COLON) 
			{
				labels [lexemvar->getName()] = row + 1;

			//	cout << "    ! " << labels [lexemvar->getName()] << endl;

				delete infix[i - 1];
				delete infix[i];
				infix[i - 1] = nullptr;
				infix[i] = nullptr;
				i++;
			}
		}
	}
}

void initJumps(vector < vector<Lexem* >> infixLines)
{
	stack<Lexem *> stackIfElse;
	stack<Lexem *> stackWhile;
	for (int row = 0; row < (int)infixLines.size(); row++)
	{
		for (int i = 0; i < (int)infixLines[row].size(); i++)
		{
			if (infixLines[row][i]->getLexType() == OPER)
			{
				if(((Oper *)(infixLines[row][i]))->getType() == IF)
					stackIfElse.push(infixLines[row][i]);
				if(((Oper *)(infixLines[row][i]))->getType() == ELSE)
				{
					stackIfElse.top()->setRow(row + 1);
					stackIfElse.pop();
					stackIfElse.push(infixLines[row][i]);
				}
				if(((Oper *)(infixLines[row][i]))->getType() == ENDIF)
				{
					stackIfElse.top()->setRow(row + 1);
					stackIfElse.pop();
		
				}
	
				if(((Oper *)(infixLines[row][i]))->getType() == WHILE)
				{
					(infixLines[row][i])->setRow(row);
					stackWhile.push(infixLines[row][i]);
				}
	
				if(((Oper *)(infixLines[row][i]))->getType() == ENDWHILE)
				{
					infixLines[row][i]->setRow(stackWhile.top()->getRow());
					stackWhile.top()->setRow(row + 1);
					stackWhile.pop();
				}
			       
			}
		}
	}
}

vector<Lexem *> buildPostfix(vector<Lexem *> infix)
{
        int i, j;
        stack<Oper *> opstack;
        vector<Lexem *> postfix;
        for (i = 0; i < (int)infix.size(); i++)
	{
		if (infix[i] == nullptr)
			continue;
                if (infix[i]->getLexType() == NUMBER)
		{
                        postfix.push_back(infix[i]);
                }
		if (infix[i]->getLexType() == VAR)
		{
			Var* lexemvar = (Var*)infix[i];
			if (labels[lexemvar->getName()] != 0)
			{
				Oper *gt = (Oper*)opstack.top();
				opstack.pop();
				gt->setRow(labels[lexemvar->getName()]);
				opstack.push(gt);
			//	joinGotoAndLabel(lexemvar, opstack);
			}
			else 
                        	postfix.push_back(infix[i]);

		}
                if (infix[i]->getLexType() == OPER)
		{
			Oper *oper = static_cast <Oper* >(infix[i]);
                        if (oper->getType() == LBRACKET || oper->getType() == LQBRACKET)
			{
                                opstack.push((oper));
                        } else
			{
//				cout <<"1" << endl;
				if (oper->getType() == RBRACKET || oper->getType() == RQBRACKET)
				{
					for (j = opstack.size(); j > 0 && opstack.top()->getType() != LBRACKET && opstack.top()->getType() != LQBRACKET; j--)
					{
                                                postfix.push_back(opstack.top());
                                                opstack.pop();
					}
					if (oper->getType() == RQBRACKET)
					{	
						postfix.push_back(new Oper("deref"));
					}
                                	opstack.pop();
				} else {
        	                	while (!opstack.empty() && opstack.top()->getPriority() >= oper->getPriority())
					{
                        	                        postfix.push_back(opstack.top());
                                	                opstack.pop();
					}
                                    			opstack.push((Oper *)infix[i]);
	                                }
                        }
                }
        }
        for (i = opstack.size(); i > 0; i--)
	{
                postfix.push_back(opstack.top());
                opstack.pop();
        }
        return postfix;
}

int evaluatePostfix(vector<Lexem *> poliz, int row)
{
        int value, left, right;
	string name;
        stack<Lexem*> computationStack;
        for (int i = 0; i < (int)poliz.size(); i++)
	{
                if (poliz[i]->getLexType() == NUMBER || poliz[i]->getLexType() == VAR)
		{
                        computationStack.push(poliz[i]);
                }
                if (poliz[i]->getLexType() == OPER)
		{
			Oper *oper = static_cast <Oper* >(poliz[i]);
			if (oper->getType() == ARR || oper->getType() == DEREF) 
			{
				right = (computationStack.top())->getValue();
	                        computationStack.pop();
				name = ((Var *)computationStack.top())->getName();
				computationStack.pop();

				Array *arr = (new Array(name, right));

				if (oper->getType() == ARR)
				{
					arr->mall();
                      		computationStack.push(arr);
			           //	continue;
				   	return row + 1;
				}
                      		computationStack.push(arr);
				continue;
				

			}
			if (oper->getType() == GOTO)
				return oper->getRow();
			if (oper->getType() == IF || oper->getType() == WHILE)
			{
				if(computationStack.top()->getValue() == 0)
				{
					computationStack.pop();
					return oper->getRow();
				}

//					cout << "YES" << endl;
				return row + 1;
				
			}
			if (oper->getType() == ELSE || oper->getType() == ENDWHILE)
				return oper->getRow();
			if (oper->getType() == ENDIF)
				return row + 1;

                        right = (computationStack.top())->getValue();
                       	computationStack.pop();

			if (oper->getType() != ASSIGN)
			{
        	       	        left = (computationStack.top())->getValue();
	                        computationStack.pop();
        	                value = ((Oper *)poliz[i])->getValue(left, right);
	               	        Number *val = new Number(value);
                       		computationStack.push(val);

      			}	


			if (oper->getType() == ASSIGN)
			{

//			cout << "1" << endl;
				name = ((Var *)computationStack.top())->getName();

				if (computationStack.top()->getLexType() == ARRAY)
				{

					int index = ((Array*)computationStack.top())->getIndex();
				//	cout << name << " " << index << " " << right << endl;
					((Oper *)poliz[i])->getArray(name, index, right);
					
			//cout << "1" << endl;

					return row + 1;
				}
				
				oper->getEq(name, right);
				Number *val = new Number(value);
//				computationStack.push(val);
				return row + 1;
			}

        	}
	}
	cout << computationStack.top()->getValue() << endl;
	
	computationStack.pop();
	return row + 1;
}

int main()
{
	string codeline;
	vector<vector <Lexem *> > postfixLines, infixLines;
	int row;
	while (getline(cin, codeline))
		infixLines.push_back (parseLexem(codeline));
/*	for (int i = 0; i < infixLines.size(); i++)
	{
		for (int j = 0; j < infixLines[i].size(); j++)
		{
//			cout << "[";
			infixLines[i][j]->print();
			if (infixLines[i][j]->getType() == ENDIF)
				cout << "!" << endl;
//				cout << postfixLines[i][j]->getRow() << endl;
//			cout << "]";
		}

		cout << endl;

	}
*/
//	(new Oper ("deref"))->print();
	for (row = 0; row < (int)infixLines.size(); row++)
		initLabels(infixLines[row], row);
	initJumps(infixLines);
	for (row = 0; row < (int)infixLines.size(); row++)
		postfixLines.push_back (buildPostfix(infixLines[row]));
	row = 0;
	for (int i = 0; i < (int)postfixLines.size(); i++)
	{
		for (int j = 0; j < (int)postfixLines[i].size(); j++)
		{
			cout << "[";
			postfixLines[i][j]->print();
//				cout << postfixLines[i][j]->getRow() << endl;
			cout << "]";
		}

		cout << endl;

	}
	while (row < (int)postfixLines.size())
	{
		row = evaluatePostfix(postfixLines[row], row);
	}


/*		for (int i = 0; i < postfix.size(); i++)
		{
                        postfix[i]->print();
			cout << endl;
  		}
*/	/*	for (int i = 0; i < infix.size(); i++)
		{
                        infix[i]->print();
			cout << endl;
                }

*/

//			cout << infix.size();
//		ans = evaluatePostfix(postfix);
//		cout << "ans = " << ans << endl;


	return 0;
}

