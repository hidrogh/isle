#ifndef LEGOCARRACEACTOR_H
#define LEGOCARRACEACTOR_H

#include "legoraceactor.h"

// VTABLE: LEGO1 0x100da0d8
class LegoCarRaceActor : public LegoRaceActor {
public:
	// FUNCTION: LEGO1 0x10081650
	inline const char* ClassName() const override // vtable+0x0c
	{
		// STRING: LEGO1 0x100f0568
		return "LegoCarRaceActor";
	}

	// FUNCTION: LEGO1 0x10081670
	inline MxBool IsA(const char* p_name) const override // vtable+0x10
	{
		return !strcmp(p_name, LegoCarRaceActor::ClassName()) || LegoRaceActor::IsA(p_name);
	}

	void VTable0x68() override;              // vtable+0x68
	void VTable0x6c() override;              // vtable+0x6c
	void VTable0x70(float p_float) override; // vtable+0x70
	MxS32 VTable0x90() override;             // vtable+0x90
	MxS32 VTable0x94() override;             // vtable+0x94
	void VTable0x98() override;              // vtable+0x98
	void VTable0x9c() override;              // vtable+0x9c

	// SYNTHETIC: LEGO1 0x10081610
	// LegoCarRaceActor::`scalar deleting destructor'
};

#endif // LEGOCARRACEACTOR_H
