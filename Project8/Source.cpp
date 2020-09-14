#pragma warning(disable:4996)
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <conio.h> 
#include <direct.h> 
#include <process.h> 
#include<filesystem>
using namespace std;
struct File {
	char *data;
	string name;
	long long size;
	string extension;
	int index;

};
struct Folder
{
	vector<Folder*> folders;
	vector<File*> files;
	struct Folder* prev;
	string name;
	int num_Folders;
	int num_Files;
	int index;
};
struct main_memory
{
	Folder *head, *current;
	bool Main_ram = false;
	long long total_Size = 0;
	long long max_Size = NULL;


	void create_Partition(string name)
	{
		head = new Folder();
		head->name = name;
		head->prev = NULL;
		head->num_Folders = 0;
		head->num_Files = 0;
		Main_ram = true;
		current = head;
	}

	void create_Folder(Folder *folder, string name)
	{
		Folder *temp = new Folder();
		temp->name = name;
		temp->prev = folder;
		temp->num_Folders = 0;
		temp->num_Files = 0;
		temp->index = folder->num_Folders;

		folder->folders.push_back(temp);
		folder->num_Folders++;

	}

	void create_file(Folder *folder, string name, char *data) {
		if (max_Size == NULL || total_Size + strlen(data) <= max_Size) {
			File *tempD = new File();
			tempD->data = data;
			tempD->name = name;
			tempD->extension = ".txt";
			tempD->size = strlen(data);
			tempD->index = folder->num_Files;

			folder->files.push_back(tempD);
			folder->num_Files++;

			total_Size += tempD->size;
			return;
		}
		cout << "Cannot add file" << endl;
	}




	void write_filefromDisk(Folder *folder, string path, string name) {


		for (int i = 0; i < folder->files.size(); i++) {
			if (name == folder->files[i]->name)
			{
				cout << "Cannot add file ";
				return;
			}
		}
		string extension;
		int ext = path.find_last_of(".");
		for (int i = ext; i < path.length(); i++) {
			extension += path[i];
		}

		char *p = new char[path.size() + 1];
		strcpy(p, path.c_str());
		FILE *temp;
		char *buffer;
		unsigned long long lsize;
		size_t result;
		temp = fopen(p, "rb");
		if (temp == NULL) {
			cout << "Path is invalid!" << endl;
			return;
		}

		fseek(temp, 0, SEEK_END);
		lsize = ftell(temp);
		rewind(temp);
		if (max_Size == NULL || lsize + total_Size <= max_Size) {

			buffer = new char[lsize];
			if (buffer == NULL) {
				cout << " Error in memory" << endl;
				return;
			}

			result = fread(buffer, 1, lsize, temp);
			if (result != lsize) {
				cout << "Reading Error" << endl;
				return;
			}

			File *tempD = new File();
			tempD->data = buffer;
			tempD->extension = extension;
			tempD->name = name;
			tempD->size = lsize;
			tempD->index = folder->num_Files;

			folder->files.push_back(tempD);
			folder->num_Files++;

			total_Size += lsize;
			return;
		}
		cout << "Cannot add file" << endl;
	}


	void deleteFile(Folder *folder, int fileIndex) {
		total_Size -= folder->files[fileIndex]->size;
		folder->num_Files--;
		folder->files[fileIndex]->data = NULL;
		folder->files[fileIndex]->name.erase();
		folder->files[fileIndex]->size = NULL;
		folder->files[fileIndex]->extension.erase();
		folder->files[fileIndex]->index = NULL;
		folder->files.erase(folder->files.begin() + fileIndex);
	}

	void reSortSubFolders(Folder *folder)
	{
		for (int i = 0; i < folder->num_Folders; i++)
		{
			folder->folders[i]->index = i;
		}
	}

	void deleteFolder(Folder *folder) {
		if (folder->prev != NULL) {
			Folder *temp = folder;
			folder = folder->prev;
			for (int i = 0; i < temp->num_Folders; i++) {
				deleteFolder(temp->folders[i]);
			}
			for (int j = 0; j < temp->num_Files; j++) {
				deleteFile(temp, j);
			}
			folder->num_Folders--;
			folder->folders.erase(folder->folders.begin() + temp->index);
			reSortSubFolders(folder);
		}
		else
			cout << "Can't delete main_folder " << endl;
	}



