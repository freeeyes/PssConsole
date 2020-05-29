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

enum class emum_text_color
{
	CONSOLE_FOREGROUND_BLUE = 0x0001, // text color contains blue.
    CONSOLE_FOREGROUND_GREEN = 0x0002, // text color contains green.
	CONSOLE_FOREGROUND_RED = 0x0004,  // text color contains red.
	CONSOLE_FOREGROUND_INTENSITY = 0x0008, // text color is intensified.
	CONSOLE_BACKGROUND_BLUE = 0x0010, // background color contains blue.
	CONSOLE_BACKGROUND_GREEN = 0x0020, // background color contains green.
	CONSOLE_BACKGROUND_RED = 0x0040, // background color contains red.
	CONSOLE_BACKGROUND_INTENSITY = 0x0080, // background color is intensified.
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


void Init_Console_Context(shared_ptr<CConsleContext> console_context);

void Set_Console_Input(shared_ptr<CConsleContext> console_context, string& command_info);

void Set_Console_Output(shared_ptr<CConsleContext> console_context, string output_info, emum_text_color color = emum_text_color::CONSOLE_FOREGROUND_GREEN);

void Set_Console_Output_singleLine(shared_ptr<CConsleContext> console_context, string output_info);

void Close_Console_Context(shared_ptr<CConsleContext> console_context);