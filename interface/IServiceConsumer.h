#pragma once
#include "define.h"

BEGIN_APP_NAMESPACE

class IServiceProvider;

class IServiceConsumer
{
public:
	virtual void       ConsumeService(IServiceProvider* pServiceProvider) = 0;
};

END_APP_NAMESPACE