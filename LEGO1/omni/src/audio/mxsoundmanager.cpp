#include "mxsoundmanager.h"

#include "define.h"
#include "mxautolocker.h"
#include "mxomni.h"
#include "mxpresenter.h"
#include "mxticklemanager.h"
#include "mxwavepresenter.h"

DECOMP_SIZE_ASSERT(MxSoundManager, 0x3c);

// FUNCTION: LEGO1 0x100ae740
MxSoundManager::MxSoundManager()
{
	Init();
}

// FUNCTION: LEGO1 0x100ae7d0
MxSoundManager::~MxSoundManager()
{
	Destroy(TRUE);
}

// FUNCTION: LEGO1 0x100ae830
void MxSoundManager::Init()
{
	m_directSound = NULL;
	m_dsBuffer = NULL;
}

// FUNCTION: LEGO1 0x100ae840
void MxSoundManager::Destroy(MxBool p_fromDestructor)
{
	if (this->m_thread) {
		this->m_thread->Terminate();
		delete this->m_thread;
	}
	else {
		TickleManager()->UnregisterClient(this);
	}

	this->m_criticalSection.Enter();

	if (this->m_dsBuffer) {
		this->m_dsBuffer->Release();
	}

	Init();
	this->m_criticalSection.Leave();

	if (!p_fromDestructor) {
		MxAudioManager::Destroy();
	}
}

// FUNCTION: LEGO1 0x100ae8b0
MxResult MxSoundManager::Create(MxU32 p_frequencyMS, MxBool p_createThread)
{
	MxResult status = FAILURE;
	MxBool locked = FALSE;

	if (MxAudioManager::InitPresenters() != SUCCESS) {
		goto done;
	}

	m_criticalSection.Enter();
	locked = TRUE;

	if (DirectSoundCreate(NULL, &m_directSound, NULL) != DS_OK) {
		goto done;
	}

	if (m_directSound->SetCooperativeLevel(MxOmni::GetInstance()->GetWindowHandle(), DSSCL_PRIORITY) != DS_OK) {
		goto done;
	}

	DSBUFFERDESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.dwSize = sizeof(desc);

	if (MxOmni::IsSound3D()) {
		desc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
	}
	else {
		desc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	}

	if (m_directSound->CreateSoundBuffer(&desc, &m_dsBuffer, NULL) != DS_OK) {
		if (!MxOmni::IsSound3D()) {
			goto done;
		}

		MxOmni::SetSound3D(FALSE);
		desc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;

		if (m_directSound->CreateSoundBuffer(&desc, &m_dsBuffer, NULL) != DS_OK) {
			goto done;
		}
	}

	WAVEFORMATEX format;

	format.wFormatTag = WAVE_FORMAT_PCM;

	if (MxOmni::IsSound3D()) {
		format.nChannels = 2;
	}
	else {
		format.nChannels = 1;
	}

	format.nSamplesPerSec = 11025; // KHz
	format.wBitsPerSample = 16;
	format.nBlockAlign = format.nChannels * 2;
	format.nAvgBytesPerSec = format.nBlockAlign * 11025;
	format.cbSize = 0;

	status = m_dsBuffer->SetFormat(&format);

	if (p_createThread) {
		m_thread = new MxTickleThread(this, p_frequencyMS);

		if (!m_thread || m_thread->Start(0, 0) != SUCCESS) {
			goto done;
		}
	}
	else {
		TickleManager()->RegisterClient(this, p_frequencyMS);
	}

	status = SUCCESS;

done:
	if (status != SUCCESS) {
		Destroy();
	}

	if (locked) {
		m_criticalSection.Leave();
	}
	return status;
}

// FUNCTION: LEGO1 0x100aeab0
void MxSoundManager::Destroy()
{
	Destroy(FALSE);
}

// FUNCTION: LEGO1 0x100aeac0
void MxSoundManager::SetVolume(MxS32 p_volume)
{
	MxAudioManager::SetVolume(p_volume);

	m_criticalSection.Enter();

	MxPresenter* presenter;
	MxPresenterListCursor cursor(m_presenters);

	while (cursor.Next(presenter)) {
		((MxAudioPresenter*) presenter)->SetVolume(((MxAudioPresenter*) presenter)->GetVolume());
	}

	m_criticalSection.Leave();
}

// FUNCTION: LEGO1 0x100aebd0
MxPresenter* MxSoundManager::FUN_100aebd0(const MxAtomId& p_atomId, MxU32 p_objectId)
{
	MxAutoLocker lock(&m_criticalSection);

	MxPresenter* presenter;
	MxPresenterListCursor cursor(m_presenters);

	while (cursor.Next(presenter)) {
		if (presenter->GetAction()->GetAtomId().GetInternal() == p_atomId.GetInternal() &&
			presenter->GetAction()->GetObjectId() == p_objectId) {
			return presenter;
		}
	}

	return NULL;
}

// FUNCTION: LEGO1 0x100aecf0
MxS32 MxSoundManager::FUN_100aecf0(MxU32 p_undefined)
{
	if (!p_undefined) {
		return -10000;
	}
	return g_mxcoreCount[p_undefined];
}

// FUNCTION: LEGO1 0x100aed10
void MxSoundManager::Pause()
{
	MxAutoLocker lock(&m_criticalSection);

	MxPresenter* presenter;
	MxPresenterListCursor cursor(m_presenters);

	while (cursor.Next(presenter)) {
		if (presenter->IsA("MxWavePresenter")) {
			((MxWavePresenter*) presenter)->Pause();
		}
	}
}

// FUNCTION: LEGO1 0x100aee10
void MxSoundManager::Resume()
{
	MxAutoLocker lock(&m_criticalSection);

	MxPresenter* presenter;
	MxPresenterListCursor cursor(m_presenters);

	while (cursor.Next(presenter)) {
		if (presenter->IsA("MxWavePresenter")) {
			((MxWavePresenter*) presenter)->Resume();
		}
	}
}
