// Sceleton.cpp : Defines the entry point for the application.
//

#include "stdafx.h"



char name[]="Qwerty";//Необходимо для регистрации окна

// указатель на нижнюю границу загрузки приложения
HINSTANCE hInst;

int APIENTRY WinMain(HINSTANCE hInstance,//хендлер приложения,только он действителеный 
                     HINSTANCE , //неиспользуемый параметр
                     LPSTR     lpCmdLine,//командная строка
                     int       nCmdShow)//вид окна
{
	
	hInst = hInstance;// Создание копии 
	// 1.Создание внешнего вида ОКНА
	WNDCLASS win; //объект для информации об окне,структура 
	win.style=CS_HREDRAW|CS_VREDRAW; //Стили перерисовки
	win.lpfnWndProc=WFunc; //Имя (параметр) оконной функции должно соответствовать иени ф-ци 
	win.cbClsExtra=win.cbWndExtra=0; // ВСЕГДА =0
	win.hInstance=hInstance;//имя приложения, к которому привязано окно
	win.hIcon=LoadIcon(hInst, (TCHAR*)102);//иконка, идентификатор иконки ID_APPLIKATION
	win.hCursor=LoadCursor(NULL,IDC_ARROW); //курсор
	win.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//Цвет фона
	win.lpszMenuName=(TCHAR*)101;//меню отсутствует
	win.lpszClassName=name;//Имя для регистрации = qwerty

	//2.Регистрация окна в Операционной системе
	if (!RegisterClass(&win))
	{
		MessageBox(NULL,"Window don't registered!","Error!",MB_OK);
		return -1;//если окно не зарегистрировано
	}

	//3.Создание ОКНА
	HWND hwnd; //Объявление хендлера Окна
	hwnd=CreateWindow(name,"15",WS_OVERLAPPEDWINDOW|WS_VISIBLE,50,50,
		              500,500,HWND_DESKTOP,NULL,hInstance,NULL);//Создание хендлера Окна

	

	//4*.Отображение окна

	ShowWindow(hwnd,nCmdShow); //nCmdShow - для показа  окна
	UpdateWindow (hwnd); //обновить сразу окно - перерисовка

	//5.создание насоса сообщений
	MSG msg; //описание переменной Сообщения
	while (GetMessage(&msg,0,0,0))// МОЖНО ОГРАНИЧИТЬ СООБЩЕНИЯ
	{
		TranslateMessage(&msg);  //разрешает использовать клавиатуру
		DispatchMessage(&msg); //отправка обработанного сообщения в ОС
	}
	return msg.wParam; //корректное закрытие основной программы
}


