/*
 * diagnosticHeaderGenerator.h
 *
 *  Created on: 7 gru 2015
 *      Author: Pawel
 */

#ifndef _MICROHAL_DIAGNOSTIC_HEADERGENERATOR_H_
#define _MICROHAL_DIAGNOSTIC_HEADERGENERATOR_H_


#include <cstdint>

#include "compileTime_itoa.h"
#include "crc/crc32.h"
#include "diagnostic_base.h"
#include "../utils/literal_string.h"


// define macro with lambda function that contain file name, file line and log position
#define MICROHAL_DIAGNOSTIC_HEADER_GENERATOR(level) ([](){ \
    struct LogPosition { \
        using type = const char (&)[sizeof(__FILE__)]; \
        constexpr type name() const { \
            return __FILE__; \
        } \
        constexpr uint32_t line() const { \
            return __LINE__; \
        } \
        constexpr microhal::diagnostic::LogLevel logLevel() const {\
            return level; \
        } \
    }logPos; return microhal::diagnostic::generate(logPos); }().header)


namespace microhal {
namespace diagnostic {

template<LogLevel levels>
class LogLevelHeader : public LogLevelHeader_base {
 public:
    explicit constexpr LogLevelHeader(const char* levelFileLineInfo, const char *code) noexcept
        : LogLevelHeader_base(levelFileLineInfo, code) {
    }
 private:
    friend class Diagnostic_base;
};


constexpr std::size_t find_last_of(const char *str, char sign) {
    std::size_t pos = 0, iter = 0;

    while(1) {
        if (str[iter] == sign) pos = iter;
        if (str[iter] == '\0') break;
        iter++;
    }

    return pos;
}

// ----------------------------- error code generation ------------------------
template<LogLevel level, unsigned int x>
class ErrorCode {
 public:
    // returns a pointer to the instantiated string
    static constexpr const char * c_str() { return value.data; }

 private:
    static constexpr const char logChar[8] = {'E', 'A', 'C', 'e', 'W', 'N', 'I', 'D'};
    static constexpr unsigned int base = 16;

    // generate a unique string type representing this number
    typedef typename numeric_builder<num_digits(x, base), x, base, '\0'>::type hashType;
    typedef metastring<logChar[static_cast<unsigned int>(level)-1]> codeChar;

    typedef decltype(codeChar{} + hashType{}) outType;

    // declare a static string of that type (instantiated later at file scope)
    static constexpr outType value = {};
};
// instantiate numeric_string::value as needed for different numbers
template<LogLevel level, unsigned int x>
constexpr typename ErrorCode<level, x>::outType ErrorCode<level, x>::value;

// ---------------------------------- log position generation ----------------------------
template< std::size_t N>
struct LogPositionContainer {
    template<std::size_t... FilenameIndices, std::size_t... lineIndices>
    constexpr LogPositionContainer(std::index_sequence<FilenameIndices...> s, std::index_sequence<lineIndices...> a, const char * filename, const char * line)
        : str{filename[FilenameIndices]..., ':', line[lineIndices]..., '\0'} {
        	(void)s;
        	(void)a;

    }
    const char str[N+2];
};

template<std::size_t N, LogLevel level>
struct HeaderContainer {
    constexpr HeaderContainer(LogPositionContainer<N> filename, const char *errorCode) : filename(filename), header(LogLevelHeader<level>(this->filename.str, errorCode)) {}

    const LogPositionContainer<N> filename;
    const LogLevelHeader<level> header;
};

template <typename T>
constexpr auto generate(const T file) {
    constexpr std::size_t size = sizeof( file.name() );
    // because file.name() contain full path to file we want to remove path and leave only file name
    // so first find last '/' in path
    constexpr std::size_t fileNameBegin = find_last_of(file.name(), '/') + 1;
    // calculate length of file name without path
    constexpr std::size_t fileNameLength = size - fileNameBegin;
    // calculate pointer that points to begin of file name
    constexpr const char* ptr = file.name() + fileNameBegin;

    // lets convert line number to characters
    constexpr auto line = to_string<file.line(), 10>();

    // prepare log position text
    constexpr LogPositionContainer<fileNameLength - 1 + line.size()> logPosition(std::make_index_sequence<fileNameLength - 1>{},
                                                                               std::make_index_sequence<line.size()>{}, ptr, line.c_str());

    // let calculate hash of logPosition text, this will produce unique log code
    constexpr const char * errorCode = ErrorCode<file.logLevel(), crc32__(logPosition.str)>::c_str();

    // let assign 'log position' text and errorCode to header container and return it from this function
    const HeaderContainer<sizeof(logPosition.str) - 2, file.logLevel()> headerContainer = {logPosition, errorCode};

    return headerContainer;
}

} // namespace diagnostic
} // namespace microhal

#endif // _MICROHAL_DIAGNOSTIC_HEADERGENERATOR_H_
