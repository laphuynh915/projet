// Win32Project1.cpp : Defines the entry point for the application.
/*
	Trong chương trình này, ta sẽ cố gắng lưu lại bản vẽ sau khi bản vẽ. 
	Việc vẽ các đường thẳng tạm sẽ không được vẽ trực tiếp lên bản vẽ này.
	Ý tưởng: sử dụng một bitmap để lưu bản vẽ hiện hành. Việc vẽ các đường thẳng tạm sẽ được vẽ trực tiếp lên màn hình.
			khi kết thúc việc vẽ thì ta mới vẽ đường thẳng lên bitmap
	Đối với kỹ thuật này thì khi vẽ đường thẳng ta không cần vẽ đè đường
*/

#include "stdafx.h"
#include "Win32Project1.h"
#include "Windowsx.h"
#include <math.h>
#include <Commdlg.h>
#include <Commctrl.h>
#include <windows.h> 



#define MAX_LOADSTRING 100
#define ID_TIMER1	123123
// Global Variables:
char AppCaption[40];
HINSTANCE hInst;
const int NUMBUTTONS = 10;                             // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
POINT p1, p2, p2_ancien;
HBITMAP hBitmap;
BOOL enTrainDessin;
HWND  hEdit;
TCHAR szFilter[] = TEXT("Text file(*.txt)\0*.txt\0");
TCHAR szFile[MAX_PATH]; // File Name

int saveL, saveR , saveT , saveB;
int R ;
int G ;
int B ;
int Width;

int mode; //mode de dessin==> 0 pour ligne, 1 pour libre
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AboutBox(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID				DrawLineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				FreeDrawProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				DrawRectangleProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				DrawRoundProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				DrawEllipseProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				DrawSquareProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				errhandler(LPCTSTR, HWND);
VOID				PEN(HDC hdc, int width, int R, int G, int B);
void DoOpenFile(HWND hWnd);
VOID				CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC);
PBITMAPINFO			CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);

void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
void doChooseColor(HWND hWnd);
COLORREF myColor = RGB(0, 0, 0);


VOID DrawRectangle(HDC hdc);
VOID DrawEllipse(HDC hdc);
VOID DrawSquare(HDC hdc);
VOID DrawRound(HDC hdc);

