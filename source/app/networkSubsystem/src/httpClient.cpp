#include "httpClient.hpp"

namespace network
{
httpClient::httpClient(network::networkManager *netManager)
{
	this->_netManager = netManager;
}

void httpClient::update(bool infoFlag, const char* sensorInformation, const char* timestamp)
{
	
}

bool httpClient::connect(const char* serverLink)
{
	//

	return true;
}
}
