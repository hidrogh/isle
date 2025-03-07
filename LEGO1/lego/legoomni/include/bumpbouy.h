#ifndef BUMPBOUY_H
#define BUMPBOUY_H

#include "legoanimactor.h"
#include "mxtypes.h"

// VTABLE: LEGO1 0x100d6790
class BumpBouy : public LegoAnimActor {
public:
	MxLong Notify(MxParam& p_param) override; // vtable+0x04

	// FUNCTION: LEGO1 0x100274e0
	inline const char* ClassName() const override // vtable+0x0c
	{
		// STRING: LEGO1 0x100f0394
		return "BumpBouy";
	}

	// FUNCTION: LEGO1 0x10027500
	inline MxBool IsA(const char* p_name) const override // vtable+0x10
	{
		return !strcmp(p_name, BumpBouy::ClassName()) || LegoAnimActor::IsA(p_name);
	}

	void ParseAction(char*) override;                  // vtable+0x20
	void SetWorldSpeed(MxFloat p_worldSpeed) override; // vtable+0x30
	void VTable0x70(float p_float) override;           // vtable+0x70
	void VTable0x74(Matrix4& p_transform) override;    // vtable+0x74

	// SYNTHETIC: LEGO1 0x10027490
	// BumpBouy::`scalar deleting destructor'
};

#endif // BUMPBOUY_H
