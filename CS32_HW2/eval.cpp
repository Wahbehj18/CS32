#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
#include "Set.h"
using namespace std;

string removeSpace(string s)
{
	string b;
	for (int i = 0; i != s.size(); i++)
	{
		if (s[i] != ' ')
			b += s[i];
	}
	return b;
}

bool validParentheses(string i)
{
	int cOpen = 0;
	int cClosed = 0;
	for (int j = 0; j != i.size(); j++)
	{
		if (i[j] == '(')
			cOpen++;
		if (i[j] == ')')			//must have equal number of 
			cClosed++;				//open and closed parantheses
	}
	if (cClosed != cOpen)
		return false;

	for (int j = 0; j != i.size() && i[j] != '('; j++)
	{

		if (i[j] == ')')			//cannot have closed before open
			return false;
	}

	for (int j = 0; j != i.size(); j++)
	{
		if (i[j] == '(' && i[j + 1] == ')')		//cannot have paratheses
			return false;						//with nothing inside
	}
	return true;
}

bool isValid(string i)
{
	if (!validParentheses(i))
		return false;


	if (i == "")
		return false;
	
	if (i[0] != '!' && !islower(i[0]) && i[0] != '(')      //first char must be ! ( or letter
		return false;
	if (i[i.size() - 1] != ')' && !islower(i[i.size() - 1]))   //last char must be ) or letter
		return false;


	for (int j = 0; j != i.size(); j++)
	{
		if (i[j] != '&' && i[j] != '|' && i[j] != '!' && i[j] != '(' && i[j] != ')' && !islower(i[j]))   //only valid chars
			return false;

		if (i[j] == '!' && j + 1 != i.size() && !islower(i[j + 1]) && i[j+1] != '!' && i[j + 1] != '(')   // '!' must preceed a letter or ( or !
			return false;

		if (i[j] == '(' && j + 1 != i.size() && !islower(i[j + 1]) && i[j + 1] != '!' && i[j + 1] != '(')   // '(' must preceed a letter or '!' or (
			return false;

		if (islower(i[j]) && j + 1 != i.size() && i[j + 1] != '&' && i[j + 1] != '|' && i[j + 1] != ')')   //letters must preceed &,|,)
			return false;
		if (i[j] == '&' && j + 1 != i.size() && i[j + 1] != '(' && !islower(i[j+1]) && i[j + 1] != '!')    //& must precced ( or ! or letter
			return false;
		if (i[j] == '|' && j + 1 != i.size() && i[j + 1] != '(' && !islower(i[j + 1]) && i[j + 1] != '!')  //| must precced ( or ! or letter
			return false;
	}
	return true;
}

void inToPost(string i, string& post)
{
	post = "";
	stack<char> c;
	for (int j = 0; j != i.size(); j++)
	{
		if (islower(i[j]))
			post += i[j];

		else if (i[j] == '(')
			c.push(i[j]);
		else if (i[j] == ')')
		{
			while (c.top() != '(')
			{
				post += c.top();
				c.pop();
			}
			c.pop();
		}
		else if (i[j] == '|')
		{
			while (!c.empty() && c.top() != '(')
			{
				post += c.top();
				c.pop();
			}
			c.push(i[j]);
		}
		else if (i[j] == '&')
		{
			while (!c.empty() && c.top() != '(' && c.top() != '|')
			{
				post += c.top();
				c.pop();
			}
			c.push(i[j]);
		}
		else if (i[j] == '!')
		{
			c.push(i[j]);
		}
	}
	
	while (!c.empty())
	{
		post += c.top();
		c.pop();
	}

}

void compute(const Set& trueValues, const Set& falseValues, string post, bool& result)
{
	stack<bool> n;

	//cout << post << endl;

	for (int i = 0; i != post.size(); i++)
	{
		if (trueValues.contains(post[i]))
			n.push(true);
		else if (falseValues.contains(post[i]))
			n.push(false);
		else if (post[i] == '!')
		{
			bool opposite = !n.top();
			n.pop();
			n.push(opposite);
		}
		else
		{
			bool op2 = n.top();
			n.pop();
			bool op1 = n.top();
			n.pop();

			if (post[i] == '&')
				n.push(op1 && op2);
			else if (post[i] == '|')
				n.push(op1 | op2);
		}
	}

	result = n.top();
}

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{

	string good = removeSpace(infix);

	if (!isValid(good))
		return 1;


	for (int i = 0; i != good.size(); i++)
	{
		if (islower(good[i]) && !trueValues.contains(good[i]) && !falseValues.contains(good[i]))
			return 2;
		if (islower(good[i]) && (trueValues.contains(good[i]) && falseValues.contains(good[i])))
			return 3;
	}

	inToPost(good, postfix);

	compute(trueValues, falseValues, postfix, result);

	return 0;

}


int main()
{

	string trueChars = "tywz";
	string falseChars = "fnx";
	Set trues;
	Set falses;

	for (int k = 0; k != trueChars.size(); k++)
		trues.insert(trueChars[k]);
	for (int k = 0; k != falseChars.size(); k++)
		falses.insert(falseChars[k]);

	string pf;
	bool answer;

	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" &&  answer);
	assert(evaluate("y|", trues, falses, pf, answer) == 1);
	assert(evaluate("n t", trues, falses, pf, answer) == 1);
	assert(evaluate("nt", trues, falses, pf, answer) == 1);
	assert(evaluate("()", trues, falses, pf, answer) == 1);
	assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
	assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
	assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
	assert(evaluate("", trues, falses, pf, answer) == 1);
	assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
		&& pf == "ff!tn&&|" && !answer);
	assert(evaluate(" x  ", trues, falses, pf, answer) == 0 && pf == "x" && !answer);
	trues.insert('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 3);
	falses.erase('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 0 && pf == "x"  &&  answer);
	trues.erase('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 2);
	falses.insert('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && !answer);
	cout << "Passed all tests" << endl;
	

	string tC = "ucla";
	string fC = "snx";
	Set ts;
	Set fs;

	for (int k = 0; k != tC.size(); k++)
		ts.insert(tC[k]);
	for (int k = 0; k != fC.size(); k++)
		fs.insert(fC[k]);



	string d;
	bool r;
	
	inToPost("a&!(s|u&c|n)|!!!(s&u&n)", d);
	cout << d << endl;

	compute(ts, fs, d, r);
	
	if (r == true)	
		cout << "YEEEE" << endl;
	else
		cout << "NAHHH" << endl;

	assert(isValid("a&!(s|u&c|n)|!!!(s&u&n)") == true);

	assert(evaluate("u & c&l&a&!(u&s&c)", ts, fs, d, r) == 0 && r);
	assert(evaluate("a&!(s|u&c|n)|!!!(s&u&n)", ts, fs, d, r) == 0 && r);



}