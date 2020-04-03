#include "clientconnect.h"

bool read_client_connect_info(CConsleContext& console_context, const char* config_file)
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
		console_context.server_ip = connect_node->Attribute("ServerIP");
	}
	if (NULL != connect_node->Attribute("Port"))
	{
		console_context.server_port = atoi(connect_node->Attribute("Port"));
	}
	if (NULL != connect_node->Attribute("UserName"))
	{
		console_context.user_name = connect_node->Attribute("UserName");
	}
	if (NULL != connect_node->Attribute("Password"))
	{
		console_context.user_password = connect_node->Attribute("Password");
	}

	snprintf(tmp_output, 200, "[config]server_ip=%s,Port=%d,UserName=%s,Password=%s. OK", 
		console_context.server_ip.c_str(),
		console_context.server_port,
		console_context.user_name.c_str(),
		console_context.user_password.c_str());

	config_info = tmp_output;
	Set_Console_Output(console_context, config_info, 4);

	return true;
}

bool connect_server(CConsleContext& console_context)
{
	string client_name = "console";

	auto service = TcpService::Create();
	service->startWorkerThread(1);

	auto connector = AsyncConnector::Create();
	connector->startWorkerThread();

	auto enterCallback = [client_name](const TcpConnection::Ptr& session) {
		session->setDataCallback([session](const char* buffer, size_t len) {
			//处理接收数据
			return len;
			});

		//处理连接成功信息
		session->setDisConnectCallback([](TcpConnection::Ptr& session) {
			//处理断开事件
		});
	};

	auto failedCallback = []() {
		std::cout << "connect failed" << std::endl;
		//处理的断开事件
	};

	wrapper::ConnectionBuilder connectionBuilder;
	connectionBuilder.configureService(service)
		.configureConnector(connector)
		.configureConnectionOptions({
			brynet::net::AddSocketOption::AddEnterCallback(enterCallback),
			brynet::net::AddSocketOption::WithMaxRecvBufferSize(1024 * 1024)
			});

	connectionBuilder.configureConnectOptions({
			ConnectOption::WithAddr(console_context.server_ip, console_context.server_port),
			ConnectOption::WithTimeout(std::chrono::seconds(10)),
			ConnectOption::WithFailedCallback(failedCallback),
			ConnectOption::AddProcessTcpSocketCallback([](TcpSocket& socket) {
			})
		})
		.asyncConnect();

	return true;
}
