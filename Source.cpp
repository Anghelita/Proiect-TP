#include <windows.h>
#include <stdio.h>
#include <Strsafe.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include<CommCtrl.h>

#define BUTTON1 1001
#define BUTTON2 1002

#define IMPUT1 2001
#define IMPUT2 2002

#define EXIT 3001
#define BROWSE 3002

#define ABOUT 4001
#define INSTRUCTIONS 4002

#define ELEMENTAR 5001
#define KMPT 5002
#define RK 5003
#define BM 5004



HWND hWndedit1, hWndedit2;
LPTSTR text = (LPTSTR)GlobalAlloc(GPTR, 10000000);
LPTSTR cuv = (LPTSTR)GlobalAlloc(GPTR, 100);
int ok1 = 1, ok2=1;
clock_t init, final;
bool end_program = false;

const char g_szClassName[] = "myWindowClass";
const char g_szClassName2[] = "myWindowClass2";

void preKMP(char pattern[], int f[])
{
	int m = strlen(pattern), k;
	f[0] = -1;
	for (int i = 1; i < m; i++)
	{
		k = f[i - 1];
		while (k >= 0)
		{
			if (pattern[k] == pattern[i - 1])
				break;
			else
				k = f[k];
		}
		f[i] = k + 1;
	}
}

//check whether target string contains pattern 
bool KMP(char pattern[], char target[])
{
	int m = strlen(pattern);
	int n = strlen(target);
	int f[1000];
	preKMP(pattern, f);
	int i = 0;
	int k = 0;
	while (i < n)
	{
		if (k == -1)
		{
			i++;
			k = 0;
		}
		else if (target[i] == pattern[k])
		{
			i++;
			k++;
			if (k == m)
				return 1;
		}
		else
			k = f[k];
	}
	return 0;
}

