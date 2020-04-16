// lab3.cpp : Определяет точку входа для приложения.
//

#include <windows.h>
#include <tchar.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");
//---------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE This, HINSTANCE Prev, LPSTR cmd,
    int mode)
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
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //установка цвета фона
    if (!RegisterClass(&wc)) return 0; // Регистрация класса окна
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
    } return 0; // при положительном завершении программы в главную функцию
    //возвращается «0»
}
//---------------------------------------------------------------------------
// Оконная функция вызываемая операционной системой
// и получает сообщения из очереди для данного приложения
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{ //Обработчик сообщений
        
    PAINTSTRUCT ps; 
    POINT pt[3] = { {0,100},{-100,-100},{100,-100} };
    
    const int WIDTH = 400; 
    const int HEIGHT = 300;
    static int sx, sy, i, k; 
    static HBRUSH hBrush;
    static HBRUSH arcBrush;
    HRGN hRgn;
    int a, b, x_scr, y_scr; //Экранные координаты double x, h; //Физические координаты
    HDC hdc; // обработка сообщений s
    switch (message) 
    { // определение размеров 

    case WM_SIZE: 
    { 
        sx = LOWORD(lParam); 
        sy = HIWORD(lParam); 
        break; 
    } 
        
    case WM_CREATE:
    {
        i = MessageBox(hWnd, _T("do you wan't to see the masson's eye?"), _T("world conspiracy question"), MB_YESNO | MB_ICONQUESTION);
        k = (i == IDYES) ? 1 : 0; 
        break; 
    } // отрисовка 
    case WM_PAINT:
    { 
        hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Создание кисти 
        arcBrush = CreateSolidBrush(RGB(255, 255, 255));
        hdc = BeginPaint(hWnd, &ps); // Установка режима 
        SetMapMode(hdc, MM_ANISOTROPIC); //Установка логических размеров вывода, ось У направлена вверх 
        SetWindowExtEx(hdc, WIDTH, -HEIGHT, NULL); //Установка физических размеров на окно 
        SetViewportExtEx(hdc, sx, sy, NULL); //Установка начала координат 
        SetViewportOrgEx(hdc, sx / 2, sy / 2, NULL); 
        BeginPath(hdc);
        Polyline(hdc, pt, 3); 
        CloseFigure(hdc);
        EndPath(hdc); 
        SelectObject(hdc, hBrush); 
        SetPolyFillMode(hdc, WINDING); //Режим закрашивания "вся фигура" 
        FillPath(hdc); 
        BeginPath(hdc);
        Arc(hdc, -50, 50, 50, -50, -10, 0, 10,0); // намечаем полуокружность
        CloseFigure(hdc);
        EndPath(hdc);
        SelectObject(hdc, arcBrush);
        SetPolyFillMode(hdc, WINDING); //Режим закрашивания "вся фигура" 
        FillPath(hdc);
        EndPaint(hWnd, &ps);
        break; 
    } // разрушение окна 
    case WM_DESTROY:
    { 
        PostQuitMessage(0); break; 
    } // по умолчанию 
    default: 
    { 
        return DefWindowProc(hWnd, message, wParam, lParam); 
    }
    } 
    return 0;
}