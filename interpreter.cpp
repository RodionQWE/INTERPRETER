#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include <map>

using namespace std;


map<string, int> table;

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
	virtual int getType(){}
	virtual int getPriority(){}
//	virtual int getValue(int, int){}
	virtual int getValue(){}
	virtual int getEv(int, int){}
	virtual void getEq(string, int){}
	virtual void print(){}
	virtual string getName(){}
	
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
	EQUALS
};

int PRIORITY[] = {
        -1, -1,
        0, 0,
        1,
	-2
};


class Oper: public Lexem
{
	OPERATOR opertype;
public:
	Oper(char oper);
	int getPriority();
	int getType();
	int getEv (int left, int right);
	void getEq(string, int);
//	int getValue (const string&, int);
	void print();


};

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
		case EQUALS: cout << "= "; break;
        }
}


int Oper::getEv(int left = 1, int right = 1)
{
	switch (this->getType())
	{
		case PLUS: return left + right;
		case MINUS: return left - right;
		case MULTIPLY: return left * right;
	}

}

void Oper::getEq(string name, int val)
{
	table[name] = val;
}

/*int Oper::getValue(const string& left, int right)
{
//	table[left] = right;
	//cout << table[left] << endl;
	return right;
	
}
*/
Oper::Oper(char oper)
{
	type = OPER;
	if (oper == '+')
		opertype = PLUS;
	if (oper == '-')
		opertype = MINUS;
	if (oper == '*')
		opertype = MULTIPLY;
	if (oper == '(')
		opertype = LBRACKET;
	if (oper == ')')
		opertype = RBRACKET;
	if (oper == '=')
		opertype = EQUALS;
}

int Oper::getPriority()
{
	return PRIORITY[opertype];
}



vector <Lexem* > parseLexem(string codeline)
{
	vector <Lexem* > infix;
	int number = 0;
	int j, flag_n = 0;
	string str;
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
		} else {
			if (codeline[i] != ' ' && codeline[i] != '\t' && codeline[i] != '\n')
			{
				if (codeline[i] == '+' || codeline[i] == '-' || codeline[i] == '*' || codeline[i] == '('
					       	|| codeline[i] == ')' || codeline[i] == '=')
				{
					Oper *op = new Oper(codeline[i]);
					infix.push_back(op);
				}else
				{
					str = codeline[i];
					Var *var = new Var(str);
					infix.push_back(var);
				}

                        }

		}

	}
	return infix;
}

vector<Lexem *> buildPostfix(vector<Lexem *> infix)
{
        int i, j;
        stack<Oper *> opstack;
        vector<Lexem *> postfix;
        for (i = 0; i < infix.size(); i++)
	{
                if (infix[i]->getLexType() == NUMBER || infix[i]->getLexType() == VAR)
		{
                        postfix.push_back(infix[i]);
                }
                if (infix[i]->getLexType() == OPER)
		{
                        if (infix[i]->getType() == LBRACKET)
			{
                                opstack.push((Oper *)infix[i]);
                        } else
			{
                          if (infix[i]->getType() == RBRACKET)
				{
                                        for (j = opstack.size(); j > 0 && opstack.top()->getType() != LBRACKET; j--)
					{
                                                postfix.push_back(opstack.top());
                                                opstack.pop();
                                        }
                                        opstack.pop();
                                } else {
                                        while (!opstack.empty() && opstack.top()->getPriority() > infix[i]->getPriority())
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

int evaluatePostfix(vector<Lexem *> poliz)
{
        int value, left, right;
	string name;
        stack<Lexem *> computationStack;
        for (int i = 0; i < poliz.size(); i++)
	{
                if (poliz[i]->getLexType() == NUMBER || poliz[i]->getLexType() == VAR)
		{
                        computationStack.push(poliz[i]);
                }
                if (poliz[i]->getLexType() == OPER && poliz[i]->getType() != EQUALS)
		{
                        right = (computationStack.top())->getValue();
                        computationStack.pop();
                        left = (computationStack.top())->getValue();
                        computationStack.pop();
                        value = poliz[i]->getEv(left, right);
			Number *val = new Number(value);
                        computationStack.push(val);
                }
		if (poliz[i]->getLexType() == OPER && poliz[i]->getType() == EQUALS)
		{
			right = (computationStack.top())->getValue();
                        computationStack.pop();
			name = (computationStack.top())->getName();
                        computationStack.pop();
			poliz[i]->getEq(name, right);
			Var *var = new Var(name);
                        computationStack.push(var);
		}
        }
	return computationStack.top()->getValue();
}

/*int evaluatePostfix(vector<Lexem *> poliz)
{
        int value, left, right;
        stack<int> computationStack;
        for (int i = 0; i < poliz.size(); i++)
	{
                if (poliz[i]->getLexType() == NUMBER)
		{
                        computationStack.push(poliz[i]->getValue());
                }
                if (poliz[i]->getLexType() == OPER && poliz[i]->getType() != EQUALS)
		{
                        right = computationStack.top();
                        computationStack.pop();
                        left = computationStack.top();
                        computationStack.pop();
                        value = poliz[i]->getEv(left, right);
                        computationStack.push(value);
                }
                if (poliz[i]->getLexType() == OPER && poliz[i]->getType() == EQUALS)
        }
        return computationStack.top();
}

*/

/*std :: string codeline );
std :: vector < Lexem * > buildPoliz (
std :: vector < Lexem * > infix );
*/
int main()
{
	string codeline;
	vector<Lexem* > infix;
	vector<Lexem *> postfix;
	int ans;

	while (std::getline(std::cin, codeline))
	{
		infix = parseLexem(codeline);
		postfix = buildPostfix(infix);
		for (int i = 0; i < postfix.size(); ++i)
		{
                        postfix[i]->print();
			cout << endl;
//			cout << postfix.size();
                }
		ans = evaluatePostfix(postfix);
		cout << "ans = " << ans << endl;

	}

	return 0;
}
