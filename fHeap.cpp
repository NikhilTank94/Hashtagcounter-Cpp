// ==> Function defination for MAX Fibonnaci Heap
#include<iostream> //io lib
using namespace std;
#include<string>				//string operation lib
#include<cmath>
#include<stdlib.h>				//memory allocation, process control, conversions and others
#include<vector>
#include"fHeap.h"				//header file for MAX FibHeap

fHeap::fHeap()					//method for fibonacci heap
{
	MAX = NULL;					//at init. and zero nodes the MAX is null
	numNodes = 0;				//no. of nodes in the Heap
}

node* fHeap::getNode(string hread, int freq) // create a node  with hashtag and it's frequency
{
	node* Node = new node();
	Node->hread = hread;
	Node->freq = freq;
	Node->degree = 0;				//for newNode the degree is set zero
	Node->parent = NULL;			//there will be no parent
	Node->child = NULL;				//there will be no child
	Node->leftSibling = Node;		//the leftSibling pointer will pt to same node
	Node->rightSibling = Node;		//the rightSibling pointer will pt to same node
	Node->childCut = false;			//the child cut value of the newNode is False
	
	return Node;
}

void fHeap::insert(node* nNode) 	// for insert new node in tree
{
	if (MAX == NULL)				//if the heap didn't had elements then newNode becomes MAX
		MAX = nNode;
	else                            //else the newnode is inserted to the root list (between the MAX and its right sibling)
	{
		node* next = MAX->rightSibling;
		MAX->rightSibling = nNode;
		nNode->leftSibling = MAX;
		nNode->rightSibling = next;
		next->leftSibling = nNode;
	}
	if (nNode->freq > MAX->freq)	//check whether the MAX value need update
		MAX = nNode;
	numNodes++;						//the operation will increase no. of nodes				
}

void fHeap::Nodecut(node* target)
{
	//first, we have to disconnect the targetget node form parent
	if (target->parent->degree == 1) //targetget doesn't have parent
	{
		target->parent->child = NULL;
	}
	else
	{
		node* rightSibling = target->rightSibling;
		node* leftSibling = target->leftSibling;
		leftSibling->rightSibling = rightSibling;
		rightSibling->leftSibling = leftSibling;
		if (target->parent->child == target)	//when targetget is the first child of parent then the parent have to pt to other sibling
			target->parent->child = leftSibling;
		//check whether child cut of parent have to set to True?
	}
	//Second, we have to add the targetget node to root list
	node* right = MAX->rightSibling;
	MAX->rightSibling = target;
	target->leftSibling = MAX;
	target->rightSibling = right;
	right->leftSibling = target;
	target->childCut = false; 				// childCut value for root node is undefined / false
	target->parent->degree--;				//decrease degree of parent
	target->parent = NULL;
}

void fHeap::cascadingNodeCut(node* parenttarget)		// to be safe if parent looses more than 1 child then the parent will also cut
{
	if (parenttarget->parent != NULL)
	{
		if (parenttarget->childCut == false)			//parent haven't lost a child yet
			parenttarget->childCut = true;
		else {
			node* grandparent = parenttarget->parent;
			cascadingNodeCut(grandparent);				//recursivly check cut for grandparent also
			Nodecut(parenttarget);						//cut parent also 
		}

	}
}

void fHeap::increaseKey(node* target, int key) 			// increase the value of freq when an node was found in heshmap
{
	target->freq += key;									//increase freq of the targetget
	if (target->parent != NULL && target->freq > target->parent->freq) //if by increase key the value of targetget is greater than parent
	{
		node* parenttarget = target->parent;			//parent of target
		cascadingNodeCut(parenttarget);					//recursivly check cut for parent also
		Nodecut(target);								//cut targetget
	}
	if (target->freq > MAX->freq)						//if by increase key the MAX is changed then update
		MAX = target;

}

void fHeap::meld(node* targetA, node* targetB) 			// meld nodes where freq targetA > freq targetB 
{
	//if the degree of 2 nodes are same then during pop MAX operation the targetB becomes child of targetA, and it is removed form root list
	targetB->leftSibling->rightSibling = targetB->rightSibling;	//correcting the doubly linked list direction so that targetB can be shifted on level down
	targetB->rightSibling->leftSibling = targetB->leftSibling;
	targetB->parent = targetA;
	targetB->childCut = false;
	if (targetA->child == NULL)			//if there are no other child of targetgetA
	{
		targetA->child = targetB;
		targetB->leftSibling = targetB;
		targetB->rightSibling = targetB;
	}
	else								//if there are other child then insert targetgetB between A's first and second child
	{
		node* primaryChild = targetA->child;
		node* nextChild = primaryChild->rightSibling;
		primaryChild->rightSibling = targetB;
		targetB->leftSibling = primaryChild;
		nextChild->leftSibling = targetB;
		targetB->rightSibling = nextChild;
	}
	targetA->degree++;
}

