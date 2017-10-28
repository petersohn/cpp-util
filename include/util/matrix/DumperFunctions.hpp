#ifndef UTIL_DUMPERFUNCTIONS_HPP
#define UTIL_DUMPERFUNCTIONS_HPP

#include "Matrix.hpp"
#include <iostream>
#include <string>

namespace util {
namespace matrix {

struct ToString {
    template<typename T>
    std::string operator()(T&& t) const {
        using std::to_string;
        return to_string(std::forward<T>(t));
    }

    std::string operator()(char c) const {
        return std::string{c};
    }

    const std::string& operator()(const std::string& s) const {
        return s;
    }
};

template<typename T, typename Converter = ToString>
void dumpMatrix(std::ostream& file, const Matrix<T>& table,
        const std::string& title = "", int indent = 0,
        const Converter& converter = Converter{}) {
    std::string indentString(indent, ' ');
    if (!title.empty()) {
        file << indentString << title << std::endl;
    }
    Matrix<std::string> txts(table.width(), table.height());
    size_t maxlen = 0;
    for (Point  p: matrixRange(table)) {
        txts[p] = converter(table[p]);
        maxlen = std::max(maxlen, txts[p].size());
    }
    // leave a space between characters
    ++maxlen;
    Point p;
    for (p.y = 0; p.y < static_cast<int>(table.height()); p.y++) {
        file << indentString;
        for (p.x = 0; p.x < static_cast<int>(table.width()); p.x++) {
            file.width(maxlen);
            file << txts[p];
            file.width(0);
        }
        file << "\n";
    }
    file << "\n";
}

} // namespace matrix
} // namespace util

#endif // UTIL_DUMPERFUNCTIONS_HPP
