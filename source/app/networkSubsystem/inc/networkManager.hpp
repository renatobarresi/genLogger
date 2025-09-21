#pragma once

#include "mongoose.h"

namespace network
{
class networkManager
{
  public:
	networkManager(const char* ip, const char* netmask, const char* gateway);

	/**
	 * @brief Constructor used for DHCP
	 */
	networkManager();

	/**
	 * @brief release network resources
	 */
	~networkManager();

	bool init();
	bool httpConnectPost(const char** url, const char** postContent);
	void setIp(const char* ip);
	void setNetmask(const char* netMask);
	void setGateway(const char* gateway);
	void setMacAddress(const uint8_t* macAddress);

	bool done		= false;
	bool connStatus = false;

  private:
	char		 _ip[16];
	char		 _netmask[16];
	char		 _gateway[16];
	bool		 _staticIP = true;
	uint8_t		 _macAddress[6]{0};
	bool		 _macAddressIsSet = false;
	bool		 _firstCall		  = true;
	const char** _pPayload		  = nullptr;
	const char** _pURL			  = nullptr;

	struct mg_mgr mgr;
	static void	  mgEventHandler(struct mg_connection* c, int ev, void* ev_data);
#ifdef TARGET_MICRO
	struct mg_tcpip_driver_stm32f_data driver_data;
	struct mg_tcpip_if				   mif = {0};
#endif
};
} // namespace network
