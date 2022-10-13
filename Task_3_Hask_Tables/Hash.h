#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;


int hashFunction(const char key[6], unsigned tableSize);


struct HashTableCell
{
	char key_date[6];
	char name[8];
	int selfIndexInBinaryFile;
	bool isAddressOpen;
	bool isCellDeleted;

	HashTableCell(const char* key_date, const char* name)
	{
		strcpy_s(this->key_date, key_date);
		strcpy_s(this->name, name);
		selfIndexInBinaryFile = -1;
		isAddressOpen = true;
		isCellDeleted = false;
	}

	HashTableCell()
	{
		key_date[5] = '\0';
		name[5] = '\0';
		selfIndexInBinaryFile = -1;
		isAddressOpen = true;
		isCellDeleted = false;
	}
};

struct HashTable
{
private:
	unsigned size;
	unsigned closedAddressesAmount;
	HashTableCell* table;

	void resizeTable();

public:

	HashTable(unsigned size);

	unsigned getSize();

	char* get(const char* key);

	void add(const char* key, const char* value);

	void remove(const char* key);

	void printTable();
};


HashTable::HashTable(unsigned size = 0)
{
	this->size = size;
	closedAddressesAmount = 0;
	table = new HashTableCell[size];
}

unsigned HashTable::getSize()
{
	return size;
}


char* HashTable::get(const char* key)
{
	int startIndex = hashFunction(key, size);
	int index = startIndex;
	while (strcmp(table[index].key_date, key) != 0)
	{
		index++;
		index %= size;
		if (startIndex == index)
		{
			break;
		}
	}
	if (table[index].isAddressOpen == false
		&& table[index].isCellDeleted == false
		&& strcmp(table[index].key_date, key) == 0)
	{
		return table[index].name;
	}
	else
	{
		throw std::invalid_argument("Ничего не найдено");
	}
	
}


void HashTable::add(const char* key, const char* value)
{
	int startIndex = hashFunction(key, size);
	int index = startIndex;
	while (table[index].isAddressOpen == false)
	{
		index++;
		index %= size;
		if (startIndex == index)
		{
			break;
		}
	}

	strcpy_s(table[index].key_date, key);
	strcpy_s(table[index].name, value);
	table[index].isAddressOpen = false;

	closedAddressesAmount++;

	cout << "Элемент {" << key << ":" << value << "} вставлен по индексу " << index << '\n';

	if ((closedAddressesAmount / (double)size) > 0.75)
	{
		closedAddressesAmount = 0;
		this->resizeTable();
	}
}


void HashTable::remove(const char* key)
{
	int startIndex = hashFunction(key, size);
	int index = startIndex;
	while (strcmp(table[index].key_date, key) != 0 &&
		table[index].isAddressOpen == false)
	{
		index++;
		index %= size;
		if (startIndex == index)
		{
			break;
		}
	}
	if (strcmp(table[index].key_date, key) == 0 &&
		table[index].isCellDeleted == false)
	{
		table[index].isCellDeleted = true;

		cout << "Элемент {" << key << ":" << table[index].name << "} удалён с индекса " << index << '\n';
	}
	else
	{
		throw std::invalid_argument("Ничего не найдено");
	}
}


void HashTable::resizeTable()
{
	unsigned newSize = size * 2;
	HashTableCell* newTable = new HashTableCell[newSize];

	for (int i = 0; i < size; i++)
	{
		if (table[i].isCellDeleted == false && table[i].isAddressOpen == false)
		{
			int index = hashFunction(table[i].key_date, newSize);
			while (newTable[index].isAddressOpen == false)
			{
				index++;
				index %= newSize;
			}
			strcpy_s(newTable[index].key_date, table[i].key_date);
			strcpy_s(newTable[index].name, table[i].name);
			newTable[index].isAddressOpen = false;
			closedAddressesAmount++;
		}
	}
	delete[] table;
	table = newTable;
	size = newSize;
}

void HashTable::printTable()
{
	int indexFieldSize = to_string(size).length();
	string horizontalDelimeter = "";
	for (int i = 0; i < indexFieldSize + 22; i++)
	{
		horizontalDelimeter += "=";
	}

	cout << horizontalDelimeter << '\n';
	for (int i = 0; i < size; i++)
	{
		cout << "| " << setw(indexFieldSize)
			<< i + 1 << " | ";
		if (table[i].isAddressOpen != true && table[i].isCellDeleted != true)
		{
			cout << table[i].key_date << " | "
				<< table[i].name;
		}
		else
		{
			cout << setw(5) << "" << " | " 
				<< setw(7) << "";
		}
		cout << " |" << '\n' << horizontalDelimeter << '\n';
	}
}


