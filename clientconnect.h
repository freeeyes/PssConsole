#pragma once

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include "base.h"

static const char CLIENT_CONFIG_FILE[] = "config.xml";

bool read_client_connect_info(shared_ptr<CConsleContext> console_context, const char* config_file);

bool connect_server(shared_ptr<CConsleContext> console_context);
