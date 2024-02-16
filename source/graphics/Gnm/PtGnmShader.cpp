#include <assert.h>
#include "PtGnmShader.h"

CGnmShader::CGnmShader(size_t key, const void* shaderCode)
{
}

void CGnmShader::Compile()
{
	if (m_pRHIShader == nullptr)
	{
	}
}

const SShaderBinaryInfo* GetShaderInfo(const void* code)
{
	const uint32_t* base = reinterpret_cast<const uint32_t*>(code);
	assert(base[0] == 0xBEEB03FF);
	return reinterpret_cast<const SShaderBinaryInfo*>(base + (base[1] + 1) * 2);
}
