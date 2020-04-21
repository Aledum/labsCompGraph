#include <windows.h>
#include <tchar.h>
#define _USE_MATH_DEFINES
#include <Math.h>
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
    POINT pt[3] = { {0,100},{-100,-100},{100,-100}};
    POINT pt1[3];
    
    const int WIDTH = 400;
    const int HEIGHT = 300;
    int n = 8, i, j;
    int sx, sy;
    static HBRUSH hBrush;
    double alpha = M_PI / n;
    static HBRUSH arcBrush;
    HRGN hRgn;
    double arc_x1, arc_y1, arc_x2, arc_y2, arc_x_3_1, arc_y_3_1, arc_x_3_2, arc_y_3_2;
    int a, b, x_scr, y_scr; //Экранные координаты 
    double x, h; //Физические координаты
    HDC hdc; // обработка сообщений s
    switch (message)
    { // определение размеров 

    case WM_SIZE:
    {
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);
        break;
    }
    /*
    case WM_CREATE:
    {
        i = MessageBox(hWnd, _T("do you wan't to see the masson's eye?"), _T("world conspiracy question"), MB_YESNO | MB_ICONQUESTION);
        k = (i == IDYES) ? 1 : 0;
        break;
    } 
    */// отрисовка 
    case WM_PAINT:
    {   hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Создание кисти 
        arcBrush = CreateSolidBrush(RGB(255, 255, 255));
    hdc = BeginPaint(hWnd, &ps); // Установка режима 
        for (i = 0; i <= 6; i++)
    {
        // Создание изменения цвета кисти
         
        hBrush = CreateSolidBrush(RGB(255, 255 - 255. / n * i, 255 - 255. / n * i));
        SelectObject(hdc, hBrush);
        for (j = 0; j <= 2; j++)
        {
            pt1[j].x = 500 + (pt[j].x * cos(alpha * i) - pt[j].y * sin(alpha * i)) + i * 10;
            pt1[j].y = 300 + (pt[j].x * sin(alpha * i) + pt[j].y * cos(alpha * i));
        }
        Polygon(hdc, pt1, 3);
        
        arcBrush = CreateSolidBrush(RGB(0+ 255. / n * i, 0 + 255. / n * i, 0 + 255. / n * i));
        BeginPath(hdc);
        
        arc_x1 = 500 +( -50 ) + i * 10;
        arc_y1 = 300 + 50;

        arc_x2 = 500 + (50) + i * 10;
        arc_y2 = 300 + (-50);

        arc_x_3_1 = 500 + ((-10) * cos(alpha * i) - 0 * sin(alpha * i)) + i * 10;
        arc_y_3_1 = 300 + ((-10) * sin(alpha * i) + 0 * cos(alpha * i));

        arc_x_3_2 = 500 + (10 * cos(alpha * i) - 0 * sin(alpha * i)) + i * 10;
        arc_y_3_2 = 300 + (10 * sin(alpha * i) + 0 * cos(alpha * i));
     
        Arc(hdc, arc_x1, arc_y1, arc_x2, arc_y2, arc_x_3_2, arc_y_3_2, arc_x_3_1, arc_y_3_1); // намечаем полуокружность
        CloseFigure(hdc);
        EndPath(hdc);
        SelectObject(hdc, arcBrush);
        SetPolyFillMode(hdc, WINDING); //Режим закрашивания "вся фигура" 
        FillPath(hdc);
    }
        

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
