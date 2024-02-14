#include "PtGnmShader.h"

CGnmShader::CGnmShader(size_t key, const void* shaderCode)
	:m_gcnModule((uint8_t*)shaderCode)
{
}

void CGnmShader::Compile()
{
	if (m_pRHIShader == nullptr)
	{
		m_gcnModule.Compile();
	}
}
