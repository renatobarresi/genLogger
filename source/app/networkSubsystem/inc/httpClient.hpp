#pragma once

#include "processing_manager.hpp"
#include "networkManager.hpp"
namespace network
{
class httpClient : public observerInterface
{
  public:
  httpClient(network::networkManager *netManager);
  
  void update(bool infoFlag, const char* sensorInformation, const char* timestamp) override;
  
  bool connect(const char*& url);
  
  void sendData(const char* data);
  
  //todo void sendFile(void) {}
  
  private:
  
  network::networkManager *_netManager;
  
  void post(const char* body, const char* contentType);
};
}
