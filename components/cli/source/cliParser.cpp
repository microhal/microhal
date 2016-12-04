#include "cliParser.h"

namespace microhal {

CliParser::ParserStatus CliParser::parseNumerical(std::list<char*> &words, std::pair<int,char*> &val, int defalt, int min, int max, int base){
	ParserStatus fin = ParserStatus::NoMnemonic;
	if(!words.size())
		return fin;
	long arg;

	// parsing the line
	std::list<char*>::iterator it = words.begin();
	do {
		// if there is such a word in a call list
		if (!strcmp(val.second, *it)) {
			// searching for the argument
			++it;
			if (words.end() != it) {
				// numerical argument parsing
				char *endPtr;
				errno = 0;
				arg = std::strtol(*it, &endPtr, base);
				if (endPtr == *it || *endPtr != '\0' || errno == ERANGE){
					fin = ParserStatus::WrongArg;
				} else {
					val.first = arg;
					fin = ParserStatus::OK;
				}
			} else {
				// no more words in calling line, there is no argument
				fin = ParserStatus::NoArg;
			}
			break;
		}
		++it;
	} while (it != words.end());

	// if above was a match, checking given borders
	if(ParserStatus::OK == fin){
		if(arg > max){
			fin = ParserStatus::MaxViolation;
			arg = max;
		} else if(arg < min){
			fin = ParserStatus::MinViolation;
			arg = min;
		}
	} else {
		arg = defalt;
	}

	val.first = arg;
	return fin;
}

CliParser::ParserStatus CliParser::parseNumerical(std::list<char*> &words, std::pair<float,char*> &val, float defalt, float min, float max, int base){
	ParserStatus fin = ParserStatus::NoMnemonic;
	if(!words.size())
		return fin;

	float arg;

	// parsing the line
	std::list<char*>::iterator it = words.begin();
	do {
		// if there is such a word in a call list
		if (!strcmp(val.second, *it)) {
			// searching for the argument
			++it;
			if (words.end() != it) {
				// numerical argument parsing
				char *endPtr;
				errno = 0;
				arg = strtof(*it, &endPtr);
				if (endPtr == *it || *endPtr != '\0' || errno == ERANGE){
					fin = ParserStatus::WrongArg;
				} else {
					val.first = arg;
					fin = ParserStatus::OK;
				}
			} else {
				// no more words in calling line, there is no argument
				fin = ParserStatus::NoArg;
			}
			break;
		}
		++it;
	} while (it != words.end());

	// if above was a match, checking given borders
	if(ParserStatus::OK == fin){
		if(arg > max){
			fin = ParserStatus::MaxViolation;
			arg = max;
		} else if(arg < min){
			fin = ParserStatus::MinViolation;
			arg = min;
		}
	} else {
		arg = defalt;
	}

	val.first = arg;
	return fin;
}

CliParser::ParserStatus CliParser::parseString(std::list<char*> &words, std::pair<char*,char*> &val, unsigned int maxLength){
	ParserStatus fin = ParserStatus::NoMnemonic;
	if(!words.size())
		return fin;
	// parsing the line
	std::list<char*>::iterator it = words.begin();
	do {
		// if there is such a word in a call list
		if (!strcmp(val.second, *it)) {
			// searching for the argument
			++it;
			if (words.end() != it) {
				// argument copying
				if(strlen(*it) > maxLength){
					fin = ParserStatus::LengthViolation;
				} else {
					strcpy(val.first, *it);
					fin = ParserStatus::OK;
				}
			} else {
				// no more words in calling line, there is no argument
				fin = ParserStatus::NoArg;
			}
			break;
		}
		++it;
	} while (it != words.end());

	return fin;
}


template <typename T> bool CliParser::argumentCheck(std::list<char*> &words, std::pair<T,char*> &val){
	for (std::list<char*>::iterator it = words.begin(); it != words.end(); ++it) {
		// if there is such a word in a call list
		if(!strcmp(val.second(), *it)){
			return true;
		}
	}
	return false;
}

} // namespace microhal
