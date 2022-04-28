#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include <map>
#include "interpreter.h"

using namespace std;

int main()
{
        string codeline;
        vector<vector <Lexem *> > postfixLines, infixLines;
        int row;
        while (getline(cin, codeline))
                infixLines.push_back (parseLexem(codeline));
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
                        cout << "]";
                }

                cout << endl;

        }
        while (row < (int)postfixLines.size())
        {
                row = evaluatePostfix(postfixLines[row], row);
        }

        return 0;
}


