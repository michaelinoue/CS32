#include <string>
#include <stack>
#include "Set.h"
#include <iostream> //pls check my dude
#include <cctype>
#include <cassert>

using namespace std;

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
	if (infix.size() < 1)
		return 1;

	string temp = "";
	for (int i = 0; i < infix.size(); i++)
	{
		if (infix[i] != '&' && infix[i] != ' ' && infix[i] != '|' && !('a' <= infix[i] && infix[i] <= 'z') && infix[i] != '(' && infix[i] != ')' && infix[i] != '!')
			return 1;
		if (infix[i] != ' ')
		{
			
			temp += infix[i];
		}
	}
	
	
	if (temp.size() < 1)
		return 1;

	if (!(temp[temp.size() - 1] >= 'a' && temp[temp.size() - 1] <= 'z') && temp[temp.size()-1] != ')')
		return 1;
	if (temp[0] != '!' && !('a' <= temp[0] && temp[0] <= 'z') && temp[0] != '(')
		return 1;
	for (int i = 0; i < temp.size() - 1; i++)
	{
		if (temp[i] == '!')
			if (temp[i + 1] != '!' && !('a' <= temp[i + 1] && temp[i + 1] <= 'z') && temp[i+1]!= '(')
			return 1;
		if ((temp[i] == '&' || temp[i] == '|') && !('a' <= temp[i + 1] && temp[i + 1] <= 'z') && temp[i + 1] != '!' && temp[i+1] != '(')
		{
			return 1;
		}
		if ('a' <= temp[i] && temp[i] <= 'z' && temp[i + 1] != '&' && temp[i + 1] != '|' && temp[i+1] != ')')
			return 1;
		if (temp[i] == '(' && !(('a' <= temp[i + 1] && temp[i + 1] <= 'z') || temp[i + 1] == '!' || temp[i+1] == '('))
			return 1;
		if (temp[i] == ')' && temp[i + 1] != '&' && temp[i + 1] != '|' && temp[i + 1] != ')')
			return 1;
	}
	

	postfix = "";
	stack<char> operatorStack;
	for (int i = 0; i < infix.size(); i++)
	{
		char x = infix[i];

		if ('a' <= x && x <= 'z')
		{
			postfix += x;
			continue;
		}
		switch (x)
		{
		case '(':
			operatorStack.push(x);
			break;
		case ')':
			if (operatorStack.empty())
				return 1;
			while (operatorStack.top() != '(')
			{
				if (operatorStack.empty()) //check this boi
					return 1;
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.pop();
			break;
			return false; //couldn't find matching paren theses?? CHECK
		case '|':
			while (!operatorStack.empty() && operatorStack.top() != '(')
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.push(x);
			break;
		case '&':
			while (!operatorStack.empty() && operatorStack.top() != '(' && operatorStack.top() != '|')
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.push(x);
			break;
		case '!':
			while (!operatorStack.empty() && operatorStack.top() != '(' && operatorStack.top() != '|' && operatorStack.top() != '&')
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.push(x);
			break;
		case ' ':
			break;
		default:
			return 1; //found something other than these things in string
		}
	}
	while (!operatorStack.empty())
	{
		if (operatorStack.top() == '(')
			return 1;
		postfix += operatorStack.top();
		operatorStack.pop();
	}
	
	
	stack<bool> operandStack;
	for (int i = 0; i < postfix.size(); i++)
	{
		char ch = postfix[i];
		if ('a' <= ch && ch <= 'z')
		{
			if (trueValues.contains(ch))
			{
				if (falseValues.contains(ch))
					return 3;
				operandStack.push(true);
			}

			else if (falseValues.contains(ch))
			{
				if (trueValues.contains(ch))
					return 3;
				operandStack.push(false);
			}
			else
				return 2;
		}
		else
		{
			bool operand2;
			bool operand1;
			if (operandStack.empty())
				return 1; //not syntatically valid 
			operand2 = operandStack.top();
			operandStack.pop();
			if (ch == '!')
			{
				operandStack.push(!operand2);
				continue;
			}
			if (operandStack.empty())
				return 1;
			operand1 = operandStack.top();
			operandStack.pop(); //TODO: check that there are no two operators next to each other
			if (ch == '|')
				operandStack.push(operand2 || operand1);
			if (ch == '&')
				operandStack.push(operand2 && operand1);
		}
	}
	result = operandStack.top();
	
	return 0;
}
// Evaluate a boolean expression
//   If infix is a syntactically valid infix boolean expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values sets), then postfix is set to the postfix
//   form of the expression.  If not, postfix might or might not be
//   changed, result is unchanged, and the function returns 1.
//
//   If infix is a syntactically valid infix boolean expression whose
//   only operands are single lower case letters:
//
//      If every operand letter in the expression appears in either
//      trueValues or falseValues but not both, then result is set to the
//      result of evaluating the expression (using for each letter in the
//      expression the value true if that letter appears in trueValues or
//      false if that letter appears in false values) and the function
//      returns 0.
//
//      Otherwise, result is unchanged and the value the function returns
//      depends on these two conditions:
//        at least one letter in the expression is in neither the
//            trueValues nor the falseValues sets; and
//        at least one letter in the expression is in both the
//            trueValues and the falseValues set.
//      If only the first condition holds, the function returns 2; if
//      only the second holds, the function returns 3.  If both hold
//      the function returns either 2 or 3 (and the function is not
//      required to return the same one if called another time with the
//      same arguments).



