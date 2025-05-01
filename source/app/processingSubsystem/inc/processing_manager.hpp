#pragma once

#include <cstdint>

class observerInterface
{
    public:
    virtual void update(bool infoFlag, char &sensorInformation) = 0;
    virtual ~observerInterface() = default;
};

class processingManager
{
    public:

    uint8_t numOfObservers = 0;
    
    observerInterface *listOfObservers[10]; 
    
    void setObserver(observerInterface *ref, uint8_t index)
    {
        //observerInterface[index] = ref;
    }

    private:

    /**
     * @brief iterates through array of listeners 
     * 
     * @param pStr 
     */
     void notify(char &pStr);
};