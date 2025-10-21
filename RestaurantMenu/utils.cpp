#include "utils.h"

// Реализации методов StringUtils

bool StringUtils::isDigitASCII(char c) {
	return c >= '0' && c <= '9';
}

bool StringUtils::isOnlyDigits(const std::string& str) {
	if (str.empty()) return false;
	for (char c : str) {
		if (!isDigitASCII(c)) return false;
	}
	return true;
}

bool StringUtils::isSafeSpace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

std::string StringUtils::trim(const std::string& str) {
	std::string result = str;
	while (!result.empty() && isSafeSpace(result[0])) {
		result.erase(0, 1);
	}
	while (!result.empty() && isSafeSpace(result[result.length() - 1])) {
		result.erase(result.length() - 1, 1);
	}
	return result;
}

std::vector<std::string> StringUtils::tokenize(const std::string& str) {
	std::vector<std::string> tokens;
	std::string token;
	bool inToken = false;

	for (char c : str) {
		if (isSafeSpace(c)) {
			if (inToken && !token.empty()) {
				tokens.push_back(token);
				token.clear();
				inToken = false;
			}
		}
		else {
			token += c;
			inToken = true;
		}
	}

	if (!token.empty()) {
		tokens.push_back(token);
	}
	return tokens;
}