LRESULT CALLBACK WndKMP(HWND KMPW, UINT msg2, WPARAM wParam, LPARAM lParam)
{
	switch (msg2)
	{
	case WM_CLOSE:
		DestroyWindow(KMPW);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(KMPW, msg2, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case EXIT:
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
		break;

		case BUTTON1:
		{
			int len = GetWindowTextLength(GetDlgItem(hwnd, IMPUT1));
			if (len > 0)
			{
				int i;
				GetDlgItemText(hwnd, IMPUT1, text, len + 1);
			}
			if (strcmp(text, "Introduceti textul") == 0)
			{
				MessageBox(hwnd, "Introduceti textul!", "ATENTIE!", MB_OK |
					MB_ICONWARNING);
				break;
			}
			len = GetWindowTextLength(GetDlgItem(hwnd, IMPUT2));
			if (len > 0)
			{
				int i;
				GetDlgItemText(hwnd, IMPUT2, cuv, len + 1);
			}
			if (strcmp(cuv, "Introduceti cuvantul") == 0)
			{
				MessageBox(hwnd, "Introduceti cuvantul!", "ATENTIE!", MB_OK |
					MB_ICONWARNING);
				break;
			}
			if (KMP(cuv, text))
			{
				init = clock();
				for (int i = 0; i < 1000000; i++)
					KMP(cuv, text);
				char ff[10000];
				final = clock() - init;
				itoa((double)final, ff, 10);
				strcat(ff, " -KMP");
				MessageBox(hwnd, ff, "ATENTIE!", MB_OK |
					MB_ICONWARNING);
			}
			else
				MessageBox(hwnd, "Cuvantul nu a fost gasit in text", "ATENTIE!", MB_OK |
					MB_ICONWARNING);
		}
		break;

		case BROWSE:
		{
			OPENFILENAME ofn1;
			char upath[1000] = "";

			ZeroMemory(&ofn1, sizeof(ofn1));

			ofn1.lStructSize = sizeof(ofn1);
			ofn1.hwndOwner = hwnd;
			ofn1.lpstrFilter = ".txt,*\0";
			ofn1.lpstrFile = upath;
			ofn1.nMaxFile = 10000;
			ofn1.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn1.lpstrDefExt = "";
			if (GetOpenFileName(&ofn1)&&(strcmp(upath+strlen(upath)-4,".txt")==0))
			{
				ok1 = 0;
				FILE *f = fopen(upath, "r");
				char cuv[1000],lit;
				int i=0;
				SendMessage(hWndedit1, WM_SETTEXT, NULL, NULL);
				while (!feof(f))
				{
					lit = getc(f);
					if (lit == '\n'|| lit == '\0')
					{
						cuv[i] = '\0';
						i = 0;
						SendMessage(hWndedit1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
						SendMessage(hWndedit1, EM_REPLACESEL, TRUE, (LPARAM)"\r\n");
						strcpy(cuv, "                                  ");
						lit = ' ';
					}
					else
					{
						cuv[i] = lit;
						i++;
					}
					cuv[i] = '\0';
				}
				cuv[i - 1] = '\0';
				SendMessage(hWndedit1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
				fclose(f);
			}
			else
			{
				MessageBox(hwnd, "Doar fisierele de tip .txt sunt suportate!", "ERROR!", MB_OK |
					MB_ICONWARNING);
			}
		}
		break;

		case ELEMENTAR:
		{
			HWND KMPW = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				g_szClassName2,
				"KMP",
				WS_OVERLAPPED | WS_SYSMENU,
				CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
				NULL, NULL, NULL, NULL);
			if (KMPW == NULL)
			{
				MessageBox(NULL, "Window Creation Failed!", "Error!",
					MB_ICONEXCLAMATION | MB_OK);
				return 0;
			}
			HWND hWndtxt1 = CreateWindowEx(WS_EX_APPWINDOW,
				"EDIT",
				"Introduceti textul",
				WS_VSCROLL | WS_TABSTOP | WS_VISIBLE |
				WS_CHILD | WS_BORDER | ES_MULTILINE,
				10,
				10,
				560,
				540,
				KMPW,
				(HMENU)IMPUT1,
				GetModuleHandle(NULL),
				NULL);
			ShowWindow(KMPW, 1);
			UpdateWindow(KMPW);
			ok1 = 0;
			FILE *g = fopen("ELEMENTAR.txt", "r");
			char cuv[1000], lit;
			int i = 0;
			SendMessage(hWndtxt1, WM_SETTEXT, NULL, NULL);
			while (!feof(g))
			{
				lit = getc(g);
				if (lit == '\n' || lit == '\0')
				{
					cuv[i] = '\0';
					i = 0;
					SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
					SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)"\r\n");
					strcpy(cuv, "                                  ");
					lit = ' ';
				}
				else
				{
					cuv[i] = lit;
					i++;
				}
				cuv[i] = '\0';
			}
			cuv[i - 1] = '\0';
			SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
			fclose(g);
		}
		break;

		case KMPT:
		{
			HWND KMPW = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				g_szClassName2,
				"KMP",
				WS_OVERLAPPED | WS_SYSMENU,
				CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
				NULL, NULL, NULL, NULL);
			if (KMPW== NULL)
			{
				MessageBox(NULL, "Window Creation Failed!", "Error!",
					MB_ICONEXCLAMATION | MB_OK);
				return 0;
			}
			HWND hWndtxt1 = CreateWindowEx(WS_EX_APPWINDOW,
				"EDIT",
				"Introduceti textul",
				WS_VSCROLL | WS_TABSTOP | WS_VISIBLE |
				WS_CHILD | WS_BORDER | ES_MULTILINE,
				10,
				10,
				560,
				540,
				KMPW,
				(HMENU)IMPUT1,
				GetModuleHandle(NULL),
				NULL);
			ShowWindow(KMPW, 1);
			UpdateWindow(KMPW);
				ok1 = 0;
				FILE *f = fopen("KMP.txt", "r");
				char cuv[1000], lit;
				int i = 0;
				SendMessage(hWndtxt1, WM_SETTEXT, NULL, NULL);
				while (!feof(f))
				{
					lit = getc(f);
					if (lit == '\n' || lit == '\0')
					{
						cuv[i] = '\0';
						i = 0;
						SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
						SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)"\r\n");
						strcpy(cuv, "                                  ");
						lit = ' ';
					}
					else
					{
						cuv[i] = lit;
						i++;
					}
					cuv[i] = '\0';
				}
				cuv[i - 1] = '\0';
				SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
				fclose(f);

		}
		break;

		case RK:
		{
			HWND KMPW = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				g_szClassName2,
				"RK",
				WS_OVERLAPPED | WS_SYSMENU,
				CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
				NULL, NULL, NULL, NULL);
			if (KMPW == NULL)
			{
				MessageBox(NULL, "Window Creation Failed!", "Error!",
					MB_ICONEXCLAMATION | MB_OK);
				return 0;
			}
			HWND hWndtxt1 = CreateWindowEx(WS_EX_APPWINDOW,
				"EDIT",
				"Introduceti textul",
				WS_VSCROLL | WS_TABSTOP | WS_VISIBLE |
				WS_CHILD | WS_BORDER | ES_MULTILINE,
				10,
				10,
				560,
				540,
				KMPW,
				(HMENU)IMPUT1,
				GetModuleHandle(NULL),
				NULL);
			ShowWindow(KMPW, 1);
			UpdateWindow(KMPW);
			ok1 = 0;
			FILE *f = fopen("RK.txt", "r");
			char cuv[1000], lit;
			int i = 0;
			SendMessage(hWndtxt1, WM_SETTEXT, NULL, NULL);
			while (!feof(f))
			{
				lit = getc(f);
				if (lit == '\n' || lit == '\0')
				{
					cuv[i] = '\0';
					i = 0;
					SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
					SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)"\r\n");
					strcpy(cuv, "                                  ");
					lit = ' ';
				}
				else
				{
					cuv[i] = lit;
					i++;
				}
				cuv[i] = '\0';
			}
			cuv[i - 1] = '\0';
			SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
			fclose(f);
		}
		break;

		case BM:
		{
			HWND KMPW = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				g_szClassName2,
				"RK",
				WS_OVERLAPPED | WS_SYSMENU,
				CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
				NULL, NULL, NULL, NULL);
			if (KMPW == NULL)
			{
				MessageBox(NULL, "Window Creation Failed!", "Error!",
					MB_ICONEXCLAMATION | MB_OK);
				return 0;
			}
			HWND hWndtxt1 = CreateWindowEx(WS_EX_APPWINDOW,
				"EDIT",
				"Introduceti textul",
				WS_VSCROLL | WS_TABSTOP | WS_VISIBLE |
				WS_CHILD | WS_BORDER | ES_MULTILINE,
				10,
				10,
				560,
				540,
				KMPW,
				(HMENU)IMPUT1,
				GetModuleHandle(NULL),
				NULL);
			ShowWindow(KMPW, 1);
			UpdateWindow(KMPW);
			ok1 = 0;
			FILE *f = fopen("BM.txt", "r");
			char cuv[1000], lit;
			int i = 0;
			SendMessage(hWndtxt1, WM_SETTEXT, NULL, NULL);
			while (!feof(f))
			{
				lit = getc(f);
				if (lit == '\n' || lit == '\0')
				{
					cuv[i] = '\0';
					i = 0;
					SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
					SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)"\r\n");
					strcpy(cuv, "                                  ");
					lit = ' ';
				}
				else
				{
					cuv[i] = lit;
					i++;
				}
				cuv[i] = '\0';
			}
			cuv[i - 1] = '\0';
			SendMessage(hWndtxt1, EM_REPLACESEL, TRUE, (LPARAM)cuv);
			fclose(f);
		}
		break;
		}
		break;
	}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		end_program = true;
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW -2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"The title of my window",
		WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	HMENU hMenu, hSubMenu;
	hMenu = CreateMenu();
	hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING, BROWSE, "Browse");
	AppendMenu(hSubMenu, MF_STRING, EXIT, "Exit");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");
	hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING, ELEMENTAR, "&Algoritm elementar");
	AppendMenu(hSubMenu, MF_STRING, KMPT, "&Knuth Morris Pratt");
	AppendMenu(hSubMenu, MF_STRING, RK, "&Rabin-Karp");
	AppendMenu(hSubMenu, MF_STRING, BM, "&Boyer-Moore");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Pseudocod algoritmi");
	SetMenu(hwnd, hMenu);

	HWND hWndtxt1 = CreateWindowEx(WS_EX_APPWINDOW,
		"STATIC",
		"Pasul 1:",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		10,
		20,
		560,
		20,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	hWndedit1 = CreateWindowEx(WS_EX_APPWINDOW,
		"EDIT",
		"Introduceti textul",
		WS_VSCROLL | WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | WS_BORDER | ES_MULTILINE,
		10,
		50,
		560,
		210,
		hwnd,
		(HMENU)IMPUT1,
		GetModuleHandle(NULL),
		NULL);

	HWND hWndtxt2 = CreateWindowEx(WS_EX_APPWINDOW,
		"STATIC",
		"Pasul 2:",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		10,
		280,
		560,
		20,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	hWndedit2 = CreateWindowEx(WS_EX_APPWINDOW,
		"EDIT",
		"Introduceti cuvantul",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | WS_BORDER | ES_MULTILINE,
		10,
		310,
		560,
		20,
		hwnd,
		(HMENU)IMPUT2,
		GetModuleHandle(NULL),
		NULL);

	HWND hWndtxt3 = CreateWindowEx(WS_EX_APPWINDOW,
		"STATIC",
		"Pasul 3:",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		10,
		350,
		560,
		20,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	HWND hWndButton = CreateWindowEx(NULL,
		"BUTTON",//-stilul
		"COMPARA",//-ce srie pe el
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		190,//-poz
		380,//-poz
		200,//-marime
		100,//-matime
		hwnd,
		(HMENU)BUTTON1,
		GetModuleHandle(NULL),
		NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	WNDCLASSEX w;
	MSG Msg2;
	w.cbSize = sizeof(WNDCLASSEX);
	w.style = 0;
	w.lpfnWndProc = WndKMP;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;
	w.hInstance = NULL;
	w.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hbrBackground = (HBRUSH)(COLOR_WINDOW - 2);
	w.lpszMenuName = NULL;
	w.lpszClassName = g_szClassName2;
	w.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&w))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	while (!end_program)
	{
		GetMessage(&Msg, NULL, 0, 0);
		if (GetFocus() == hWndedit1 && ok1 == 1)
		{
			SendMessage(hWndedit1, WM_SETTEXT, NULL, NULL);
			ok1 = 0;
		}
		if (GetFocus() != hWndedit1 && GetWindowTextLength(GetDlgItem(hwnd, IMPUT1)) == NULL)
		{
			SendMessage(hWndedit1, WM_SETTEXT, NULL, (LPARAM)"Introduceti textul");
			ok1 = 1;
		}

		if (GetFocus() == hWndedit2 && ok2 == 1)
		{
			SendMessage(hWndedit2, WM_SETTEXT, NULL, NULL);
			ok2 = 0;
		}
		if (GetFocus() != hWndedit2 && GetWindowTextLength(GetDlgItem(hwnd, IMPUT2)) == NULL)
		{
			SendMessage(hWndedit2, WM_SETTEXT, NULL, (LPARAM)"Introduceti cuvantul");
			ok2 = 1;
		}

		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		UpdateWindow(hwnd);
	}

	return Msg.wParam;
}