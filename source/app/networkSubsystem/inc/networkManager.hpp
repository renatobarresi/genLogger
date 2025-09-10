#pragma once

#include "mongoose.h"

namespace network
{
class networkManager
{
  public:
	networkManager(const char* ip, const char* netmask, const char* gateway);

	/**
	 * @brief Constructor used for dynamic IP
	 */
	networkManager();

	bool init();
	bool httpConnectPost(const char* url, const char* postContent);
	void setIp(const char* ip);
	void setNetmask(const char* netMask);
	void setGateway(const char* gateway);
	void setMacAddress(const uint8_t* macAddress);

	bool done		= false;
	bool connStatus = false;
	char url[150];
	char postBuffer[1024];

  private:
	char	_ip[16];
	char	_netmask[16];
	char	_gateway[16];
	bool	_staticIP = true;
	uint8_t _macAddress[6]{0};
	bool	_macAddressIsSet = false;

	struct mg_mgr mgr;
	static void	  mgEventHandler(struct mg_connection* c, int ev, void* ev_data);

	void post();
};
} // namespace network
