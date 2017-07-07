#pragma once
#include "define.h"

class IServiceProvider;

class IServiceConsumer
{
public:
	virtual void       ConsumeService(IServiceProvider* pServiceProvider) = 0;
};