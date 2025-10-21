#include "file_parser.h"

MenuFileParser::MenuFileParser()
	: numberParser_(std::make_unique<NumberParser>())
	, timeParser_(std::make_unique<TimeParser>()) {}

void MenuFileParser::parseFile(const std::string& filename, IMenuStorage& storage, int& invalidCount) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Ошибка открытия файла!");
	}

	std::string line;
	while (std::getline(file, line)) {
		if (!line.empty() && !parseDishLine(line, storage)) {
			invalidCount++;
		}
	}
	file.close();
}

bool MenuFileParser::parseDishLine(const std::string& line, IMenuStorage& storage) {
	std::string trimmedLine = StringUtils::trim(line);
	if (trimmedLine.empty()) return false;

	std::string name;
	std::string remaining;
	if (!extractQuotedName(trimmedLine, name, remaining)) return false;

	std::vector<std::string> tokens = StringUtils::tokenize(remaining);
	if (tokens.size() != 2) return false;

	double price = 0.0;
	Time time;
	bool hasPrice = false, hasTime = false;

	// Парсим токены как цену или время
	for (const auto& token : tokens) {
		if (!hasTime && timeParser_->parse(token)) {
			time = timeParser_->getResult();
			hasTime = true;
		}
		else if (!hasPrice && numberParser_->parse(token)) {
			price = numberParser_->getResult();
			hasPrice = true;
		}
	}

	if (!hasPrice || !hasTime) return false;

	Time normalizedTime = TimeParser::normalizeTime(time);
	storage.addDish(name, price, normalizedTime.hours, normalizedTime.minutes);
	return true;
}

bool MenuFileParser::extractQuotedName(const std::string& line, std::string& name, std::string& remaining) {
	size_t startQuote = line.find('"');
	if (startQuote == std::string::npos) return false;

	size_t endQuote = line.find('"', startQuote + 1);
	if (endQuote == std::string::npos) return false;

	name = line.substr(startQuote + 1, endQuote - startQuote - 1);
	if (name.empty()) return false;

	remaining = line.substr(0, startQuote) + " " + line.substr(endQuote + 1);
	return true;
}