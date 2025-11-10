#include "utils.h"

// ==================== РЕАЛИЗАЦИЯ STRING UTILS ====================

/// Проверяет, является ли символ цифрой в кодировке ASCII
bool StringUtils::isDigitASCII(char c) {
	return c >= '0' && c <= '9';
}

/// Проверяет, состоит ли строка исключительно из цифровых символов
bool StringUtils::isOnlyDigits(const std::string& str) {
	if (str.empty()) return false;

	// Проверяем каждый символ строки на цифровое значение
	for (char c : str) {
		if (!isDigitASCII(c)) return false;
	}
	return true;
}

/// Проверяет, является ли символ безопасным пробельным символом
bool StringUtils::isSafeSpace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/// Удаляет пробельные символы с начала и конца строки
std::string StringUtils::trim(const std::string& str) {
	std::string result = str;

	// Удаляем пробелы в начале строки
	while (!result.empty() && isSafeSpace(result[0])) {
		result.erase(0, 1);
	}

	// Удаляем пробелы в конце строки
	while (!result.empty() && isSafeSpace(result[result.length() - 1])) {
		result.erase(result.length() - 1, 1);
	}

	return result;
}

/// Разбивает строку на токены по пробельным символам
std::vector<std::string> StringUtils::tokenize(const std::string& str) {
	std::vector<std::string> tokens;
	std::string token;
	bool inToken = false;

	// Проходим по каждому символу строки
	for (char c : str) {
		if (isSafeSpace(c)) {
			if (inToken && !token.empty()) {
				// Завершаем текущий токен и добавляем в результат
				tokens.push_back(token);
				token.clear();
				inToken = false;
			}
		}
		else {
			// Добавляем символ к текущему токену
			token += c;
			inToken = true;
		}
	}

	// Добавляем последний токен, если он есть
	if (!token.empty()) {
		tokens.push_back(token);
	}

	return tokens;
}