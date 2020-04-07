/*
The objective of the porject is to implement a system to find the n most popular hashtags
that appear on social media such as Facebook or Twitter. For the scope of this project hashtags
will be given from an input file. Basic idea for the implementation is to use a max priority
structure to find out  the most popular hashtags.
The following data structures were used:

1. Max Fibonacci heap: use to keep track of the frequencies of hashtags.
2.  Hash table: The key for the hash table is the hashtag, and the value is the pointer to the
	corresponding node in the Fibonacci heap.

Assumptions:
1. No uppercase letters in the input stream
2. No spaces  in the hashtags. (only one  word per  one  hashtag)
3. One query has only one integer.
4. Query integer, n<=20. i.e. you need to find at most 20 popular hashtags.
5. Input file may consist of more than 1 million hashtags

*/

#include<iostream>				//io lib
using namespace std;
#include<fstream>				//create, write or read files
#include<map>					//map operation lib 
#include<string>				//string operation lib
#include<stdlib.h>				//memory allocation, process control, conversions and others
#include"fHeap.h"				//header file for Max FibHeap
int main(int bname, char* fin[])
{
	//using fstream lib we will read input file and create/open output file 
	bool printconsole = false;
	ifstream inputFile(fin[1]);							// read input file
	ofstream outputFile(fin[2]);						// open output file
	if (!fin[2])										// if no output file assigned
		printconsole = true;							// boolean for printing in console if it is true

	//Map operation 
	std::map<string, node*> hashMap;					//create map with name hashmap(string dtype key,nodeptr value)
	//std::map is an associative container that store elements in key value combination 
	//where key are unique, otherwise it overrises the prevalue

	fHeap h;
	string lread;

	while (getline(inputFile, lread))					//read input file line by line for queries
	{
		//Case 1: input is a hashtag
		if (lread[0] == '#')							//if first element of the line is '#' then it a insert or increase operation
		{
			//partition the input
			int s;
			for (s = 0;; s++)							//search for first white space in line
			{
				if (lread[s] == ' ')
					break;								//break when white space apprears
			}

			string f = lread.substr(s + 1);				//get frequency from the line
			const char* a = f.c_str();					//Convert string to integer
			int freq = atoi(a);
			string hread = lread.substr(1, s - 1);		//get the hashtag name form the line

			//Case1: hashtag was alread in Hashtable
			if (hashMap.find(hread) != hashMap.end())	//if hashtag is found 
			{
				// this means the hashtag is alread in fHeap and just require increase key
				node* t = hashMap.find(hread)->second;	//accessing second member of the Map using pointer
				h.increaseKey(t, freq);					//method from fHeap class
			}

			//Case2: hashtag is not in Hashtable
			else
			{
				// this means the hashtag is not in fHeap and require us to insert
				node* t = h.getNode(hread, freq); 		//method from fHeap class
				hashMap[hread] = t;
				h.insert(t);
			}
		}

		//Case 2: input is a query
		else if (lread[0] >= '0' && lread[0] <= '9')
		{
			const char* a = lread.c_str();				//Convert string to integer
			int t = atoi(a);
			if (t <= 20)
			{
				node** lreadarr = new node * [t];		// array to store the removed nodes, to be inserted later
				int j;
				// perform t-times removeMax operations on the Max FibHeap
				for (j = 0; j < t; j++)
				{
					lreadarr[j] = h.removeMAX();		//method from fHeap class
					if (printconsole == false)
						outputFile << (j > 0 ? "," : "") << lreadarr[j]->hread; //storing the most popular hashtags with comma seperated 
						// and without space in non increasing order
					else
						cout << (j > 0 ? "," : "") << lreadarr[j]->hread;		//storing the most popular hashtags with comma seperated						 
						// and without space in non increasing order
				}
				(printconsole ? cout << "\n" : outputFile << "\n");				//ternary operator
				// reinsert extracted nodes
				for (j = 0; j < t; j++)
					h.insert(lreadarr[j]);				//inserting back the most popular hastag in the Max Fibheap
				delete[] lreadarr;						// remove this buffer array
			}
			else {
				cout << "one query out of bound" << endl;
			}
		}

		//Case 3: input is a stop/STOP command
		else if ((lread[0] == 's' || lread[0] == 'S') && (lread[1] == 't' || lread[1] == 'T') && (lread[2] == 'o' || lread[2] == 'O') && (lread[3] == 'p' || lread[3] == 'P'))
		{
			break;
		}
	}

	inputFile.close();
	outputFile.close();
	cout << "\nExecution completed...\n";
	return 0;
}