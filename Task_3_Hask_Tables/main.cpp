
#include <locale.h>
#include "Hash.h"
#include "Binary.h"
#include "HashBinary.h"


using namespace std;

void main()
{
	setlocale(LC_ALL, "ru");
	srand(time(NULL));

	//Переменные для пользовательского выбора.
	int userMenuChoice = -1;

	//Переменные для работы программы
	HashTable* table = new HashTable();
	string binFileName;
	bool isBinFileNameEntered = false;

	// Главный меню-цикл.
	while (userMenuChoice != 7)
	{
		std::system("cls");
		cout << "Практическая работа #3 ИКБО-03-21 Мазанов А.Е. Вариант 15\n\n"
			"Задание - Хэш-таблицы.\n"
			"~~~~~~~~~~~~~~~~~Меню~~~~~~~~~~~~~~~\n"
			"Таблица:\n";
		if (table->getSize() != 0)
		{
			table->printTable();
		}
		else
		{
			cout << "Не создана\n";
		}

		cout << "Введите [0], чтобы иннициализировать первоначальные данные:"
				"\n_____________/Ввести имя файла и создать его"
				"\n             \\Сгенерировать хэш-таблицу и синхронизировать с двоичным файлом.\n"
				"\nВведите [2], чтобы вставить элемент в таблицу."
				"\nВведите [3], чтобы получить значение по ключу."
				"\nВведите [4], чтобы удалить значение по ключу."
				"\nВведите [5], чтобы запустить тестирование."
				"\nВведите [], чтобы закончить работу программы."
				"\nВаш выбор: ";
		cin >> userMenuChoice;
		cin.ignore();
		switch (userMenuChoice)
		{
		case 0:
			cout << "Введите имя бинарного файла:\n";
			cin >> binFileName;
			binFileName += ".dat";

			int generatingRecordsAmount;
			cout << "Введите количество генерируемых записей: ";
			cin >> generatingRecordsAmount;
			int tableStartSize;
			cout << "Введите изначальный размер таблицы: ";
			cin >> tableStartSize;
			table = generateTable(generatingRecordsAmount, tableStartSize);

			break;
		case 5:
			testHeshT();
			break;

		case 1:
			
			break;

		case 2:
		{
			cout << "Введите ключ (дата рождения) в формате dd.mm: ";
			char key_date[6];
			cin.getline(key_date, 6, '\n');
			cout << "Введите имя (ИИИИ Ф.) родившегося в эту дату: ";
			char name[6];
			cin.getline(name, 8, '\n');
			table->add(key_date, name);
			break;
		}
			
		case 3:
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

		case 4:
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
			
		case 7:
			cout << "\nДо свидания!\n";
			break;

		default:
			cout << "\nЯ не понимаю вас.\n";
			cin.clear();
			break;
		}
		std::system("pause");
	}
}