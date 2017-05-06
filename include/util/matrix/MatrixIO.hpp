#include "Matrix.hpp"
#include "DumperFunctions.hpp"

#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/max_element.hpp>

#include <algorithm>
#include <functional>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace detail {

template<typename T>
void readValue(std::istream& s, T& t) {
    s >> t;
}

inline
void readValue(std::istream& s, char& c) {
    s.get(c);
}

}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    dumpMatrix(os, matrix, " ");
    return os;
}

template<typename T>
Matrix<T> loadMatrix(std::istream& is, const T& defaultValue = T{},
        std::size_t width = 0, std::size_t height = 0, bool flip = false,
        char delimiter = '\n') {
    std::vector<std::string> lines;
    std::string line;
    while (is.good() && (height == 0 || lines.size() != height)) {
        std::getline(is, line, delimiter);
        std::cerr << "--> " << line << "\n";
        if (line.empty()) {
            break;
        }
        lines.push_back(line);
    }
    if (flip) {
        std::reverse(lines.begin(), lines.end());
    }

    height = lines.size();
    if (width == 0) {
        std::function<std::size_t(const std::string&)> sizeGetter =
                [](const std::string& s) { return s.size(); };
        width = height != 0 ?
                *boost::max_element(lines | boost::adaptors::transformed(
                        sizeGetter)) : 0;
    }
    std::cerr << width << ":" << height << "\n";
    Matrix<T> result{width, height, defaultValue};
    Point p;
    for (p.y = 0; p.y < static_cast<int>(height); ++p.y) {
        std::istringstream ss{lines[p.y]};
        for (p.x = 0; p.x < static_cast<int>(width) && ss.good(); ++p.x) {
            detail::readValue(ss, result[p]);
        }
    }

    return result;
}

template<typename T>
std::istream& operator>>(std::istream& is, Matrix<T>& matrix) {
    matrix = loadMatrix<T>(is);
    return is;
}


