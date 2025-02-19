#pragma once

// used: fnv-1a hash
#include "FNV1A.h"

#define CS_CLASS_NO_CONSTRUCTOR(CLASS)	\
CLASS(CLASS&&) = delete;				\
CLASS(const CLASS&) = delete;

#define CS_CLASS_NO_ASSIGNMENT(CLASS)	\
CLASS& operator=(CLASS&&) = delete;		\
CLASS& operator=(const CLASS&) = delete;

#define CS_CLASS_NO_INITIALIZER(CLASS)	\
CS_CLASS_NO_CONSTRUCTOR(CLASS)			\
CS_CLASS_NO_ASSIGNMENT(CLASS)

#define SCHEMA_ADD_OFFSET(TYPE, NAME, OFFSET)                                                                 \
	[[nodiscard]] __forceinline std::add_lvalue_reference_t<TYPE> NAME()                                      \
	{                                                                                                         \
		static const std::uint32_t uOffset = OFFSET;                                                          \
		return *reinterpret_cast<std::add_pointer_t<TYPE>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset));\
	}

#define SCHEMA_ADD_POFFSET(TYPE, NAME, OFFSET)                                                               \
	[[nodiscard]] __forceinline std::add_pointer_t<TYPE> NAME()                                              \
	{                                                                                                        \
		const static std::uint32_t uOffset = OFFSET;                                                         \
		return reinterpret_cast<std::add_pointer_t<TYPE>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset));\
	}

#define SCHEMA_ADD_FIELD_OFFSET(TYPE, NAME, FIELD, ADDITIONAL) SCHEMA_ADD_OFFSET(TYPE, NAME, Schema::GetOffset(FNV1A::HashConst(FIELD)) + ADDITIONAL)
#define SCHEMA_ADD_FIELD(TYPE, NAME, FIELD) SCHEMA_ADD_FIELD_OFFSET(TYPE, NAME, FIELD, 0U)
#define SCHEMA_ADD_PFIELD_OFFSET(TYPE, NAME, FIELD, ADDITIONAL) SCHEMA_ADD_POFFSET(TYPE, NAME, Schema::GetOffset(FNV1A::HashConst(FIELD)) + ADDITIONAL)
#define SCHEMA_ADD_PFIELD(TYPE, NAME, FIELD) SCHEMA_ADD_PFIELD_OFFSET(TYPE, NAME, FIELD, 0U)

namespace Schema
{
	// store the offset of the field in the class
	// dump stored data to file
	bool Setup(const char* szModuleName);

	/* @section: get */
	// get offset of the field in the class
	// @note: only client.dll class & fields
	[[nodiscard]] std::uint32_t GetOffset(const FNV1A_t uHashedFieldName);

	// get foregin offset from other .dll
	[[nodiscard]] std::uint32_t GetForeignOffset(const char* szModulenName, const FNV1A_t uHashedClassName, const FNV1A_t uHashedFieldName);
}