	void write_FiletoDisk(Folder *folder, int index, string p = "") {
		p += "\\" + folder->files[index]->name + folder->files[index]->extension;
		char *path = new char[p.size() + 1];
		strcpy(path, p.c_str());
		FILE * pFile;
		pFile = fopen(path, "wb");
		if (pFile == NULL) {
			cout << "File could not be created!" << endl;
			return;
		}
		fwrite(folder->files[index]->data, sizeof(char), folder->files[index]->size, pFile);
		fclose(pFile);
	}

	void change_Directory(Folder* folder, string index) {
		if (index == ".." && folder != head) {
			current = folder->prev;
			return;
		}
		for (int i = 0; i < current->folders.size(); i++) {
			if (current->folders[i]->name == index)
			{
				current = current->folders[i];
			}
		}
	}
	void show_list(Folder *folder) {
		if (folder->num_Folders > 0) {
			cout << "Folders: \n";
			for (int i = 0; i < folder->num_Folders; i++) {
				cout << i << ": " << folder->folders[i]->name << endl;
			}
		}

		if (folder->num_Files > 0) {
			cout << "Files: \n";
			for (int i = 0; i < folder->num_Files; i++) {
				cout << i << ": " << folder->files[i]->name << endl;
			}
		}
	}

};
main_memory *_folder = NULL;
void main()
{
	_folder = new main_memory();
	_folder->create_Partition("root");
	string command;

	cout
		<< "mkdir: Create new folder" << endl
		<< "cd: Change current directory" << endl
		<< "ls: show directories " << endl
		<< "addfi: create new file" << endl
		<< "cpffd: Copy file from disk " << endl
		<< "cpftd: Paste File to disk" << endl
		<< "delfol: Remove directory" << endl
		<< "delfi: Remove file" << endl
		<< "cls: Clear Screen" << endl
		<< "exit: Close program" << endl
		<< "help: to display commands" << endl;





	do {
		cout << _folder->current->name + ">>";
		cin >> command;
		if (command == "mkdir")
		{
			cout << "Enter folder name ";
			string fname;
			cin >> fname;
			_folder->create_Folder(_folder->current, fname);
		}
		if (command == "cd") {
			string com;
			cin >> com;
			_folder->change_Directory(_folder->current, com);
		}
		if (command == "addfi") {
			cout << "Enter file name: ";
			string fname;
			cin >> fname;
			cout << "Enter data: ";
			string data;
			cin >> data;
			char *datax = new char();
			strcpy(datax, data.c_str());
			_folder->create_file(_folder->current, fname, datax);
		}
		if (command == "cpffd") {
			string name;
			cout << "Enter file name: ";
			cin >> name;
			cout << "Enter file path: ";
			string path;
			cin >> path;
			_folder->write_filefromDisk(_folder->current, path, name);
		}

		if (command == "delfol") {
			cout << "Enter Folder name: ";
			string name;
			cin >> name;
			for (int i = 0; i <_folder->current->folders.size(); i++) {
				if (_folder->current->folders[i]->name == name) {
					_folder->deleteFolder(_folder->current->folders[i]);
					break;
				}
			}
		}
		if (command == "delfi") {
			cout << "Enter File name: ";
			string name;
			cin >> name;
			for (int i = 0; i < _folder->current->files.size(); i++) {
				if (_folder->current->files[i]->name == name) {
					_folder->deleteFile(_folder->current, i);
					break;
				}
			}
		}

		if (command == "cpftd") {
			string name;
			string path;
			bool isFound = false;
			cout << "Enter name: ";
			cin >> name;
			for (int i = 0; i < _folder->current->files.size(); i++) {
				if (_folder->current->files[i]->name == name) {
					isFound = true;
					cout << "Enter path: ";
					cin >> path;
					_folder->write_FiletoDisk(_folder->current, i, path);
				}
			}
			if (!isFound)
				cout << "There is no file with this name." << endl;
		}
		if (command == "cls") {
			system("CLS");
		}
		if (command == "ls") {
			_folder->show_list(_folder->current);
		}
		if (command == "help") {

			cout
				<< "mkdir: Create new folder" << endl
				<< "cd: Change current directory" << endl
				<< "ls: show directories " << endl
				<< "addf: create new file" << endl
				<< "cpffd: Copy file from disk " << endl
				<< "cpftd: Paste File to disk" << endl
				<< "delfol: Remove directory" << endl
				<< "delfi: Remove file" << endl
				<< "cls: Clear Screen" << endl
				<< "exit: Close program" << endl
				<< "help: to display commands" << endl;
		}
	} while (command != "exit");
}