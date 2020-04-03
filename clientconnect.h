#pragma once

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include "base.h"
#include "brynet/net/SocketLibFunction.hpp"
#include "brynet/net/TcpService.hpp"
#include "brynet/net/AsyncConnector.hpp"
#include "brynet/net/wrapper/ConnectionBuilder.hpp"

static const char CLIENT_CONFIG_FILE[] = "config.xml";

using namespace brynet;
using namespace brynet::net;

bool read_client_connect_info(CConsleContext& console_context, const char* config_file);

bool connect_server(CConsleContext& console_context);
