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
		cout << cell.selfIndexInBinaryFile << " "
			<< cell.key_date << " " << cell.name << '\n';
		binFile.read((char*)&cell, sizeof(cell));
	}
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


void addRecord(string binFileName, char* date, char* name, int recordsAmount)
{
	ofstream binFile;
	binFile.open(binFileName, ios::out | ios::binary | std::ios::app);
	HashTableCell* cell = new HashTableCell(date, name, recordsAmount);
	cell->isAddressOpen = false;
	binFile.write((char*)cell, sizeof(HashTableCell));
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


void generateBinFile(string binFileName, int recordsAmount)
{
	ofstream binFile;
	binFile.open(binFileName, ios::out | ios::binary);

	HashTableCell* currentCell = new HashTableCell();
	for (int i = 0; i < recordsAmount; i++)
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
		strcpy_s(currentCell->key_date, key_date.c_str());
		string name = names.at(getRandomInt(0, names.size() - 1)) +
			" " + surnames.at(getRandomInt(0, surnames.size() - 1));
		strcpy_s(currentCell->name, name.c_str());
		currentCell->selfIndexInBinaryFile = i;
		binFile.write((char*)currentCell, sizeof(HashTableCell));
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