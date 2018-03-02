
#include <algorithm>
#include <iostream>
#include "skipNode.h"

using namespace std;

template<class K, class E>
class skipList
{

public:
	skipList(K largetKey, int maxPairs, float prob);
	~skipList();

	pair<const K, E>* find(const K& theKey);
	void insert(const pair<const K, E>& thePair);
	void erase(const K& theKey);
	void print(ostream& out) const;
private:
	skipNode<K, E>* search(const K& theKey);
	int level() const;

private:
	float cutOff; //用来确定层数
	int levels;	//当前最大的非空链表
	int dsize;	//字典的数对个数
	int maxLevel;	//允许的最大链表层数
	K tailKey; //最大关键字
	skipNode<K, E> *headerNode; //头节点指针
	skipNode<K, E> *tailNode;	//尾节点指针
	skipNode<K, E> **last;	//last[i]表示i层的最后节点
};

template<class K, class E>
skipList<K, E>::skipList(K largetKey, int maxPairs, float prob) :
	tailKey(largetKey),
	cutOff(prob * RAND_MAX),
	maxLevel((int)ceil(logf((float)maxPairs) / logf(1 / prob)) - 1),
	levels(0),
	dsize(0)
{
	pair<K, E> tailPair;
	tailPair.first = tailKey;
	headerNode = new skipNode<K, E>(tailPair, maxLevel + 1);
	tailNode = new skipNode<K, E>(tailPair, 0);
	last = new skipNode<K, E>*[maxLevel + 1];

	for (int i = 0; i <= maxLevel; ++i)
	{
		headerNode->next[i] = tailNode;
	}
}


template<class K, class E>
skipList<K, E>::~skipList()
{
	skipNode<K, E> *targetNode = headerNode;
	skipNode<K, E> *nextNode;
	while (targetNode != tailNode)
	{
		nextNode = targetNode->next[0];
		delete targetNode;
		targetNode = nextNode;
	}

	delete tailNode;
}


template<class K, class E>
pair<const K, E>* skipList<K, E>::find(const K& theKey)
{
	if (theKey > tailKey)
		return NULL;

	skipNode<K, E> *beforeNode = headerNode;
	for (int i = levels; i >= 0; --i)
	{
		while (beforeNode->next[i]->element.first < theKey)
			beforeNode = beforeNode->next[i];
	}

	if (beforeNode->next[0]->element.first == theKey)
		return &beforeNode->next[0]->element;

	return NULL;
}

template<class K, class E>
skipNode<K, E> *skipList<K, E>::search(const K& theKey)
{
	skipNode<K, E>* beforeNode = headerNode;
	for (int i = levels; i >=0; --i)
	{
		while (beforeNode->next[i]->element.first < theKey)
			beforeNode = beforeNode->next[i];
		last[i] = beforeNode;
	}

	return beforeNode->next[0];
}

template<class K, class E>
int skipList<K, E>::level() const
{
	int theLevel = 0;
	while (rand() > cutOff)
		theLevel++;

	return theLevel;
}


template<class K, class E>
void skipList<K, E>::insert(const pair<const K, E>& thePair)
{
	if (thePair.first > tailKey)
		return;

	skipNode<K, E> *theNode = search(thePair.first);
	if (theNode->element.first == thePair.first)
	{
		theNode->element.second = thePair.second;
		return;
	}

	int theLevel = level();
	if (theLevel > levels)
	{
		theLevel = ++levels;
		last[theLevel] = headerNode;
	}

	skipNode<K, E>* newNode = new skipNode<K, E>(thePair, theLevel + 1);
	for (int i = 0; i <= theLevel; ++i)
	{
		newNode->next[i] = last[i]->next[i];
		last[i]->next[i] = newNode;
	}

	dsize++;
}

template<class K, class E>
void skipList<K, E>::erase(const K& theKey)
{
	if (theKey >= tailKey)
		return;

	skipNode<K, E> *theNode = search(theKey);
	if (theNode->element.first != theKey)
		return;

	int i = 0;
	while (last[i]->next[i]->element.first == theKey)
	{
		last[i]->next[i] = theNode->next[i];
		i++;
	}

	while (last[levels]->next[levels] == tailNode)
		levels--;

	delete theNode;
	dsize--;
}

template<class K, class E>
void skipList<K, E>::print(ostream &out)  const
{
	for(int i = 0; i <= levels; ++i)
	{
		cout << "levels :" << i << endl;
		skipNode<K, E> *targetNode = headerNode->next[i];
		while (targetNode != tailNode)
		{
			out << "Key: " << targetNode->element.first
				<< " Value: " << targetNode->element.second << endl;
			targetNode = targetNode->next[i];
		}
		cout << endl;
	}
}


template<class K, class E>
ostream &operator<<(ostream &out, const skipList<K, E>& theList)
{
	theList.print(out);
	return out;
}