VOID DrawLine	(HDC hdc);
VOID FreeDraw	(HDC hdc);
VOID PenBrushDemo(HDC hdc);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_CROSS);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_MAXIMIZEBOX| WS_MINIMIZEBOX| WS_SYSMENU,
      0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   INITCOMMONCONTROLSEX InitCtrlEx;

   InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
   InitCtrlEx.dwICC = ICC_BAR_CLASSES;
   InitCommonControlsEx(&InitCtrlEx);

   TBBUTTON tbrButtons[10];

   tbrButtons[0].iBitmap = 0;
   tbrButtons[0].idCommand = ID_FILE_OPEN;
   tbrButtons[0].fsState = TBSTATE_ENABLED;
   tbrButtons[0].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[0].dwData = 0L;
   tbrButtons[0].iBitmap = 0;
   tbrButtons[0].iString = 0;

   tbrButtons[1].iBitmap = 1;
   tbrButtons[1].idCommand = ID_FILE_SAVE;
   tbrButtons[1].fsState = TBSTATE_ENABLED;
   tbrButtons[1].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[1].dwData = 0L;
   tbrButtons[1].iString = 0;

   tbrButtons[2].iBitmap = 0;
   tbrButtons[2].idCommand =0;
   tbrButtons[2].fsState = TBSTATE_ENABLED;
   tbrButtons[2].fsStyle = TBSTYLE_SEP;
   tbrButtons[2].dwData = 0L;
   tbrButtons[2].iString = 0;

   tbrButtons[3].iBitmap = 2;
   tbrButtons[3].idCommand = ID_FORMAT_COLOR;
   tbrButtons[3].fsState = TBSTATE_ENABLED;
   tbrButtons[3].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[3].dwData = 0L;
   tbrButtons[3].iString = 0;

   tbrButtons[4].iBitmap = 3;
   tbrButtons[4].idCommand = ID_SHAPE_SQUARE;
   tbrButtons[4].fsState = TBSTATE_ENABLED;
   tbrButtons[4].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[4].dwData = 0L;
   tbrButtons[4].iString = 0;

   tbrButtons[5].iBitmap = 4;
   tbrButtons[5].idCommand = ID_SHAPE_ROUND;
   tbrButtons[5].fsState = TBSTATE_ENABLED;
   tbrButtons[5].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[5].dwData = 0L;
   tbrButtons[5].iString = 0;

   tbrButtons[6].iBitmap = 5;
   tbrButtons[6].idCommand = ID_LINE_STRAIGHTLINE;
   tbrButtons[6].fsState = TBSTATE_ENABLED;
   tbrButtons[6].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[6].dwData = 0L;
   tbrButtons[6].iString = 0;

   tbrButtons[7].iBitmap = 6;
   tbrButtons[7].idCommand = ID_SHAPE_ELLIPSE;
   tbrButtons[7].fsState = TBSTATE_ENABLED;
   tbrButtons[7].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[7].dwData = 0L;
   tbrButtons[7].iString = 0;

   tbrButtons[8].iBitmap = 7;
   tbrButtons[8].idCommand = ID_SHAPE_RECTANGLE;
   tbrButtons[8].fsState = TBSTATE_ENABLED;
   tbrButtons[8].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[8].dwData = 0L;
   tbrButtons[8].iString = 0;

   tbrButtons[9].iBitmap = 8;
   tbrButtons[9].idCommand = ID_LINE_FREESTYLE;
   tbrButtons[9].fsState = TBSTATE_ENABLED;
   tbrButtons[9].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[9].dwData = 0L;
   tbrButtons[9].iString = 0;

   HWND hWndToolbar;
   hWndToolbar = CreateToolbarEx(hWnd,
	   WS_VISIBLE | WS_CHILD | WS_BORDER,
	   IDB_BITMAP1,
	   NUMBUTTONS,
	   hInst,
	   IDB_BITMAP1,
	   tbrButtons,
	   NUMBUTTONS,
	   16, 16, 16, 16,
	   sizeof(TBBUTTON));

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = GetDC(hWnd);
	switch (mode)
	{
	case 0:
		FreeDrawProc(hWnd, message, wParam, lParam);
		break;
	case 1:
		DrawLineProc(hWnd, message, wParam, lParam);
	case 2:
		DrawEllipseProc(hWnd, message, wParam, lParam);
	case 3:
		DrawRoundProc(hWnd, message, wParam, lParam);
	case 4:
		DrawSquareProc(hWnd, message, wParam, lParam);
	case 5:
		DrawRectangleProc(hWnd, message, wParam, lParam);
	}
	
	switch (message)
    {
	case WM_CREATE:
	{
		p1.x = p1.y = 0;
		p2 = p1;
		mode = 1;
		enTrainDessin = FALSE;

		//Tạo bitmap
	
		RECT rect;
		GetClientRect(hWnd, &rect);
		//Tạo ra một bitmap tương thích với DC màn hình
		hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		
		//Tô background cho bitmap
		//Tạo memory dc để tương tác với Bitmap
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		HBRUSH hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
		FillRect(memDC, &rect, hBrush);
		DeleteObject(hBrush);
		DeleteDC(memDC);
		ReleaseDC(hWnd, hdc);
		break;
	}
    case WM_COMMAND:
        {
			
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_LINE_STRAIGHTLINE:
				mode = 1;
			break;
			case ID_LINE_FREESTYLE:
				mode = 0;
				break;
			
			case ID_SHAPE_ELLIPSE:
				mode = 2;
				break;
			
			case ID_SHAPE_ROUND:
				mode = 3;
				break;
			
			case ID_SHAPE_SQUARE:
				mode = 4;
				break;
			
			case ID_SHAPE_RECTANGLE:
				mode = 5;
				break;

			case ID_DRAW_WIDTH:
				DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, AboutBox);
				break;

			case ID_COLOR_RED:
				R = 255;
				G = 0;
				B = 0;
					break;

			case ID_COLOR_GREEN:
				R = 0;
				G = 255;
				B = 0;
					break;


			case ID_COLOR_BLUE:
				R = 0;
				G = 0;
				B = 255;
					break;

			case ID_FORMAT_COLOR:
				doChooseColor(hEdit);

				break;
            
			case ID_FILE_OPEN:

				OPENFILENAME ofn;
				TCHAR szFile[260];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = _T("All Files *.*\0*.*\0Text Files *.txt\0*.TXT\0 Doc Files\0*.TXT;*.DOC;*.BAK\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = _T("C:\\");
				ofn.lpstrTitle = _T("My Application - Open file");
				ofn.lpstrDefExt = _T("txt");
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				// Display the Open dialog box. 
				if (GetOpenFileName(&ofn) == TRUE) {
					PAINTSTRUCT ps;
					HDC hdc = GetDC(hWnd);
					//			HDC hdcDestination;
					HBITMAP hbitmap = (HBITMAP)::LoadImage(NULL, ofn.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					//	MessageBox(NULL, ofn.lpstrFile, _T("Selected file"), MB_OK);
					//hdc = getOb
					HDC memDC = CreateCompatibleDC(hdc);
					SelectObject(memDC, hbitmap);
					RECT rect;
					GetClientRect(hWnd, &rect);
					BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);
				}

				break;
			case ID_FILE_SAVE:
			{
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				char szFileName[MAX_LOADSTRING];
				ZeroMemory(szFileName, MAX_LOADSTRING);
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFilter = _T("All Files(*.*)\0 * .*\0Bmp Files(*.bmp)\0 * .bmp\0Text Files(*.txt)\0 * .txt\0");
				ofn.lpstrFile = (LPWSTR)szFileName;
				ofn.nMaxFile = MAX_LOADSTRING;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = (LPCWSTR)L"bmp";
				GetSaveFileName(&ofn);
				PBITMAPINFO PBi = CreateBitmapInfoStruct(hWnd, hBitmap);
				CreateBMPFile(hWnd, ofn.lpstrFile, PBi, hBitmap, hdc);
				/*save = 1;*/
			}
				break;
			case ID_FILE_EXIT:
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
            
            // TODO: Add any drawing code that uses hdc here...
			HDC hdc = BeginPaint(hWnd, &ps);
			//Vẽ bitmap ra màn hình trước
			HDC memDC = CreateCompatibleDC(hdc);
			SelectObject(memDC, hBitmap);
			RECT rect;
			GetClientRect(hWnd, &rect);
			BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);
				switch (mode)
				{
				case 0:
					//Vẽ tự do thì ta vẽ luôn trên màn hình và bitmap
					FreeDraw(memDC);
					FreeDraw(hdc);
					break;
				case 1:
					//Vẽ đường thẳng thì ta chỉ vẽ lên màn hình
					DrawLine(hdc);
					break;
				case 2:
					
					DrawEllipse(hdc);
					break;
				case 3:

					DrawRound(hdc);
					break;
				case 4:

					DrawSquare(hdc);
					break;
				case 5:
					//DrawRectangle(memDC);
					DrawRectangle(hdc);
					break;
				}
				
				DeleteObject(memDC);
			//SelectObject(hdc, oldPen);
			
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

VOID DrawLine(HDC hdc)
{
	
		PEN(hdc, Width, R, G, B);
	
	if (enTrainDessin == TRUE)
	{
		
		MoveToEx(hdc, p1.x, p1.y, NULL);
		LineTo(hdc, p2.x, p2.y);
	}
}

VOID DrawRectangle(HDC hdc)
{
	if (enTrainDessin == TRUE)
	{

		//MoveToEx(hdc, p1.x, p1.y, NULL);
		Rectangle(hdc, p1.x, p1.y,p2.x,p2.y);


	}
}

VOID FreeDraw(HDC hdc)
{
	
		PEN(hdc, Width, R, G, B);
	
	if (enTrainDessin == TRUE)
	{
		MoveToEx(hdc, p1.x, p1.y, NULL);
		LineTo(hdc, p2.x, p2.y);
		p1 = p2;
	}
}
VOID	DrawSquareProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		p1.x = GET_X_LPARAM(lParam);
		p1.y = GET_Y_LPARAM(lParam);
		//p2_ancien = p2 = p1;
		enTrainDessin = TRUE;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (wParam&MK_LBUTTON)
		{
			p2.x = GET_X_LPARAM(lParam);
			p2.y = GET_Y_LPARAM(lParam);
			GetWindowRect(hWnd, &windowRect);
			InvalidateRect(hWnd, &windowRect, FALSE);
			UpdateWindow(hWnd);
		}
		break;
	}
	case WM_LBUTTONUP:

		//Tiến hành cho vẽ lên bitmap
		HDC hdc = GetDC(hWnd);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		DrawSquare(memDC);
		DeleteObject(memDC);
		ReleaseDC(hWnd, hdc);

		enTrainDessin = FALSE;
		break;
	}
}
VOID DrawSquare(HDC hdc)
{
	
		PEN(hdc, Width, R, G, B);
	
	if (enTrainDessin == TRUE)
	{
		MoveToEx(hdc, p1.x, p1.y, NULL);
		saveL = min(p1.x,p2.x);

		saveT = min(p1.y, p2.y);

		saveR = min(p1.x, p2.x);

		saveB = min(p1.y, p2.y);
		int leng = min(abs(p2.x - p1.x), abs(p2.y - p1.y));
		if (p2.x < p1.x)
			p1.x = saveR - leng;
		else
			p2.x = saveL + leng;

		if (p2.y < p1.y)
			p1.y = saveB - leng;
		else
			p2.y = saveT + leng;

		Rectangle(hdc,p1.x,p1.y,p2.x,p2.y);



	}
}
VOID	DrawEllipseProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		p1.x = GET_X_LPARAM(lParam);
		p1.y = GET_Y_LPARAM(lParam);
		//p2_ancien = p2 = p1;
		enTrainDessin = TRUE;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (wParam&MK_LBUTTON)
		{
			p2.x = GET_X_LPARAM(lParam);
			p2.y = GET_Y_LPARAM(lParam);
			GetWindowRect(hWnd, &windowRect);
			InvalidateRect(hWnd, &windowRect, FALSE);
			UpdateWindow(hWnd);
		}
		break;
	}
	case WM_LBUTTONUP:

		//Tiến hành cho vẽ lên bitmap
		HDC hdc = GetDC(hWnd);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		DrawEllipse(memDC);
		DeleteObject(memDC);
		ReleaseDC(hWnd, hdc);

		enTrainDessin = FALSE;
		break;
	}
}
VOID				DrawRoundProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		p1.x = GET_X_LPARAM(lParam);
		p1.y = GET_Y_LPARAM(lParam);
		//p2_ancien = p2 = p1;
		enTrainDessin = TRUE;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (wParam&MK_LBUTTON)
		{
			p2.x = GET_X_LPARAM(lParam);
			p2.y = GET_Y_LPARAM(lParam);
			GetWindowRect(hWnd, &windowRect);
			InvalidateRect(hWnd, &windowRect, FALSE);
			UpdateWindow(hWnd);
		}
		break;
	}
	case WM_LBUTTONUP:

		//Tiến hành cho vẽ lên bitmap
		HDC hdc = GetDC(hWnd);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		DrawRound(memDC);
		DeleteObject(memDC);
		ReleaseDC(hWnd, hdc);

		enTrainDessin = FALSE;
		break;
	}
}
VOID DrawRound(HDC hdc)
{

	
		PEN(hdc, Width, R, G, B);
	
	if (enTrainDessin == TRUE)
	{
		MoveToEx(hdc, p1.x, p1.y, NULL);
		saveL = min(p1.x, p2.x);

		saveT = min(p1.y, p2.y);

		saveR = min(p1.x, p2.x);

		saveB = min(p1.y, p2.y);
		int leng = min(abs(p2.x - p1.x), abs(p2.y - p1.y));
		if (p2.x < p1.x)
			p1.x = saveR - leng;
		else
			p2.x = saveL + leng;

		if (p2.y < p1.y)
			p1.y = saveB - leng;
		else
			p2.y = saveT + leng;
		//MoveToEx(hdc, p1.x, p1.y, NULL);
		Ellipse(hdc, p1.x, p1.y, p2.x, p2.y);
	}
}
VOID DrawEllipse(HDC hdc)
{
	
		PEN(hdc, Width, R, G, B);
	
	if (enTrainDessin == TRUE)
	{
		//MoveToEx(hdc, p1.x, p1.y, NULL);
		Ellipse(hdc, p1.x, p1.y,p2.x,p2.y);
	}
}
VOID	DrawRectangleProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		p1.x = GET_X_LPARAM(lParam);
		p1.y = GET_Y_LPARAM(lParam);
		//p2_ancien = p2 = p1;
		enTrainDessin = TRUE;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (wParam&MK_LBUTTON)
		{
			p2.x = GET_X_LPARAM(lParam);
			p2.y = GET_Y_LPARAM(lParam);
			GetWindowRect(hWnd, &windowRect);
			InvalidateRect(hWnd, &windowRect, FALSE);
			UpdateWindow(hWnd);
		}
		break;
	}
	case WM_LBUTTONUP:

		//Tiến hành cho vẽ lên bitmap
		HDC hdc = GetDC(hWnd);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		DrawRectangle(memDC);
		DeleteObject(memDC);
		ReleaseDC(hWnd, hdc);

		enTrainDessin = FALSE;
		break;
	}
}
VOID DrawLineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
		case WM_LBUTTONDOWN:
		{
			p1.x = GET_X_LPARAM(lParam);
			p1.y = GET_Y_LPARAM(lParam);
			p2_ancien = p2 = p1;
			enTrainDessin = TRUE;
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (wParam&MK_LBUTTON)
			{
				p2.x = GET_X_LPARAM(lParam);
				p2.y = GET_Y_LPARAM(lParam);
				GetWindowRect(hWnd, &windowRect);
				InvalidateRect(hWnd, &windowRect, FALSE);
				UpdateWindow(hWnd);
			}
			break;
		}
		case WM_LBUTTONUP:
		
			//Tiến hành cho vẽ lên bitmap
			HDC hdc = GetDC(hWnd);
			HDC memDC = CreateCompatibleDC(hdc);
			SelectObject(memDC, hBitmap);
			DrawLine(memDC);
			DeleteObject(memDC);
			ReleaseDC(hWnd, hdc);

			enTrainDessin = FALSE;
			break;
	}
}

