#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include <map>

using namespace std;


map<string, int> table;
map<string, int> labels;

enum LEX_TYPE
{
	NUMBER,
	OPER,
	VAR
};

class Lexem
{
public:
	LEX_TYPE type;
	Lexem(){}
	int getLexType();
	virtual int getType(){return 0;}
	virtual int getPriority(){return 0;}
//	virtual int getValue(int, int){return 0;}
	virtual int getValue(){return 0;}
	virtual int getEv(int, int){return 0;}
	virtual void getEq(string, int){return;}
	virtual void print(){return;}
	virtual string getName(){return 0;}
	virtual void setRow(int){return;}
	virtual int getRow(){return 0;}
	
};

int Lexem::getLexType()
{
	return type;
}

/*class NumVar: public Lexem
{
public:
	int getValue();
};

*/
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

//Variable::Variable(char nName)
//{
//	name = nName;
//	type = VAR;	
//}

	
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
	COLON, GOTO, IF, ELSE, ENDIF
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
	0, 0, 0, 0, 0
	
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
"*" , "/" , "%", ":", "goto", "if", "else", "endif"
};
class Oper: public Lexem
{
	OPERATOR opertype;
	int nStr;
public:
	Oper(string oper);
	int getPriority();
	int getType();
	int getEv (int left, int right);
	void getEq (string name, int val);
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
		   

        }
}


int Oper::getEv(int left = 1, int right = 1)
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
}

int Oper::getPriority()
{
	return PRIORITY[opertype];
}

vector <Lexem* > parseLexem(string codeline)
{
	vector <Lexem* > infix;
	int number = 0;
	int j, flag_n = 0, flag_op = 0;
	int nOp = 0;
	string notNum = "";
	for (int i = 0; i < codeline.size(); i++)
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
			while (((codeline[i] >= 'a' && codeline[i] <= 'z') || (codeline[i] >= 'A' && codeline[i] <= 'Z')) && i < codeline.size())
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
	int i = 1;


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
	for (int row = 0; row < (int)infixLines.size(); row++)
	{
		for (int i = 0; i < infixLines[row].size(); i++)
		{
			if((infixLines[row][i])->getType() == IF)
				stackIfElse.push(infixLines[row][i]);
			if((infixLines[row][i])->getType() == ELSE)
			{
				stackIfElse.top()->setRow(row + 1);
				stackIfElse.pop();
				stackIfElse.push(infixLines[row][i]);
			}
			if((infixLines[row][i])->getType() == ENDIF)
			{
				stackIfElse.top()->setRow(row + 1);
				stackIfElse.pop();
	
			}
			       
		}
	}
}

/*void joinGotoAndLabel (Var * lexemvar, <Oper *> &stack)
{
	if (stack.top()->operType() == GOTO )
	{
		lexemgoto->setRow (lexemvar->getName());
	}
}
*/
vector<Lexem *> buildPostfix(vector<Lexem *> infix)
{
        int i, j;
        stack<Oper *> opstack;
        vector<Lexem *> postfix;
        for (i = 0; i < infix.size(); i++)
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
                        if (oper->getType() == LBRACKET)
			{
                                opstack.push((oper));
                        } else
			{
                          if (oper->getType() == RBRACKET)
				{
                                        for (j = opstack.size(); j > 0 && opstack.top()->getType() != LBRACKET; j--)
					{
                                                postfix.push_back(opstack.top());
                                                opstack.pop();
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
        for (int i = 0; i < poliz.size(); i++)
	{
                if (poliz[i]->getLexType() == NUMBER || poliz[i]->getLexType() == VAR)
		{
                        computationStack.push(poliz[i]);
                }
                if (poliz[i]->getLexType() == OPER)
		{
			Oper *oper = static_cast <Oper* >(poliz[i]);
			if (oper->getType() == GOTO)
				return oper->getRow();
			if (oper->getType() == IF)
			{
//				cout << "YES" << oper->getRow() << endl;
			//	return oper->getRow();
//				cout << "ANS:" << computationStack.top()->getValue() << endl;	
				if(computationStack.top()->getValue() == 0)
				{
					computationStack.pop();
					return oper->getRow();
				}

//					cout << "YES" << endl;
				return row + 1;
				
				
			}
			if (oper->getType() == ELSE)
				return oper->getRow();
			if (oper->getType() == ENDIF)
				return row + 1;
                        right = (computationStack.top())->getValue();
                       	computationStack.pop();
			if (poliz[i]->getType() != ASSIGN)
			{
        	       	        left = (computationStack.top())->getValue();
	                        computationStack.pop();
        	                value = poliz[i]->getEv(left, right);
	               	        Number *val = new Number(value);
                       		computationStack.push(val);

      			}	


			if (poliz[i]->getType() == ASSIGN)
			{
				name = (computationStack.top())->getName();
				poliz[i]->getEq(name, right);
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
	int ans;
	int row;

	while (getline(cin, codeline))
		infixLines.push_back (parseLexem(codeline));
	for (row = 0; row < infixLines.size(); row++)
		initLabels(infixLines[row], row);
	initJumps(infixLines);

	for (row = 0; row < infixLines.size(); row++)
		postfixLines.push_back (buildPostfix(infixLines[row]));
	row = 0;
/*	for (int i = 0; i < postfixLines.size(); i++)
	{
		for (int j = 0; j < postfixLines[i].size(); j++)
		{
			cout << "[";
			postfixLines[i][j]->print();
			if (postfixLines[i][j]->getType() == ENDIF)
				cout << "!" << endl;
//				cout << postfixLines[i][j]->getRow() << endl;
			cout << "]";
		}

		cout << endl;

	}
*/
	while (row < postfixLines.size())
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
