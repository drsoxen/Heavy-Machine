#include <ctime>
#include <iostream>
#include <windows.h>
#include <stdio.h>

#include "glee/GLee.h"
#include "wglext.h"
#include "glwindow.h"
#include "example.h"
#include "InputManager.h"



GLWindow::GLWindow(HINSTANCE hInstance):
m_isRunning(false),
	m_example(NULL),
	m_hinstance(hInstance),
	m_lastTime(0)
{

}

bool GLWindow::create(int width, int height, int bpp, bool fullscreen)
{
	DWORD      dwExStyle;       // Window Extended Style
	DWORD      dwStyle;         // Window Style

	m_isFullscreen = fullscreen; //Store the fullscreen flag

	m_windowRect.left = (long)0; // Set Left Value To 0
	m_windowRect.right = (long)width; // Set Right Value To Requested Width
	m_windowRect.top = (long)0;  // Set Top Value To 0
	m_windowRect.bottom = (long)height;   // Set Bottom Value To Requested Height

	// fill out the window class structure
	m_windowClass.cbSize          = sizeof(WNDCLASSEX);
	m_windowClass.style           = CS_HREDRAW | CS_VREDRAW;
	m_windowClass.lpfnWndProc     = GLWindow::StaticWndProc; //We set our static method as the event handler
	m_windowClass.cbClsExtra      = 0;
	m_windowClass.cbWndExtra      = 0;
	m_windowClass.hInstance       = m_hinstance;
	m_windowClass.hIcon           = LoadIcon(NULL, IDI_APPLICATION);  // default icon
	m_windowClass.hCursor         = LoadCursor(NULL, IDC_HAND);      // default arrow
	m_windowClass.hbrBackground   = NULL;                             // don't need background
	m_windowClass.lpszMenuName    = NULL;                             // no menu
	m_windowClass.lpszClassName   = "GLClass";
	m_windowClass.hIconSm         = LoadIcon(NULL, IDI_WINLOGO);      // windows logo small icon

	// register the windows class
	if (!RegisterClassEx(&m_windowClass))
	{
		return false;
	}

	if (m_isFullscreen) //If we are fullscreen, we need to change the display mode                             
	{
		DEVMODE dmScreenSettings;                   // device mode

		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings); 

		dmScreenSettings.dmPelsWidth = width;         // screen width
		dmScreenSettings.dmPelsHeight = height;           // screen height
		dmScreenSettings.dmBitsPerPel = bpp;             // bits per pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// setting display mode failed, switch to windowed
			MessageBox(NULL, "Display mode failed", NULL, MB_OK);
			m_isFullscreen = false; 
		}
	}

	if (m_isFullscreen)                             // Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;                  // Window Extended Style
		dwStyle = WS_POPUP;                       // Windows Style
		ShowCursor(false);                      // Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;                    // Windows Style
	}

	AdjustWindowRectEx(&m_windowRect, dwStyle, false, dwExStyle);     // Adjust Window To True Requested Size

	// class registered, so now create our window
	m_hwnd = CreateWindowEx(NULL,                                 // extended style
		"GLClass",                          // class name
		"BOGLGP - Chapter 2 - Simple OpenGL Application",      // app name
		dwStyle | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,                               // x,y coordinate
		m_windowRect.right - m_windowRect.left,
		m_windowRect.bottom - m_windowRect.top, // width, height
		NULL,                               // handle to parent
		NULL,                               // handle to menu
		m_hinstance,                          // application instance
		this);                              // we pass a pointer to the GLWindow here

	// check if window creation failed (hwnd would equal NULL)
	if (!m_hwnd)
		return 0;

	m_hdc = GetDC(m_hwnd);

	ShowWindow(m_hwnd, SW_SHOW);          // display the window
	UpdateWindow(m_hwnd);                 // update the window

	m_lastTime = GetTickCount() / 1000.0f; //Initialize the time
	return true;
}

void GLWindow::destroy() 
{
	if (m_isFullscreen)
	{
		ChangeDisplaySettings(NULL, 0);          // If So Switch Back To The Desktop
		ShowCursor(true);                       // Show Mouse Pointer
	}
}

void GLWindow::attachExample(Example* example)
{
	m_example = example;
}

bool GLWindow::isRunning()
{
	return m_isRunning;
}

