//author: Gregory Austin Murphy
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

string ReadLine(fstream * CurrentFile)
{
	string retval;
	char in;
	while(in != '\n' && in != EOF)
	{
		retval += in;
		in = (*CurrentFile).get();
	}
	return retval;
}

bool FindMatch(string key, string searchspace)
{
	size_t location = searchspace.find(key);
	if(location == string::npos)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void TokenizeLine(string line, vector<string> * retbuf)
{
	retbuf->clear();
	string temp = "";
	for(int i = 0; i < line.size(); i++)
	{
		if(line[i] != '|')
		{
			temp += line[i];
		}
		else
		{
			retbuf->push_back(temp);
			temp.clear();
		}
	}
	retbuf->push_back(temp);
}

bool GetNextTokenStream(fstream * CurrentFile, vector<string> * retbuf)
{
	if((*CurrentFile).is_open() && !(*CurrentFile).eof())
	{
		string line = ReadLine(CurrentFile);
		if(CurrentFile->eof())
		{
			return false;
		}
		TokenizeLine(line, retbuf);
		return true;
	}
	else
	{
		return false;
	}
}

struct EmployeeProfile
{
	string Name;
	string Salary;
	string Address;
};

int BuildList(fstream * File, string Key, vector<EmployeeProfile> * retbuf)
{
	vector<string> CurrentLine;
	bool MoreTokens = GetNextTokenStream(File, &CurrentLine);
	while(MoreTokens)
	{
		if(CurrentLine.size() == 2)
		{
			if(FindMatch(Key,CurrentLine[1]))
			{
				EmployeeProfile temp;
				temp.Name = CurrentLine[0];
				temp.Address = CurrentLine[1];
				retbuf->push_back(temp);
			}
		}
		else
		{
			cout << "improper file formating detected, program aborting!" << endl;
			return -1;
		}
		MoreTokens = GetNextTokenStream(File, &CurrentLine);
	}
	return 0;
}

int AddSalaryData(fstream * File, vector<EmployeeProfile> * retbuf)
{
	vector<string> CurrentLine;
	bool MoreTokens = GetNextTokenStream(File, &CurrentLine);
	while(MoreTokens)
	{
		if(CurrentLine.size() == 2)
		{
			for(int i = 0; i < retbuf->size(); i++)
			{
				if(CurrentLine[0] == (*retbuf)[i].Name)
				{
					(*retbuf)[i].Salary = CurrentLine[1];
					break;
				}
			}
		}
		else
		{
			cout << "improper file formating detected, program aborting!" << endl;
			return -1;
		}
		MoreTokens = GetNextTokenStream(File, &CurrentLine);
	}
	return 0;
}

int main()
{
	fstream Addresses;
	Addresses.open("personnel_addresses.txt", fstream::in);
	fstream Salary;
	Salary.open("personnel_salaries.txt", fstream::in);
	if(!Addresses.is_open())
	{
		cout << "program failed to open file one: personnel_addresses.txt" << endl << "program will now terminate." << endl;
		return 1;
	}
	if(!Salary.is_open())
	{
		cout << "program failed to open file two: personnel_salaries.txt" << endl << "program will now terminate." << endl;
		return 2;
	}
	string Key;
	cout << "what town would you like to search for: ";
	cin >> Key;
	cout << endl;
	vector<EmployeeProfile> Employee;
	int flag = BuildList(&Addresses,Key,&Employee);
	if(flag < 0)
	{
		return 3;
	}
	flag = AddSalaryData(&Salary,&Employee);
	if(flag < 0)
	{
		return 4;
	}
	for(int i = 0; i < Employee.size(); i++)
	{
		cout << Employee[i].Name << ':' << Employee[i].Salary << endl;
	}
	return 0;
	
}