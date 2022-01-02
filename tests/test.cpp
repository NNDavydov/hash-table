// Copyright Davydov Nikolay 2021

#include <gtest/gtest.h>
#include <string>

#include "hash_table.hpp"

TEST(HashTable, Constructor) {
    HashTable<int, int> hash_table1;
    HashTable<std::string, int> hash_table2;
    HashTable<int, std::string> hash_table3;

    EXPECT_EQ(hash_table1.capacity(), 500);
    EXPECT_EQ(hash_table1.size(), 0);
    EXPECT_EQ(hash_table1.used_elements(), 0);

    EXPECT_EQ(hash_table2.capacity(), 500);
    EXPECT_EQ(hash_table2.size(), 0);
    EXPECT_EQ(hash_table2.used_elements(), 0);

    EXPECT_EQ(hash_table3.capacity(), 500);
    EXPECT_EQ(hash_table3.size(), 0);
    EXPECT_EQ(hash_table3.used_elements(), 0);
}

TEST(HashTable, Add) {
    HashTable<int, std::string> hash_table;
    EXPECT_NO_THROW(hash_table.Add(3, "Hello"));

    EXPECT_EQ(hash_table.size(), 1);
    EXPECT_EQ(hash_table.used_elements(), 1);
    EXPECT_EQ(hash_table.Get(3), "Hello");

    EXPECT_NO_THROW(hash_table.Add(4, "World"));

    EXPECT_EQ(hash_table.size(), 2);
    EXPECT_EQ(hash_table.used_elements(), 2);
    EXPECT_EQ(hash_table.Get(4), "World");

    EXPECT_NO_THROW(hash_table.Add(3, "Haha"));

    EXPECT_EQ(hash_table.size(), 3);
    EXPECT_EQ(hash_table.used_elements(), 3);
    EXPECT_EQ(hash_table.Get(3), "Hello");
}

TEST(HashTable, Delete) {
    HashTable<int, std::string> hash_table;
    EXPECT_NO_THROW(hash_table.Add(3, "Hello"));
    EXPECT_NO_THROW(hash_table.Add(4, "World"));
    EXPECT_NO_THROW(hash_table.Add(3, "Haha"));

    EXPECT_EQ(hash_table.size(), 3);
    EXPECT_EQ(hash_table.used_elements(), 3);
    EXPECT_EQ(hash_table.Get(3), "Hello");

    EXPECT_NO_THROW(hash_table.Delete(3));

    EXPECT_EQ(hash_table.size(), 3);
    EXPECT_EQ(hash_table.used_elements(), 2);
    EXPECT_EQ(hash_table.Get(3), "Haha");

    EXPECT_NO_THROW(hash_table.Delete(4));
    EXPECT_EQ(hash_table.size(), 3);
    EXPECT_EQ(hash_table.used_elements(), 1);
    EXPECT_FALSE(hash_table.Find(4));

    EXPECT_NO_THROW(hash_table.Delete(3));
    EXPECT_EQ(hash_table.size(), 3);
    EXPECT_EQ(hash_table.used_elements(), 0);
    EXPECT_FALSE(hash_table.Find(3));

    EXPECT_NO_THROW(hash_table.Add(4, "New World"));
    EXPECT_EQ(hash_table.size(), 3);
    EXPECT_EQ(hash_table.used_elements(), 1);
    EXPECT_EQ(hash_table.Get(4), "New World");
}

TEST(HashTable, Find) {
    HashTable<int, std::string> hash_table;
    EXPECT_NO_THROW(hash_table.Add(3, "Hello"));
    EXPECT_NO_THROW(hash_table.Add(4, "World"));
    EXPECT_NO_THROW(hash_table.Add(3, "Haha"));
    EXPECT_NO_THROW(hash_table.Add(5, "Hi"));
    EXPECT_NO_THROW(hash_table.Add(8, "Yes"));
    EXPECT_NO_THROW(hash_table.Add(9, "No"));

    EXPECT_TRUE(hash_table.Find(3));
    EXPECT_EQ(hash_table.Get(3), "Hello");
    EXPECT_TRUE(hash_table.Find(4));
    EXPECT_EQ(hash_table.Get(4), "World");
    EXPECT_TRUE(hash_table.Find(5));
    EXPECT_EQ(hash_table.Get(5), "Hi");
    EXPECT_TRUE(hash_table.Find(8));
    EXPECT_EQ(hash_table.Get(8), "Yes");
    EXPECT_TRUE(hash_table.Find(9));
    EXPECT_EQ(hash_table.Get(9), "No");

    EXPECT_NO_THROW(hash_table.Delete(4));
    EXPECT_FALSE(hash_table.Find(4));
    EXPECT_NO_THROW(hash_table.Add(4, "New World"));
    EXPECT_TRUE(hash_table.Find(4));
    EXPECT_EQ(hash_table.Get(4), "New World");

    EXPECT_NO_THROW(hash_table.Delete(3));
    EXPECT_TRUE(hash_table.Find(3));
    EXPECT_EQ(hash_table.Get(3), "Haha");
    EXPECT_NO_THROW(hash_table.Delete(3));
    EXPECT_FALSE(hash_table.Find(3));
}

