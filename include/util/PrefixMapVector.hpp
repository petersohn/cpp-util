#ifndef PREFIXMAPVECTOR_HPP
#define PREFIXMAPVECTOR_HPP

#include "PrefixMapException.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>

namespace util {

template <typename Char, typename Value>
class BasicPrefixMapVector {
public:

	using string = std::basic_string<Char>;
	using value_type = std::pair<const string, Value>;

private:

	using StoredValue = std::pair<string, Value>;
	using Values = std::vector<StoredValue>;

	struct ValueCompare {
		bool operator()(const StoredValue& lhs, const string& rhs) const
		{
			return lhs.first < rhs;
		}
		bool operator()(const string& lhs, const StoredValue& rhs) const
		{
			return lhs < rhs.first;
		}
	};

public:

	friend class iterator;
	class iterator {
		friend class BasicPrefixMapVector;
	public:

		using iterator_category = std::forward_iterator_tag;
		using value_type = BasicPrefixMapVector::value_type;
		using pointer = value_type*;
		using reference = value_type&;
		using difference_type = int;

		iterator() = default;
		iterator(const iterator&) = default;
		iterator(iterator&&) = default;

		iterator& operator=(const iterator&) = default;
		iterator& operator=(iterator&&) = default;

		bool operator==(const iterator& other) {
			return it == other.it;
		}
		bool operator!=(const iterator& other) {
			return !(*this == other);
		}

		value_type& operator*() {
			return *it;
		}
		value_type* operator->() {
			return nullptr;
		}

		iterator& operator++() {
			++it;
			return *this;
		}
		iterator operator++(int) {
			return iterator{it++};
		}

	private:

		typename Values::iterator it;

		explicit iterator(typename Values::iterator it):
			it(std::move(it))
		{}

	};

	friend class const_iterator;
	class const_iterator {
		friend class BasicPrefixMapVector;
	public:

		using iterator_category = std::forward_iterator_tag;
		using value_type = BasicPrefixMapVector::value_type;
		using pointer = value_type*;
		using reference = value_type&;
		using difference_type = int;

		const_iterator() = default;
		const_iterator(const const_iterator&) = default;
		const_iterator(const_iterator&&) = default;
		const_iterator(const iterator& other):it(other.it) {}

		const_iterator& operator=(const const_iterator&) = default;
		const_iterator& operator=(const_iterator&&) = default;

		bool operator==(const const_iterator& other) {
			return it == other.it;
		}
		bool operator!=(const const_iterator& other) {
			return !(*this == other);
		}

		const value_type& operator*() {
			return *it;
		}
		const value_type* operator->() {
			return nullptr;
		}

		const_iterator& operator++() {
			++it;
			return *this;
		}
		const_iterator operator++(int) {
			return const_iterator{it++};
		}

	private:

		typename Values::const_iterator it;

		explicit const_iterator(typename Values::const_iterator it):
			it(std::move(it))
		{}

	};


	BasicPrefixMapVector() = default;
	BasicPrefixMapVector(const BasicPrefixMapVector&) = default;
	BasicPrefixMapVector& operator=(const BasicPrefixMapVector&) = default;
	BasicPrefixMapVector(BasicPrefixMapVector&&) = default;
	BasicPrefixMapVector& operator=(BasicPrefixMapVector&&) = default;

	template <typename InputIterator>
	BasicPrefixMapVector(InputIterator begin, InputIterator end):
		values(begin, end)
	{
		std::sort(values.begin(), values.end());
	}

	BasicPrefixMapVector(std::initializer_list<value_type> values):
		values(values.begin(), values.end())
	{
	}

	template <typename... Args>
	void emplace(Args&&... args) {
		value_type value{std::forward<Args>(args)...};
		auto it = std::upper_bound(values.begin(), values.end(), value.first,
				ValueCompare{});

		if (it->first == value.first) {
			throw DuplicateValue{};
		}

		values.insert(it, std::move(value));
	}

	void insert(const value_type& value) {
		emplace(value);
	}

	template <typename P, typename = typename std::enable_if<
		std::is_constructible<value_type, P&&>::value>::type>
	void insert(P&& value) {
		emplace(std::forward<P>(value));
	}

	template <typename InputIterator>
	void insert(InputIterator begin, InputIterator end) {
		for (; begin != end; ++begin) {
			insert(*begin);
		}
	}

	void insert(std::initializer_list<value_type> values) {
		insert(values.begin(), values.end());
	}

	const Value& at(const string& key) const {
		auto it = std::lower_bound(values.begin(), values.end(), key,
				ValueCompare{});

		if (it == values.end() ||
				!boost::algorithm::starts_with(it->first, key)) {
			throw ValueNotFound{};
		}

		auto next = std::next(it);
		if (next != values.end() &&
				boost::algorithm::starts_with(next->first, key)) {
			throw AmbiguousValue{};
		}

		return it->second;
	}
	Value& at(const string& key) {
		return const_cast<Value&>(const_cast<const BasicPrefixMapVector*>(this)->at(key));
	}

	iterator find(const string& key) {
		auto it = std::lower_bound(values.begin(), values.end(), key,
				ValueCompare{});

		if (it == values.end() ||
				!boost::algorithm::starts_with(it->first, key)) {
			return end();
		}

		auto next = std::next(it);
		if (next != values.end() &&
				boost::algorithm::starts_with(next->first, key)) {
			return end();
		}

		return iterator{it};
	}
	const_iterator find(const string& key) const {
		return const_cast<BasicPrefixMapVector*>(this)->find(key);
	}

	const_iterator cbegin() const {
		return const_iterator{values.cbegin()};
	}
	const_iterator cend() const {
		return const_iterator{values.cend()};
	}
	iterator begin() {
		return iterator{values.begin()};
	}
	iterator end() {
		return iterator{values.end()};
	}
	const_iterator begin() const {
		return const_iterator{values.begin()};
	}
	const_iterator end() const {
		return const_iterator{values.end()};
	}

private:

	Values values;

};


} // namespace util

#endif /* PREFIXMAPVECTOR_HPP */
