/*
Author: Nathan Smith
Title: Word Unscrambler

----------
Objective:
----------
From a supplied dictionary of words, the program asks the user to enter a string (scrambled or otherwise) and the program then determines
whether or not the user-provided string matches any from the dictionary.

--------
Details:
--------
-For the dictionary: The hash table uses a global variable to determine its size, it is not dynamically allocated, so the dictionary size must be known beforehand.
-The program uses a hash table to store the dictionary. Once the user asks for a string, it is hashed to find an index and then compared with the strings inside that hash index.
-To compare the two strings, they are sorted by insertion, then compared directly.
-Collisions on the hash table are solved by making linked lists for each hashed index. Words that collide are added to the end of the list in that index.
*/


#include <iostream>
#include <string>
#include <fstream>
using std::cin;
using std::cout;
using std::string;
using std::fstream;

// WordNode Linked List
struct WordNode
{
	string word;
	WordNode *next;
};
typedef WordNode *WordNodePtr;


// Determines size of table
// A prime number is used to reduce collisions
const int TABLE_SIZE = 101;
const string FILE_NAME = "dic.txt";	// dictionary file name


int string_hash(const string str);
void addToTable(WordNodePtr *hash_table, const string word);
void printTable(WordNodePtr *hash_table);
void deleteTable(WordNodePtr *hash_table);
string sortString(const string string);
void compare(WordNodePtr *hash_table, const string word);
void openDictionary(const string filename, WordNodePtr *AllWords);

int main()
{
	char repeat = '\0';
	string option;
	WordNodePtr AllWords[TABLE_SIZE] = { nullptr };
	
	openDictionary(FILE_NAME, AllWords);

	// menu
	cout << "Welcome to the unscrambler!\n";
	
	while (repeat == '\0')
	{
		option = "";
		repeat = '\0';
		cout << "1 - Print All\n"
			 << "3 - Exit\n"
			 << "Please include spaces!\n"
			 << "> ";
		getline(cin, option);

		// input formatting
		for (int i = 0;  option[i]; i++)
		{
			option[i] = tolower(option[i]);
		}

		// options: exit, print, compare
		// exit
		if (option ==  "3")
		{
			repeat = 'n';
			deleteTable(AllWords);
			break;
		}
		// print
		else if (option == "1")
		{
			printTable(AllWords);
			continue;
		}
		// compare
		else
		{
			option = sortString(option);
			compare(AllWords, option);
		}
	}
	// end of menu

	return 0;
}

// Fills the table with the words at hashed values
// Parameters:
//		- filename			name of dictionary file
//		- hash_table		pointer to the hash table
void openDictionary(const string filename, WordNodePtr *hash_table)
{
	string file_string;
	fstream in_file;
	in_file.open(FILE_NAME);
	if (in_file.fail())
	{
		cout << "File opening failure: Exiting.\n";
	}
	while (!(in_file.eof()))
	{
		getline(in_file, file_string);
		if (file_string != "")
		{
			addToTable(hash_table, file_string);
		}
	}
	in_file.close();
}


// Hash function
// Parameters:
//		- str				a string to be hashed in the function.
// Returns:
//		- hash_index		the index where the string is supposed to be stored.
int string_hash(const string str)
{
	int hash_index = 0;

	for (int i = 0; str[i]; i++)
	{
		hash_index += str[i];
	}
	hash_index %= TABLE_SIZE;

	return hash_index;
}

// Adds a new word to the hash table
// Parameters:
//		- hash_table			pointer to the hash table
//		- word					the word to be added to the hash table
void addToTable(WordNodePtr *hash_table, string word)
{
	WordNodePtr new_word = new WordNode;
	WordNodePtr temp_ptr = nullptr;

	new_word->word = word;
	new_word->next = nullptr;
	int position = string_hash(word);
	
	if (hash_table[position] == nullptr)
	{
		hash_table[position] = new_word;
	}
	else
	{
		temp_ptr = hash_table[position];
		while (temp_ptr->next != nullptr)
			temp_ptr = temp_ptr->next;
		temp_ptr->next = new_word;
	}
}


// Prints all the words in the hash table
// Parameters:
//		- hash_table				pointer to the hash table
void printTable(WordNodePtr *hash_table)
{
	cout << "List of available words to scramble: \n";
	WordNodePtr temp_ptr = nullptr;
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		temp_ptr = hash_table[i];
		if (temp_ptr != nullptr)
		{
			while (temp_ptr != nullptr)
			{
				if (temp_ptr->word == "")
					continue;
				cout << temp_ptr->word << "\n";
				temp_ptr = temp_ptr->next;
			}
		}
	}
	cout << "\n";
}


// Adds a new word to the hash table
// Parameters:
//		- hash_table				pointer to the hash table
void compare(WordNodePtr *hash_table, const string sorted_word)
{
	WordNodePtr temp_ptr = nullptr;
	string temp_string;
	int position = string_hash(sorted_word);
	bool found = false;

	temp_ptr = hash_table[position];

	if (temp_ptr != nullptr)
	{
		while (temp_ptr != nullptr && found == false)
		{
			temp_string = temp_ptr->word;
			temp_string = sortString(temp_string);
			if (temp_string == sorted_word)
			{
				found = true;
				cout << "The word is " << temp_ptr->word << "!\n\n";
			}
			temp_ptr = temp_ptr->next;
		}
	}
	if (!found)
	{
		cout << "There is no word that matches!\n\n";
	}
}


// Sorts a string by ASCII value
// Parameters:
//		- input_string			a raw input string from user or dictionary
// Returns:
//		- sorted_string			the input_string sorted by ASCII value
string sortString(const string input_string)
{
	// insertion sort from left to right
	string sorted_string = input_string;
	char temp = '\0';

	for (int i = 0, j = 0; sorted_string[i]; i++)
	{	
		j = i;
		while ((j > 0)  && (sorted_string[j-1] > sorted_string[j]))
		{
			temp = sorted_string[j];
			sorted_string[j] = sorted_string[j-1];
			sorted_string[j-1] = temp;
			j--;
		}
	}

	return sorted_string;
}

// Deletes the hash table
// Parameters:
//		- hash_table		pointer to the hash table
void deleteTable(WordNodePtr *hash_table)
{
	WordNodePtr prev;
	WordNodePtr temp_ptr;

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		temp_ptr = hash_table[i];
		while (temp_ptr != nullptr)
		{
			prev = temp_ptr;
			temp_ptr = temp_ptr->next;
			delete prev;
		}
	}
}