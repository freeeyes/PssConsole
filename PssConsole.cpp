#include "clientconnect.h"
#include "messagebusthread.h"
#include "pssconsolemessage.h"

int main()
{
    char szBuff[10] = { '\0' };
    int nSrc = 10;
    memcpy_s(szBuff, sizeof(int), (char*)&nSrc, sizeof(int));
    memcpy_s(&szBuff[4], sizeof(int), (char*)&nSrc, sizeof(int));
    int ndes = *(reinterpret_cast<int*>(szBuff));
    printf_s("[Data]=%d.\n", ndes);

    shared_ptr<CConsleContext> console_context = std::make_shared<CConsleContext>();
    string _user_command;
    CMessageThread _message_bus_thread;

    Init_Console_Context(console_context);

    cout << console_title << endl;

    if (false == read_client_connect_info(console_context, CLIENT_CONFIG_FILE))
    {
        cout << "read (" << CLIENT_CONFIG_FILE << ") is error." << endl;
        return 0;
    }

    //初始化TCP线程
    shared_ptr<TcpService> service = TcpService::Create();
    shared_ptr<AsyncConnector> connector = AsyncConnector::Create();
    wrapper::ConnectionBuilder connectionBuilder;

	//启动服务线程
	service->startWorkerThread(1);
	//启动链接线程
	connector->startWorkerThread();

    //创建消息总线
    _message_bus_thread.Create(1);

    while (true)
    {
        Set_Console_Input(console_context, _user_command);

        if (strcmp(_user_command.c_str(),"bye") == 0)
        {
            break;
        }
        else
        {
            if (_user_command.length() > 0)
            {
                //Set_Console_Output(console_context, "Command is (" + _user_command + ")");
                //Set_Console_Output_singleLine(console_context, "OK");

                _message_bus_thread.Add_do_function([_user_command, console_context, service, connector, connectionBuilder]
                    {
                        //处理消息发送事件
                        CPSSConsoleMessage pss_console_message;
                        string console_message = pss_console_message.Get_console_command(console_context->user_name, _user_command);

						if (false == console_context->connect_state)
						{
							connect_server(console_context, service, connector, connectionBuilder);
						}

                        //如果连接已经存在
                        if (true == console_context->connect_state)
                        {
                            //发送数据
                            console_context->tcp_session->send(console_message.c_str(), 
                                console_message.length(), 
                                [console_context]()
                                { 
                                    Set_Console_Output(console_context, "send data is ok.");
                                });
                        }
                    });
            }
        }
    }

    cout << R"(Pss Consloe is exit.)" << endl;
    Close_Console_Context(console_context);

    return 0;
}