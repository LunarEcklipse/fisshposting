#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <vector>
#include <iostream>

static TCHAR szWindowClass[] = _T("fissh");
static TCHAR szTitle[] = _T("WARNING");
HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    // Check for class registration failure
    if (!RegisterClassEx(&wcex))
    {
        // Fuck
        MessageBox(NULL,
            _T("Fiish broke!"),
            _T("fuck"),
            NULL);

        return 1;
    }

    hInst = hInstance;

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        (WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU), // This looks like shit but prevents the window from resizing which is cash money
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 230, // Determines window size
        NULL,
        NULL,
        hInstance,
        NULL
    );

    // Check if windows actually created this shit
    if (!hWnd)
    {
        // Fuck
        MessageBox(NULL,
            _T("Fiish broke!"),
            _T("fuck"),
            NULL);

        return 1;
    }

    // Makes windows actually show the fucking thing
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Lets windows tell the window to get its shit together
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HICON errorIco = NULL;
    static HBITMAP fishSrc = NULL;
    static HDC hdcSource = NULL;
    static std::vector<wchar_t> pathBuf;

    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR fisshStr[] = _T("fissh"); // The text that shows up

    switch (message)
    {
    case WM_CREATE:
    {
        errorIco = LoadIcon(NULL, IDI_ERROR);
        DWORD copied = 0;
        do
        {
            pathBuf.resize(pathBuf.size() + MAX_PATH);
            copied = GetModuleFileName(NULL, &pathBuf.at(0), pathBuf.size());
        } while (copied >= pathBuf.size());

        pathBuf.resize(copied);
        std::wstring path(pathBuf.begin(), pathBuf.end());
        std::string::size_type pos = path.find_last_of(L"\\/");
        std::wstring outDir = path.substr(0, pos);
        outDir.append(L"\\fissh.bmp");
        LPCWSTR dir_str = outDir.c_str();
        try
        {
            fishSrc = (HBITMAP)LoadImage(NULL, dir_str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        }
        catch(...)
        {
            MessageBox(NULL, _T("FISSH IS MISSING!\nYOU STOLE IT."), _T("WHERE IS FISSH"), MB_OK);
            PostQuitMessage(0);
            return 1;
        }
        hdcSource = CreateCompatibleDC(GetDC(0));
        SelectObject(hdcSource, fishSrc);
        
        HWND btn1 = CreateWindow(TEXT("button"), TEXT("ok"),
            WS_VISIBLE | WS_CHILD,
            15, 150, 70, 25,
            hWnd, (HMENU)1, NULL, NULL);

        HWND btn2 = CreateWindow(TEXT("button"), TEXT("ok"),
            WS_VISIBLE | WS_CHILD,
            105, 150, 70, 25,
            hWnd, (HMENU)1, NULL, NULL);

        HWND btn3 = CreateWindow(TEXT("button"), TEXT("dont"),
            WS_VISIBLE | WS_CHILD,
            195, 150, 70, 25,
            hWnd, (HMENU)1, NULL, NULL);
        break;
    }
    case WM_PAINT: // For whatever reason WM_PAINT is what windows tells your app when it needs to redraw itself.
    {
       
        hdc = BeginPaint(hWnd, &ps);
 
        DrawIcon(hdc, 10, 10, errorIco);
        TextOut(hdc,
           60, 10, // X, Y,
            fisshStr, _tcslen(fisshStr));
        BitBlt(hdc, 100, 10, 400, 400, hdcSource, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY: // Windows said to quit your shit
    {
        PostQuitMessage(0);
        break;
    }
    default: 
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    }

    return 0;
}