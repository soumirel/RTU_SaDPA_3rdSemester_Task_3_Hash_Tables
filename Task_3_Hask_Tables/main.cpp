
#include <locale.h>
#include "Hash.h"
#include "Binary.h"
#include "HashBinary.h"


using namespace std;

void main()
{
	setlocale(LC_ALL, "ru");
	srand(time(NULL));

	//���������� ��� ����������������� ������.
	int userMenuChoice = -1;

	//���������� ��� ������ ���������
	HashTable* table = new HashTable();
	string binFileName;
	bool isBinFileNameEntered = false;

	// ������� ����-����.
	while (userMenuChoice != 7)
	{
		std::system("cls");
		cout << "������������ ������ #3 ����-03-21 ������� �.�. ������� 15\n\n"
			"������� - ���-�������.\n"
			"~~~~~~~~~~~~~~~~~����~~~~~~~~~~~~~~~\n"
			"�������:\n";
		if (table->getSize() != 0)
		{
			table->printTable();
		}
		else
		{
			cout << "�� �������\n";
		}

		cout << "������� [0], ����� ����������������� �������������� ������:"
				"\n_____________/������ ��� ����� � ������� ���"
				"\n             \\������������� ���-������� � ���������������� � �������� ������.\n"
				"\n������� [2], ����� �������� ������� � �������."
				"\n������� [3], ����� �������� �������� �� �����."
				"\n������� [4], ����� ������� �������� �� �����."
				"\n������� [5], ����� ��������� ������������."
				"\n������� [], ����� ��������� ������ ���������."
				"\n��� �����: ";
		cin >> userMenuChoice;
		cin.ignore();
		switch (userMenuChoice)
		{
		case 0:
			cout << "������� ��� ��������� �����:\n";
			cin >> binFileName;
			binFileName += ".dat";

			int generatingRecordsAmount;
			cout << "������� ���������� ������������ �������: ";
			cin >> generatingRecordsAmount;
			int tableStartSize;
			cout << "������� ����������� ������ �������: ";
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
			cout << "������� ���� (���� ��������) � ������� dd.mm: ";
			char key_date[6];
			cin.getline(key_date, 6, '\n');
			cout << "������� ��� (���� �.) ����������� � ��� ����: ";
			char name[6];
			cin.getline(name, 8, '\n');
			table->add(key_date, name);
			break;
		}
			
		case 3:
		{
			char key_date[6];
			cout << "������� ���� (���� � ������� dd.mm) �� �������� ������ ������: ";
			cin.getline(key_date, 6, '\n');
			char name[8];
			cout << "�� �����: " << key_date << ' ';
			try
			{
				cout << "������� �������� " << table->get(key_date) << '\n';
			}
			catch (const std::invalid_argument& invArg)
			{
				cout << invArg.what() << '\n';
			}
			break;
		}

		case 4:
			char key_date[6];
			cout << "������� ���� (���� � ������� dd.mm) �� �������� ������� ������: ";
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
			cout << "\n�� ��������!\n";
			break;

		default:
			cout << "\n� �� ������� ���.\n";
			cin.clear();
			break;
		}
		std::system("pause");
	}
}