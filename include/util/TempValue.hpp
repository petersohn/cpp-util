#ifndef INCLUDE_UTIL_TEMPVALUE_HPP
#define INCLUDE_UTIL_TEMPVALUE_HPP

#include <utility>

namespace util {

template <class T>
class TempValue {
    T& variable;
    T oldValue;
public:
    TempValue(T& variable, T value):
        variable(variable), oldValue(variable)
    {
        variable = std::move(value);
    }
    ~TempValue()
    {
        variable = oldValue;
    }

    TempValue(const TempValue&) = delete;
    TempValue& operator=(const TempValue&) = delete;
    TempValue(TempValue&&) = delete;
    TempValue& operator=(TempValue&&) = delete;

};

}

#endif /* INCLUDE_UTIL_TEMPVALUE_HPP */
