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

private:

    Map actions;
    Char escapeChar;
    Char argumentSeparator;

public:

    BasicStringFormatter(Map actions, Char escapeChar = '%',
            Char argumentSeparator = ':'):
        actions(std::move(actions)),
        escapeChar(escapeChar),
        argumentSeparator(argumentSeparator)
    {}

    String formatString(const String& input)
    {
        using Iterator = typename String::const_iterator;
        String result;
        Iterator it = std::begin(input);
        Iterator end = std::end(input);

        while (it != end) {
            if (*it == escapeChar) {
                Iterator actionEnd = std::find(++it, end, escapeChar);

                if (actionEnd == end) {
                    throw StringFormatterException{"Unterminated action"};
                }

                if (it == actionEnd) {
                    result += escapeChar;
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
