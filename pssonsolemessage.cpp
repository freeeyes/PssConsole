#include "pssconsolemessage.h"

CPSSConsoleMessage::CPSSConsoleMessage()
{

}

string CPSSConsoleMessage::Get_console_command(string _user, string _user_command)
{
	//ƴ�ӷ����ַ���
	string _console_message = "i " + _user + " " + _user_command + "&";
	return _console_message;
}
