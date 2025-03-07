#ifndef ACT2BRICK_H
#define ACT2BRICK_H

#include "legopathactor.h"

// VTABLE: LEGO1 0x100d9b60
// SIZE 0x194
class Act2Brick : public LegoPathActor {
public:
	Act2Brick();
	~Act2Brick() override; // vtable+0x00

	MxLong Notify(MxParam& p_param) override; // vtable+0x04
	MxResult Tickle() override;               // vtable+0x08

	// FUNCTION: LEGO1 0x1007a360
	inline const char* ClassName() const override // vtable+0x0c
	{
		// STRING: LEGO1 0x100f0438
		return "Act2Brick";
	}

	// FUNCTION: LEGO1 0x1007a370
	inline MxBool IsA(const char* p_name) const override // vtable+0x10
	{
		return !strcmp(p_name, Act2Brick::ClassName()) || LegoEntity::IsA(p_name);
	}

	MxS32 VTable0x94() override; // vtable+0x94

	// SYNTHETIC: LEGO1 0x1007a450
	// Act2Brick::`scalar deleting destructor'
};

#endif // ACT2BRICK_H
