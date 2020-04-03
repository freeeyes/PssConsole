#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>   
#endif
#include <iostream>
#include <string>

using namespace std;

const string console_title = R"(==========================================================
 _____   _____ _____    _____                      _      
|  __ \ / ____/ ____|  / ____|                    | |     
| |__) | (___| (___   | |     ___  _ __  ___  ___ | | ___ 
|  ___/ \___ \\___ \  | |    / _ \| '_ \/ __|/ _ \| |/ _ \
| |     ____) |___) | | |___| (_) | | | \__ \ (_) | |  __/
|_|    |_____/_____/   \_____\___/|_| |_|___/\___/|_|\___|
==========================================================
)";

enum class enum_screen_Type
{
	SCREEN_IN = 0,
	SCREEN_OUT,
};


class CConsleContext
{
public:
	CConsleContext()
	{
#ifdef _WIN32
		console_cursorPos.X = 0;
		console_cursorPos.Y = 0;
#endif
	};

#ifdef _WIN32
	HANDLE console_stdout = 0;
	COORD  console_cursorPos;
#endif
	unsigned int console_line    = 9;
	unsigned int last_line_size  = 0;
	enum_screen_Type screen_type = enum_screen_Type::SCREEN_OUT;

	string server_ip;
	int server_port = 0;
	string user_name;
	string user_password;
};


void Init_Console_Context(CConsleContext& console_context);

void Set_Console_Input(CConsleContext& console_context, string& command_info);

void Set_Console_Output(CConsleContext& console_context, string output_info, short color = 2);

void Set_Console_Output_singleLine(CConsleContext& console_context, string output_info);

void Close_Console_Context(CConsleContext& console_context);