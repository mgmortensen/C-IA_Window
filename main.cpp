
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//#include <cassert>

//LRESULT CALLBACK WinProcedure(
//  HWND hWnd,
//  UINT message,
//  WPARAM wParam,
//  LPARAM lParam);
LRESULT CALLBACK WinProcedure(HWND, UINT, WPARAM, LPARAM);

class Window
{
public:
  Window(HWND hWnd = 0) :
      _hWnd(hWnd)
  {
  }

  void Display(int cmdShow)
  {
//    assert(_hWnd != 0);
    ::ShowWindow(_hWnd, cmdShow);
    ::UpdateWindow(_hWnd);
  }

private:
  HWND _hWnd;
};

class WinClassMaker
{
public:
  WinClassMaker(
    WNDPROC WinProcedure,
    LPCWSTR className,
    HINSTANCE hInstance);

  void Register(void)
  {
    if (::RegisterClassExW(&_class) == 0)
    {
      throw "RegisterClass failed";
    }
  }

private:
  WNDCLASSEXW _class;
};

class WinMaker
{
public:
  WinMaker(LPCWSTR className, HINSTANCE hInst);
  HWND Create(LPCWSTR title);

private:
  HINSTANCE _hInst;     // program instance
  LPCWSTR _className;   // name of Window class
  DWORD _style;         // window style
  DWORD _exStyle;       // window extended style
  int _x;               // horizontal position
  int _y;               // vertical position
  int _width;           // window width
  int _height;          // window height
  HWND _hWndParent;     // parent or owner
  HMENU _hMenu;         // menu, or child-window id
  void* _data;          // window-creation data
};

//int APIENTRY wWinMain(
//  _In_ HINSTANCE hInst,
//  _In_opt_ HINSTANCE hPrevInst,
//  _In_ LPWSTR cmdParam,
//  _In_ int cmdShow)
int APIENTRY wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  const WCHAR className[] = L"Winnie";
  WinClassMaker winClass(WinProcedure, className, hInstance);
  winClass.Register();
  WinMaker maker(className, hInstance);
  Window win = maker.Create(L"Hello Windows!");
  win.Display(nCmdShow);

  MSG msg;
  int status = 0;
  while ((status = ::GetMessageW(&msg, 0, 0, 0)) != 0)
  {
    if (status == -1)
    {
      return -1;
    }
    ::DispatchMessageW(&msg);
  }
  return static_cast<int>(msg.wParam);
}

WinClassMaker::WinClassMaker(
  WNDPROC WinProcedure,
  LPCWSTR className,
  HINSTANCE hInst)
{
  _class.lpfnWndProc = WinProcedure;  // mandatory
  _class.hInstance = hInst;           // mandatory
  _class.lpszClassName = className;   // mandatory

  _class.cbSize = sizeof (WNDCLASSEXW);
  _class.hCursor = ::LoadCursorW(0, IDC_ARROW);
  _class.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

  _class.style = 0;
  _class.cbClsExtra = 0;
  _class.cbWndExtra = 0;
  _class.hIcon = 0;
  _class.hIconSm = 0;
  _class.lpszMenuName = 0;
}

WinMaker::WinMaker(
  LPCWSTR className,
  HINSTANCE hInst) :
    _hInst(hInst),
    _className(className),
    _style(WS_OVERLAPPEDWINDOW),
    _exStyle(0),
    _x(CW_USEDEFAULT),      // horizontal position
    _y(0),                  // vertical position
    _width(CW_USEDEFAULT),  // window width
    _height(0),             // window height
    _hWndParent(0),         // parent or owner window
    _hMenu(0),              // menu, or child-window identifier
    _data(nullptr)          // window-creation data
{
}

HWND WinMaker::Create(LPCWSTR title)
{
  HWND hWnd = ::CreateWindowExW(
    _exStyle,
    _className,
    title,
    _style,
    _x,
    _y,
    _width,
    _height,
    _hWndParent,
    _hMenu,
    _hInst,
    _data);

  if (hWnd == 0)
  {
    throw "Window Creation Failed";
  }
  return hWnd;
}

LRESULT CALLBACK WinProcedure(
  HWND hWnd,
  UINT message,
  WPARAM wParam,
  LPARAM lParam)
{
  switch (message)
  {
  case WM_DESTROY:
    ::PostQuitMessage(0);
    return 0;
  }
  return ::DefWindowProcW(hWnd, message, wParam, lParam);
}