int hashFunction(const char key[6], unsigned tableSize)
{
	int day = (key[0] - '0') * 10 + (key[1] - '0');
	int month = (key[3] - '0') * 10 + (key[4] - '0');
	int hashed = abs(sin(day) + cos(month)) * 10000 / 1;
	return hashed % tableSize;
}


vector<string> names
{
	"Вася", "Петя", "Гоша", "Дима", "Саша",
	"Лиза", "Анна", "Лена", "Крис", "Маня"
};
vector<string> surnames
{
	"П.", "Ж.", "А.", "Г.", "Д.", "П.", "Н."
};


int getRandomInt(int left, int right)
{
	return left + rand() % (right - left + 1);
}


HashTable* generateTable(int recordsAmount, int startSize)
{
	HashTable* newTable = new HashTable(startSize);
	for (int i = 0; i < recordsAmount; i++)
	{
		int day = getRandomInt(1, 30);
		int month = getRandomInt(1, 12);
		string date;
		if (day > 9)
		{
			date += to_string(day);
		}
		else
		{
			date += '0' + to_string(day);
		}
		date += '.';
		if (month > 9)
		{
			date += to_string(month);
		}
		else
		{
			date += '0' + to_string(month);
		}
		string name = names.at(getRandomInt(0, names.size() - 1)) +
			" " + surnames.at(getRandomInt(0, surnames.size() - 1));
		newTable->add(date.c_str(), name.c_str());
	}
	return newTable;
}


void testHeshT()
{
	unsigned startSize = 3;
	HashTable* newTable = new HashTable(startSize);

	//тест 1 - добавление элемента без коллизии
	string test1Key = "10.10";
	string test1Value = "Aaaa B.";
	newTable->add(test1Key.c_str(), test1Value.c_str());
	if (strcmp(newTable->get(test1Key.c_str()), test1Value.c_str()) != 0)
	{
		throw std::exception("Ошибка в: Тест 1");
	}

	//тест 2 - добавление элемента с возникновением коллизии
	string test2Key = "10.20";
	string test2Value = "Bbbb C.";
	newTable->add(test2Key.c_str(), test2Value.c_str());
	if (strcmp(newTable->get(test2Key.c_str()), test2Value.c_str()) != 0)
	{
		throw std::exception("Ошибка в: Тест 2");
	}

	//тест 3 - добавление элемента с возникающих рехэшированием
	string test3Key = "20.20";
	string test3Value = "Cccc D.";
	newTable->add(test3Key.c_str(), test3Value.c_str());
	if (strcmp(newTable->get(test3Key.c_str()), test3Value.c_str()) != 0
		|| startSize * 2 != newTable->getSize())
	{
		throw std::exception("Ошибка в: Тест 3");
	}

	//тест 4 - получение элемента, который есть в таблице
	string test4Key = "20.20";
	string test4Value = "Cccc D.";
	if (strcmp(newTable->get(test4Key.c_str()), test4Value.c_str()) != 0)
	{
		throw std::exception("Ошибка в: Тест 4");
	}

	//тест 5 - получение несуществующего элемента
	try
	{
		string test5Key = "30.30";
		newTable->get(test5Key.c_str());
		throw std::exception("Ошибка в: Тест 5");
	}
	catch (const std::exception& invArg)
	{
		//Всё нормально - метод вернул исключение о том, что ничего не нашел
	}

	//тест 6 - удаление существующего элемента
	try
	{
		string test6Key = "20.20";
		newTable->remove(test6Key.c_str());
	}
	catch (const std::exception& invArg)
	{
		throw std::exception("Ошибка в: Тест 6");
	}

	//тест 7 - удаление несуществующего элемента
	try
	{
		string test7Key = "30.30";
		newTable->remove(test7Key.c_str());
		throw std::exception("Ошибка в: Тест 7");
	}
	catch (const std::exception& invArg)
	{
		//Всё нормально - метод вернул исключение о том, что ничего не нашел
	}

	//тест 8 - получение элемента, располагающегося после удалённого элемента (с одинаковыми хэшами)
	string test8Key = "30.40";
	string test8Value = "Dddd E.";
	newTable->add(test8Key.c_str(), test8Value.c_str());
	if (strcmp(newTable->get(test8Key.c_str()), test8Value.c_str()) != 0)
	{
		throw std::exception("Ошибка в: Тест 8");
	}
	
	cout << "Тестирование успешно!\n";
}

