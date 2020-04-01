#include "base.h"

void Init_Console_Context(CConsleContext& console_context)
{
	console_context.console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Set_Console_Input(CConsleContext& console_context, string& command_info)
{
	if (console_context.screen_type == enum_screen_Type::SCREEN_IN)
	{
		/*
		console_context.cursorPos.X = 0;
		console_context.cursorPos.Y = console_context.nLine;
		SetConsoleCursorPosition(console_context.hStdout, console_context.cursorPos);
		*/
	}
	else
	{
		console_context.console_line++;
	}

	cout << "Command:>";
	cin.clear();
	getline(cin, command_info);
	console_context.screen_type = enum_screen_Type::SCREEN_IN;
}

void Set_Console_Output(CConsleContext& console_context, string output_info)
{
	cout << output_info.c_str() << endl;
	console_context.screen_type = enum_screen_Type::SCREEN_OUT;
	console_context.console_line++;
}

void Close_Console_Context(CConsleContext& console_context)
{
	CloseHandle(console_context.console_stdout);
}

