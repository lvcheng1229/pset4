#pragma once
#include <memory>
#include "graphics\Gcn\PtGcnModule.h"
#include "graphics\RHI\RHIShader.h"
class CGnmShader
{
public:
	CGnmShader(size_t key,const void* shaderCode);
	void Compile();
private:
	CPtGcnModule m_gcnModule;
	std::shared_ptr<CRHIShader>m_pRHIShader;
};