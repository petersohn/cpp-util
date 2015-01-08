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

struct InvalidAction: public StringFormatterException {
    InvalidAction(const std::string& name):
        StringFormatterException{"Invalid action: " + name}
    {}
};

struct InvalidEscapeSequence: public StringFormatterException {
    InvalidEscapeSequence(const std::string& name):
        StringFormatterException{"Invalid action: " + name}
    {}
};

struct UnterminatedAction: public StringFormatterException {
    UnterminatedAction():
        StringFormatterException{"Unterminated action"}
    {}
};

struct UnterminatedEscapeSequence: public StringFormatterException {
    UnterminatedEscapeSequence():
        StringFormatterException{"Unterminated escape sequence"}
    {}
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
                    throw UnterminatedAction{};
                }

                if (it == actionEnd) {
                    result += argumentChar;
                } else {
                    Iterator nextSeparator = std::find(it, actionEnd,
                            argumentSeparator);
                    String name(it, nextSeparator);
                    auto actionValue = actions.find(name);

                    if (actionValue == actions.end()) {
                        throw InvalidAction{name};
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
                    throw UnterminatedEscapeSequence{};
                }

                auto escapeValue = escapeMap.find(*it);
                if (escapeValue == escapeMap.end()) {
                    throw InvalidEscapeSequence{std::string{*it}};
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
