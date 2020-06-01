#include "clientconnect.h"

bool read_client_connect_info(shared_ptr<CConsleContext> console_context, const char* config_file)
{
	string config_info;
	char tmp_output[200] = { '\0' };

	TiXmlDocument xml_document;
	if (false == xml_document.LoadFile(config_file))
	{
		snprintf(tmp_output, 200, "[config](%s) file not exist.", config_file);
		config_info = tmp_output;
		Set_Console_Output(console_context, config_info);
		return false;
	}

	TiXmlElement* root_node = xml_document.RootElement();
	if (NULL == root_node)
	{
		snprintf(tmp_output, 200, "[config](%s) root not exist.", config_file);
		config_info = tmp_output;
		Set_Console_Output(console_context, config_info);
		return false;
	}

	TiXmlElement* connect_node = root_node->FirstChildElement("PssConnectInfo");
	if (NULL == connect_node)
	{
		snprintf(tmp_output, 200, "[config](%s) PssConnectInfo node not exist.", config_file);
		config_info = tmp_output;
		Set_Console_Output(console_context, config_info);
		return false;
	}

	if (NULL != connect_node->Attribute("ServerIP"))
	{
		console_context->server_ip = connect_node->Attribute("ServerIP");
	}
	if (NULL != connect_node->Attribute("Port"))
	{
		console_context->server_port = atoi(connect_node->Attribute("Port"));
	}
	if (NULL != connect_node->Attribute("UserName"))
	{
		console_context->user_name = connect_node->Attribute("UserName");
	}
	if (NULL != connect_node->Attribute("Password"))
	{
		console_context->user_password = connect_node->Attribute("Password");
	}

	snprintf(tmp_output, 200, "[config]server_ip=%s,Port=%d,UserName=%s,Password=%s. OK", 
		console_context->server_ip.c_str(),
		console_context->server_port,
		console_context->user_name.c_str(),
		console_context->user_password.c_str());

	config_info = tmp_output;
	Set_Console_Output(console_context, config_info, emum_text_color::CONSOLE_BACKGROUND_GREEN);

	return true;
}

bool connect_server(shared_ptr<CConsleContext> console_context, shared_ptr<TcpService> service, shared_ptr<AsyncConnector> connector, wrapper::ConnectionBuilder connectionBuilder)
{
	string client_name = "console";

	//启动服务线程
	service->startWorkerThread(1);

	//启动链接线程
	connector->startWorkerThread();

	auto enterCallback = [client_name, console_context](const TcpConnection::Ptr& session) {
		session->setDataCallback([session, console_context](const char* buffer, size_t len) {
			//处理接收数据
			char* recv_data = new char[len + 1];
			memset(recv_data, 0, len + 1);
			memcpy_s(recv_data, len, buffer, len);
			string recv_message = (string)recv_data;
			Set_Console_Output(console_context, recv_message);
			delete[] recv_data;

			return len;
			});

		//处理链接成功信息
		session->setDisConnectCallback([console_context](TcpConnection::Ptr session) {
			//处理断开事件
			string config_info;
			char tmp_output[200] = { '\0' };

			snprintf(tmp_output, 200, "[config](%s:%d) connect is disconnect.", console_context->server_ip.c_str(), console_context->server_port);
			config_info = tmp_output;
			Set_Console_Output(console_context, config_info, emum_text_color::CONSOLE_FOREGROUND_RED);
			console_context->connect_state = false;
		});

		//处理链接建立成功消息
		console_context->connect_state = true;
		string config_info;
		char tmp_output[200] = { '\0' };

		snprintf(tmp_output, 200, "[config](%s:%d) connect is ok.", console_context->server_ip.c_str(), console_context->server_port);
		config_info = tmp_output;
		Set_Console_Output(console_context, config_info, emum_text_color::CONSOLE_FOREGROUND_GREEN);
		console_context->tcp_session = session;
	};

	auto failedCallback = [console_context]() {
		//处理的断开事件
		string config_info;
		char tmp_output[200] = { '\0' };

		snprintf(tmp_output, 200, "[config](%s:%d) connect is error.", console_context->server_ip.c_str(), console_context->server_port);
		config_info = tmp_output;
		Set_Console_Output(console_context, config_info, emum_text_color::CONSOLE_FOREGROUND_RED);
		console_context->connect_state = false;
	};

	connectionBuilder.configureService(service)
		.configureConnector(connector)
		.configureConnectionOptions({
			brynet::net::AddSocketOption::AddEnterCallback(enterCallback),
			brynet::net::AddSocketOption::WithMaxRecvBufferSize(1024 * 1024)
			});

	connectionBuilder.configureConnectOptions({
			ConnectOption::WithAddr(console_context->server_ip, console_context->server_port),
			ConnectOption::WithTimeout(std::chrono::seconds(10)),
			ConnectOption::WithFailedCallback(failedCallback),
			ConnectOption::AddProcessTcpSocketCallback([](TcpSocket& socket) {
			})
		})
		.syncConnect();

	return true;
}
