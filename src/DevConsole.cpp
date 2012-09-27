#include <conio.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <string>
#include <sstream>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <tchar.h>

#include "DevConsole.h"

using namespace std;

DevConsole* DevConsole::m_Singletone = NULL;

DevConsole::DevConsole()
{
  CONSOLE_SCREEN_BUFFER_INFO coninfo;
  int hConHandle;
  long lStdHandle;
  FILE *fp;

  if(!AllocConsole())
  {
    std::cerr << "Could not initialize SDL" << std::endl;
  }

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

  lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "w" );
  *stdout = *fp;
  setvbuf( stdout, NULL, _IONBF, 0 );

  lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "r" );
  *stdin = *fp;
  setvbuf( stdin, NULL, _IONBF, 0 );

  lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "w" );
  *stderr = *fp;
  setvbuf( stderr, NULL, _IONBF, 0 );

}

DevConsole* DevConsole::Init()
{
  if(m_Singletone == NULL)
		m_Singletone = new DevConsole();
  return m_Singletone;
}

void DevConsole::logMessage(const char* message)
{
	cout << message;
	cout << "\n";
}

void DevConsole::logError(const char* error)
{
	cerr << error;
	cerr << "\n";
}