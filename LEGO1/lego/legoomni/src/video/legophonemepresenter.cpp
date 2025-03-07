#include "legophonemepresenter.h"

DECOMP_SIZE_ASSERT(LegoPhonemePresenter, 0x88);

// FUNCTION: LEGO1 0x1004e180
LegoPhonemePresenter::LegoPhonemePresenter()
{
	Init();
}

// FUNCTION: LEGO1 0x1004e340
LegoPhonemePresenter::~LegoPhonemePresenter()
{
}

// FUNCTION: LEGO1 0x1004e3b0
void LegoPhonemePresenter::Init()
{
	m_unk0x68 = 0;
	m_unk0x6c = 0;
	m_unk0x70 = 0;
	m_unk0x84 = 0;
}

// STUB: LEGO1 0x1004e3d0
void LegoPhonemePresenter::StartingTickle()
{
	// TODO

	EndAction();
}

// STUB: LEGO1 0x1004e800
void LegoPhonemePresenter::LoadFrame(MxStreamChunk* p_chunk)
{
	// TODO
}

// STUB: LEGO1 0x1004e840
void LegoPhonemePresenter::PutFrame()
{
	// TODO
}

// STUB: LEGO1 0x1004e870
void LegoPhonemePresenter::EndAction()
{
	// TODO

	if (m_action != NULL) {
		MxFlcPresenter::EndAction();
	}
}
