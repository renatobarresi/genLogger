#pragma once

namespace network
{
class networkManager
{
  public:
	networkManager(const char* ip, const char* netmask, const char* gateway);
	bool init();
	bool httpConnect();
	void setIp(const char*& ip);
	void setNetmask(const char*& netMask);
	void setGateway(const char*& gateway);

  private:
	char _ip[15];
	char _netmask[15];
	char _gateway[15];
};
} // namespace network
