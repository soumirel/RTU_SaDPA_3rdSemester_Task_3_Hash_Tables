
#include <locale.h>
#include "Hash.h"
#include "Binary.h"
#include "HashBinary.h"

using namespace std;

enum MenuState
{
	INIT = 0,
	PRINT_FILE = 1,
	PRINT_TABLE = 2,
	INSERT = 3,
	GET = 4,
	DELETE = 5,
	EXIT = 6
};

void main()
{
	setlocale(LC_ALL, "ru");
	srand(time(NULL));

	//Переменные для пользовательского выбора.
	int userMenuChoice = -1;

	//Переменные для работы программы
	HashTable* table = nullptr;
	string binFileName;
	int recordsAmount = 0;
	bool isDataInit = false;

	// Главный меню-цикл.
	while (userMenuChoice != EXIT)
	{
		std::system("cls");
		cout << "Практическая работа #3 ИКБО-03-21 Мазанов А.Е. Вариант 15\n\n"
			"Задание - Хэш-таблицы.\n"
			"~~~~~~~~~~~~~~~~~Меню~~~~~~~~~~~~~~~\n";

		cout << "Введите [0], чтобы иннициализировать первоначальные данные:"
			"\n\tВвести имя файла и сгенерировать его"
			"\n\tСинхронизировать хэш-таблицу с двоичным файлом.\n"
			"\nВведите [1], чтобы вывести содержимое двоичного файла."
			"\nВведите [2], чтобы вывести содержимое хэш-таблицы."
			"\nВведите [3], чтобы вставить элемент в таблицу."
			"\nВведите [4], чтобы получить значение по ключу."
			"\nВведите [5], чтобы удалить значение по ключу."
			"\nВведите [6], чтобы запустить тестирование."
			"\nВведите [], чтобы закончить работу программы."
			"\nВаш выбор: ";
		cin >> userMenuChoice;
		if (!isDataInit && userMenuChoice > 0 && userMenuChoice != EXIT)
		{
			cout << "Сначала иннициализируйте первоначальные данные!\n";
		}
		else
		{
			cin.ignore();
			switch (userMenuChoice)
			{
			case INIT:
				cout << "Введите имя бинарного файла: ";
				cin >> binFileName;
				binFileName += ".dat";

				int generatingRecordsAmount;
				cout << "Введите количество генерируемых записей: ";
				cin >> generatingRecordsAmount;
				generateBinFile(binFileName, generatingRecordsAmount);
				recordsAmount = generatingRecordsAmount;

				if (table != nullptr)
				{
					delete table;
				}
				table = new HashTable();
				synchronizeTableWithFile(binFileName, table);

				if (!isDataInit) isDataInit = true;

				break;

			case PRINT_FILE:
				printBinFile(binFileName);
				break;

			case PRINT_TABLE:
				table->printTable();
				break;

			case INSERT:
			{
				cout << "Введите ключ (дата рождения) в формате dd.mm: ";
				char key_date[6];
				cin.getline(key_date, 6, '\n');
				cout << "Введите имя (ИИИИ Ф.) родившегося в эту дату: ";
				char name[6];
				cin.getline(name, 8, '\n');
				table->add(key_date, name, ++recordsAmount);
				addRecord(binFileName, key_date, name, recordsAmount);
				break;
			}

			case GET:
			{
				char key_date[6];
				cout << "Введите ключ (дату в формате dd.mm) по которому искать запись: ";
				cin.getline(key_date, 6, '\n');
				char name[8];
				cout << "По ключу: " << key_date << ' ';
				try
				{
					cout << "найдено значение " << table->get(key_date) << '\n';
				}
				catch (const std::invalid_argument& invArg)
				{
					cout << invArg.what() << '\n';
				}
				break;
			}

			case DELETE:
				char key_date[6];
				cout << "Введите ключ (дату в формате dd.mm) по которому удалять запись: ";
				cin.getline(key_date, 6, '\n');
				char name[8];
				try
				{
					table->remove(key_date);
				}
				catch (const std::invalid_argument& invArg)
				{
					cout << invArg.what() << '\n';
				}
				break;

			case EXIT:
				cout << "\nДо свидания!\n";
				break;

			default:
				cout << "\nЯ не понимаю вас.\n";
				cin.clear();
				break;
			}
		}

		std::system("pause");
	}
}