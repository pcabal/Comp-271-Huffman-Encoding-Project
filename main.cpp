#include <iostream>
#include <string>

using namespace std;

typedef struct BinaryTreeNode {       //node has a left child, right child, a character, and the character's frequency
	BinaryTreeNode *left, *right;
    char character;
	int frequency;
} *treeNode;                        // BinaryTreeNode called treeNode with a left child, right child, and character with a frequency

struct BinaryTreeNode possibleChar[256] = {{0}};      //There are 256 possible characters that can be encoded with 8 bits
int n_nodes = 0, endOfQueue = 1;
char *tempcode[128] = {0}, temp[1024];  //These will be used in the tree to assign the binary and to read the tree for the Huffman code
treeNode q[255], *priorityQueue = q - 1;        //Create a priority queue made of tree nodes

treeNode new_node(char character, treeNode leftChild, treeNode rightChild, int frequency)        //Creates a new tree node with a frequency of the character's appearance, the character, and the left and right children
{
	treeNode n = possibleChar + n_nodes++;
	if (frequency)                  //If frequency, assign the node the character and the frequenct
    {
        n->character = character;   //character assigned
        n->frequency = frequency;   //frequency assigned
    }
	else
    {
		n->left = leftChild, n->right = rightChild;            //Else the children get created and the frequency of the node will be the sum of the children's frequency
		n->frequency = leftChild->frequency + rightChild->frequency;    //The node's frequency is the sum of the children's
	}
	return n; //return the new node
}

/* priority queue made of nodes(basically the frequency of the characters) */
void PriorityQueueInsert(treeNode n)        //Insert the node into the priority queue and make the priority focus on the frequency
{
	/* higher frequency has lower priority (less bits needed)
	   lower frequency nodes are moved up*/
	int j, i = endOfQueue++;
	while ((j = i / 2))
    {
		/* compare the new node frequency with the parent's frequency*/
		if (priorityQueue[j] -> frequency <= n -> frequency)
        {
            break;
        }
		priorityQueue[i] = priorityQueue[j], i = j;  //index at i becomes index at j, i = j to exit the while loop
	}
	priorityQueue[i] = n;       //Node is inserted
}

/*This function removes the top element at priorityQueue[1] and moves everything else up*/
treeNode priorityQueueremove()
{
	int i, j;
	i = 1;
	treeNode n = priorityQueue[i];  //node n is assigned the node in the priorityQueue at index 1

	if (endOfQueue < 2) return 0;       //End of queue keeps being decremented until the Queue is empty
	endOfQueue--;
	while ((j = i * 2) < endOfQueue)
    {
		if (j + 1 < endOfQueue && priorityQueue[j + 1]->frequency < priorityQueue[j]->frequency)
        {
            j++;
        }
        priorityQueue[i] = priorityQueue[j], i = j; //Node is moved up
	}
	priorityQueue[i] = priorityQueue[endOfQueue];
	return n;
}

/* Reads the tree and assigns a 1 (left child) or a 0 (right child), an encoded huffman string will eventually be created */
void Huffman_Code(treeNode n, char *c, int length)
{
	static char *create = temp;
	if (n->character)
    {
		c[length] = 0;
		strcpy(create, c); //copies the char into create
		tempcode[n->character] = create; //the nodes new character becomes the char that was copied into create
		create += length + 1;
		return;
	}
	c[length] = '0';
	Huffman_Code(n->left,  c, length + 1); //A left child is a zero

	c[length] = '1';
	Huffman_Code(n->right, c, length + 1); //A right child is a one
}

void HUFFMAN_TIME(const char *c)
///HUFFMAN BINARY TREE///
//In order to make the tree, the top two nodes in the priority queue (lowest frequency, high priority) are removed,
//a parent node is created with the sum of the prior two nodes as the frequency
//the two nodes that were removed become the left and right children
//The priority queue is emptied and nodes are made until there is only one node left
{
	int i, frequency[128] = {0};
	char character[100];

	/* count frequency for each character */
	while (*c)
    {
        frequency[(int)*c++]++;
    }
	/*heap/binary tree is made*/
	for (i = 0; i < 128; i++)
    {
		if (frequency[i])
        {
            PriorityQueueInsert(new_node(i, 0, 0,frequency[i]));    //Node from queue is put into the tree
        }
	}
	while (endOfQueue > 2)
    {    //While the priority queue still has nodes in it
		PriorityQueueInsert(new_node(0, priorityQueueremove(), priorityQueueremove(),0));
	}

	// Tree is traversed and huffman code is assigned

	Huffman_Code(priorityQueue[1], character, 0);
}

void encode(const char *c, char *temp)        //This code is used to get the huffman code for every char
                                            //I don't know why, but in main the code needed const char* and not just char*
{
	while (*c)
    {                                   //For each char
		strcpy(temp, tempcode[*c]);     //huffman binary code is assigned to it
		temp += strlen(tempcode[*c++]);
	}
}


int main(void)
{

    char temp[1000000]; //contains the Huffman binary codes
    string x; //user input


    cout << "Enter anything longer than one character to compress: ";
    getline(cin,x);  //If the input is a sentence, it gets read in as a sentence... CIN alone doesn't read in the sentence
    if(x.length() < 2)      //If the string only has one letter, the compression doesn't work
    {
        while (x.length() < 2)      //This will make sure that the input will be at least 2 strings
        {
            cout << "Enter anything longer than one character to compress: ";
            getline(cin,x);
        }
    }
    char *str = new char[x.length() + 1]; //Converts string to char*

    strcpy(str, x.c_str());

	HUFFMAN_TIME(str);              //Huffmanize the char*
	for (int i = 0; i < 128; i++)
    {
		if (tempcode[i]) printf("'%c': %s\n", i, tempcode[i]); //Got this from the cplusplus.com reference, takes the string and writes to console every char and it's respective Huffman code
    }

	cout << "\n\nNon-Huffman Code:\n" << str;       //Shows the encoded and decoded form of the input

	encode(str, temp);                              //Most frequent chars will have smaller huffman encoding, less frequent chars will have longer codes
	cout << "\n\nHuffman Code:\n" << temp;
    delete[] str;               //Delete the char* array
	return 0;
}
