/// <summary>
///  This lets us build in some useful enum tools (like parsing to string), in a nice and easy to use way
/// Borrowed from https://stackoverflow.com/questions/28828957/enum-to-string-in-modern-c11-c14-c17-and-future-c20
/// </summary>
#pragma once 
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

/* Removes the given character from a string */
#define STRING_REMOVE_CHAR(str, ch) str.erase(std::remove(str.begin(), str.end(), ch), str.end())

/* Splits a string into multiple substrings, using the given separator character */
inline std::vector<std::string> splitString(const std::string& str, char sep = ',') {
	std::vector<std::string> vecString;
	std::string item;

	std::stringstream stringStream(str);

	while (std::getline(stringStream, item, sep)) {
		vecString.push_back(item);
	}

	return vecString;
}

#define ENUM_OPS(EnumType) \
	inline EnumType operator   |(EnumType a,  EnumType b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) | static_cast<bt(EnumType)>(b)); } \
	inline EnumType& operator |=(EnumType& a, EnumType b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) | static_cast<bt(EnumType)>(b)); return a; } \
	inline EnumType operator   &(EnumType a,  EnumType b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) & static_cast<bt(EnumType)>(b)); } \
	inline EnumType& operator &=(EnumType& a, EnumType b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) & static_cast<bt(EnumType)>(b)); return a; } \
	inline EnumType operator   ^(EnumType a,  EnumType b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) ^ static_cast<bt(EnumType)>(b)); } \
	inline EnumType& operator ^=(EnumType& a, EnumType b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) ^ static_cast<bt(EnumType)>(b)); return a; } \
	inline EnumType operator   |(EnumType a,  bt(EnumType) b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) | b); } \
	inline EnumType& operator |=(EnumType& a, bt(EnumType) b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) | b); return a; } \
	inline EnumType operator   &(EnumType a,  bt(EnumType) b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) & b); } \
	inline EnumType& operator &=(EnumType& a, bt(EnumType) b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a)    & b); return a; } \
	inline EnumType operator   ^(EnumType a,  bt(EnumType) b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) ^ b); } \
	inline EnumType& operator ^=(EnumType& a, bt(EnumType) b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) ^ b); return a; }

#define ENUM_OPS_SUBCLASS(EnumType) \
	inline friend EnumType operator   |(EnumType a,  EnumType b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) | static_cast<bt(EnumType)>(b)); } \
	inline friend EnumType& operator |=(EnumType& a, EnumType b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) | static_cast<bt(EnumType)>(b)); return a; } \
	inline friend EnumType operator   &(EnumType a,  EnumType b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) & static_cast<bt(EnumType)>(b)); } \
	inline friend EnumType& operator &=(EnumType& a, EnumType b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) & static_cast<bt(EnumType)>(b)); return a; } \
	inline friend EnumType operator   ^(EnumType a,  EnumType b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) ^ static_cast<bt(EnumType)>(b)); } \
	inline friend EnumType& operator ^=(EnumType& a, EnumType b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) ^ static_cast<bt(EnumType)>(b)); return a; } \
	inline friend EnumType operator   |(EnumType a,  bt(EnumType) b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) | b); } \
	inline friend EnumType& operator |=(EnumType& a, bt(EnumType) b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) | b); return a; } \
	inline friend EnumType operator   &(EnumType a,  bt(EnumType) b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) & b); } \
	inline friend EnumType& operator &=(EnumType& a, bt(EnumType) b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a)    & b); return a; } \
	inline friend EnumType operator   ^(EnumType a,  bt(EnumType) b) { return static_cast<EnumType>(static_cast<bt(EnumType)>(a) ^ b); } \
	inline friend EnumType& operator ^=(EnumType& a, bt(EnumType) b) { a =    static_cast<EnumType>(static_cast<bt(EnumType)>(a) ^ b); return a; }

/*
 * Declare an enum value with string conversion support, using a specified base type
 * @param E The name of the enum
 * @param T The underlying type of the enum
 */
