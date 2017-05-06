#ifndef UTIL_DUMPERFUNCTIONS_HPP
#define UTIL_DUMPERFUNCTIONS_HPP

#include "Matrix.hpp"
#include <iostream>
#include <string>

struct ToString {
    template<typename T>
    std::string operator()(T&& t) const {
        using std::to_string;
        return to_string(std::forward<T>(t));
    }

    std::string operator()(char c) const {
        return std::string{c};
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
            if (p.x % 10 == 0 || p.y % 10 == 0) {
                file << "\e[0;31m";
            } else if (p.x % 5 == 0 || p.y % 5 == 0) {
                file << "\e[0;33m";
            }
            file.width(maxlen);
            file << txts[p];
            file.width(0);
            if (p.x % 5 == 0 || p.y % 5 == 0) {
                file << "\e[0m";
            }
        }
        file << "\n";
    }
    file << "\n";
}


#endif // UTIL_DUMPERFUNCTIONS_HPP
