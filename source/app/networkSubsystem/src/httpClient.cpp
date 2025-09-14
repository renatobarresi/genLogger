#include "httpClient.hpp"

namespace network
{
httpClient::httpClient(network::networkManager* netManager, const char* url)
{
	this->_netManager = netManager;
	this->_pURL		  = url;
}

void httpClient::update(bool infoFlag)
{
	this->_availableDataFlag = true;
}

bool httpClient::postSensorData()
{
	if (nullptr == this->_pDataBuff)
	{
		return false;
	}

	return this->_netManager->httpConnectPost(this->_pURL, this->_pDataBuff);
}

bool httpClient::getAvailableDataFlag()
{
	return this->_availableDataFlag;
}

void httpClient::setRunTaskFlag(bool val)
{
	this->_availableDataFlag = val;
}

void httpClient::setMailBox(const char* pDataBuff)
{
	this->_pDataBuff = pDataBuff;
}
} // namespace network
