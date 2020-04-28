// lab5.cpp : Определяет точку входа для приложения.
//

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


/*
Задание 1: Изобразить на экране вращающийся при помощи мыши куб с
удалёнными гранями и закраской (рис. ), применив фрагменты кода:
*/

//-------------------------------------------------------------------------—
PAINTSTRUCT ps;
static HBRUSH hBrush;
const int WIDTH = 400;
const int HEIGHT = 300;
float v11, v12, v13, v21, v22, v23, v31, v32, v33, v43;
float rho = 300., thetta = 75., phi = 30., ScreenDist = 500.;
float A, B, C, D, An, Bn, Cn;
float xt[3], yt[3], zt[3];
float Al, Bl, Cl;
float alpha;
float th, ph, costh, cosph, sinth, sinph;
float factor = atan(1.0) / 45.;

class TFPoint
{
public:
    float X;
    float Y;
    float Z;
};

//Задаём координаты точек куба
TFPoint CubePoints[] = { {-50, -50, -50},
{50, -50, -50},
{50, 50, -50},
{-50, 50, -50},
{-50, 50, 50},
{-50, -50, 50},
{50, -50, 50},
{50, 50, 50} };
int Gran[6][4] = { {0, 3, 4, 5}, {0, 5, 6, 1}, {2, 7, 4, 3},
{7, 6, 5, 4}, {0, 1, 2, 3}, {2, 1, 6, 7} };

void VidMatCoeff(float rho, float thetta, float phi)
{
    //Определение компонентов направления наблюдения для сведения с
    // лучом освещения
    th = thetta * factor;
    ph = phi * factor;
    costh = cos(th);
    sinth = sin(th);
    cosph = cos(ph);
    sinph = sin(ph);
    // Элементы матрицы V
    v11 = -sinth;
    v12 = -cosph * costh;
    v13 = -sinph * costh;
    v21 = costh;
    v22 = -cosph * sinth;
    v23 = -sinph * sinth;
    v31 = 0.;
    v32 = sinph;
    v33 = -cosph;
    v43 = rho;
}

POINT Perspective(float x, float y, float z)
{
    POINT point;
    float xe, ye, ze;
    VidMatCoeff(rho, thetta, phi);
    xe = v11 * x + v21 * y;
    ye = v12 * x + v22 * y + v32 * z;
    ze = v13 * x + v23 * y + v33 * z + v43;
    // Экранные координаты
    point.x = ScreenDist * xe / ze + 400.;
    point.y = ScreenDist * ye / ze + 300.;
    return point;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    PAINTSTRUCT ps;
    static HBRUSH hBrush;

    POINT point1[4];
    HDC hdc;
    int sx, sy, xPos, yPos, zDelta;

    // обработка сообщений
    switch (message) {
    case WM_MOUSEMOVE: {
        sx = LOWORD(lParam); //координата мыши по оси Х
        sy = HIWORD(lParam); //координата мыши по оси У
        thetta += ((sx % 180) - 90) / 10;
        phi += ((sy % 180) - 90) / 10;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_MOUSEWHEEL: {
        zDelta = (int)wParam; // wheel rotation
        ScreenDist -= zDelta / 1000000.;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
                      // отрисовка
    case WM_PAINT: {
        hdc = BeginPaint(hWnd, &ps);
        th = thetta *

            factor;
        ph = phi * factor;
        costh = cos(th);
        sinth = sin(th);
        cosph = cos(ph);
        sinph = sin(ph);
        A = rho * sinph * costh;
        B = rho * sinph * sinth;
        C = rho * cosph;
        Al = A / (sqrt(A * A + B * B + C * C));
        Bl = B / (sqrt(A * A + B * B + C * C));
        Cl = C / (sqrt(A * A + B * B + C * C));
        for (int i = 0; i < 6; i++) { //
            for (int j = 0; j < 3; j++) {
                xt[j] = CubePoints[Gran[i][j]].X;
                yt[j] = CubePoints[Gran[i][j]].Y;
                zt[j] = CubePoints[Gran[i][j]].Z;
            }
            //
            A = yt[0] * (zt[1] - zt[2]) - yt[1] * (zt[0] - zt[2]) + yt[2] * (zt[0] - zt[1]);
            B = -(xt[0] * (zt[1] - zt[2]) - xt[1] * (zt[0] - zt[2]) + xt[2] * (zt[0] - zt[1]));
            C = xt[0] * (yt[1] - yt[2]) - xt[1] * (yt[0] - yt[2]) + xt[2] * (yt[0] - yt[1]);

            An = A / (sqrt(A * A + B * B + C * C));
            Bn = B / (sqrt(A * A + B * B + C * C));
            Cn = C / (sqrt(A * A + B * B + C * C));

            alpha = (An * Al + Bn * Bl + Cn * Cl);
            for (int j = 0; j < 4; j++)
            {
                point1[j] = Perspective(CubePoints[Gran[i][j]].X,
                    CubePoints[Gran[i][j]].Y,
                    CubePoints[Gran[i][j]].Z);
            }
            //Определение направления обхода точек грани для выявления ориентации к
            // наблюдателю
            D = point1[0].x * (point1[1].y - point1[2].y)
                - point1[1].x * (point1[0].y - point1[2].y)
                + point1[2].x * (point1[0].y - point1[1].y);
            if (D < 0) {
                hBrush = CreateSolidBrush(
                    RGB((1 - alpha) * 255, (1 - alpha) * 255, (1 - alpha) * 255));
                SelectObject(hdc, hBrush);
                Polygon(hdc, point1, 4);
            }
        }
        EndPaint(hWnd, &ps);
        break;
    }
                 // разрушение окна
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
                   // по умолчанию
    default: {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    }

    return 0;
}

