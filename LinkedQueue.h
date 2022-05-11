#pragma once
#include "Mission.h"

template <typename T>
class LinkedQueue
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	LinkedQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	int count();
	void print() const;
	//==================================== Functions for Rovers Queue ====================================
	bool getFastestRover(T& output);
	//====================================================================================================
	//==================================== Functions for Missions Queue ==================================
	bool DeleteByID(int ID);
	bool FindByID(int inputID, Mission& RMission);
	bool getHighestSIG(Mission& output); // For Polar & Mountainous Use
	bool getHighestPriority(Mission& output); // For Emergency Use only
	void reOrderCMissions();
	//====================================================================================================

	~LinkedQueue();
	LinkedQueue(const LinkedQueue<T>& LQ);
};

template <typename T>
LinkedQueue<T>::LinkedQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;
}

template <typename T>
bool LinkedQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

template <typename T>
bool LinkedQueue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	if (isEmpty())	
		frontPtr = newNodePtr; 
	else
		backPtr->setNext(newNodePtr);

	backPtr = newNodePtr; 
	return true;
}

template <typename T>
bool LinkedQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	if (nodeToDeletePtr == backPtr)	
		backPtr = nullptr;

	delete nodeToDeletePtr;
	return true;
}

template <typename T>
bool LinkedQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;
}

template <typename T>
int LinkedQueue<T>::count()
{
	if (isEmpty())
		return 0;

	LinkedQueue Temp;
	T TTemp;

	int cnt = 0;

	while (dequeue(TTemp)) {
		cnt++;
		Temp.enqueue(TTemp);
	}

	while (Temp.dequeue(TTemp))
		enqueue(TTemp);

	return cnt;
}

template <typename T>
LinkedQueue<T>::~LinkedQueue()
{
	T temp;
	while (dequeue(temp));
}

template <typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& LQ)
{
	Node<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr)
	{
		frontPtr = backPtr = nullptr;
		return;
	}

	Node<T>* ptr = new Node<T>(NodePtr->getItem());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	while (NodePtr)
	{
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}
}

//==================================== Functions for Rovers Queue ====================================
template <typename T>
bool LinkedQueue<T>::getFastestRover(T& output) {
	if (isEmpty())
		return false;

	LinkedQueue Temp;
	Rover TTemp;
	int speed = 0;
	bool found = false;

	while (dequeue(TTemp)) {
		if (TTemp.getSpeed() > speed) speed = TTemp.getSpeed();
		Temp.enqueue(TTemp);
	}

	while (Temp.dequeue(TTemp)) {
		if (TTemp.getSpeed() == speed && !found) {
			output = TTemp;
			found = true;
		}
		else {
			enqueue(TTemp);
		}
	}

	return true;
}
//====================================================================================================
//==================================== Functions for Missions Queue ==================================

template <typename T>
bool LinkedQueue<T>::DeleteByID(int ID) {
	if (isEmpty())
		return false;

	LinkedQueue Temp;
	Mission TTemp;
	bool found = false;

	while (dequeue(TTemp)) {
		if (TTemp.getInfo().ID != ID)
			Temp.enqueue(TTemp);
		else
			found = true;
	}

	while (Temp.dequeue(TTemp))
		enqueue(TTemp);

	return found;
}

template <typename T>
bool LinkedQueue<T>::FindByID(int inputID, Mission &RMission) {
	if (isEmpty())
		return false;

	LinkedQueue Temp;
	Mission TTemp;
	bool found = false;

	while (dequeue(TTemp)) {
		if (TTemp.getInfo().ID != inputID)
			Temp.enqueue(TTemp);
		else {
			RMission = TTemp;
			found = true;
		}
	}

	while (Temp.dequeue(TTemp))
		enqueue(TTemp);

	return found;
}

template <typename T>
bool LinkedQueue<T>::getHighestSIG(Mission& output) {
	if (isEmpty())
		return false;

	LinkedQueue Temp;
	Mission TTemp;
	bool found = false;
	int SIG = 0;

	while (dequeue(TTemp)) {
		if (TTemp.getInfo().SIG > SIG) SIG = TTemp.getInfo().SIG;
		Temp.enqueue(TTemp);
	}

	while (Temp.dequeue(TTemp)) {
		if (TTemp.getInfo().SIG == SIG && !found) {
			output = TTemp;
			found = true;
		}
		else {
			enqueue(TTemp);
		}
	}

	return true;
}

template <typename T>
bool LinkedQueue<T>::getHighestPriority(Mission& output) {
	if (isEmpty())
		return false;

	LinkedQueue Temp;
	Mission TTemp;
	bool found = false;
	float priority = 0;

	while (dequeue(TTemp)) {
		if (TTemp.calculatePriority() > priority) priority = TTemp.calculatePriority();
		Temp.enqueue(TTemp);
	}

	while (Temp.dequeue(TTemp)) {
		if (TTemp.calculatePriority() == priority && !found) {
			output = TTemp;
			found = true;
		}
		else {
			enqueue(TTemp);
		}
	}

	return true;
}

template <typename T>
void LinkedQueue<T>::reOrderCMissions() {
	if (isEmpty())
		return;

	LinkedQueue Temp;
	Mission TTemp;
	Mission TTemp2;
	Mission TTemp3;

	dequeue(TTemp);
	Temp.enqueue(TTemp);

	while (dequeue(TTemp)) {
		peek(TTemp2);
		if (TTemp.getCD() == TTemp2.getCD()) {
			if (TTemp.getED() <= TTemp2.getED()) {
				Temp.enqueue(TTemp);
			}
			else {
				dequeue(TTemp2);
				Temp.enqueue(TTemp2);
				Temp.enqueue(TTemp);
			}
		}
		else {
			Temp.enqueue(TTemp);
		}
	}

	while (Temp.dequeue(TTemp)) enqueue(TTemp);
}

//====================================================================================================