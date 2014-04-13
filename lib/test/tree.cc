#include <iostream>
#include <algorithm>

#include <gtest/gtest.h>

#include <panopticon/tree.hh>

class tree : public ::testing::Test
{
protected:
	virtual void SetUp(void)
	{
		t1 = po::tree<std::string>("root");
		i = t1.insert(t1.root(),"one");
		j = t1.insert(t1.root(),"two");
		k = t1.insert(i,"three");
	}

	po::tree<std::string> t1;
	po::tree<std::string>::iterator i,j,k;
};

TEST_F(tree,add)
{
	ASSERT_EQ(*t1.root(), "root");
	ASSERT_EQ(*t1.begin(t1.root()), "one");
	ASSERT_EQ(*std::next(t1.begin(t1.root())), "two");
	ASSERT_EQ(std::next(t1.begin(t1.root()),2), t1.end(t1.root()));

	ASSERT_EQ(t1.begin(j), t1.end(j));
	ASSERT_EQ(t1.begin(k), t1.end(k));

	ASSERT_EQ(*t1.begin(i), "three");
	ASSERT_EQ(std::next(t1.begin(i)), t1.end(i));
}

TEST_F(tree,write)
{
	*i = "five";
	*j = "four";

	ASSERT_EQ(*t1.root(), "root");
	ASSERT_EQ(*t1.begin(t1.root()), "five");
	ASSERT_EQ(*std::next(t1.begin(t1.root())), "four");
	ASSERT_EQ(std::next(t1.begin(t1.root()),2), t1.end(t1.root()));

	ASSERT_EQ(t1.begin(j), t1.end(j));
	ASSERT_EQ(t1.begin(k), t1.end(k));

	ASSERT_EQ(*t1.begin(i), "three");
	ASSERT_EQ(std::next(t1.begin(i)), t1.end(i));
}

TEST_F(tree,remove_leaf)
{
	t1.remove(j);

	ASSERT_EQ(*t1.root(), "root");
	ASSERT_EQ(*t1.begin(t1.root()), "one");
	ASSERT_EQ(std::next(t1.begin(t1.root()),1), t1.end(t1.root()));

	ASSERT_EQ(*t1.begin(i), "three");
	ASSERT_EQ(std::next(t1.begin(i)), t1.end(i));
}

TEST_F(tree,remove_inner)
{
	t1.remove(i);

	ASSERT_EQ(*t1.root(), "root");
	ASSERT_EQ(*t1.begin(t1.root()), "two");
	ASSERT_EQ(std::next(t1.begin(t1.root()),1), t1.end(t1.root()));
}

TEST_F(tree,remove_root)
{
	t1.remove(t1.root());

	ASSERT_EQ(*t1.root(), "");
	ASSERT_EQ(t1.begin(t1.root()), t1.end(t1.root()));
}

TEST_F(tree,value_semantics)
{
	po::tree<std::string> t2("root");
	auto ii = t2.insert(t2.root(),"one");
	t2.insert(t2.root(),"two");
	t2.insert(ii,"three");

	ASSERT_TRUE(t1 == t2);
	ASSERT_TRUE(t2 == t1);

	po::tree<std::string> t3( t2 );

	ASSERT_TRUE(t1 == t3);
	ASSERT_TRUE(t3 == t1);
	ASSERT_TRUE(t2 == t3);
	ASSERT_TRUE(t3 == t2);
	ASSERT_TRUE(t1 == t2);
	ASSERT_TRUE(t2 == t1);

	po::tree<std::string> t4("root");
	auto iii = t4.insert(t4.root(),"five");
	t4.insert(t4.root(),"six");
	t4.insert(iii,"seven");

	ASSERT_TRUE(t1 != t4);
	ASSERT_TRUE(t2 != t4);
	ASSERT_TRUE(t3 != t4);
	ASSERT_TRUE(t4 != t1);
	ASSERT_TRUE(t4 != t2);
	ASSERT_TRUE(t4 != t3);

	t2 = t4;

	ASSERT_TRUE(t1 != t4);
	ASSERT_TRUE(t2 == t4);
	ASSERT_TRUE(t3 != t4);
	ASSERT_TRUE(t4 != t1);
	ASSERT_TRUE(t4 == t2);
	ASSERT_TRUE(t1 != t2);
	ASSERT_TRUE(t3 != t2);
	ASSERT_TRUE(t2 != t1);
	ASSERT_TRUE(t2 != t3);

	t4.insert(iii,"eight");
	ASSERT_TRUE(t1 != t4);
	ASSERT_TRUE(t2 != t4);
	ASSERT_TRUE(t3 != t4);
	ASSERT_TRUE(t4 != t1);
	ASSERT_TRUE(t4 != t2);
	ASSERT_TRUE(t4 != t3);
}