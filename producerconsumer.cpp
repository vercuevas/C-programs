// producerconsumer.cpp : Defines the entry point for the console application.
//
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

class sortItems{
private:
	mutex m1, m2;
	condition_variable con;	
	once_flag iflag;
	once_flag oflag;
	ifstream ifile;
	ofstream ofile;
	vector<char> vlist;
	string item;
	int size;
	bool isSorted;
	
	struct Node
	{
		char key;
		struct Node *left, *right;
	};

public:
	sortItems()
	{
		item = "";		
		isSorted = false;		
		
	}
	void openItemFile(char* filename) {

		call_once(iflag, [&]() {ifile.open(filename); });
		
		if (!ifile.is_open()) {
			printf("%s did not open\n\r", filename);
			exit(1);
		}
	}
	void openOutFile(char *filename) {
		call_once(oflag, [&]() {ofile.open(filename); });
		if (!ofile.is_open()) {
			printf("%s did not open\n\r", filename);
			exit(1);
		}
	}
	void writeOutFile()
	{			
		std::unique_lock<mutex> locker2(m2);
		
		int len = vlist.size();
		for (int i = 0; i < len; i++)
		{
			ofile << vlist[i];
			if (i != (vlist.size() - 1))
				ofile << ","  ;
		}
		ofile << endl;	
		vlist.clear();
		vlist.resize(0);
		locker2.unlock();
		con.notify_one();
		
	}
	void sortSelect(char* sortID)
	{	
		unsigned n = 0;
		unsigned end = 0;
				
		while(true){
			std::this_thread::sleep_for(chrono::milliseconds(30));
			n = vlist.size();
			end = vlist.size();			
			std::unique_lock<mutex> locker(m1);			
			con.wait(locker, [&]() {return vlist.size() == item.length(); });
			n = vlist.size();
			end = vlist.size();
			if (vlist.empty())
				break;
			switch (*sortID) {
			case 't': treeSort(vlist,n);
				isSorted = true;
				printf("tree sort...\n\r");
				break;
			case 'm': mergeSort( 0, n - 1);
				isSorted = true;
				printf("merge sort...\n\r");
				break;
			case 'T': treeSort(vlist, n);
				isSorted = true;
				printf("tree sort...\n\r");
				break;
			case 'M': mergeSort( 0, n - 1);
				isSorted = true;
				printf("merge sort...\n\r");
				break;
			default: printf("Please select t for tree sort or m for merge sort");
				exit(1);
				break;
			}			
			
			writeOutFile();			
			isSorted = false;
			ofile << endl;
			item.clear();						
			locker.unlock();
			con.notify_one();			
		}
	}
	void closeOutFile()
	{
		ofile.close();
	}
	void closeItemFile()
	{
		ifile.close();
	}
	void readItem() {
		printf("starting to read from input file...\n\r");		
		
		int i = 0,j=0,len = 0;		
		while (!ifile.eof())
		{				
			std::unique_lock<std::mutex> locker(m1);
			con.wait(locker, [&]() {return vlist.empty(); });
			getline(ifile, item,'\n');			
			i = 0;			
			
			while (item[i] != '\0')//remove spaces from item if any
			{
				if (item[i] == ' ') {
					item.erase(i, 1);
					printf("Space found, thread will sleep for 1 second ....\n\r");
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				vlist.push_back(item[i]);//put item from file into vector one char at a time
				i++;
				len = item.length();				
			}
			if (ifile.eof())
				break;
				printf("vlist size: %ld \n\r", vlist.size());	
				
				size = vlist.size();	
				
				for (i = 0; i< len; i++)
					printf("%c",vlist[i]);	
				printf("\n\r");
				
				locker.unlock();
				con.notify_one();
		}
		closeItemFile();
		closeOutFile();		
	}
	void storeSorted(Node *root, vector<char>& vlist, int &i)
	{
		if (root != NULL)
		{
			storeSorted(root->left, vlist, i);
			vlist[i++] = root->key;
			storeSorted(root->right, vlist, i);
		}
	}
	/* A  function to insert a new
	Node with given key in Binary Sort Tree */
	Node* insert(Node* node, char key)
	{
		/* If the tree is empty, return a new Node */
		if (node == NULL) return newNode(key);

		/* Otherwise, recur down the tree */
		if (key <= node->key)
			node->left = insert(node->left, key);
		else if (key >= node->key)
			node->right = insert(node->right, key);

		/* return the (unchanged) Node pointer */
		return node;
	}
	void treeSort(vector<char>& vlist, int n)
	{
		struct Node *root = NULL;

		// Construct the BST 
		root = insert(root, vlist[0]);
		for (int i = 1; i<n; i++)
			insert(root, vlist[i]);

		// Store inoder traversal of the BST 
		// in vlist[] 
		int i = 0;
		storeSorted(root, vlist, i);
	}
	struct Node *newNode(char item)
	{
		struct Node *temp = new Node;
		temp->key = item;
		temp->left = temp->right = NULL;
		return temp;
	}
	//Merge sort
	void mergeSort(int start, int end)
	{
		
		if (start >= end)
			return;
		int mid = (start + end) / 2;
		mergeSort(start, mid);
		mergeSort(mid + 1, end);
		merge(start, end);
		
	}
	void merge(int start, int end)
	{
		
		int mid = (start + end) / 2;
		int i = start;
		int j = mid + 1;
		int k = start;

		char temp[100];
		while (i <= mid && j <= end)
		{
			if (vlist[i] < vlist[j])
				temp[k++] = vlist[i++];
			else
				temp[k++] = vlist[j++];
		}
		while (i <= mid) {
			temp[k++] = vlist[i++];
		}
		while (j <= end) {
			temp[k++] = vlist[j++];
		}
		//copy all elements to vlist
		for (int i = start; i <= end; i++) {
			vlist[i]=temp[i];
		}
		
	}
	
};

int main(int argc,char *argv[])
{
	std::vector<char> vlist;
	char *sortID;		
	sortItems s;
	std::thread t1,t2,t3,t4;
	
	if (argc < 4)
	{	
		printf( "Not enough arguments");
		exit(1);
	}
	if (argc > 4)
	{
		printf( "Too many arguments");
		exit(1);
	}
	
	sortID = argv[3];
	
	t1 = std::thread (&sortItems::openItemFile, &s, std::move(argv[1]));
	t2 = std::thread (&sortItems::openOutFile, &s, std::move(argv[2]));
	t3 = std::thread (&sortItems::readItem, &s );
	t4 = std::thread (&sortItems::sortSelect,&s, std::move(sortID) );	
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();	
	
	return 0;
}


