// bezie_lab5.cpp : Определяет точку входа для приложения.
//
#include <windows.h>
#include <tchar.h>
#define _USE_MATH_DEFINES
#include <Math.h>
#include <fstream>

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



static int sx, sy;

const int SCALE = 1000; //размер логического окна

const int MARK = 4; //размер прямоугольного маркера точек

void DcInLp(POINT& point)
{
    point.x = point.x * SCALE / sx;
    point.y = SCALE - point.y * SCALE / sy;
}

void transform(HDC& hdc)
{
    SetMapMode(hdc, MM_ANISOTROPIC); //Устанавливаем локальную систему
    SetWindowExtEx(hdc, SCALE, -SCALE, NULL); //координат 1000х1000 с центром в
    SetViewportExtEx(hdc, sx, sy, NULL); //левом нижнем углу
    SetViewportOrgEx(hdc, 0, sy, NULL); //
}
void b_spline(HDC hdc, POINT pt[], int count)
{
    double X, Y, t, eps = 0.01;
    float xA, xB, xC, xD, yA, yB, yC, yD;
    float a0, b0, a1, b1, a2, b2, a3, b3;
    int i, j, first;
    //Функция для отрисовки B-сплайна.
    for (i = 0; i <= count; i++)
    {
        X = pt[i].x;
        Y = pt[i].y;
        MoveToEx(hdc, X - eps, Y - eps, NULL);
        LineTo(hdc, X + eps, Y + eps);
        MoveToEx(hdc, X + eps, Y - eps, NULL);
        LineTo(hdc, X - eps, Y + eps);
    }
    first = 1;
    for (i = 1; i < count - 1; i++)
    {
        xA = pt[i - 1].x;
        xB = pt[i].x;
        xC = pt[i + 1].x;
        xD = pt[i + 2].x;
        yA = pt[i - 1].y;
        yB = pt[i].y;
        yC = pt[i + 1].y;
        yD = pt[i + 2].y;
        a3 = (-xA + 3 * (xB - xC) + xD) / 6.0;
        b3 = (-yA + 3 * (yB - yC) + yD) / 6.0;
        a2 = (xA - 2 * xB + xC) / 2.0;
        b2 = (yA - 2 * yB + yC) / 2.0;
        a1 = (xC - xA) / 2.0;
        b1 = (yC - yA) / 2.0;
        a0 = (xA + 4 * xB + xC) / 6.0;
        b0 = (yA + 4 * yB + yC) / 6.0;
        for (j = 0; j <= 20; j++)
        {
            t = (float)j / (float)20;
            X = ((a3 * t + a2) * t + a1) * t + a0;
            Y = ((b3 * t + b2) * t + b1) * t + b0;
            if (first)
            {
                first = 0;
                MoveToEx(hdc, X, Y, NULL);
            }
            else
            {
                LineTo(hdc, X, Y);
            }
        }
    }
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static HPEN hDash, hBezier,hSpline; // два пера
    static HBRUSH hRect, hSel; // две кисти
    static POINT pt[20]; // массив хранения плоских точек
    static POINT point; // структура под одну плоскую точку
    RECT rt; // структура точек прямоугольника
    static int count, index; // счётчик точек,
    static bool capture; // логическая переменная для мыши
    int i;
    std::ifstream in; //класс файлового потокового ввода-вывода
    std::ofstream out;
    switch (message)
    {
    case WM_CREATE:
        in.open("dat.txt"); // открытие файлового потока
        if (in.fail())
        {
            MessageBox(hWnd, _T("Файл dat.txt не найден"),
                _T("Открытие файла"), MB_OK | MB_ICONEXCLAMATION);
            PostQuitMessage(0);
            return 1;
        }
        // пока запоняется координата x - заполняем y
       
            // в переменной count накапливается размер массива точек
        for (count = 0; in >> pt[count].x; count++) in >> pt[count].y;
        in.close(); // закрытие файлового потока
        hDash = CreatePen(PS_DASH, 1, 0);
        hBezier = CreatePen(PS_SOLID, 4, RGB(0, 0, 255));
        hSpline = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
        hRect = CreateSolidBrush(RGB(128, 0, 128));
        hSel = CreateSolidBrush(RGB(255, 0, 0));
        break;
    case WM_SIZE:
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);
        break;
    case WM_LBUTTONDOWN:
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        //Преобразование экранных координат мыши в логические
        DcInLp(point);
        for (i = 0; i <= count; i++)
        {
            SetRect(&rt, pt[i].x - MARK, pt[i].y - MARK,
                pt[i].x + MARK, pt[i].y + MARK);
            if (PtInRect(&rt, point))
            { // Курсор мыши попал в точку
                index = i;
                capture = true;
                hdc = GetDC(hWnd);
                transform(hdc); //Переход в логические координаты
                FillRect(hdc, &rt, hSel);//Отметим прямоугольник цветом
                ReleaseDC(hWnd, hdc);
                SetCapture(hWnd);
                return 0;
            }
        }
        break;
    case WM_LBUTTONUP:
        if (capture)
        {
            ReleaseCapture(); //Освобождение мыши
            capture = false;
        }
        break;
    case WM_MOUSEMOVE:
        if (capture)
        { //Мышь захвачена
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            DcInLp(point); //Преобразование экранных координат мыши
            pt[index] = point; // в логические координаты
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        transform(hdc); //Переход в логические координаты
        SelectObject(hdc, hDash);
        Polyline(hdc, pt, count); //Строим ломанную линию
  
        SelectObject(hdc, hBezier);
        PolyBezier(hdc, pt, count);
        SelectObject(hdc, hSpline);
        b_spline(hdc,pt,count); //Строим B-Cплайн
        for (i = 0; i < count; i++)
        { //Закрашиваем точки графика прямоугольниками
            SetRect(&rt, pt[i].x - MARK, pt[i].y - MARK,
                pt[i].x + MARK, pt[i].y + MARK);
            FillRect(hdc, &rt, hRect);
        }
        break;
    case WM_DESTROY:
    {
        DeleteObject(hDash);
        DeleteObject(hBezier);
        DeleteObject(hRect);
        DeleteObject(hSel);
        out.open("dat.txt"); //открыть файловый поток для записи даннах
        for (i = 0; i < count; i++) out << pt[i].x << '\t' << pt[i].y << '\n';
        out.close();   
        PostQuitMessage(0);
        break;
        }

    default: {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    }
    return 0;

    
}