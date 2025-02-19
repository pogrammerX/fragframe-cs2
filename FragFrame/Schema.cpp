// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "schema.h"

// used: [stl] vector
#include <vector>
// used: [stl] find_if
#include <algorithm>

// used: ischemasystem
#include <shlobj.h>

// used: l_print
#include "Data.h"
#include "ischemasystem.h"

struct SchemaData_t
{
	FNV1A_t uHashedFieldName = 0x0ULL;
	std::uint32_t uOffset = 0x0U;

	SchemaData_t(FNV1A_t hash, uint32_t offset) {
		uHashedFieldName = hash;
		uOffset = offset;
	}
};

static std::vector<SchemaData_t> vecSchemaData;

struct SchemaType_t {
	std::string szName;
	int nSize;
};

struct SchemaField_t {
	std::string szName;
	SchemaType_t sType;
	int nOffset;
};

struct SchemaClass_t {
	std::string szName;
	std::string szInherits;
	std::vector<SchemaField_t> arrFields;
};

bool Schema::Setup(const char* szModuleName)
{
	CSchemaSystemTypeScope* pTypeScope = CSData::g_pSchemaSystem->FindTypeScopeForModule(szModuleName);
	if (pTypeScope == nullptr)
		return false; 

	const int nTableSize = pTypeScope->hashClasses.Count();

	// allocate memory for elements
	UtlTSHashHandle_t* pElements = new UtlTSHashHandle_t[nTableSize + 1U];
	const auto nElements = pTypeScope->hashClasses.GetElements(0, nTableSize, pElements);

	for (int i = 0; i < nElements; i++)
	{
		const UtlTSHashHandle_t hElement = pElements[i];

		if (hElement == 0)
			continue;

		CSchemaClassBinding* pClassBinding = pTypeScope->hashClasses[hElement];
		if (pClassBinding == nullptr)
			continue;

		SchemaClassInfoData_t* pDeclaredClassInfo;
		pTypeScope->FindDeclaredClass(&pDeclaredClassInfo, pClassBinding->szBinaryName);

		if (pDeclaredClassInfo == nullptr)
			continue;

		if (pDeclaredClassInfo->nFieldSize == 0)
			continue;

		for (auto j = 0; j < pDeclaredClassInfo->nFieldSize; j++)
		{
			SchemaClassFieldData_t* pFields = pDeclaredClassInfo->pFields;

			std::string szFieldClassBuffer = std::string(pClassBinding->szBinaryName) + "->" + std::string(pFields[j].szName) + "\0";

			vecSchemaData.emplace_back(FNV1A::Hash(szFieldClassBuffer.c_str()), pFields[j].nSingleInheritanceOffset);
		}
	}


	// free allocated memory
	delete[] pElements;

	return true;
}

std::uint32_t Schema::GetOffset(const FNV1A_t uHashedFieldName)
{
	if (const auto it = std::ranges::find_if(vecSchemaData, [uHashedFieldName](const SchemaData_t& data)
		{ return data.uHashedFieldName == uHashedFieldName; });
		it != vecSchemaData.end())
		return it->uOffset;

	return 0U;
}

// @todo: optimize this, this is really poorly do and can be done much better?
std::uint32_t Schema::GetForeignOffset(const char* szModulenName, const FNV1A_t uHashedClassName, const FNV1A_t uHashedFieldName)
{
	CSchemaSystemTypeScope* pTypeScope = CSData::g_pSchemaSystem->FindTypeScopeForModule(szModulenName);
	if (pTypeScope == nullptr)
		return false;

	const int nTableSize = pTypeScope->hashClasses.Count();
	// allocate memory for elements
	UtlTSHashHandle_t* pElements = new UtlTSHashHandle_t[nTableSize + 1U];
	const auto nElements = pTypeScope->hashClasses.GetElements(0, nTableSize, pElements);
	std::uint32_t uOffset = 0x0;

	for (int i = 0; i < nElements; i++)
	{
		const UtlTSHashHandle_t hElement = pElements[i];

		if (hElement == 0)
			continue;

		CSchemaClassBinding* pClassBinding = pTypeScope->hashClasses[hElement];
		if (pClassBinding == nullptr)
			continue;

		SchemaClassInfoData_t* pDeclaredClassInfo;
		pTypeScope->FindDeclaredClass(&pDeclaredClassInfo, pClassBinding->szBinaryName);

		if (pDeclaredClassInfo == nullptr)
			continue;

		if (pDeclaredClassInfo->nFieldSize == 0)
			continue;

		for (auto j = 0; j < pDeclaredClassInfo->nFieldSize; j++)
		{
			SchemaClassFieldData_t* pFields = pDeclaredClassInfo->pFields;
			if (pFields == nullptr)
				continue;

			SchemaClassFieldData_t field = pFields[j];
			if (FNV1A::Hash(pClassBinding->szBinaryName) == uHashedClassName && FNV1A::Hash(field.szName) == uHashedFieldName)
				uOffset = field.nSingleInheritanceOffset;
		}
	}

	return uOffset;
}