void GLWindow::processEvents()
{
	MSG msg;

	//While there are messages in the queue, store them in msg
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//Process the messages one-by-one
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void GLWindow::setupPixelFormat(void)
{
	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd =
	{   
		sizeof(PIXELFORMATDESCRIPTOR),  // size
		1,                          // version
		PFD_SUPPORT_OPENGL |        // OpenGL window
		PFD_DRAW_TO_WINDOW |        // render to window
		PFD_DOUBLEBUFFER,           // support double-buffering
		PFD_TYPE_RGBA,              // color type
		32,                         // preferred color depth
		0, 0, 0, 0, 0, 0,           // color bits (ignored)
		0,                          // no alpha buffer
		0,                          // alpha bits (ignored)
		0,                          // no accumulation buffer
		0, 0, 0, 0,                 // accum bits (ignored)
		16,                         // depth buffer
		0,                          // no stencil buffer
		0,                          // no auxiliary buffers
		PFD_MAIN_PLANE,             // main layer
		0,                          // reserved
		0, 0, 0,                    // no layer, visible, damage masks
	};

	pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
	SetPixelFormat(m_hdc, pixelFormat, &pfd);
}

LRESULT GLWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:         // window creation
		{
			m_hdc = GetDC(hWnd);//sets the hdc to the handle to the window
			setupPixelFormat();//sets the colour depth eg. 8 bit 16 bit, 32 bit

			//Set the version that we want, in this case 3.0
			int attribs[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,//the major version
				WGL_CONTEXT_MINOR_VERSION_ARB, 0,// the minor version
				0}; //zero indicates the end of the array

				//Create temporary context so we can get a pointer to the function
				HGLRC tmpContext = wglCreateContext(m_hdc);//makes the tmpContext which is a handle to the OpenGL rendering context
				//Make it current
				wglMakeCurrent(m_hdc, tmpContext);//uses the handle to the OpenGL rendering context, tmpContext to make it the current

				//Get the function pointer
				wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");//asks the computer if it has wglCreateContextAttribsARB which
				//returns a void* then we cast it to a PFNWGLCREATECONTEXTATTRIBSARBPROC

				//If this is NULL then OpenGL 3.0 is not supported
				if (!wglCreateContextAttribsARB)//if the void* returns NULL it is using an older version so we fall back on OpenGL 2.1
				{
					std::cerr << "OpenGL 3.0 is not supported, falling back to GL 2.1" << std::endl;//output a cerr
					m_hglrc = tmpContext;//set the handle to the OpenGL rendering context to the tmpContext
				} 
				else
				{
					// Create an OpenGL 3.0 context using the new function
					m_hglrc = wglCreateContextAttribsARB(m_hdc, 0, attribs);//if the void* isn't NULL then set the handle to the OpenGL rendering context to the void*
					//Delete the temporary context
					wglDeleteContext(tmpContext);//delete the tmpContext cause we don't need it any more
				}

				//Make the GL3 context current
				wglMakeCurrent(m_hdc, m_hglrc);// sets the handle to the OpenGL rendering context

				m_isRunning = true; //Mark our window as running
		}
		break;
	case WM_DESTROY: // window destroy
	case WM_CLOSE: // windows is closing
		wglMakeCurrent(m_hdc, NULL);//NULLs the current handle to the OpenGL rendering context
		wglDeleteContext(m_hglrc);//and deletes the handle to the OpenGL rendering context
		m_isRunning = false; //Stop the main loop
		PostQuitMessage(0); //Send a WM_QUIT message
		return 0;
		break;
	case WM_SIZE:
		{
			int height = HIWORD(lParam);        // retrieve width and height
			int width = LOWORD(lParam);
			getAttachedExample()->onResize(width, height); //Call the example's resize method
		}
		break;
	case WM_KEYDOWN:
		InputManager::GetInstance()->handleInput(wParam, true);

		if (wParam == VK_ESCAPE) //If the escape key was pressed
		{
			DestroyWindow(m_hwnd); //Send a WM_DESTROY message
		}
		break;
	case WM_KEYUP:
		InputManager::GetInstance()->handleInput(wParam, false);
		break;
	case WM_MOUSEWHEEL:
		{
			switch(GET_WHEEL_DELTA_WPARAM(wParam))
			{
			case -120: 
					m_example->m_CamZ+=15;
				break;
			case 120:
					m_example->m_CamZ-=15;
				break;
			}
		}

		break;
	case WM_LBUTTONDOWN:
		m_example->isPressed = true;
		break;

	case WM_LBUTTONUP:
		m_example->isPressed = false;
		break;

	case WM_MOUSEMOVE:

		m_example->mouseX = LOWORD(lParam);
		m_example->mouseY = HIWORD(lParam);
		if(m_example->isPressed)
		{
			if(m_example->mouseY > m_example->prevMouseY)
				m_example->m_CamY++;

			if(m_example->mouseY < m_example->prevMouseY)
				m_example->m_CamY--;

			if(m_example->mouseX > m_example->prevMouseX)
				m_example->m_CamX--;

			if(m_example->mouseX < m_example->prevMouseX)
				m_example->m_CamX++;

			m_example->prevMouseX = m_example->mouseX;
			m_example->prevMouseY = m_example->mouseY;
		}
		break;

		//m_example->handleInput(wParam);

		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK GLWindow::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	GLWindow* window = NULL;

	//If this is the create message
	if(uMsg == WM_CREATE)
	{
		//Get the pointer we stored during create
		window = (GLWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;

		//Associate the window pointer with the hwnd for the other events to access
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)window);
	}
	else
	{
		//If this is not a creation event, then we should have stored a pointer for the window
		window = (GLWindow*)GetWindowLongPtr(hWnd, GWL_USERDATA);

		if(!window) 
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);    
		}
	}

	//Call our window's member WndProc (allows us to access member variables)
	return window->WndProc(hWnd, uMsg, wParam, lParam);
}

float GLWindow::getElapsedSeconds()
{
	float currentTime = float(GetTickCount()) / 1000.0f;
	float seconds = float(currentTime - m_lastTime);
	m_lastTime = currentTime;
	return seconds;
}
