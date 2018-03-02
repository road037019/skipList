
#include <algorithm>
#include <iostream>
using namespace std;

template<class K, class E>
class skipNode
{
public:
	skipNode(const pair<const K, E>& theElement, int levels) :
		element(theElement)
	{
		next = new skipNode<K, E>*[levels];
	}

	~skipNode()
	{
		delete[]next;
	}
	pair<const K, E> element;
	skipNode<K, E> **next;
};
