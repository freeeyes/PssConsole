#include "clientconnect.h"

int main()
{
    CConsleContext console_context;
    string strCommand;

    Init_Console_Context(console_context);

    cout << console_title << endl;

    read_client_connect_info(console_context, CLIENT_CONFIG_FILE);

    while (true)
    {
        Set_Console_Input(console_context, strCommand);

        if (strcmp(strCommand.c_str(),"bye") == 0)
        {
            break;
        }
        else
        {
            if (strCommand.length() > 0)
            {
                Set_Console_Output(console_context, "Command is (" + strCommand + ")");
                //Set_Console_Output_singleLine(console_context, "OK");
            }
        }
    }

    cout << R"(Pss Consloe is exit.)" << endl;
    Close_Console_Context(console_context);

    return 0;
}