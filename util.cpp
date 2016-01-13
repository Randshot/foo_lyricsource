#include "stdafx.h"
#include "util.h"
	std::string util::removeStrRegex(std::string& str, const char* pattern) {
		std::regex re(pattern);
		std::string ret = std::regex_replace(str, re, "");
		return ret;
	}


	void util::strReplace(std::string& str, const std::string& from, const std::string& to) {
		std::string::size_type pos = 0;
		while (pos = str.find(from, pos), pos != std::string::npos) {
			str.replace(pos, from.length(), to);
			pos += to.length();
		}
	}

	std::string util::trim(const std::string& string, const char* trimCharacterList)
	{
		std::string result;

		// ��������g�������镶���ȊO��������ʒu���������܂��B
		std::string::size_type left = string.find_first_not_of(trimCharacterList);
		if (left != std::string::npos)
		{
			// ��������g�������镶���ȊO�����������ꍇ�́A�����悤�ɉE��������������܂��B
			std::string::size_type right = string.find_last_not_of(trimCharacterList);

			// �߂�l�����肵�܂��B�����ł͉E�����猟�����Ă��A�g�������镶���ȊO���K�����݂���̂Ŕ���s�v�ł��B
			result = string.substr(left, right - left + 1);
		}
		return result;
	}

	void util::removeChars(std::string& str, const char* chars) {
		for (size_t c = str.find_first_of(chars); c != std::string::npos; c = c = str.find_first_of(chars)) {
			str.erase(c, 1);
		}

	}
