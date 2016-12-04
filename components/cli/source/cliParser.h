#ifndef MICROHAL_CLI_GETOPT__CPP_
#define MICROHAL_CLI_GETOPT__CPP_

/* ************************************************************************************************
 * INCLUDES
 */

#include <cstring>
#include <iostream>
#include <list>
#include <cstdlib>

/** Przyk³ad zastosowania
 *
 * 		class SterujSth : GetOpt(), MenuItem()
 * 		{
 * 			virtual int execute(std::list<char*> &words, IODevice& port) {
 *				std::pair<char*, int> max_speed(":id", -1);
 *				std::pair<char*, char[30]> owner("own", "\0");
 *				std::pair<char*, float> ("ms", 0.0);
 *
 *				int status;
 *				status = toInt(words, port, std::pair(int car_id,
 *				status = toFloat(words, port,
 *			}
 * 		}
 *
 */

namespace microhal {

class CliParser {
private:

	CliParser(){};

public:

	typedef enum {
		Error = 0x00,
		MaxViolation = 0x01,		// argument value bigger than permitted (set biggest)
		MinViolation = 0x02,		// argument value lesser than permitted (set smallest)
		LengthViolation = 0x04,	// argument string do not suit given buffer (done nothing)
		NoArg = 0x08,				// no argument given (returns default value)
		WrongArg = 0x10,			// argument has wrong format (returns default value)
		NoMnemonic = 0x20,			// no such mnemonic (returns default value)
		OK = 0x40					// returns appropriate value
	} ParserStatus;

//	template <typename T>
//	ParserStatus parseNumerical(std::list<char*> &words, std::pair<T,char*> &val, T defalt, T min, T max, int base = 10){
//		std::list<char*>::iterator it = words.begin();
//		do {
//			// if there is such a word in a call list
//			if (!strcmp(val.second(), *it)) {
//				++it;
//				if (words.end() != it) {
//					// numerical argument
//					T arg;
//					if(std::typeid(unsigned int) == std::typeid(T)){
//					} else if (std::typeid(uint8_t))
//				} else {
//					// no more words in calling line, there is no argument
//					return ParserStatus::NoArg;
//				}
//			}
//			++it;
//		} while (it != val.end());
//
//		return ParserStatus::NoMnemonic;
//	}

	/*
	 * void(float sth);
	 * void(long sth);
	 *
	 * template <typename T>
	 * void foo(T sth){
	 * 	bar(sth);
	 * }
	 */

//	static int bar(int sth){
//		return ++sth;
//	}
//	static float bar(float sth){
//		return ++sth;
//	}
//	static unsigned long bar(unsigned long sth){
//		return ++sth;
//	}
//
//
//	template <typename T>
//	static T foo(T sth){
//		if
//		return bar((T)sth);
//	}

	static ParserStatus parseNumerical(std::list<char*> &words, std::pair<int,char*> &val, int defalt, int min, int max, int base = 10);
	static ParserStatus parseNumerical(std::list<char*> &words, std::pair<float,char*> &val, float defalt, float min, float max, int base = 10);
	static ParserStatus parseString(std::list<char*> &words, std::pair<char*,char*> &val, unsigned int maxLength);

	template <typename T>
	static bool argumentCheck(std::list<char*> &words, std::pair<T,char*> &val);

	inline static int argumentsCount(std::list<char*> &words) {
		return words.size();
	}

};

} // namespace microhal

#endif /* MICROHAL_CLI_GETOPT__CPP_ */
