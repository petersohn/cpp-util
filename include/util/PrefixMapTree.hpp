#ifndef PREFIXMAPTREE_HPP
#define PREFIXMAPTREE_HPP

#include "PrefixMapException.hpp"
#include <boost/variant.hpp>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <memory>
#include <assert.h>

namespace util {

// TODO make it more std container compliant
template <typename Char, typename Value>
class BasicPrefixMapTree {
public:

	using string = std::basic_string<Char>;
	using value_type = std::pair<const string, Value>;

private:

	struct Node {
		std::unique_ptr<value_type> value;
		using MapType = std::map<Char, Node>;
		MapType children;

		Node() = default;
		Node(const Node& other): children(other.children)
		{
			if (other.value) {
				value.reset(new value_type(*other.value));
			}
		}
		Node& operator=(const Node& other)
		{
			value.reset(other.value ?  new value_type(*other.value) : nullptr);
			children = other.children;
		}
		Node(Node&&) = default;
		Node& operator=(Node&&) = default;
	};

	struct NotFound {};
	struct Ambiguous {};

	using FindNodeResult = boost::variant<NotFound, Ambiguous, const Node&>;

	struct AtVisitor {
		using result_type = const Node&;

		result_type operator()(NotFound) const { throw ValueNotFound{}; }
		result_type operator()(Ambiguous) const { throw AmbiguousValue{}; }
		result_type operator()(const Node& node) const { return node; }
	};

public:

	friend class const_iterator;
	class const_iterator {
		friend class BasicPrefixMapTree;
	public:

		using iterator_category = std::forward_iterator_tag;
		using value_type = BasicPrefixMapTree::value_type;
		using pointer = value_type*;
		using reference = value_type&;
		using difference_type = int;

		const_iterator() = default;
		const_iterator(const const_iterator&) = default;
		const_iterator(const_iterator&&) = default;

		const_iterator& operator=(const const_iterator&) = default;
		const_iterator& operator=(const_iterator&&) = default;

		bool operator==(const const_iterator& other) {
			return stack == other.stack;
		}
		bool operator!=(const const_iterator& other) {
			return !(*this == other);
		}

		const value_type& operator*() {
			return *stack.back().first->second.value;
		}
		const value_type* operator->() {
			return stack.back().first->second.value.get();
		}

		const_iterator& operator++() {
			increment();
			findNextValue();
			return *this;
		}
		const_iterator operator++(int) {
			auto old = *this;
			++*this;
			return old;
		}

	private:

		using StackIterator = typename Node::MapType::const_iterator;
		using Stack = std::vector<std::pair<StackIterator, StackIterator>>;

		Stack stack;

		explicit const_iterator(const Node& node) {
			stack.push_back({node.children.begin(), node.children.end()});
			findNextValue();
		}

		explicit const_iterator(Stack stack):
			stack(std::move(stack))
		{
			findNextValue();
		}

		void increment() {
			if (!stack.empty()) {
				assert(stack.back().first != stack.back().second);
				++stack.back().first;
			}
		}

		void findNextValue() {
			while (!stack.empty()) {
				auto& top = stack.back();
				auto& it = top.first;
				const auto& end = top.second;

				if (it == end) {
					stack.pop_back();
					increment();
				} else {
					const auto& node = it->second;

					if (!node.children.empty()) {
						assert(!node.value);
						stack.push_back({node.children.begin(), node.children.end()});
					}

					if (node.value) {
						return;
					}
				}
			}
		}

	};

	class iterator {

		friend class BasicPrefixMapTree;

	public:

		using iterator_category = std::forward_iterator_tag;
		using value_type = BasicPrefixMapTree::value_type;
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
			return const_cast<value_type&>(*it);
		}
		const value_type* operator->() {
			return &const_cast<value_type&>(*it);
		}

		iterator& operator++() {
			++it;
			return *this;
		}
		iterator operator++(int) {
			auto old = *this;
			++*this;
			return old;
		}

	private:
		const_iterator it;

		explicit iterator(const_iterator it):
			it(std::move(it))
		{}
	};

	BasicPrefixMapTree() = default;
	BasicPrefixMapTree(const BasicPrefixMapTree&) = default;
	BasicPrefixMapTree& operator=(const BasicPrefixMapTree&) = default;
	BasicPrefixMapTree(BasicPrefixMapTree&&) = default;
	BasicPrefixMapTree& operator=(BasicPrefixMapTree&&) = default;

	BasicPrefixMapTree(std::initializer_list<value_type> values) {
		insert(values);
	}

	template <typename InputIterator>
	BasicPrefixMapTree(InputIterator begin, InputIterator end) {
		insert(begin, end);
	}

	template <typename... Args>
	void emplace(Args&&... args) {
		std::unique_ptr<value_type> value{new value_type{std::forward<Args>(args)...}};
		insertNode(value->first).value = std::move(value);
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
		auto result = findNode(key);
		return boost::apply_visitor(AtVisitor{}, result).value->second;
	}
	Value& at(const string& key) {
		return const_cast<Value&>(const_cast<const BasicPrefixMapTree*>(this)->at(key));
	}

	const_iterator find(const string& key) const {
		typename const_iterator::Stack stack;
		auto result = findNode(key, &stack);
		if (boost::get<const Node&>(&result)) {
			return const_iterator{stack};
		} else {
			return end();
		}
	}
	iterator find(const string& key) {
		return iterator{const_cast<const BasicPrefixMapTree*>(this)->find(key)};
	}

	const_iterator cbegin() const {
		return const_iterator{rootNode};
	}
	const_iterator cend() const {
		return const_iterator{};
	}
	iterator begin() {
		return iterator{const_iterator{rootNode}};
	}
	iterator end() {
		return iterator{};
	}
	const_iterator begin() const {
		return const_iterator{rootNode};
	}
	const_iterator end() const {
		return const_iterator{};
	}


private:
	Node rootNode;

	struct NodePosition {
		const Node* node;
		std::size_t depth = 0;

		NodePosition(const Node& node):node(&node) {}
	};

	NodePosition findBestNode(const string& key,
			typename const_iterator::Stack* stack = nullptr) const {
		NodePosition result{rootNode};

		for (const auto& character: key) {
			assert(result.node);
			auto it = result.node->children.find(character);
			if (it == result.node->children.end()) {
				return result;
			}

			if (stack) {
				stack->emplace_back(it, result.node->children.end());
			}

			result.node = &it->second;
			++result.depth;
		}

		return result;
	}

	Node& insertNode(const string& key) {
		auto nodePosition = findBestNode(key);
		assert(nodePosition.node);

		if (nodePosition.node->value) {
			throw DuplicateValue{};
		}

		Node* node = const_cast<Node*>(nodePosition.node);
		for (std::size_t i = nodePosition.depth; i < key.size(); ++i) {
			node = &node->children[key[i]];
			assert(node);
			assert(!node->value);
			assert(node->children.empty());
		}

		return *node;
	}

	FindNodeResult findNode(const string& key,
			typename const_iterator::Stack* stack = nullptr) const {
		auto nodePosition = findBestNode(key, stack);

		if (nodePosition.depth != key.size()) {
			return NotFound{};
		}

		const Node* result = nullptr;
		std::deque<const Node*> queue{nodePosition.node};

		while (!queue.empty()) {
			auto node = queue.front();
			if (node->value) {

				if (result) {
					return Ambiguous{};
				}
				result = node;

			}
			queue.pop_front();
			for (const auto& child: node->children) {
				queue.push_back(&child.second);
			}
		}

		assert(result);
		return *result;
	}
};

} // namespace util


#endif /* PREFIXMAPTREE_HPP */
