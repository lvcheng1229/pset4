#pragma once
#include <stdint.h>
#include "graphics\AMD\gfx9\PtChip.h"

struct SGcnMetaCommon
{
	uint32_t userSgprCount;
};

struct SGcnMetaVS : public SGcnMetaCommon
{

};

struct SGcnMetaPS : public SGcnMetaCommon
{
	SPI_PS_INPUT_ENA m_psInputEna;
};

union SGcnShaderMeta
{
	SGcnMetaVS vs;
	SGcnMetaPS ps;
};