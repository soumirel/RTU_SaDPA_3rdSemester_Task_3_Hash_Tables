#pragma once

#include "Hash.h"
#include <fstream>
#include <istream>

using namespace std;

bool isFileExist(string fileName)
{
	ifstream file;
	file.open(fileName);
	char a[3];
	return file.good();
}


bool tryOpenFile(string fileName, char type)
{
	ifstream file;
	if (type == 't')
		file.open(fileName);
	if (type == 'b')
		file.open(fileName, ios::binary);
	if (file.is_open())
	{
		if (!file.good())
		{
			cout << "Ошибка с файлом " << fileName << '\n';
		}
		file.close();
		return true;
	}
	else
	{
		file.close();
		return false;
	}
}


void printBinFile(string binFileName)
{
	ifstream binFile;
	binFile.open(binFileName, ios::in | ios::binary);
	HashTableCell cell;
	binFile.read((char*)&cell, sizeof(cell));
	cout << "Содержимое бинарного файла:\n";
	while (!binFile.eof())
	{
		cout << cell.key_date << " " << cell.name << '\n';
		binFile.read((char*)&cell, sizeof(cell));
	}
}


void translateBinToText(string textFileName, string binFileName)
{
	ofstream textFile;
	ifstream binFile;
	textFile.open(textFileName);
	binFile.open(binFileName, ios::in | ios::binary);
	HashTableCell cell;
	binFile.read((char*)&cell, sizeof(HashTableCell));
	while (binFile.good())
	{
		textFile << cell.key_date << " " << cell.name;
		binFile.read((char*)&cell, sizeof(HashTableCell));
		if (!binFile.eof())
		{
			textFile << '\n';
		}
	}
}


void translateTextToBin(string textFileName, string binFileName)
{
	ifstream textFile;
	ofstream binFile;
	textFile.open(textFileName);
	binFile.open(binFileName, ios::out | ios::binary);
	while (!textFile.eof())
	{
		HashTableCell cell;
		char name[8];
		char key_date[6];
		name[7] = '\0';
		key_date[5] = '\0';

		textFile.read((char*)&key_date, 5);
		strcpy_s(cell.key_date, key_date);
		
		textFile.get();
		textFile.read((char*)&name, 7);
		strcpy_s(cell.name, name);

		textFile.get();

		binFile.write((char*)&cell, sizeof(HashTableCell));
	}

	textFile.close();
	binFile.close();
}


string getRecordByID(string binFileName, int ID)
{
	ifstream binFile;
	binFile.open(binFileName, ios::in | ios::binary);
	HashTableCell cell;
	string record = "";
	binFile.seekg(sizeof(HashTableCell) * (ID - 1), ios::beg);
	binFile.read((char*)&cell, sizeof(HashTableCell));
	record += cell.key_date;
	record += " ";
	record += cell.name;
	binFile.close();
	return record;
}


bool deleteRecordByKey(string binFileName, char recordKey[6])
{
	HashTableCell lastRecord;
	fstream binFile;

	binFile.open(binFileName, ios::in | ios::out | ios::binary);
	binFile.seekg(-(int)sizeof(HashTableCell), ios::end);
	binFile.read((char*)&lastRecord, sizeof(HashTableCell));

	binFile.seekg(0, ios::beg);
	HashTableCell currentRecord;
	while (binFile.good())
	{
		binFile.read((char*)&currentRecord, sizeof(HashTableCell));
		if (strcmp(currentRecord.key_date, recordKey) == 0)
		{
			binFile.seekg(-(int)sizeof(HashTableCell), ios::cur);
			binFile.write(reinterpret_cast<const char*>(&lastRecord),
				sizeof(HashTableCell));
			binFile.close();
			return true;
		}
	}
	binFile.close();
	return false;
}


HashTableCell getRecordByKey(string binFileName, char* recordKey)
{
	ifstream binFile;
	binFile.open(binFileName, ios::in | ios::binary);
	HashTableCell currentRecord;

	while (binFile.good())
	{
		binFile.read((char*)&currentRecord, sizeof(currentRecord));
		if (strcmp(currentRecord.key_date, recordKey) == 0)
		{
			binFile.close();
			return currentRecord;
		}
	}
	throw std::invalid_argument("There is no record by key");
}


void printFriendCell(HashTableCell cell)
{
	cout << cell.key_date << " день рождения у " << cell.name << '\n';
}


int countRecordsInBin(string binFileName)
{
	ifstream binFile;
	binFile.open(binFileName, ios::in | ios::binary);
	binFile.seekg(0, std::ios::end);
	int size = binFile.tellg();
	binFile.close();
	return size / sizeof(HashTableCell);
}


void createBinFile(string binFileName)
{
	ofstream binFile;
	binFile.open(binFileName, ios::out | ios::binary);

	HashTableCell currentcell;
	for (int i = 0; i < 10; i++)
	{
		int day = getRandomInt(1, 30);
		int month = getRandomInt(1, 12);
		string key_date;
		if (day > 9)
		{
			key_date += to_string(day);
		}
		else
		{
			key_date += '0' + to_string(day);
		}
		key_date += '.';
		if (month > 9)
		{
			key_date += to_string(month);
		}
		else
		{
			key_date += '0' + to_string(month);
		}
		strcpy_s(currentcell.key_date, key_date.c_str());
		string name = names.at(getRandomInt(0, names.size() - 1)) +
			" " + surnames.at(getRandomInt(0, surnames.size() - 1));
		strcpy_s(currentcell.name, name.c_str());
		binFile.write((char*)&currentcell, sizeof(HashTableCell));
	}
	binFile.close();
}


void copyRecordsByMonth(string binFileName, string textFileName, char month[3])
{
	ifstream binFile;
	binFile.open(binFileName, ios::in | ios::binary);
	ofstream textFile;
	textFile.open(textFileName);
	HashTableCell currentCell;
	while (binFile.good())
	{
		binFile.read((char*)&currentCell, sizeof(HashTableCell));
		char currentMonth[3];
		copy(currentCell.key_date + 3, currentCell.key_date + 6, currentMonth);
		if (strcmp(currentMonth, month) == 0)
		{
			textFile << currentCell.key_date << " " << currentCell.name << "\n";
		}
	}

}