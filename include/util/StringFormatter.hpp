#ifndef INCLUDE_UTIL_STRINGFORMATTER_HPP
#define INCLUDE_UTIL_STRINGFORMATTER_HPP

#include "PrefixMap.hpp"
#include "LazyArgumentEnum.hpp"
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

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

LAZY_ARGUMENT_ENUM(TextAlignment, textAlignments, (left)(right)(center))

template <typename T, typename Char>
class GenericFormat {
public:
    using String = std::basic_string<Char>;

    GenericFormat(T value,
            TextAlignment defaultTextAlignment = TextAlignment::right,
            Char defaultPaddingChar = ' ',
            std::size_t defaultMinimumLength = 0):
        value(std::move(value)),
        defaultTextAlignment(defaultTextAlignment),
        defaultPaddingChar(defaultPaddingChar),
        defaultMinimumLength(defaultMinimumLength)
    {}

    String operator()(const std::vector<String>& args)
    {
        std::size_t minimumLength = args.size() > 0 ?
            boost::lexical_cast<std::size_t>(args[0]) :
            defaultMinimumLength;
        Char paddingChar = args.size() > 1 ?
            boost::lexical_cast<Char>(args[1]) :
            defaultPaddingChar;
        TextAlignment textAlignment = args.size() > 2 ?
            textAlignments().at(args[2]) :
            defaultTextAlignment;

        auto result = boost::lexical_cast<String>(value);
        if (result.size() >= minimumLength) {
            return result;
        }

        std::size_t padBefore = 0;
        std::size_t padAfter = 0;
        switch(textAlignment) {
        case TextAlignment::left:
            padAfter = minimumLength - result.size();
            break;
        case TextAlignment::right:
            padBefore = minimumLength - result.size();
            break;
        case TextAlignment::center:
            std::size_t difference = minimumLength - result.size();
            std::size_t halfDifference = difference / 2;
            padBefore = halfDifference;
            padAfter = halfDifference + difference % 2;
            break;
        }

        String paddedResult;
        paddedResult.reserve(minimumLength);

        for (std::size_t i = 0; i < padBefore; ++i) paddedResult += paddingChar;
        paddedResult += result;
        for (std::size_t i = 0; i < padAfter; ++i) paddedResult += paddingChar;

        return paddedResult;
    }

    GenericFormat(const GenericFormat&) = default;
    GenericFormat& operator=(const GenericFormat&) = default;
    GenericFormat(GenericFormat&&) = default;
    GenericFormat& operator=(GenericFormat&&) = default;

private:
    T value;
    TextAlignment defaultTextAlignment;
    Char defaultPaddingChar;
    std::size_t defaultMinimumLength;
};

template <typename T, typename Char = char>
GenericFormat<T, Char> genericFormat(T value,
        TextAlignment defaultTextAlignment = TextAlignment::right,
        Char defaultPaddingChar = ' ',
        std::size_t defaultMinimumLength = 0)
{
    return GenericFormat<T, Char>{value, defaultTextAlignment,
            defaultPaddingChar, defaultMinimumLength};
}

}


#endif /* INCLUDE_UTIL_STRINGFORMATTER_HPP */
