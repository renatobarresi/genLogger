#include "networkManager.hpp"
#include <cstring>
#include <iostream>

namespace network
{

void networkManager::mgEventHandler(struct mg_connection* c, int ev, void* ev_data)
{
	auto objInfo = static_cast<networkManager*>(c->fn_data);

	if (ev == MG_EV_CONNECT)
	{
		struct mg_str host = mg_url_host(objInfo->url);

		mg_printf(c,
				  "POST %s HTTP/1.0\r\n"
				  "Host: %.*s\r\n"
				  "\r\n"
				  "%s",
				  mg_url_uri(objInfo->url), (int)host.len, host.buf, objInfo->postBuffer);
	}

	if (ev == MG_EV_HTTP_MSG)
	{
		objInfo->done = true;
	}
}

bool networkManager::httpConnectPost(const char* url, const char* postContent)
{
	strncpy(this->url, url, sizeof(url));
	strncpy(this->postBuffer, postContent, sizeof(this->postBuffer));

	mg_http_connect(&mgr, url, networkManager::mgEventHandler, this);

	uint64_t start = mg_millis();
	while (!this->done && mg_millis() - start < 5000)
	{
		mg_mgr_poll(&this->mgr, 100);
	}

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
	this->_staticIP = false;
}

void networkManager::setMacAddress(uint32_t macAddress)
{
	this->_macAddress = macAddress;
}

bool networkManager::init()
{
#ifdef TARGET_MICRO
	struct mg_tcpip_driver_stm32f_data driver_data = {.mdc_cr = 4};
	struct mg_tcpip_if				   mif;

	mg_log_set(MG_LL_DEBUG);
	mg_mgr_init(&mgr); // Initialise event manager

	//todo what if static ip is false
	if (true == this->_staticIP)
	{
		mif = {.mac = this->_macAddress, .ip = mg_htonl(MG_U32(192, 168, 1, 2)), .mask = mg_htonl(MG_U32(255, 255, 255, 0)), .gw = mg_htonl(MG_U32(192, 168, 1, 1)), .driver = &mg_tcpip_driver_stm32f, .driver_data = &driver_data};
	}

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
