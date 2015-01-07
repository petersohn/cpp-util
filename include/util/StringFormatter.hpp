#ifndef INCLUDE_UTIL_STRINGFORMATTER_HPP
#define INCLUDE_UTIL_STRINGFORMATTER_HPP

#include "PrefixMap.hpp"
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>

namespace util {

struct StringFormatterException: public std::logic_error {
    using std::logic_error::logic_error;
};

template <typename Char>
class BasicStringFormatter {
public:

    using CharType = Char;
    using String = std::basic_string<Char>;
    using Action = std::function<String(const std::vector<String>&)>;
    using Map = BasicPrefixMap<Char, Action>;
    using EscapeMap = std::map<Char, String>;

private:

    Map actions;
    EscapeMap escapeMap;
    Char argumentChar;
    Char argumentSeparator;
    Char escapeChar;

public:

    BasicStringFormatter(Map actions,
            Char argumentChar = '%',
            Char argumentSeparator = ':',
            Char escapeChar = '\\',
            EscapeMap escapeMap = {
                    {'a', "\a"}, {'b', "\b"}, {'f', "\f"}, {'n', "\n"},
                    {'r', "\r"}, {'t', "\t"}, {'\\', "\\"}}):
        actions(std::move(actions)),
        escapeMap(std::move(escapeMap)),
        argumentChar(argumentChar),
        argumentSeparator(argumentSeparator),
        escapeChar(escapeChar)
    {}

    String formatString(const String& input)
    {
        using Iterator = typename String::const_iterator;
        String result;
        Iterator it = std::begin(input);
        Iterator end = std::end(input);

        while (it != end) {
            if (*it == argumentChar) {
                Iterator actionEnd = std::find(++it, end, argumentChar);

                if (actionEnd == end) {
                    throw StringFormatterException{"Unterminated action"};
                }

                if (it == actionEnd) {
                    result += argumentChar;
                } else {
                    Iterator nextSeparator = std::find(it, actionEnd,
                            argumentSeparator);
                    auto actionValue = actions.find(String(it, nextSeparator));

                    if (actionValue == actions.end()) {
                        throw StringFormatterException{"Unknown action"};
                    }

                    std::vector<String> arguments;

                    while (nextSeparator != actionEnd) {
                        it = nextSeparator;
                        ++it;
                        nextSeparator = std::find(it, actionEnd,
                                argumentSeparator);
                        arguments.emplace_back(it, nextSeparator);
                    }

                    it = actionEnd;
                    result += actionValue->second(arguments);
                }
            } else if (*it == escapeChar) {
                if (++it == end) {
                    throw StringFormatterException{"Unterminated escape sequence"};
                }

                auto escapeValue = escapeMap.find(*it);
                if (escapeValue == escapeMap.end()) {
                    throw StringFormatterException{"Invalid escape character"};
                }

                result += escapeValue->second;
            } else {
                result += *it;
            }

            ++it;
        }

        return result;
    }

};

using StringFormatter = BasicStringFormatter<char>;

}


#endif /* INCLUDE_UTIL_STRINGFORMATTER_HPP */
