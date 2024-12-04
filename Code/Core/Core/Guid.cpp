#include "Core/Guid.h"

#include "Core/Assert.h"
#include "Core/StringHelpers.h"

#include <Objbase.h>

str::Guid str::Guid::Unassigned = str::Guid();

namespace
{
	constexpr const char* s_Compact = "%08x%04x%04x%04x%04x%04x%04x";
	constexpr const char* s_Hyphenated = "%08x-%04x-%04x-%04x-%04x%04x%04x";
}

str::String str::Guid::ToString() const
{
	char buffer[64];
	const char* data = reinterpret_cast<const char*>(m_Data.m_U64);

	snprintf(
		buffer,
		sizeof(buffer),
		s_Compact,
		m_Data.m_U32[0],
		m_Data.m_U16[2],
		m_Data.m_U16[3],
		m_Data.m_U16[4],

		// #note: these last 3 are ordered in reverse on purpose
		m_Data.m_U16[7],
		m_Data.m_U16[6],
		m_Data.m_U16[5]);

	return str::String(buffer);
}

str::Guid str::Guid::Create(const str::StringView& string)
{
#ifdef ASSERTS_ENABLED
	ValidateString(string);
#endif // ASSERTS_ENABLED

	str::Guid guid;
	guid.m_Data.m_U32[0] = 
		HexToByte(string[0]) << 28 | HexToByte(string[1]) << 24 |
		HexToByte(string[2]) << 20 | HexToByte(string[3]) << 16 |
		HexToByte(string[4]) << 12 | HexToByte(string[5]) <<  8 |
		HexToByte(string[6]) <<  4 | HexToByte(string[7]) <<  0;

	guid.m_Data.m_U16[2] = 
		HexToByte(string[8]) << 12 | HexToByte(string[9]) <<  8 |
		HexToByte(string[10]) << 4 | HexToByte(string[11]) <<  0;

	guid.m_Data.m_U16[3] =
		HexToByte(string[12]) << 12 | HexToByte(string[13]) << 8 |
		HexToByte(string[14]) <<  4 | HexToByte(string[15]) << 0;

	guid.m_Data.m_U16[4] =
		HexToByte(string[16]) << 12 | HexToByte(string[17]) << 8 |
		HexToByte(string[18]) <<  4 | HexToByte(string[19]) << 0;

	// #note: these last 3 are ordered in reverse on purpose
	guid.m_Data.m_U16[7] =
		HexToByte(string[20]) << 12 | HexToByte(string[21]) << 8 |
		HexToByte(string[22]) <<  4 | HexToByte(string[23]) << 0;
	guid.m_Data.m_U16[6] =
		HexToByte(string[24]) << 12 | HexToByte(string[25]) << 8 |
		HexToByte(string[26]) <<  4 | HexToByte(string[27]) << 0;
	guid.m_Data.m_U16[5] =
		HexToByte(string[28]) << 12 | HexToByte(string[29]) << 8 |
		HexToByte(string[30]) <<  4 | HexToByte(string[31]) << 0;

	return guid;
}

str::Guid str::Guid::Generate()
{
	str::Guid guid;
	static_assert(sizeof(GUID) == sizeof(str::Guid), "str::Guid's size doesn't match Win32 GUID.");
	CoCreateGuid((GUID*)&guid);
	return guid;
}

bool str::Guid::IsValidString(const str::StringView& string)
{
	if (string.length() != 32)
		return false;

	return isxdigit(string[0])
		&& isxdigit(string[1])
		&& isxdigit(string[2])
		&& isxdigit(string[3])
		&& isxdigit(string[4])
		&& isxdigit(string[5])
		&& isxdigit(string[6])
		&& isxdigit(string[7])
		&& isxdigit(string[8])
		&& isxdigit(string[9])
		&& isxdigit(string[10])
		&& isxdigit(string[11])
		&& isxdigit(string[12])
		&& isxdigit(string[13])
		&& isxdigit(string[14])
		&& isxdigit(string[15])
		&& isxdigit(string[16])
		&& isxdigit(string[17])
		&& isxdigit(string[18])
		&& isxdigit(string[19])
		&& isxdigit(string[20])
		&& isxdigit(string[21])
		&& isxdigit(string[22])
		&& isxdigit(string[23])
		&& isxdigit(string[24])
		&& isxdigit(string[25])
		&& isxdigit(string[26])
		&& isxdigit(string[27])
		&& isxdigit(string[28])
		&& isxdigit(string[29])
		&& isxdigit(string[30])
		&& isxdigit(string[31]);
}

void str::Guid::ValidateString(const str::StringView& string)
{
	if (string.length() != 32)
	{
		Z_LOG(ELog::Assert, "Guid string length is invalid! Expected: 36, Received: {}", string.length());
		return;
	}

	str::String positions;
	if (!isxdigit(string[ 0])) { positions += " 0, "; }
	if (!isxdigit(string[ 1])) { positions += " 1, "; }
	if (!isxdigit(string[ 2])) { positions += " 2, "; }
	if (!isxdigit(string[ 3])) { positions += " 3, "; }
	if (!isxdigit(string[ 4])) { positions += " 4, "; }
	if (!isxdigit(string[ 5])) { positions += " 5, "; }
	if (!isxdigit(string[ 6])) { positions += " 6, "; }
	if (!isxdigit(string[ 7])) { positions += " 7, "; }
	if (!isxdigit(string[ 8])) { positions += " 8, "; }
	if (!isxdigit(string[ 9])) { positions += " 9, "; }
	if (!isxdigit(string[10])) { positions += "10, "; }
	if (!isxdigit(string[11])) { positions += "11, "; }
	if (!isxdigit(string[12])) { positions += "12, "; }
	if (!isxdigit(string[13])) { positions += "13, "; }
	if (!isxdigit(string[14])) { positions += "14, "; }
	if (!isxdigit(string[15])) { positions += "15, "; }
	if (!isxdigit(string[16])) { positions += "16, "; }
	if (!isxdigit(string[17])) { positions += "17, "; }
	if (!isxdigit(string[18])) { positions += "18, "; }
	if (!isxdigit(string[19])) { positions += "19, "; }
	if (!isxdigit(string[20])) { positions += "20, "; }
	if (!isxdigit(string[21])) { positions += "21, "; }
	if (!isxdigit(string[22])) { positions += "22, "; }
	if (!isxdigit(string[23])) { positions += "23, "; }
	if (!isxdigit(string[24])) { positions += "24, "; }
	if (!isxdigit(string[25])) { positions += "25, "; }
	if (!isxdigit(string[26])) { positions += "26, "; }
	if (!isxdigit(string[27])) { positions += "27, "; }
	if (!isxdigit(string[28])) { positions += "28, "; }
	if (!isxdigit(string[29])) { positions += "29, "; }
	if (!isxdigit(string[30])) { positions += "30, "; }
	if (!isxdigit(string[31])) { positions += "31, "; }

	if (!positions.empty())
	{
		positions.pop_back();
		positions.pop_back();
		Z_LOG(ELog::Assert, "Guid contains invalid characters at positions {}!", positions.c_str());
	}
}
