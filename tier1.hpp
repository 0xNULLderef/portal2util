#ifndef TIER1_HPP
#define TIER1_HPP

#include <interface.hpp>
#include <sdk.hpp>
#include <utils.hpp>

#define TIER1 "libvstdlib"

typedef int HKeySymbol;
#define INVALID_KEY_SYMBOL (-1)

class IKeyValuesSystem {
public:
	virtual void RegisterSizeofKeyValues(int size) = 0;
	virtual void* AllocKeyValuesMemory(int size) = 0;
	virtual void FreeKeyValuesMemory(void* pMem) = 0;
	virtual HKeySymbol GetSymbolForString(const char* name, bool bCreate = true) = 0;
	virtual const char* GetStringForSymbol(HKeySymbol symbol) = 0;
	virtual void AddKeyValuesToMemoryLeakList(void* pMem, HKeySymbol name) = 0;
	virtual void RemoveKeyValuesFromMemoryLeakList(void* pMem) = 0;
	virtual void SetKeyValuesExpressionSymbol(const char* name, bool bValue) = 0;
	virtual bool GetKeyValuesExpressionSymbol(const char* name) = 0;
	virtual HKeySymbol GetSymbolForStringCaseSensitive( HKeySymbol &hCaseInsensitiveSymbol, const char* name, bool bCreate = true ) = 0;
};

class KeyValues {
public:
	KeyValues(const char* setName);
	void SetName(const char* setName);
	KeyValues* FindKey(const char* keyName, bool bCreate = false);
	void SetStringValue( char const *strValue );
	void SetString(const char* keyName, const char* value);
	void SetInt(const char* keyName, int value);
	void SetColor(const char* keyName, unsigned char* value);

	void* operator new(size_t iAllocSize);
	void operator delete(void* pMem);

private:
	void Init();

public:
	enum types_t {
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_COMPILED_INT_BYTE,			// hack to collapse 1 byte ints in the compiled format
		TYPE_COMPILED_INT_0,			// hack to collapse 0 in the compiled format
		TYPE_COMPILED_INT_1,			// hack to collapse 1 in the compiled format
		TYPE_NUMTYPES, 
	};

public:
	uint32_t m_iKeyName : 24;
	uint32_t m_iKeyNameCaseSensitive1 : 8;

	char* m_sValue;
	wchar_t* m_wsValue;

	union {
		int m_iValue;
		float m_flValue;
		void* m_pValue;
		unsigned char m_Color[4];
	};

	char m_iDataType;
	char m_bHasEscapeSequences;
	uint16_t m_iKeyNameCaseSensitive2;

	KeyValues* m_pPeer;
	KeyValues* m_pSub;
	KeyValues* m_pChain;

	void* m_pExpressionGetSymbolProc;
};

class Tier1 {
public:
	Interface* g_pCVar = nullptr;

	_RegisterConCommand RegisterConCommand = nullptr;
	_UnregisterConCommand UnregisterConCommand = nullptr;
	_FindCommandBase FindCommandBase = nullptr;
	_InstallGlobalChangeCallback InstallGlobalChangeCallback = nullptr;
	_RemoveGlobalChangeCallback RemoveGlobalChangeCallback = nullptr;

	ConCommandBase* m_pConCommandList = nullptr;

	void* ConCommand_VTable = nullptr;
	void* ConVar_VTable = nullptr;
	void* ConVar_VTable2 = nullptr;
	_AutoCompletionFunc AutoCompletionFunc = nullptr;

	using _Dtor = int(__rescall*)(ConVar* thisptr);
	using _Create = int(__rescall*)(ConVar* thisptr, const char* pName, const char* pDefaultValue, int flags, const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback);

	_Dtor Dtor = nullptr;
	_Create Create = nullptr;

	using _KeyValuesSystem = IKeyValuesSystem*(__rescall*)();
	_KeyValuesSystem KeyValuesSystem = nullptr;

public:
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE(TIER1); }
};

extern Tier1* tier1;

#endif // TIER1_HPP
