#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include "Soundtrack.h"
#include "AVL.h"

using namespace std;

template <class T>
void Traversal(T dataProc)
{
	cout << dataProc << endl;
}

void Traversal1950s(soundtrack dataProc)
{
	stringstream ss;
	char c;
	int yearRecorded;

	ss << dataProc.getYearRecorded();
	if(ss >> yearRecorded)
	{
		if(yearRecorded >= 1950 && yearRecorded <= 1959) 
		cout << dataProc << endl;
		else if(ss >> c && c == '/')
		{
			if(ss >> yearRecorded)
			{
				if(yearRecorded >= 50 && yearRecorded <= 59) 
					cout << dataProc << endl;
			}
		}
	}
}

// Test driver program
int main()
{
	cout << "Create and populate AVL tree : " << endl << endl;

	AvlTree<soundtrack> tree;

	string fileName("Topic F Soundtrack.txt");
	ifstream inFile(fileName.c_str());

	while(!inFile.is_open())
	{
		cout << "The file \"" << fileName << "\" not found. Enter another file : "; getline(cin, fileName);
		inFile.clear(); inFile.open(fileName.c_str()); cout << endl;
	}

	int yearReleased;
	stringstream ss;
	soundtrack track;
	soundtrack track2;
	while(inFile >> track)
	{
		track2 = soundtrack();
		track2.setCatalogNumber(track.getCatalogNumber());

		ss << track.getYearReleasedString();
		if(!(ss >> yearReleased))
		{
			cout << endl;
			cout << track.getYearReleasedString() << " is not a numeric value. This input ignored : " << endl;
			cout << track << endl;
		} 
		else if(tree.AVL_Retrieve(track2, track2))
		{
			cout << track << " is a duplicate" << endl;
		}
		else tree.AVL_Insert(track);

		ss.str("");
		ss.clear();
	}
	inFile.close();

	cout << endl;
	cout << "************************************ Press Enter to continue  -->"; cin.get();

	cout << "Get item with key \"FSMBox 03 Disc 8\" : " << endl << endl;

	track2 = soundtrack();
	track2.setCatalogNumber("FSMBox 03 Disc 8");

	if(tree.AVL_Retrieve(track2, track2))
	{
		cout << "Found an item with key \"FSMBox 03 Disc 8\"" << endl << endl;		
	}
	else
	{
		cout << "No items found key \"FSMBox 03 Disc 8\"" << endl << endl;			
	}

	cout << endl;

	cout << "Get item with key \"FSMBox 07 Disc 8\" : " << endl << endl;

	track2 = soundtrack();
	track2.setCatalogNumber("FSMBox 07 Disc 8");

	if(tree.AVL_Retrieve(track2, track2))
	{
		cout << "Found an item with key \"FSMBox 07 Disc 8\"" << endl << endl;		
	}
	else
	{
		cout << "No items found key \"FSMBox 07 Disc 8\"" << endl << endl;			
	}

	cout << endl;
	cout << "************************************ Press Enter to continue  -->"; cin.get();

	cout << "Listing of all items in the tree : (There are " << tree.AVL_Count() << " items in the table) " << endl << endl;

	tree.AVL_Traverse(&(Traversal<soundtrack>));

	cout << endl;
	cout << "************************************ Press Enter to continue  -->"; cin.get();

	cout << endl;
	cout << "List all soundtracks recorded in the 1950s : " << endl << endl;

	tree.AVL_Traverse(&(Traversal1950s));

	cout << endl;
	cout << "************************************ Press Enter to continue  -->"; cin.get();

	cout << "Delete all items with key \"FSM V8N11\"" << endl << endl;

	track2 = soundtrack();
	track2.setLabel("FSM");
	track2.setCatalogNumber("V8N11");

	if(tree.AVL_Retrieve(track2, track2) && tree.AVL_Delete(track2))
	{
		cout << track2 << endl << "has been deleted" << endl << endl;
	}
	else
	{
		cout << "NO items for \"FSM V8N11\"" << endl << endl;
	}

	cout << "Again delete all items with key \"FSM V8N11\"" << endl;

	track2 = soundtrack();
	track2.setLabel("FSM");
	track2.setCatalogNumber("V8N11");

	if(tree.AVL_Retrieve(track2, track2) && tree.AVL_Delete(track2))
	{
		cout << track2 << endl << "has been deleted" << endl;
	}
	else
	{
		cout << "NO items for \"FSM V8N11\"" << endl;
	}

	cout << endl;
	cout << "************************************ Press Enter to continue  -->"; cin.get();

	cout << "Listing of all items in the tree : (There are " << tree.AVL_Count() << " items in the table) " << endl << endl;

	tree.AVL_Traverse(&(Traversal<soundtrack>));

	cout << endl;
	cout << "Program Ending" << endl << endl;

	cout << "Press Enter to end -->"; cin.get();

	return 0;
}