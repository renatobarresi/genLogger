#include "networkManager.hpp"
#include "debug_log.hpp"
#include <cstdio>
#include <cstring>

namespace network
{

static constexpr uint32_t HTTP_SERVER_TIMEOUT_IN_MS = 5000;

void networkManager::mgEventHandler(struct mg_connection* c, int ev, void* ev_data)
{
	auto objInfo = static_cast<networkManager*>(c->fn_data);

	if (ev == MG_EV_CONNECT)
	{
		struct mg_str host = mg_url_host(*objInfo->_pURL);

		debug::log<true, debug::logLevel::LOG_ALL>("Network Manager: mongoose MG_EVENT_CONNECT action\r\n");
		// printf("POST %s HTTP/1.1\r\n"
		// 	   "Host: %.*s\r\n"
		// 	   "Content-Length: %d\r\n"
		// 	   "\r\n"
		// 	   "%s",
		// 	   mg_url_uri(*objInfo->_pURL), (int)host.len, host.buf, strlen(*objInfo->_pPayload), *objInfo->_pPayload);
		mg_printf(c,
				  "POST %s HTTP/1.1\r\n"
				  "Host: %.*s\r\n"
				  "Content-Length: %d\r\n"
				  "\r\n"
				  "%s",
				  mg_url_uri(*objInfo->_pURL), (int)host.len, host.buf, strlen(*objInfo->_pPayload), *objInfo->_pPayload);
	}

	if (ev == MG_EV_HTTP_MSG)
	{
		debug::log<true, debug::logLevel::LOG_ALL>("Network Manager: mongoose MG_EV_HTTP_MSG action\r\n");
		struct mg_http_message* hm = (struct mg_http_message*)ev_data;
		printf("%.*s", (int)hm->message.len, hm->message.buf);
		objInfo->done		= true;
		objInfo->_firstCall = true;
	}
}

bool networkManager::httpConnectPost(const char** url, const char** postContent)
{
	if (true == _firstCall)
	{
		this->_pPayload = postContent;
		this->_pURL		= url;
		_firstCall		= false;

		debug::log<true, debug::logLevel::LOG_ALL>("Network Manager: Performing HTTP connect for the first time\r\n");

		mg_http_connect(&mgr, *url, networkManager::mgEventHandler, this);
	}

	mg_mgr_poll(&this->mgr, 100);

	if (this->done == false)
	{
		return false;
	}

	return true;
}

networkManager::networkManager(const char* ip, const char* netmask, const char* gateway)
{
	strncpy(this->_ip, ip, sizeof(_ip));
	strncpy(this->_netmask, netmask, sizeof(_netmask));
	strncpy(this->_gateway, gateway, sizeof(_gateway));
}

networkManager::networkManager()
{
	// this->_staticIP		   = false;
	// this->_macAddressIsSet = false;
}

// TODO zero copy buffer
void networkManager::setIp(const char* ip)
{
	strncpy(this->_ip, ip, sizeof(_ip));
}
void networkManager::setNetmask(const char* netMask)
{
	strncpy(this->_netmask, netMask, sizeof(_netmask));
}
void networkManager::setGateway(const char* gateway)
{
	strncpy(this->_gateway, gateway, sizeof(_gateway));
}

networkManager::~networkManager()
{
	debug::log<true, debug::logLevel::LOG_ALL>("Network Manager: releasing resources\r\n");
	mg_mgr_free(&this->mgr);
}

void networkManager::setMacAddress(const uint8_t* macAddress)
{
	memcpy(this->_macAddress, macAddress, sizeof(this->_macAddress));
	this->_macAddressIsSet = true;
}

bool networkManager::init()
{
#ifdef TARGET_MICRO
	driver_data = {.mdc_cr = 4};
	mif			= {0};

	mg_log_set(MG_LL_DEBUG);
	mg_mgr_init(&mgr); // Initialise event manager

	if (!this->_macAddressIsSet)
	{
		// MAC address must be set via setMacAddress before calling init()
		return false;
	}

	memcpy(mif.mac, this->_macAddress, sizeof(mif.mac));

	mif.driver		= &mg_tcpip_driver_stm32f;
	mif.driver_data = &driver_data;

	if (true == this->_staticIP)
	{
		unsigned int b[4];
		// Parse the IP strings into uint32_t
		if (sscanf(this->_ip, "%u.%u.%u.%u", &b[0], &b[1], &b[2], &b[3]) == 4)
		{
			mif.ip = mg_htonl(MG_U32(b[0], b[1], b[2], b[3]));
		}
		if (sscanf(this->_netmask, "%u.%u.%u.%u", &b[0], &b[1], &b[2], &b[3]) == 4)
		{
			mif.mask = mg_htonl(MG_U32(b[0], b[1], b[2], b[3]));
		}
		if (sscanf(this->_gateway, "%u.%u.%u.%u", &b[0], &b[1], &b[2], &b[3]) == 4)
		{
			mif.gw = mg_htonl(MG_U32(b[0], b[1], b[2], b[3]));
		}
	}
	// If not static, mif.ip, mif.mask, and mif.gw remain 0, so Mongoose will use DHCP.

	mg_tcpip_init(&mgr, &mif);

	// TODO add timeout
	while (mif.state != MG_TCPIP_STATE_READY)
	{
		mg_mgr_poll(&mgr, 0);
	}
#else // host implementation

	mg_mgr_init(&this->mgr);

#endif
	return true;
}
} // namespace network
