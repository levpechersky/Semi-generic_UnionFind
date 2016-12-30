/*
 * UnionFind_test.cpp
 *
 *  Created on: 2016-12-27
 *      Author: Lev
 */
#include <gtest\gtest.h>
#include <string>
#include <algorithm>
#include <vector>
#include "UnionFind.hpp"

class OrdinalToCardinal {
public:
	int operator()(int id) {
		return id - 1; //maps {1,2,...n} to {0,1,...n-1}
	}
};
class Sum {
public:
	int operator()(int lhs, int rhs) {
		return lhs + rhs;
	}
};

TEST(UnionFind, simple_test) {
	const int size = 7;
	UnionFind<int, OrdinalToCardinal, Sum> uf(size);
	for (int i = 1; i <= size; i++) {
		uf.Makeset(i, i);
	}
	ASSERT_NO_THROW(uf.Union(uf.Find(1), uf.Find(3)));
	ASSERT_NO_THROW(uf.Union(uf.Find(2), uf.Find(7)));
	ASSERT_NO_THROW(uf.Union(uf.Find(2), uf.Find(4)));
	ASSERT_NO_THROW(uf.Union(uf.Find(6), uf.Find(4)));
	// Sets: {1,3} {2,4,6,7} {5}
	ASSERT_EQ(uf.Find(1), uf.Find(3));
	ASSERT_EQ(uf.Find(2), uf.Find(4));
	ASSERT_EQ(uf.Find(2), uf.Find(7));
	ASSERT_EQ(uf.Find(2), uf.Find(6));
	ASSERT_NE(uf.Find(2), uf.Find(1));
	ASSERT_NE(uf.Find(5), uf.Find(3));
	ASSERT_NE(uf.Find(2), uf.Find(5));
	ASSERT_NE(uf.Find(7), uf.Find(3));

	ASSERT_EQ(uf.Size(uf.Find(2)), 4);
	ASSERT_EQ(uf.Size(uf.Find(3)), 2);
	ASSERT_EQ(uf.Size(uf.Find(5)), 1);

	ASSERT_EQ(uf.Data(uf.Find(2)), 19);
	ASSERT_EQ(uf.Data(uf.Find(3)), 4);
	ASSERT_EQ(uf.Data(uf.Find(5)), 5);
}

class Identity {
public:
	int operator()(int id) {
		return id;
	}
};

class MaxString {
public:
	std::string operator()(std::string lhs, std::string rhs) {
		return std::max(lhs, rhs);
	}
};

TEST(UnionFind, strings_test) {
	std::string strings[] = {
			std::string("Daddy he once told me"), //0
			std::string("Son, you be hard workin' man"), //1
			std::string("And momma she once told me"), //2
			std::string("Son, you do the best you can"), //3
			std::string("Then one day I meet a man"), //4
			std::string("He came to me and said"), //5
			std::string("Hard work good and hard work fine"), //6
			std::string("But first take care of head") //7
			};
	const int size = 8;
	UnionFind<std::string, Identity, MaxString> uf(
			size);
	for (int i = 0; i < size; i++) {
		uf.Makeset(i, strings[i]);
	}
	ASSERT_NO_THROW(uf.Union(uf.Find(0), uf.Find(2)));
	ASSERT_NO_THROW(uf.Union(uf.Find(5), uf.Find(7)));
	ASSERT_NO_THROW(uf.Union(uf.Find(6), uf.Find(5)));
	ASSERT_NO_THROW(uf.Union(uf.Find(6), uf.Find(4)));
	// Sets: {0,2} {3} {4,5,6,7} {1}
	ASSERT_EQ(uf.Find(0), uf.Find(2));
	ASSERT_EQ(uf.Find(2), uf.Find(0));
	ASSERT_EQ(uf.Find(1), uf.Find(1));
	ASSERT_EQ(uf.Find(4), uf.Find(6));
	ASSERT_EQ(uf.Find(7), uf.Find(6));
	ASSERT_EQ(uf.Find(5), uf.Find(7));
	ASSERT_EQ(uf.Find(6), uf.Find(5));
	ASSERT_NE(uf.Find(2), uf.Find(1));
	ASSERT_NE(uf.Find(5), uf.Find(3));
	ASSERT_NE(uf.Find(2), uf.Find(5));
	ASSERT_NE(uf.Find(7), uf.Find(3));
	ASSERT_NE(uf.Find(1), uf.Find(3));

	ASSERT_EQ(uf.Size(uf.Find(2)), 2);
	ASSERT_EQ(uf.Size(uf.Find(3)), 1);
	ASSERT_EQ(uf.Size(uf.Find(4)), 4);
	ASSERT_EQ(uf.Size(uf.Find(6)), 4);
	ASSERT_EQ(uf.Size(uf.Find(1)), 1);

	ASSERT_EQ(uf.Data(uf.Find(2)), "Daddy he once told me");
	ASSERT_EQ(uf.Data(uf.Find(3)), "Son, you do the best you can");
	ASSERT_EQ(uf.Data(uf.Find(4)), "Then one day I meet a man");
	ASSERT_EQ(uf.Data(uf.Find(6)), "Then one day I meet a man");
	ASSERT_EQ(uf.Data(uf.Find(1)), "Son, you be hard workin' man");
}
