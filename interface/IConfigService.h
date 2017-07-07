#include "define.h"

namespace Json
{
	class Value;
}

BEGIN_APP_NAMESPACE

class IConfigService
{
public:
	virtual Json::Value GetConfigNode(const tstring& strNodeName) = 0;
};

END_APP_NAMESPACE