#define ENUM_(E, T, ...)                                                                                                          \
    enum class E : T {                                                                                                            \
        __VA_ARGS__                                                                                                               \
    };                                                                                                                            \
    namespace impl { static std::map<T, std::string> E##MapName(generateEnumMap<T>(#__VA_ARGS__)); }                              \
    /* Writes the string name of an enum value to an output stream */                                                             \
    inline std::ostream &operator<<(std::ostream &os, E enumTmp) { os << impl::E##MapName[static_cast<T>(enumTmp)]; return os; }  \
	/* Returns the underlying value of this enum */                                                                               \
    inline T operator*(E enumTmp) { return static_cast<T>(enumTmp); }                                                             \
	/* Converts an enum into its string name */                                                                                   \
	inline const std::string& operator~(E enumTmp) { return impl::E##MapName[static_cast<T>(enumTmp)]; }                          \
	/* Appends an enums name to the end of a string */                                                                           \
    inline std::string operator+(std::string &&str, E enumTmp) { return str + impl::E##MapName[static_cast<T>(enumTmp)]; }        \
	/* Appends a string to an enums name, and returns it */                                                                      \
    inline std::string operator+(E enumTmp, std::string &&str) { return impl::E##MapName[static_cast<T>(enumTmp)] + str; }        \
	/* Appends an enums name to the end of a string */                                                                           \
    inline std::string &operator+=(std::string &str, E enumTmp) { str += impl::E##MapName[static_cast<T>(enumTmp)]; return str; } \
    /* Advances an enum to it's next possible value */                                                                            \
    inline E operator++(E &enumTmp) {                                                                                             \
        auto iter = impl::E##MapName.find(static_cast<T>(enumTmp));                                                               \
        if (iter == impl::E##MapName.end() || std::next(iter) == impl::E##MapName.end()) { iter = impl::E##MapName.begin(); }     \
        else { ++iter; }                                                                                                          \
        enumTmp = static_cast<E>(iter->first);                                                                                    \
        return enumTmp;                                                                                                           \
    }                                                                                                                             \
    /* Advances an enum to it's next possible value */                                                                            \
    inline E operator++(E &enumTmp, int) {                                                                                        \
        E temp = enumTmp;                                                                                                         \
        ++enumTmp;                                                                                                                \
        return temp;                                                                                                              \
    }                                                                                                                             \
    /* Determines if a given value is valid for an enum */                                                                        \
    inline bool IsValid##E(T value) { return (impl::E##MapName.find(value) != impl::E##MapName.end()); }		                  \
	/* Determines if a given value is valid for an enum */                                                                        \
	inline size_t CountOf##E(E value) { (void)value; return impl::E##MapName.size(); }											  \
	inline E Parse##E(const std::string& value, E defaultValue) {																  \
		auto it = impl::E##MapName.begin();																						  \
		while (it != impl::E##MapName.end()) {																					  \
			if (it->second == value) { return static_cast<E>(it->first); }														  \
			it++;																												  \
		}																														  \
		return defaultValue;																									  \
	}																															  \

#define ENUM(E, T, ...) ENUM_(E, T, __VA_ARGS__)
#define ENUM_FLAGS(E, T, ...) ENUM_(E, T, __VA_ARGS__); ENUM_OPS(E);

/*
 * Declare an enum value with string support, using the int32_t base type
 * @param E The name for the enum
 */
#define DECLARE_ENUM(E, ...) ENUM_(E, int32_t, __VA_ARGS__)

inline std::string GetVals(const std::string& text, int& base) {
	base = 10;
	int ix = 0;
	if (text[0] == '0' && text.length() > 1) {
		base = 8;
		if (text[1] == 'b') {
			base = 2;
			ix++;
		}
		else if (text[1] == 'x') {
			base = 16;
			ix++;
		}
		ix++;
	}
	std::vector<char> number;
	while (ix < text.length()) {
		if (std::isdigit(text[ix])) {
			if (!(text[ix] == '0' && number.empty())) {
				number.push_back(text[ix]);
			}
		} else if (base == 16) {
			char l = std::tolower(text[ix]);
			if (l >= 'a' && l <= 'e') {
				number.push_back(l);
			}
		}
		ix++;
	}
	if (number.empty())
		number.push_back('0');
	number.push_back('\0');
	std::string result = std::string(number.begin(), number.end());
	return result;
}

template <typename T>
inline std::map<T, std::string> generateEnumMap(std::string strMap)
{
	STRING_REMOVE_CHAR(strMap, ' ');
	STRING_REMOVE_CHAR(strMap, '(');

	std::vector<std::string> enumTokens(splitString(strMap));
	std::map<T, std::string> retMap;
	T inxMap;

	inxMap = 0;
	for (auto& str : enumTokens)
	{
		// Token: [EnumName | EnumName=EnumValue]
		std::string enumName;
		if (str.find('=') == std::string::npos) {
			enumName = str;
		}
		else {
			std::vector<std::string> enumNameValue(splitString(str, '='));
			enumName = enumNameValue[0];
			int base = 10;
			std::string res = GetVals(enumNameValue[1], base);
			STRING_REMOVE_CHAR(res, '\n');
			if (std::is_unsigned<T>::value) {
				inxMap = static_cast<T>(std::stoull(res, nullptr, base));
			}
			else {
				inxMap = static_cast<T>(std::stoll(res, nullptr, base));
			}
		}
		retMap[inxMap++] = enumName;
	}

	return retMap;
}

#define bt(x) std::underlying_type<x>::type
