#include "skipList.h"
#include <iostream>
using namespace std;

int main()
{
	skipList<int, int> skip(100, 20, 0.7);
	for (int i = 0; i < 10; ++i)
		skip.insert(pair<const int, int>(i, i));
	cout << skip;
	cout << skip.find(3)->second << endl;
	skip.erase(3);
	cout << skip << endl;
	skip.insert(pair<const int, int>(3, 3));
	cout << skip << endl;
	return 0;
}
