#ifndef INCLUDE_UTIL_CONFIG_HPP
#define INCLUDE_UTIL_CONFIG_HPP

#if defined(BOOST_PP_VARIADICS)
#if !BOOST_PP_VARIADICS
#error config.hpp should be included before any BOOST_PP headers
#endif
#else
#define BOOST_PP_VARIADICS 1
#endif

#endif /* INCLUDE_UTIL_CONFIG_HPP */
