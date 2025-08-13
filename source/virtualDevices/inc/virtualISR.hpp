#pragma once

class virtualISR
{
  private:
	bool isrCalledFlag = false;

  public:
	bool getFlag();
};