#include <vector>
#include <string>
#include <assert.h>

#include "Common.h"
#include "Log.h"
#include "PtInvalidFuncLogger.h"

typedef void (PSET_SYSV_ABI* FLogInvalidFunc)();
//recursive template instantiation maximum depth: 1024	
static constexpr uint32_t m_maxMessage = 512;
static std::vector<std::string> m_messageList;
static std::vector<FLogInvalidFunc> m_stubFuncTemplates;
static uint32_t m_stubindex = 0;

template<int T>
static void PSET_SYSV_ABI LogInvalidFuncNid()
{
	PSET_LOG_ERROR("invalid function NID:" + m_messageList[T]);
	__debugbreak();
}

template<int T>
void GenerateFuncTemplate()
{
	m_stubFuncTemplates[T] = LogInvalidFuncNid<T>;
	GenerateFuncTemplate<T - 1>();
}

// terminate recursive
template<>
void GenerateFuncTemplate<0>()
{
	m_stubFuncTemplates[0] = LogInvalidFuncNid<0>;
}

class CStubFuncTemplateManager
{
public:
	CStubFuncTemplateManager()
	{
		m_messageList.resize(m_maxMessage);
		m_stubFuncTemplates.resize(m_maxMessage);
		GenerateFuncTemplate<m_maxMessage - 1>();
	}
};

static CStubFuncTemplateManager gStubFuncTemplateManager;

void* GenStubFunc(const std::string& message)
{
	assert(m_stubindex < m_maxMessage);
	m_messageList[m_stubindex] = message;
	return m_stubFuncTemplates[m_stubindex++];
}