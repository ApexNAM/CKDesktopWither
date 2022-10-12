#include <Windows.h>
#include <math.h>
#define BSIZE 40

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Oh No.");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

double OnCollisonEnter(int x1, int y1, int x2, int y2)
{
	return (sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;

	static int Vector2[3][3];
	static BOOL selection;

	static int mx, my;
	static POINT ptMouse;
	int tempx = 0, tempy = 0;

	static int teamIndex = 0;

	switch (iMessage)
	{
	case WM_CREATE:
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				Vector2[x][y] = 0;
			}
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				MyBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
				OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
				Rectangle(hdc, x * 30, 0 + y * 30, 30 + x * 30, 30 + y * 30);
				SelectObject(hdc, OldBrush);

				if (Vector2[x][y] == 1)
				{
					MyBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
					OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
					Ellipse(hdc, x * 30, 0 + y * 30, 30 + x * 30, 30 + y * 30);
					SelectObject(hdc, OldBrush);
				}
				else if (Vector2[x][y] == 2)
				{
					MyBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
					OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
					Ellipse(hdc, x * 30, 0 + y * 30, 30 + x * 30, 30 + y * 30);
					SelectObject(hdc, OldBrush);
				}
			}
		}

		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		tempx = mx / 30;
		tempy = my / 30;

		if (tempx < 3 && tempy < 3)
		{
			if (teamIndex >= 2) teamIndex = 1;
			else teamIndex++;

			Vector2[tempx][tempy] = teamIndex;
			InvalidateRect(hWnd, NULL, TRUE);
		}



		return 0;
	case WM_RBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		tempx = mx / 30;
		tempy = my / 30;

		if (tempx < 3 && tempy < 3)
		{
			Vector2[tempx][tempy] = 0;
			InvalidateRect(hWnd, NULL, TRUE);
		}

		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_SPACE )
		{
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					Vector2[x][y] = 0;
				}
			}

			InvalidateRect(hWnd, NULL, TRUE);
		}
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
