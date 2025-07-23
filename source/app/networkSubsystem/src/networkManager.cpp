#include "networkManager.hpp"
#include "mongoose.h"
#include <cstring>

namespace network
{
static struct mg_mgr mgr;

networkManager::networkManager(const char* ip, const char* netmask, const char* gateway)
{
	std::strncpy(this->_ip, ip, std::sizeof(_ip));
	std::strncpy(this->_netmask, netmask, std::sizeof(netmask));
	std::strncpy(this->_gateway, gateway, std::sizeof(gateway));
}

bool networkManager::init()
{
	// struct mg_tcpip_driver_stm32f_data driver_data = {.mdc_cr = 4};

	// mg_log_set(MG_LL_DEBUG);
	// mg_mgr_init(&mgr); // Initialise event manager

	// struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(), .ip = mg_htonl(MG_U32(192, 168, 1, 2)), .mask = mg_htonl(MG_U32(255, 255, 255, 0)), .gw = mg_htonl(MG_U32(192, 168, 1, 1)), .driver = &mg_tcpip_driver_stm32f, .driver_data = &driver_data};

	// mg_tcpip_init(&mgr, &mif);

	// while (mif.state != MG_TCPIP_STATE_READY)
	// {
	// 	mg_mgr_poll(&mgr, 0);
	// }

	return true;
}
} // namespace network