VOID FreeDrawProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		p1.x = GET_X_LPARAM(lParam);
		p1.y = GET_Y_LPARAM(lParam);
		p2 = p1;
		enTrainDessin = TRUE;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (wParam&MK_LBUTTON)
		{
			
			p2.x = GET_X_LPARAM(lParam);
			p2.y = GET_Y_LPARAM(lParam);
			GetWindowRect(hWnd, &windowRect);
			InvalidateRect(hWnd, &windowRect, FALSE);
			UpdateWindow(hWnd);
		}
		break;
	}
	case WM_LBUTTONUP:
		enTrainDessin = FALSE;
		break;


	}
}
VOID PenBrushDemo(HDC hdc)
{
	HPEN hPen = CreatePen(PS_DASHDOT, 3, myColor);
	SelectObject(hdc, hPen);
	MoveToEx(hdc, 100, 100, NULL);
	LineTo(hdc, 200, 200);
	Rectangle(hdc, 100, 200, 300, 300);
	Ellipse(hdc, 100, 200, 300, 300);
	HRGN hRegion = CreateEllipticRgn(100, 200, 300, 300);
	HBRUSH hbrush = CreateSolidBrush(RGB(23, 100, 40));
	FillRgn(hdc, hRegion, hbrush);
	Ellipse(hdc, 100, 200, 300, 300);
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

VOID PEN(HDC hdc, int width, int R, int G, int B)
{
	HPEN  pen = CreatePen(PS_DASHDOT, width, myColor);
	SelectObject(hdc,pen);
}

void doChooseColor(HWND hWnd)
{
	CHOOSECOLOR cc;
	COLORREF acrCustClr[16];

	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = myColor;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColor(&cc))
	{
		HDC hdc = GetDC(hWnd);
		myColor = cc.rgbResult;
		SetTextColor(hdc, myColor);
	}
}
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;
	// Retrieve the bitmap color format, width, and height.  
	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
		errhandler(_T("GetObject"), hwnd);
	// Convert the color format to a count of bits.  
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;
	// Allocate memory for the BITMAPINFO structure. (This structure  
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
	// data structures.)  
	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD)* (1 << cClrBits));
	// There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));
	// Initialize the fields in the BITMAPINFO structure.  
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
	// If the bitmap is not compressed, set the BI_RGB flag.  
	pbmi->bmiHeader.biCompression = BI_RGB;
	// Compute the number of bytes in the array of color  
	// indices and store the result in biSizeImage.  
	// The width must be DWORD aligned unless the bitmap is RLE 
	// compressed. 
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the  
	// device colors are important.  
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC)
{
	HANDLE hf;                 // file handle  
	BITMAPFILEHEADER hdr;       // bitmap file-header  
	PBITMAPINFOHEADER pbih;     // bitmap info-header  
	LPBYTE lpBits;              // memory pointer  
	DWORD dwTotal;              // total count of bytes  
	DWORD cb;                   // incremental count of bytes  
	BYTE *hp;                   // byte pointer  
	DWORD dwTmp;
	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
	if (!lpBits)
		errhandler(_T("GlobalAlloc"), hwnd);
	// Retrieve the color table (RGBQUAD array) and the bits  
	// (array of palette indices) from the DIB.  
	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS))
	{
		errhandler(_T("GetDIBits"), hwnd);
	}
	// Create the .BMP file.  
	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE)
		errhandler(_T("CreateFile"), hwnd);
	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
								// Compute the size of the entire file.  
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	// Compute the offset to the array of color indices.  
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);
	// Copy the BITMAPFILEHEADER into the .BMP file.  
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL))
	{
		errhandler(_T("WriteFile"), hwnd);
	}
	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
		errhandler(_T("WriteFile"), hwnd);
	// Copy the array of color indices into the .BMP file.  
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
		errhandler(_T("WriteFile"), hwnd);
	// Close the .BMP file.  
	if (!CloseHandle(hf))
		errhandler(_T("CloseHandle"), hwnd);
	// Free memory.  
	GlobalFree((HGLOBAL)lpBits);
}
void errhandler(LPCTSTR sz, HWND hwnd)
{
	MessageBox(hwnd, sz, _T("Error"), MB_ICONERROR);
	PostQuitMessage(0);
}
LRESULT CALLBACK AboutBox(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(NULL, IDOK);
			break;
		case IDCANCEL:
			EndDialog(NULL, IDCANCEL);
		case IDC_CHECK1:
			Width = 1;
			break;
		case IDC_CHECK2:
			Width = 2;
			break;
		case IDC_CHECK3:
			Width = 3;
			break;
		}
		break;
	default:
		return 0;
	}
	return 1;
}