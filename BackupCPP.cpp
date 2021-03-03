// BackupCPP.cpp : Defines the entry point for the application.
//

#include"stdafx.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<shellapi.h>
#include<direct.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// misc. strings
	char cStr1[_MAX_PATH];
	char cStr2[_MAX_PATH];
	char cStr3[_MAX_PATH];

	// time data
	SYSTEMTIME st;
	tm tm;

	// file handle
	HANDLE hFile=(HANDLE)INVALID_HANDLE_VALUE;

	// bytes written
	DWORD dwBytes=0;

	// create backup path (if necessary)
	CreateDirectory("Backup",
					NULL);

	// get current time
	GetLocalTime(&st);

	// convert to tm data
	tm.tm_hour =st.wHour;
	tm.tm_isdst=-1;
	tm.tm_mday =st.wDay;
	tm.tm_min  =st.wMinute;
	tm.tm_mon  =st.wMonth-1;
	tm.tm_sec  =st.wSecond;
	tm.tm_wday =st.wDayOfWeek;
	tm.tm_yday =0;
	tm.tm_year=st.wYear-1900;

	// prepare folder path
	strftime(cStr1,
			 _MAX_PATH-1,
			 "Backup\\%Y.%m.%d.%H.%M.%S",
			 &tm);

	// create backup folder
	if(!CreateDirectory(cStr1,
						NULL))
	{
		MessageBox(NULL,
				   "Failed to create backup folder.",
				   "BackupCPP.exe",
				   MB_OK);
		return(0);
	}

	// create output file
	hFile=CreateFile("backup.bat",
					 GENERIC_WRITE,
					 NULL,
					 NULL,
					 CREATE_ALWAYS,
					 NULL,
					 NULL);

	// check for error
	if(hFile==INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,
				   "Failed to create backup file.",
				   "BackupCPP.exe",
				   MB_OK);
		return(0);
	}

	// prepare commands
	sprintf(cStr3,
			"copy *.h %s\\ /y\ncopy *.cpp %s\\ /y\nxcopy data %s\\data /e /c /y /i\n",
			cStr1,
			cStr1,
			cStr1);

	// output commands
	WriteFile(hFile,
			  (LPVOID)cStr3,
			  (DWORD)strlen(cStr3),
			  &dwBytes,
			  NULL);

	// check for error
	if(dwBytes!=(DWORD)strlen(cStr3))
	{
		MessageBox(NULL,
				   "Failed to write to backup file.",
				   "BackupCPP.exe",
				   MB_OK);
		CloseHandle(hFile);
		return(0);
	}

	// close file
	CloseHandle(hFile);

	// get current folder
	getcwd(cStr2,
		   _MAX_PATH);

	// execute backup command
	ShellExecute(NULL,
				 "open",
				 "backup.bat",
				 NULL,
				 cStr2,
				 SW_SHOW);

	// confirmation message
	MessageBox(NULL,
			   "Click when backup completes.",
			   "BackupCPP.exe",
			   MB_OK);

	// delete backup file
	DeleteFile("backup.bat");
	
	// ok
	return(0);
}



