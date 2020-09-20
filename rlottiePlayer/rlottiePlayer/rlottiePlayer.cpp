// rlottiePlayer.cpp : Defines the entry point for the application.
//


#include "framework.h"
#include "rlottiePlayer.h"
using namespace Gdiplus;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND mainWindow;                                    // Main Window Instance
HWND hTextFileToBeOpened;                               // openDialog file path
HWND hBtnPlay, hBtnWhite, hBtnBlack, hBtnRed, hBtnGreen, hBtnBlue;
HWND hSliderPlay;
Bitmap* img = NULL;                                     // rendered Animation Bitmap
double curFrame = 0.0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void openJSONFileDialog(HWND);
void initUIControl(HWND);
void dlgUICommand(HWND, WPARAM);

// Animation Rendering Functions
void draw(HDC);
Bitmap* CreateBitmap(void* data, unsigned int width, unsigned int height);
void renderAnimation();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // initialize Gdiplus
    Gdiplus::GdiplusStartupInput gdiplusStartUpInput;
    ULONG_PTR gdiplustoken;
    Gdiplus::GdiplusStartup(&gdiplustoken, &gdiplusStartUpInput, nullptr);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RLOTTIEPLAYER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RLOTTIEPLAYER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Gdiplus::GdiplusShutdown(gdiplustoken);
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RLOTTIEPLAYER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RLOTTIEPLAYER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    mainWindow = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!mainWindow)
    {
        return FALSE;
    }

    ShowWindow(mainWindow, nCmdShow);
    UpdateWindow(mainWindow);
    DialogBox(hInst, MAKEINTRESOURCE(MAIN_WINDOW), mainWindow, About);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (img != NULL) draw(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        initUIControl(hDlg);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else dlgUICommand(hDlg, wParam);
        break;
    }
    return (INT_PTR)FALSE;
}

void openJSONFileDialog(HWND hDlg)
{
    OPENFILENAME ofn;       // common dialog box structure
    TCHAR szFile[260] = { 0 };       // if using TCHAR macros

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hDlg;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("JSON\0*.json\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        SetWindowText(hTextFileToBeOpened, ofn.lpstrFile);
        // LPWSTR(w_char*) -> LPSTR(char*)
        USES_CONVERSION;
        LPSTR path = W2A(ofn.lpstrFile);
        
        setAnimation(path, 500, 500);
        // init play slider control
        SendMessage(hSliderPlay, TBM_SETRANGE, FALSE, MAKELPARAM(0, getTotalFrame() - 1));
        SendMessage(hSliderPlay, TBM_SETPOS, TRUE, 0);
        curFrame = 0.0;
        renderAnimation();
    }
}

void dlgUICommand(HWND hDlg, WPARAM wParam) {
    static bool isplay = false;

    switch (LOWORD(wParam))
    {
    case BTN_BROWSE:
        openJSONFileDialog(hDlg);
        break;

    case BTN_PLAY:
    {
        LPWSTR textBtnPlay;
        USES_CONVERSION;
        if (isplay)
        {
            isplay = false;
            textBtnPlay = A2W("pause");
        }
        else
        {
            isplay = true;
            textBtnPlay = A2W("play");
        }
        SetWindowText(hBtnPlay, textBtnPlay);
        break;
    }
    case WM_DROPFILES:
        break;
    case BTN_WHITE:
        break;
    case BTN_BLACK:
        break;
    case BTN_RED:
        break;
     case BTN_GREEN:
        break;
    case BTN_BLUE:
        break;
    }
}

void draw(HDC hdc)
{
    if (img != NULL)
    {
        Gdiplus::Graphics gf(hdc);
        gf.DrawImage(img, 0, 0, 500, 500);
    }
}

Bitmap* CreateBitmap(void* data, unsigned int width, unsigned int height)
{
    BITMAPINFO Info;
    memset(&Info, 0, sizeof(Info));

    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Info.bmiHeader.biWidth = width;
    Info.bmiHeader.biHeight = height;
    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = 32;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = 0;  //(((32 * width + 31) & ~31) / 8) * height;

    return new Gdiplus::Bitmap(&Info, data);
}

void renderAnimation()
{
    // render
    auto resRender = renderRLottieAnimation(curFrame);
    img = CreateBitmap(resRender->buffer(), resRender->width(), resRender->height());
    img->RotateFlip(RotateNoneFlipY);
    // call WM_PAINT message
    InvalidateRect(mainWindow, NULL, TRUE);
}

void initUIControl(HWND hDlg)
{
    hTextFileToBeOpened = GetDlgItem(hDlg, TEXT_FILENAME);
    hBtnPlay = GetDlgItem(hDlg, BTN_PLAY);
    hBtnWhite = GetDlgItem(hDlg, BTN_WHITE);
    hBtnBlack = GetDlgItem(hDlg, BTN_BLACK);
    hBtnRed = GetDlgItem(hDlg, BTN_RED);
    hBtnGreen = GetDlgItem(hDlg, BTN_GREEN);
    hBtnBlue = GetDlgItem(hDlg, BTN_BLUE);

    // play
    hSliderPlay = GetDlgItem(hDlg, SLIDER_PLAY);
}