/*
 * UnionFind.hpp
 *
 *  Created on: 2016-12-27
 *      Author: Lev
 */
#ifndef UNIONFIND_HPP_
#define UNIONFIND_HPP_
#include <algorithm>
#include "O1Array.hpp"

typedef int Set;
static const Set NONE = -1;


/* Semi-generic Union/Find DS.
 *
 * Each element, stored in Union/Find should have some integer id and can be
 * provided with additional data of any type, associated with any non-empty set.
 *
 * On creation of Union/Find you must provide "world's size" - maximal number of
 * elements for Union/Find to hold.
 *
 * Template parameters:
 * T - additional data type
 * Mapping - hashing functor, getting an integer id and returning integer in
 *     [0, n) range.
 *     For each T of n T's in the world, Mapping must return distinct integer.
 *     Must be a class with () operator id->{0,1,2...n-1}
 * UnionFunctor - function, that defines what should be done to data on each
 * 		union.
 * 		Must be a class with () operator T*T->T, where first operand is data of
 * 		larger set.
 *
 */
template<typename T, typename Mapping, typename UnionFunctor>
class UnionFind {
	int world_size;
	O1Array<Set> parent;
	O1Array<int> size;
	O1Array<T*> data;

	class UnionFindException: public std::exception {
	public:
		explicit UnionFindException(const char* message) :
				msg_(message) {
		}
		explicit UnionFindException(const std::string& message) :
				msg_(message) {
		}
		virtual ~UnionFindException() throw () {
		}
		virtual const char* what() const throw () {
			return msg_.c_str();
		}
	protected:
		std::string msg_;
	};

	void check_bounds(int mapped_index) const {
		if (mapped_index < 0 || mapped_index >= world_size)
			throw UnionFindException(
					"Index out of bounds, check Mapping functor");
	}

	void check_set(Set set) const {
		if (parent[set] != NONE)
			throw UnionFindException(
					"Invalid set: received set element instead of set");
		if (size[set] == 0)
			throw UnionFindException("Invalid set: Set does not exist");
	}

	Set get_root(int mapped_index) {
		check_bounds(mapped_index);
		int root = mapped_index;
		while (parent[root] != NONE) {
			root = parent[root];
		}
		return root;
	}

	void collapse_paths(int mapped_index, Set set) {
		while (parent[mapped_index] != NONE) {
			parent[mapped_index] = set;
			mapped_index = parent[mapped_index];
		}
	}

public:
	UnionFind(int n) :
			world_size(n), parent(n, NONE), size(n, 0),
					data(n, NULL) {
	}

	Set Makeset(int i, const T& additional_data) {
		i = Mapping()(i);
		check_bounds(i);
		if (size[i] != 0)
			throw UnionFindException("Makeset: Item already in a set");
		size[i] = 1;
		data[i] = new T(additional_data);
		return i;
	}

	Set Find(int i) {
		i = Mapping()(i);
		check_bounds(i);
		if (size[i] == 0)
			return NONE;
		Set root = get_root(i);
		collapse_paths(i, root);
		return root;
	}

	Set Union(Set x, Set y) {
		check_set(x);
		check_set(y);
		Set smaller_set = x, bigger_set = y;
		if (smaller_set == bigger_set)
			return bigger_set;
		if (size[smaller_set] > size[bigger_set])
			std::swap(smaller_set, bigger_set);
		size[bigger_set] += size[smaller_set];
		*data[bigger_set] = UnionFunctor()(*data[bigger_set],
				*data[smaller_set]);
		parent[smaller_set] = bigger_set;
		return bigger_set;
	}

	int Size(Set set) {
		check_set(set);
		return size[set];
	}

	T Data(Set set) {
		check_set(set);
		return *data[set];
	}

	~UnionFind() {
		for (int i = 0; i < world_size; i++) {
			delete data[i];
		}
	}

};

#endif /* UNIONFIND_HPP_ */
