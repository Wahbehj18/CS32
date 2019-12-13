#include "SSNSet.h"
#include <iostream>
using namespace std;

int main()
{
	SSNSet w;
	w.add(1000);
	w.add(2000);
	w.add(1002);
	w.add(2004);
	w.add(1005);
	w.add(2006);
	w.add(1000);
	w.add(2000);
	cout << w.size() << endl;
	w.print();



}