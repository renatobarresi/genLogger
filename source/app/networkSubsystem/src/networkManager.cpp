#include "networkManager.hpp"
#include "debug_log.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>

namespace network
{

static constexpr uint32_t HTTP_SERVER_TIMEOUT_IN_MS = 5000;

void networkManager::mgEventHandler(struct mg_connection* c, int ev, void* ev_data)
{
	auto objInfo = static_cast<networkManager*>(c->fn_data);

	if (ev == MG_EV_CONNECT)
	{
		struct mg_str host = mg_url_host(objInfo->url);

		mg_printf(c,
				  "POST %s HTTP/1.1\r\n"
				  "Host: %.*s\r\n"
				  "Content-Length: %d\r\n"
				  "\r\n"
				  "%s",
				  mg_url_uri(objInfo->url), (int)host.len, host.buf, (int)strlen(objInfo->postBuffer), objInfo->postBuffer);
	}

	if (ev == MG_EV_HTTP_MSG)
	{
		objInfo->done = true;
	}
}

bool networkManager::httpConnectPost(const char* url, const char* postContent)
{
	// todo this should only be called once
	strncpy(this->url, url, sizeof(this->url));
	strncpy(this->postBuffer, postContent, sizeof(this->postBuffer));

	mg_http_connect(&mgr, url, networkManager::mgEventHandler, this);

	// TODO
	// Make this method not blocking.

	uint64_t start = mg_millis();
	while (!this->done && mg_millis() - start < HTTP_SERVER_TIMEOUT_IN_MS)
	{
		mg_mgr_poll(&this->mgr, 100);
	}

	if (this->done == false)
	{
		debug::log<true, debug::logLevel::LOG_ERROR>("Coudn't connect to server\r\n");

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
	this->_staticIP		   = false;
	this->_macAddressIsSet = false;
}

void networkManager::setMacAddress(const uint8_t* macAddress)
{
	memcpy(this->_macAddress, macAddress, sizeof(this->_macAddress));
	this->_macAddressIsSet = true;
}

bool networkManager::init()
{
#ifdef TARGET_MICRO
	struct mg_tcpip_driver_stm32f_data driver_data = {.mdc_cr = 4}; // STM32F specific
	struct mg_tcpip_if				   mif		   = {0};

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
