#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

/// Утилиты для работы со строками
class StringUtils {
public:
	/// Проверяет, является ли символ цифрой (ASCII)
	static bool isDigitASCII(char c);

	/// Проверяет, состоит ли строка только из цифр
	static bool isOnlyDigits(const std::string& str);

	/// Проверяет, является ли символ пробельным
	static bool isSafeSpace(char c);

	/// Удаляет пробелы в начале и конце строки
	static std::string trim(const std::string& str);

	/// Разбивает строку на токены по пробельным символам
	static std::vector<std::string> tokenize(const std::string& str);
};

#endif // UTILS_H