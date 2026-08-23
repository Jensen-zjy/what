#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void FillEllipse(HDC hdc, HBRUSH brush,
                 int left, int top, int right, int bottom)
{
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    Ellipse(hdc, left, top, right, bottom);
    SelectObject(hdc, oldBrush);
}

void FillPolygon(HDC hdc, HBRUSH brush, POINT points[], int count)
{
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    Polygon(hdc, points, count);
    SelectObject(hdc, oldBrush);
}

void DrawCat(HDC hdc, int cx, int cy, int facingRight, COLORREF color)
{
    HPEN outline = CreatePen(PS_SOLID, 3, RGB(60, 45, 45));
    HPEN oldPen = (HPEN)SelectObject(hdc, outline);

    HBRUSH bodyBrush = CreateSolidBrush(color);
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH blackBrush = CreateSolidBrush(RGB(35, 25, 25));
    HBRUSH pinkBrush = CreateSolidBrush(RGB(255, 150, 170));

    /* 身体 */
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, bodyBrush);
    RoundRect(hdc, cx - 70, cy + 35, cx + 70, cy + 185, 55, 55);
    SelectObject(hdc, oldBrush);

    /* 耳朵 */
    POINT leftEar[] = {
        {cx - 52, cy - 35},
        {cx - 43, cy - 95},
        {cx - 5,  cy - 58}
    };

    POINT rightEar[] = {
        {cx + 5,  cy - 58},
        {cx + 43, cy - 95},
        {cx + 52, cy - 35}
    };

    FillPolygon(hdc, bodyBrush, leftEar, 3);
    FillPolygon(hdc, bodyBrush, rightEar, 3);

    /* 耳朵内侧 */
    POINT leftInner[] = {
        {cx - 43, cy - 52},
        {cx - 39, cy - 78},
        {cx - 20, cy - 57}
    };

    POINT rightInner[] = {
        {cx + 20, cy - 57},
        {cx + 39, cy - 78},
        {cx + 43, cy - 52}
    };

    FillPolygon(hdc, pinkBrush, leftInner, 3);
    FillPolygon(hdc, pinkBrush, rightInner, 3);

    /* 头 */
    FillEllipse(hdc, bodyBrush, cx - 65, cy - 65, cx + 65, cy + 65);

    /* 眼睛 */
    int eye1 = facingRight ? cx + 18 : cx - 38;
    int eye2 = facingRight ? cx + 42 : cx - 14;

    FillEllipse(hdc, whiteBrush, eye1 - 10, cy - 20,
                eye1 + 10, cy + 5);
    FillEllipse(hdc, whiteBrush, eye2 - 10, cy - 20,
                eye2 + 10, cy + 5);

    FillEllipse(hdc, blackBrush, eye1 - 4, cy - 13,
                eye1 + 5, cy - 1);
    FillEllipse(hdc, blackBrush, eye2 - 4, cy - 13,
                eye2 + 5, cy - 1);

    /* 鼻子和嘴 */
    FillEllipse(hdc, pinkBrush, cx - 7, cy + 5, cx + 7, cy + 16);

    MoveToEx(hdc, cx, cy + 15, NULL);
    LineTo(hdc, cx - 10, cy + 27);
    MoveToEx(hdc, cx, cy + 15, NULL);
    LineTo(hdc, cx + 10, cy + 27);

    /* 胡须 */
    MoveToEx(hdc, cx - 18, cy + 22, NULL);
    LineTo(hdc, cx - 70, cy + 12);
    MoveToEx(hdc, cx - 18, cy + 32, NULL);
    LineTo(hdc, cx - 68, cy + 38);

    MoveToEx(hdc, cx + 18, cy + 22, NULL);
    LineTo(hdc, cx + 70, cy + 12);
    MoveToEx(hdc, cx + 18, cy + 32, NULL);
    LineTo(hdc, cx + 68, cy + 38);

    /* 前爪 */
    FillEllipse(hdc, bodyBrush, cx - 52, cy + 145, cx - 5, cy + 195);
    FillEllipse(hdc, bodyBrush, cx + 5, cy + 145, cx + 52, cy + 195);

    
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;

    const char className[] = "CuteCatsWindow";

    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    if (!RegisterClassExA(&wc)) {
        return 1;
    }

    HWND hwnd = CreateWindowExA(
        0,
        className,
        "靠在一起的小猫",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        700,
        420,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
                         WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        SetBkMode(hdc, TRANSPARENT);

        /* 两只小猫头部相碰、身体靠在一起 */
        DrawCat(hdc, 275, 145, 1, RGB(255, 190, 205));
        DrawCat(hdc, 405, 145, 0, RGB(190, 220, 255));

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
}
