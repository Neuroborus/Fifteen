// stdafx.cpp : source file that includes just the standard includes
//	Sceleton.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

extern HINSTANCE hInst;


LRESULT CALLBACK WFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HWND hBut[16];
	static int sumOfLesserElements;	//	Для проверки возможности сложить выстроеную комбинацию (проверка по Лойду)
	int indexOfBlank; // Индекс для пустого квадрата
	int differenceOfIndexes; // Для расчета возможности хода по разности индексов выбранного и пустого квадрата
	static int mas[16] = {};
	static LONG sx4, sy4;
	int i, j; //index
	static BOOL gameActive = false;
	

	switch (message)
	{
	case WM_CREATE:
		srand(time(0));
		for (i = 0; i < 16; i++)
			hBut[i] = CreateWindow("Button", "", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hwnd, 0, hInst, 0);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0; i < 16; i++)
			MoveWindow(hBut[i], (i % 4)*sx4, (i / 4)*sy4, sx4, sy4, 1);
		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
		if (gameActive)
		{
			for (i = 0; i < 16; i++)
				if (!mas[i])
					indexOfBlank = i;
			for (i = 0; i < 16; i++) {
				if ((HWND)lParam == hBut[i])
				{
					differenceOfIndexes = i - indexOfBlank;

					switch (i) {	//	Проверка возможности и замена эл-тов, в зависимости от расположения

					case 0: case 4: case 8: case 12:	//	Левая сторона
						if (differenceOfIndexes == -1 || differenceOfIndexes == -4 || differenceOfIndexes == 4)
							std::swap(mas[i], mas[indexOfBlank]);

						break;

					case 3: case 7: case 11: case 15:	//	Правая сторона
						if (differenceOfIndexes == 1 || differenceOfIndexes == -4 || differenceOfIndexes == 4)
							std::swap(mas[i], mas[indexOfBlank]);

						break;

					default:	//	Середина
						if (differenceOfIndexes == -1 || differenceOfIndexes == 1 || differenceOfIndexes == -4 || differenceOfIndexes == 4)
							std::swap(mas[i], mas[indexOfBlank]);

						break;
					}
					ShowNum(mas, hBut);	//	Перестройка
				}
			}
		}


		switch (LOWORD(wParam))
		{
		case 40001: //Game New
			if (gameActive)
				if (MessageBox(hwnd, "Start new game?", "Are you sure?", MB_YESNO | MB_ICONQUESTION) == IDNO)
					break;
			gameActive = true;
		rebuild:
			for (i = 0; i < 16; i++) {	//	Заполнение поля
				mas[i] = rand() % 16;
			anotherEl:
				for (j = 0; j < i; j++)
					if (mas[j] == mas[i])
					{
						mas[i] = rand() % 16;
						goto anotherEl;
					}
			}
			sumOfLesserElements = 0;
			for (i = 0; i < 16; i++)	//	Проверка по Лойду на возможность собрать конечную комбинацию
				for (j = i; j < 16; j++)
					if (mas[i] > mas[j])
						sumOfLesserElements++;
			if (sumOfLesserElements % 2)
				goto rebuild;

			ShowNum(mas, hBut);	//	Отображение изменений
			break;



		case 40002:	//Game Save
		{
			std::fstream fout("save.dat", std::ios::out | std::ios::binary);
			if (!fout)
			{
				MessageBox(hwnd, "Can't open file for reading.", "Nope.", MB_OK | MB_ICONERROR);
				break;
			}
			if (MessageBox(hwnd, "Last save will be deleted.", "Are you sure?", MB_YESNO | MB_ICONQUESTION) == IDNO)
				break;

			for (i = 0; i < 16; i++)
				fout << mas[i] << ' ';
			fout.close();
			MessageBox(hwnd, "Game saved.", "Succes!", MB_OK | MB_ICONASTERISK);

			break;
		}

		case 40003:	//Game Load
		{
			std::fstream fin("save.dat", std::ios::in | std::ios::binary);
			if (gameActive)
				if (MessageBox(hwnd, "Load another game?", "Are you sure?", MB_YESNO | MB_ICONQUESTION) == IDNO)
					break;
			gameActive = true;
			if (!fin)
				MessageBox(hwnd, "Can't open file for writing.", "Nope.", MB_OK | MB_ICONERROR);

			for (i = 0; i < 16; i++)
				fin >> mas[i];
			ShowNum(mas, hBut);
			fin.close();
			break;
		}


		case 40004:	//Auto assembly
			break;


		case 40005: //Exit
			if (MessageBox(hwnd, "Do you want to exit?", "Are you sure?", MB_YESNO | MB_ICONQUESTION) == IDYES)
				PostQuitMessage(0); //корректная обработка окончания сообщения
			break;
		}
		break;

	case WM_SIZE:
		sx4 = LOWORD(lParam) / 4;
		sy4 = HIWORD(lParam) / 4;
		break;

	case WM_DESTROY:
		PostQuitMessage(0); //корректная обработка окончания сообщения
		break;
	default:
		return
			DefWindowProc(hwnd, message, wParam, lParam);//возврат неотработанного сообщения
	}
	return 0L;
}

void ShowNum(int m[16], HWND hB[16])
{
	char s[3] = "";
	for (int i = 0; i < 16; i++)
	{
		if (m[i] != 0)
			sprintf(s, "%d", m[i]); //stdio.h
		else
			strcpy(s, "");
		SetWindowText(hB[i], s);
	}
}

