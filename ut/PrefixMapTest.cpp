#include <boost/test/unit_test.hpp>
#include "PrefixMapTree.hpp"
#include <boost/mpl/list.hpp>

using namespace util;

BOOST_AUTO_TEST_SUITE(PrefixMapTest)

using PrefixMapTypes = boost::mpl::list<BasicPrefixMapTree<char, int>>;

BOOST_AUTO_TEST_CASE_TEMPLATE(one_value, PrefixMap, PrefixMapTypes) {
    int value = 10;
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.emplace("value", value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("va"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("val"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("valu"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("value"), value);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("valuee"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(distinct_values, PrefixMap, PrefixMapTypes) {
    int value = 324;
    int something = 238;
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.emplace("value", value);
    prefixMapUnderTest.emplace("something", something);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(prefix_values, PrefixMap, PrefixMapTypes) {
    int value = 12;
    int vaaaa = 74;
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.emplace("value", value);
    prefixMapUnderTest.emplace("vaaaa", vaaaa);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("val"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("vaa"), vaaaa);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("v"), AmbiguousValue);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("va"), AmbiguousValue);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(duplicate_values, PrefixMap, PrefixMapTypes) {
    std::string key = "value";
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.emplace(key, 234);
    BOOST_CHECK_THROW(prefixMapUnderTest.emplace(key, 745), DuplicateValue);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(initializer_list_create, PrefixMap, PrefixMapTypes) {
    int value = 1;
    int something = 3;
    PrefixMap prefixMapUnderTest{{"value", value}, {"something", something}};
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(initializer_list_insert, PrefixMap, PrefixMapTypes) {
    int value = 34;
    int something = 54;
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.insert({{"value", value}, {"something", something}});
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(iterator_create, PrefixMap, PrefixMapTypes) {
    int value = 1123;
    int something = 56;
    std::vector<typename PrefixMap::value_type> v{{"value", value}, {"something", something}};
    PrefixMap prefixMapUnderTest(v.begin(), v.end());
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(iterator_insert, PrefixMap, PrefixMapTypes) {
    int value = 7;
    int something = 1;
    std::vector<typename PrefixMap::value_type> v{{"value", value}, {"something", something}};
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.insert(v.begin(), v.end());
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(iterate, PrefixMap, PrefixMapTypes) {
    std::vector<std::pair<std::string, int>> v{
        {"value1", 12}, {"value2", 42}, {"something", 333}, {"value3", 0},
        {"soooo", 235}};
    PrefixMap prefixMapUnderTest(v.begin(), v.end());
    std::sort(v.begin(), v.end());

    auto it = prefixMapUnderTest.cbegin();

    for (const auto& value: v) {
        BOOST_CHECK_EQUAL(it->first, value.first);
        BOOST_CHECK_EQUAL(it->second, value.second);
        ++it;
    }
    BOOST_CHECK(it == prefixMapUnderTest.cend());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(find_exact_value, PrefixMap, PrefixMapTypes) {
    std::vector<std::pair<std::string, int>> v{
            {"something", 333}, {"value1", 12}, {"value2", 42}, {"xyz", 0}
        };
    PrefixMap prefixMapUnderTest(v.begin(), v.end());

    auto it = prefixMapUnderTest.find("value1");

    BOOST_CHECK_EQUAL(it->first, "value1");
    ++it;
    BOOST_CHECK_EQUAL(it->first, "value2");
    ++it;
    BOOST_CHECK_EQUAL(it->first, "xyz");
    ++it;
    BOOST_CHECK(it == prefixMapUnderTest.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(find_prefix, PrefixMap, PrefixMapTypes) {
    std::vector<std::pair<std::string, int>> v{
            {"something", 333}, {"vaaaaa", 12}, {"value", 42}, {"xyz", 0}
        };
    PrefixMap prefixMapUnderTest(v.begin(), v.end());

    auto it = prefixMapUnderTest.find("val");

    BOOST_CHECK_EQUAL(it->first, "value");
    ++it;
    BOOST_CHECK_EQUAL(it->first, "xyz");
    ++it;
    BOOST_CHECK(it == prefixMapUnderTest.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(find_ambiguous_prefix, PrefixMap, PrefixMapTypes) {
    std::vector<std::pair<std::string, int>> v{
            {"something", 333}, {"value1", 12}, {"value2", 42}, {"xyz", 0}
        };
    PrefixMap prefixMapUnderTest(v.begin(), v.end());

    auto it = prefixMapUnderTest.find("va");

    BOOST_CHECK(it == prefixMapUnderTest.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(find_last_element, PrefixMap, PrefixMapTypes) {
    std::vector<std::pair<std::string, int>> v{
            {"something", 333}, {"value1", 12}, {"value2", 42}, {"xyz", 0}
        };
    PrefixMap prefixMapUnderTest(v.begin(), v.end());

    auto it = prefixMapUnderTest.find("xyz");

    BOOST_CHECK_EQUAL(it->first, "xyz");
    ++it;
    BOOST_CHECK(it == prefixMapUnderTest.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(find_not_found, PrefixMap, PrefixMapTypes) {
    std::vector<std::pair<std::string, int>> v{
            {"something", 333}, {"value1", 12}, {"value2", 42}, {"xyz", 0}
        };
    PrefixMap prefixMapUnderTest(v.begin(), v.end());

    auto it = prefixMapUnderTest.find("u");

    BOOST_CHECK(it == prefixMapUnderTest.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(iterate_empty, PrefixMap, PrefixMapTypes) {
    PrefixMap prefixMapUnderTest;

    auto it = prefixMapUnderTest.cbegin();
    BOOST_CHECK(it == prefixMapUnderTest.cend());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(iterate_for, PrefixMap, PrefixMapTypes)
{
    std::vector<std::pair<std::string, int>> v{
        {"value1", 12}, {"value2", 42}, {"something", 333}, {"value3", 0},
        {"soooo", 235}};
    PrefixMap prefixMapUnderTest(v.begin(), v.end());
    std::sort(v.begin(), v.end());

    auto it = v.cbegin();

    for (const auto& value: prefixMapUnderTest) {
        BOOST_CHECK_EQUAL(it->first, value.first);
        BOOST_CHECK_EQUAL(it->second, value.second);
        ++it;
    }
    BOOST_CHECK(it == v.cend());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(copy_whole_map, PrefixMap, PrefixMapTypes) {
    int value1 = 44;
    int value2 = 436;
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.emplace("value1", value1);
    PrefixMap copiedPrefixMap = prefixMapUnderTest;
    prefixMapUnderTest.emplace("value2", value2);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("value1"), value1);
    BOOST_CHECK_EQUAL(prefixMapUnderTest.at("value2"), value2);
    BOOST_CHECK_EQUAL(copiedPrefixMap.at("value1"), value1);
    BOOST_CHECK_THROW(copiedPrefixMap.at("value2"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(move_whole_map, PrefixMap, PrefixMapTypes) {
    int value = -721;
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.emplace("value", value);
    PrefixMap movedPrefixMap = std::move(prefixMapUnderTest);
    BOOST_CHECK_THROW(prefixMapUnderTest.at("value"), ValueNotFound);
    BOOST_CHECK_EQUAL(movedPrefixMap.at("value"), value);
}

struct TestValue {
    bool copied = false;
    bool moved = false;
    int value;

    explicit TestValue(int value):value(value) {}
    TestValue(const TestValue& other):
        copied(true), moved(other.moved), value(other.value)
    {}
    TestValue(TestValue&& other):
        copied(other.copied), moved(true), value(other.value)
    {}

    TestValue& operator=(const TestValue& other) {
        copied = true;
        moved = other.moved;
        value = other.value;
        return *this;
    }
    TestValue& operator=(TestValue&& other) {
        copied = other.copied;
        moved = true;
        value = other.value;
        return *this;
    }
};

using PrefixMapTestValueTypes = boost::mpl::list<BasicPrefixMapTree<char, TestValue>>;

BOOST_AUTO_TEST_CASE_TEMPLATE(insert_lvalue_should_copy, PrefixMap, PrefixMapTestValueTypes) {
    std::string key = "key";
    int value = 12;
    typename PrefixMap::value_type v{key, TestValue{value}};
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.insert(v);
    const auto& storedValue = prefixMapUnderTest.at(key);
    BOOST_CHECK(storedValue.copied);
    BOOST_CHECK_EQUAL(storedValue.value, value);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(insert_rvalue_should_not_copy, PrefixMap, PrefixMapTestValueTypes) {
    std::string key = "key";
    int value = 12;
    typename PrefixMap::value_type v{key, TestValue{value}};
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.insert(std::move(v));
    const auto& storedValue = prefixMapUnderTest.at(key);
    BOOST_CHECK(!storedValue.copied);
    BOOST_CHECK_EQUAL(storedValue.value, value);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(emplace_lvalue_should_copy, PrefixMap, PrefixMapTestValueTypes) {
    std::string key = "key";
    int value = 12;
    TestValue v{value};
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.emplace(key, v);
    const auto& storedValue = prefixMapUnderTest.at(key);
    BOOST_CHECK(storedValue.copied);
    BOOST_CHECK(!storedValue.moved);
    BOOST_CHECK_EQUAL(storedValue.value, value);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(emplace_rvalue_should_move, PrefixMap, PrefixMapTestValueTypes) {
    std::string key = "key";
    int value = 12;
    TestValue v{value};
    PrefixMap prefixMapUnderTest;
    prefixMapUnderTest.emplace(key, std::move(v));
    const auto& storedValue = prefixMapUnderTest.at(key);
    BOOST_CHECK(!storedValue.copied);
    BOOST_CHECK(storedValue.moved);
    BOOST_CHECK_EQUAL(storedValue.value, value);
}


BOOST_AUTO_TEST_SUITE_END()

