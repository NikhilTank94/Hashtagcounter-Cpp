//Header ==> Function declaration for MAX Fibonacci Heap
using namespace std;
#include<string>	//string operation lib

struct node
{
	string hread;	//value of hashtag in the FibHeap at a node
	int freq;		//value of frequency in the FibHeap at a node
	int degree;		//no. of childs of a node
	node* parent;	//node pointer for parent node
	node* child;	//node pointer for child node
	node* leftSibling;		//node pointer for leftSibling node
	node* rightSibling;		//node pointer for rightSibling node
	bool childCut;	//boolean status of childCut at a node
};

//creating class for MAX Fibonachi Heap implimentation
class fHeap {
	int numNodes;	//variable for assigning new Node
	node* MAX;		//pointer for MAX frequency node
public:											//declaring public methods ands variables
	fHeap();									//method for fibonacci heap
	node* getNode(string hread, int freq);		// create a node  with hashtag and it's frequency
	void insert(node* newNode); 				// for insert new node in tree
	void Nodecut(node* target);					// when increase key leads to child freq > parent freq the node will seperate form parent 
	void cascadingNodeCut(node* target);		// when parent looses more than 1 child then the parent will also cut
	void increaseKey(node* target, int key); 		// increase the value of freq when an node was found in heshmap
	void meld(node* targetA, node* targetB); 	// meld nodes where freq targetA > freq targetA
	void Combine();						// combine the tree with same degree at level 0
	node* removeMAX(); 							// pop the MAX from the tree but trigger pairwise combile
};

