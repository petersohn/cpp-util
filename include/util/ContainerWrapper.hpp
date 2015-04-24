#ifndef CPP_UTIL_INCLUDE_UTIL_CONTAINERWRAPPER_HPP
#define CPP_UTIL_INCLUDE_UTIL_CONTAINERWRAPPER_HPP

#include <ostream>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/lexical_cast.hpp>

namespace util {

template <typename Container>
struct ContainerWrapper {
    const Container& value;
    explicit ContainerWrapper(const Container& value): value(value) {}
};

template <typename Container>
std::ostream& operator<<(std::ostream& os,
        const ContainerWrapper<Container>& containerWrapper)
{
    os << "{";
    os << boost::algorithm::join(containerWrapper.value |
            boost::adaptors::transformed(boost::lexical_cast<std::string,
                typename Container::value_type>), ", ");
    return os << "}";
}

template <typename Container>
bool operator==(const ContainerWrapper<Container>& lhs,
        const ContainerWrapper<Container>& rhs)
{
    return lhs.value == rhs.value;
}

template <typename Container>
bool operator!=(const ContainerWrapper<Container>& lhs,
        const ContainerWrapper<Container>& rhs)
{
    return lhs.value != rhs.value;
}

template <typename Container>
ContainerWrapper<Container> wrap(const Container& container)
{
    return ContainerWrapper<Container>{container};
}

}


#endif /* CPP_UTIL_INCLUDE_UTIL_CONTAINERWRAPPER_HPP */
