#include <tier1.hpp>

#include <offsets.hpp>
#include <console.hpp>

#define SPLIT_3_BYTES_INTO_1_AND_2( x1, x2, x3 ) do { x1 = (uint8_t)(x3); x2 = (uint16_t)((x3) >> 8); } while(0)

KeyValues::KeyValues(const char* setName) {
	this->Init();
	this->SetName(setName);
}

void KeyValues::SetName(const char* setName) {
	HKeySymbol hCaseSensitiveKeyName = INVALID_KEY_SYMBOL, hCaseInsensitiveKeyName = INVALID_KEY_SYMBOL;
	hCaseSensitiveKeyName = tier1->KeyValuesSystem()->GetSymbolForStringCaseSensitive(hCaseInsensitiveKeyName, setName);
	this->m_iKeyName = hCaseInsensitiveKeyName;
	SPLIT_3_BYTES_INTO_1_AND_2(m_iKeyNameCaseSensitive1, m_iKeyNameCaseSensitive2, hCaseSensitiveKeyName);
}

KeyValues* KeyValues::FindKey(const char* keyName, bool bCreate) {
	if(!keyName || !keyName[0]) return this;

	HKeySymbol iSearchStr = tier1->KeyValuesSystem()->GetSymbolForString(keyName, bCreate);
	if(iSearchStr == INVALID_KEY_SYMBOL) return nullptr;

	KeyValues* last = nullptr;
	KeyValues* dat = this->m_pSub;

	for (dat = this->m_pSub; dat; dat = dat->m_pPeer) {
		last = dat;
		if (dat->m_iKeyName == (uint32_t)iSearchStr) break;
	}

	if(!dat && this->m_pChain) dat = this->m_pChain->FindKey(keyName, false);

	if(!dat) {
		if(bCreate) {
			dat = new KeyValues(keyName);
			if(last)
				last->m_pPeer = dat;
			else
				this->m_pSub = dat;
			dat->m_pPeer = nullptr;
			this->m_iDataType = TYPE_NONE;
		} else {
			return nullptr;
		}
	}

	return dat;
}

void KeyValues::SetStringValue(char const* strValue) {
	delete [] this->m_sValue;
	delete [] this->m_wsValue;
	this->m_wsValue = nullptr;

	if(!strValue) strValue = "";

	unsigned int len = strlen(strValue);

	this->m_sValue = new char[len + 1];
	memcpy(this->m_sValue, strValue, len + 1);
	m_iDataType = TYPE_STRING;
}

void KeyValues::SetString(const char* keyName, const char* value) {
	if(KeyValues* dat = this->FindKey(keyName, true)) {
		dat->SetStringValue(value);
	}
}

void KeyValues::SetInt(const char* keyName, int value) {
	KeyValues* dat = this->FindKey( keyName, true );
	if(dat) {
		dat->m_iValue = value;
		dat->m_iDataType = TYPE_INT;
	}
}

void KeyValues::SetColor(const char* keyName, unsigned char* value) {
	KeyValues* dat = this->FindKey( keyName, true );
	if(dat) {
		dat->m_Color[0] = value[0];
		dat->m_Color[1] = value[1];
		dat->m_Color[2] = value[2];
		dat->m_Color[3] = value[3];
		dat->m_iDataType = TYPE_COLOR;
	}
}

void* KeyValues::operator new(size_t iAllocSize) {
	return tier1->KeyValuesSystem()->AllocKeyValuesMemory(iAllocSize);
}

void KeyValues::operator delete(void* pMem) {
	tier1->KeyValuesSystem()->FreeKeyValuesMemory(pMem);
}


void KeyValues::Init() {
	this->m_iKeyName = 0;
	this->m_iKeyNameCaseSensitive1 = 0;
	this->m_iKeyNameCaseSensitive2 = 0;
	this->m_iDataType = TYPE_NONE;

	this->m_pSub = nullptr;
	this->m_pPeer = nullptr;
	this->m_pChain = nullptr;

	this->m_sValue = nullptr;
	this->m_wsValue = nullptr;
	this->m_pValue = nullptr;

	this->m_bHasEscapeSequences = 0;
}

bool Tier1::Init() {
	this->g_pCVar = Interface::Create(this->Name(), "VEngineCvar007", false);
	if(this->g_pCVar) {
		this->RegisterConCommand = this->g_pCVar->Original<_RegisterConCommand>(Offsets::RegisterConCommand);
		this->UnregisterConCommand = this->g_pCVar->Original<_UnregisterConCommand>(Offsets::UnregisterConCommand);
		this->FindCommandBase = this->g_pCVar->Original<_FindCommandBase>(Offsets::FindCommandBase);

		this->m_pConCommandList = reinterpret_cast<ConCommandBase*>((uintptr_t)this->g_pCVar->ThisPtr() + Offsets::m_pConCommandList);

		auto listdemo = reinterpret_cast<ConCommand*>(this->FindCommandBase(this->g_pCVar->ThisPtr(), "listdemo"));
		if(listdemo) {
			this->ConCommand_VTable = listdemo->ConCommandBase_VTable;
			if(listdemo->m_fnCompletionCallback) {
				auto callback = (uintptr_t)listdemo->m_fnCompletionCallback + Offsets::AutoCompletionFunc;
				this->AutoCompletionFunc = Memory::Read<_AutoCompletionFunc>(callback);
			}
		}

		auto sv_lan = reinterpret_cast<ConVar*>(this->FindCommandBase(this->g_pCVar->ThisPtr(), "sv_lan"));
		if(sv_lan) {
			this->ConVar_VTable = sv_lan->ConCommandBase_VTable;
			this->ConVar_VTable2 = sv_lan->ConVar_VTable;

			auto vtable = &ConVar_VTable;

			this->Dtor = Memory::VMT<_Dtor>(vtable, Offsets::Dtor);
			this->Create = Memory::VMT<_Create>(vtable, Offsets::Create);
		}

		this->InstallGlobalChangeCallback = this->g_pCVar->Original<_InstallGlobalChangeCallback>(Offsets::InstallGlobalChangeCallback);
		this->RemoveGlobalChangeCallback = this->g_pCVar->Original<_RemoveGlobalChangeCallback>(Offsets::RemoveGlobalChangeCallback);
	}

	auto tier1 = Memory::GetModuleHandleByName(this->Name());
	if(tier1) {
		this->KeyValuesSystem = Memory::GetSymbolAddress<_KeyValuesSystem>(tier1, "KeyValuesSystem");
		Memory::CloseModuleHandle(tier1);
	}

	return this->g_pCVar && this->ConCommand_VTable && this->ConVar_VTable && this->ConVar_VTable2 && this->AutoCompletionFunc && this->KeyValuesSystem;
}

void Tier1::Shutdown() {
	Interface::Delete(g_pCVar);
}

Tier1* tier1;
