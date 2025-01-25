#include "loggerMetadata.hpp"

struct loggerMetadata metadata;

struct loggerMetadata* getLoggerMetadata()
{
	return &metadata;
}