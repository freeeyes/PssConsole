#include "base.h"

void Init_Console_Context(CConsleContext& console_context)
{
#ifdef _WIN32
	console_context.console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void Close_Console_Context(CConsleContext& console_context)
{
#ifdef _WIN32
	CloseHandle(console_context.console_stdout);
#endif
}

void Set_Console_Input(CConsleContext& console_context, string& command_info)
{
	cout << "Command:>";
	cin.clear();
	getline(cin, command_info);
	console_context.screen_type = enum_screen_Type::SCREEN_IN;
	console_context.console_line++;
	console_context.last_line_size = 9;
}

void Set_Console_Output(CConsleContext& console_context, string output_info)
{
	cout << output_info.c_str() << endl;
	console_context.screen_type = enum_screen_Type::SCREEN_OUT;
	console_context.console_line++;
	console_context.last_line_size = (unsigned int)output_info.length();
}

void Set_Console_Output_singleLine(CConsleContext& console_context, string output_info)
{
#ifdef WIN32
	console_context.console_cursorPos.X = 0;
	console_context.console_cursorPos.Y = console_context.console_line - 1;
	SetConsoleCursorPosition(console_context.console_stdout, console_context.console_cursorPos);

	string curtain;
	curtain.resize(console_context.last_line_size, ' ');
	cout << curtain << endl;
	SetConsoleCursorPosition(console_context.console_stdout, console_context.console_cursorPos);
#endif
	cout << output_info.c_str() << endl;
	console_context.screen_type = enum_screen_Type::SCREEN_OUT;
}

