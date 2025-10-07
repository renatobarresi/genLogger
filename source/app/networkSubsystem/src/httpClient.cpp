#include "httpClient.hpp"
#include "debug_log.hpp"
#include "virtualTimer.hpp"

namespace network
{
httpClient::httpClient(network::networkManager& netManager) : _netManager(netManager) {}

void httpClient::update()
{
	this->_availableDataFlag = true;
}

std::optional<bool> httpClient::postSensorData()
{
	if (nullptr == this->_pDataBuff)
	{
		return false;
	}

	// if first time calling it, start timer
	if (true == this->_firstCall)
	{
		this->_firstCall = false;
		this->_connTimer = systick::getTicks();
	}

	if (false == this->_netManager.httpConnectPost(&this->_pURL, &this->_pDataBuff))
	{
		if (systick::getTicks() - _connTimer > this->timeout)
		{
			this->_firstCall = true;
			return false;
		}
	}
	else
	{
		this->_firstCall = true;
		return true;
	}

	return std::nullopt;
}

bool httpClient::runTaskFlag()
{
	return this->_availableDataFlag;
}

bool httpClient::timeoutFlag()
{
	return this->_timeout;
}

void httpClient::setRunTaskFlag(bool val)
{
	this->_availableDataFlag = val;
}

void httpClient::setMailBox(const char* pDataBuff)
{
	this->_pDataBuff = pDataBuff;
}

void httpClient::setURL(const char* url)
{
	this->_pURL = url;
}

} // namespace network
