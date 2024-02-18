#pragma once
#include <memory>
#include <string>
#include "graphics\RHI\RHIShader.h"



class CGnmShader
{
public:
	CGnmShader(size_t key,const void* shaderCode);
	void Compile();
private:
	std::string spvShaderPath;
	std::shared_ptr<CRHIShader>m_pRHIShader;
};