void fHeap::Combine()
{
	//the fibonnaci heap follows the Fibo. property and that's why it's name after that
	//as per the property the degree is bounded as a log base of phi = 1.618
	//https://en.wikipedia.org/wiki/Fibonacci_heap
	float phi = 1.618;
	int d = ceil(log((float)numNodes) / log(phi));	// bound on MAX degree
	node** list = new node * [d];					// list for checking degree duplicasy
	int a;
	for (a = 0; a < d; a++)
		list[a] = NULL;

	if (MAX != NULL)					
	{
		//Vectors are same as dynamic arrays with the ability to resize itself automatically when an element is inserted or deleted, 
		//with their storage being handled automatically by the container.
		vector<node*> myvector;			//creating vector of node pointers		
		node* targetB = MAX;
		myvector.push_back(targetB);
		while (targetB != MAX->leftSibling)
		{
			targetB = targetB->rightSibling;
			myvector.push_back(targetB);	//push_back() function is used to push elements into a vector from the back
		}
		
		//https://www.youtube.com/watch?v=ti-ah92p35w
		for (vector<node*>::iterator I = myvector.begin(); I != myvector.end(); ++I)
		{
			targetB = *I;
			node* parenttarget = targetB;
			int degree = targetB->degree;
			while (list[degree] != NULL)
			{
				node* targetA = list[degree];
				if (targetA->freq > targetB->freq)	// targetA > targetB this implies targetA will become parent in mel
				{
					parenttarget = targetA;
					meld(targetA, targetB);
				}
				else							// else targetB will become parent in melded tree
				{
					parenttarget = targetB;
					meld(targetB, targetA);	// targetA becomes child of targetB
				}
				list[degree] = NULL; 	 
				degree++;
				targetB = parenttarget;
			}
			list[degree] = parenttarget;	//the degree+1 will pointer now to this parent
		}
	}
	//reset MAX
	MAX = NULL;
	for (a = 0; a < d; a++)
	{
		if (list[a] != NULL)			//if there are any elements in the list of non null degree
		{
			if (MAX == NULL)			//initialization if MAX was NULL
				MAX = list[a];
			else if (list[a]->freq > MAX->freq)
				MAX = list[a];
		}
	}
}

node* fHeap::removeMAX() 							// pop the MAX from the tree but trigger pairwise combile
{
	node* target = MAX;
	if (MAX != NULL)
	{
		node* rightSibling = MAX->rightSibling;
		node* leftSibling = MAX->leftSibling;
		if (leftSibling != MAX)
		{
			node* targetChild = MAX->child;
			if (targetChild != NULL) 							//the MAX element has some child
			{
				node* primaryChild = MAX->child;
				node* lastChild = primaryChild->leftSibling;
				leftSibling->rightSibling = primaryChild;		//inserting the childs of MAX node in the root of the tree
				primaryChild->leftSibling = leftSibling;
				rightSibling->leftSibling = lastChild;
				lastChild->rightSibling = rightSibling;
				int f = MAX->degree;
				while(f > 0)			//iteratively set all childcut for all children of targetget to False and thier parent to null
				{
					targetChild->childCut = false;
					targetChild->parent = NULL;
					targetChild = targetChild->rightSibling;
					f--;
				}
			}
			else				//the MAX element doesn't have child
			{
				rightSibling->leftSibling = leftSibling;
				leftSibling->rightSibling = rightSibling;				
			}
			MAX = rightSibling;
		}
		else
		{
			node* targetChild = MAX->child;
			node* primaryChild = MAX->child;
			if (targetChild != NULL) 					//atleast one child
			{
				
				while (targetChild != primaryChild)		//iteratively update the enteries of child of the old MAX element
				{
					targetChild->parent = NULL;
					targetChild->childCut = false;		//since child cut value of root list elements is notdefined
					targetChild = targetChild->rightSibling;
				} 
			}
			MAX = targetChild;							//just a wild choice for MAX as we will be doing pairwise combine to get exact MAX
		}
		Combine();
	}
	else 												// there are no elements in the heap
	{
		cout << "the MAX Fib Heap is empty" << endl;
	}
	if (numNodes > 0)
		numNodes--;
	else
		numNodes = 1;	
	target->degree = 0;
	target->parent = NULL;
	target->child = NULL;
	target->leftSibling = target;
	target->rightSibling = target;
	target->childCut = false;
	return target;
}

