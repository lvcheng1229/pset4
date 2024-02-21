#pragma once
#include "RHIResource.h"
class CRHIContext
{
public:
	virtual void SetGraphicsPipelineState() = 0;
private:
};