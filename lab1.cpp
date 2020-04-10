#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");
//---------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE This, HINSTANCE Prev, LPSTR cmd,int mode)
{
	HWND hWnd; //Дескриптор главного окна программы
	MSG msg; // Структура для хранения сообщений
	WNDCLASS wc; // Класс окна
	// Определение класса окна 
	wc.hInstance = This;
	wc.lpszClassName = WinName; // Имя класса окна
	wc.lpfnWndProc = WndProc; // Функция окна
	wc.style = CS_HREDRAW | CS_VREDRAW; // Стиль окна
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //Стандартная иконка
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Стандартный курсор 
	wc.lpszMenuName = NULL; //Нет меню
	wc.cbClsExtra = 0; //Нет дополнительных данных класса
	wc.cbWndExtra = 0; //Нет дополнительных данных окна
	// Заполнение окна белым цветом 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); //установка цвета фона
	if (!RegisterClass(&wc)) return 0; // Регистрация класса окна
	// Создание окна 
	hWnd = CreateWindow(WinName, //Имя класса окна 
		_T("Каркас Windows-приложения"), // Заголовок окна 
		WS_OVERLAPPEDWINDOW, // Стиль окна
		CW_USEDEFAULT, // X
		CW_USEDEFAULT, // Y
		CW_USEDEFAULT, // Width
		CW_USEDEFAULT, // Height
		HWND_DESKTOP, // Дескриптор родительского окна 
		NULL, // Нет меню
		This, // Дескриптор приложения
		NULL); // Дополнительной информации нет
		ShowWindow(hWnd, mode); // Показать окно
		// Цикл обработки сообщений
		while (GetMessage(&msg, NULL, 0, 0)) //цикл получения сообщений
		{
			TranslateMessage(&msg); // Функция трансляции кодов нажатой клавиши
			DispatchMessage(&msg); // Посылает сообщение функции WndProc()
		} return 0; // при положительном завершении программы в главную функцию //возвращается «0»
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ //Обработчик сообщений
	switch (message)
	{
	case WM_DESTROY: PostQuitMessage(0);
		break;
		// Обработка сообщений по умолчанию
	default:
		return DefWindowProc(hWnd, message, wParam, lParam); //Возвращение стандартных параметров по умолчанию
	} return 0;
}
// ------------------------------ task#2 --------------------------------------
/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc; int x, y; static int sx, sy;
	//Обработчик сообщений
	switch (message)
	{
	case WM_SIZE:
		sx = LOWORD(lParam); //Ширина окна
		sy = HIWORD(lParam); //Высота окна
		break;
	case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
		for (x = 0; x < sx; x += sx / 10)
		{
			MoveToEx(hdc, x, 0, NULL);
			LineTo(hdc, x, sy);
		}
		for (y = 0; y < sy; y += sy / 10)
		{
			MoveToEx(hdc, 0, y, NULL);
			LineTo(hdc, sx, y);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: PostQuitMessage(0);
		break; // Завершение программы
		// Обработка сообщения по умолчанию
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}*/


// ------------------------------ task#3 --------------------------------------
/* 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc; int x, y; static int sx, sy;
	HBRUSH creambrush = CreateSolidBrush(RGB(120, 190, 253));//кисть для фона
	HBRUSH redwoodbrush = CreateSolidBrush(RGB(124, 48, 100));//кисть для рамы
	//Обработчик сообщений
	switch (message)
	{
	case WM_SIZE:
		sx = LOWORD(lParam); //Ширина окна
		sy = HIWORD(lParam); //Высота окна
		break;
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, redwoodbrush);//выбираем кисть для рамы
		Rectangle(hdc,5, 5, sx-5 , sy-5);// создаем основу рамы
		SelectObject(hdc, creambrush);// выбираем кисть для фона
		Rectangle(hdc, 20, 20, sx/8*5 - 20, (sy - 20));// создаем левое окно
		Rectangle(hdc, sx/8*5 , sy/4 + 20 , sx-20, sy - 20);// создаем правое окно
		SelectObject(hdc, redwoodbrush);//выбираем кисть для рамы
		Rectangle(hdc, sx/8*5 ,20 ,sx - 20 , sy/4 );//выделяем раму для форточки
		SelectObject(hdc, creambrush);// выбираем кисть для фона
		Rectangle(hdc, sx / 8 * 5 + 20, 40, sx - 40, sy / 4 - 20);// создаем форточку
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: PostQuitMessage(0);
		break; // Завершение программы
		// Обработка сообщения по умолчанию
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
*/