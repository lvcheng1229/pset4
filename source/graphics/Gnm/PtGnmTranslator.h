#pragma once
#include <stdint.h>
#include "PtGcnCommandDefs.h"
#include "graphics\AMD\PtChip.h"
#include "graphics\Gnm\PtGnmCode.h"

using PtPointer = void*;

class CPtGnmTranslator
{
public:
	void TranslateAndDispatchCmd(const void* commandBuffer, uint32_t commandSize);
private:
	void ProcessPM4Type3(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody);

	void ProcessGnmPrivateOp(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody);
	void ProcessGnmPrivateOpDrawIndex(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody);

	const PM4_HEADER_COMMON* GetNextPm4(const PM4_HEADER_COMMON* currentPm4, uint32_t step);
private:
	void OnSetUConfigRegs(PM4_PT_TYPE_3_HEADER* pm4Hdr, PtGfx::PM4CMDSETDATA* itBody);
	void OnSetShRegs(PM4_PT_TYPE_3_HEADER* pm4Hdr, PtGfx::PM4CMDSETDATA* itBody);
	void SetUContextReg(uint16_t reg, uint32_t value);
	void SetShReg(uint16_t reg, uint32_t value);
	

	// Flip packet is the last pm4 packet of a command buffer,
	// when flip packet had been processed, we end processing command buffer.
	bool m_flipPacketDone = false;

	// Some Gnm calls formed with several pm4 packets
	// so after recover that call, we need to skip N packets.
	// NOte:
	// This should be the the real pm4 packet count which forms a gnm call minus one.
	// e.g. 2 packets makes gnm call, m_skipPm4Count = 1
	uint32_t m_skipPm4Count = 0;


};