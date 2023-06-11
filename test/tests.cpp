#include "element.h"
#include "fault-injection.h"
#include "set.h"
#include "test-utils.h"

#include <gtest/gtest.h>

namespace {

class correctness_test : public base_test {};

class exception_safety_test : public base_test {};

void magic([[maybe_unused]] element& c) {
  c = 42;
}

void magic(const element&) {}

} // namespace

TEST_F(correctness_test, single_element) {
  container c;
  c.insert(42);
}

TEST_F(correctness_test, insert) {
  container c;
  mass_insert(c, {1, 2, 3, 4});
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, copy_ctor) {
  container c;
  mass_insert(c, {1, 2, 3, 4});
  container c2 = c;
  expect_eq(c2, {1, 2, 3, 4});
}

TEST_F(correctness_test, copy_ctor_2) {
  container c;
  mass_insert(c, {3, 4, 2, 5, 1});
  container c2 = c;
  expect_eq(c2, {1, 2, 3, 4, 5});
}

TEST_F(correctness_test, copy_ctor_empty) {
  container c;
  container c2 = c;
  EXPECT_TRUE(c2.empty());
}

TEST_F(correctness_test, copy_assignment) {
  container c;
  mass_insert(c, {1, 2, 3, 4});
  container c2;
  mass_insert(c2, {5, 6, 7, 8});
  c2 = c;
  expect_eq(c2, {1, 2, 3, 4});
}

TEST_F(correctness_test, copy_assignment_self) {
  container c;
  mass_insert(c, {1, 2, 3, 4});
  c = c;
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, empty) {
  container c;
  EXPECT_EQ(c.begin(), c.end());
  EXPECT_TRUE(c.empty());
  std::pair<container::iterator, bool> p = c.insert(1);
  EXPECT_NE(c.begin(), c.end());
  EXPECT_FALSE(c.empty());
  c.erase(p.first);
  EXPECT_EQ(c.begin(), c.end());
  EXPECT_TRUE(c.empty());
}

TEST_F(correctness_test, reverse_iterators) {
  container c;
  mass_insert(c, {3, 1, 2, 4});
  expect_eq(reverse_view(c), {4, 3, 2, 1});

  EXPECT_EQ(4, *c.rbegin());
  EXPECT_EQ(3, *std::next(c.rbegin()));
  EXPECT_EQ(1, *std::prev(c.rend()));
}

TEST_F(correctness_test, iterator_conversions) {
  container c;
  container::const_iterator i1 = c.begin();
  container::iterator i2 = c.end();
  EXPECT_TRUE(i1 == i1);
  EXPECT_TRUE(i1 == i2);
  EXPECT_TRUE(i2 == i1);
  EXPECT_TRUE(i2 == i2);
  EXPECT_FALSE(i1 != i1);
  EXPECT_FALSE(i1 != i2);
  EXPECT_FALSE(i2 != i1);
  EXPECT_FALSE(i2 != i2);
}

TEST_F(correctness_test, iterators_postfix) {
  container s;
  mass_insert(s, {1, 2, 3});
  container::iterator i = s.begin();
  EXPECT_EQ(1, *i);
  container::iterator j = i++;
  EXPECT_EQ(2, *i);
  EXPECT_EQ(1, *j);
  j = i++;
  EXPECT_EQ(3, *i);
  EXPECT_EQ(2, *j);
  j = i++;
  EXPECT_EQ(s.end(), i);
  EXPECT_EQ(3, *j);
  j = i--;
  EXPECT_EQ(3, *i);
  EXPECT_EQ(s.end(), j);
}

TEST_F(correctness_test, iterators_decrement) {
  container s;
  mass_insert(s, {5, 3, 8, 1, 2, 6, 7, 10});
  container::iterator i = s.end();
  EXPECT_EQ(10, *--i);
  EXPECT_EQ(8, *--i);
  EXPECT_EQ(7, *--i);
  EXPECT_EQ(6, *--i);
  EXPECT_EQ(5, *--i);
  EXPECT_EQ(3, *--i);
  EXPECT_EQ(2, *--i);
  EXPECT_EQ(1, *--i);
  EXPECT_EQ(s.begin(), i);
}

TEST_F(correctness_test, iterators_decrement_2) {
  container s;
  mass_insert(s, {5, 2, 10, 9, 12, 7});
  container::iterator i = s.end();
  EXPECT_EQ(12, *--i);
  EXPECT_EQ(10, *--i);
  EXPECT_EQ(9, *--i);
  EXPECT_EQ(7, *--i);
  EXPECT_EQ(5, *--i);
  EXPECT_EQ(2, *--i);
  EXPECT_EQ(s.begin(), i);
}

TEST_F(correctness_test, iterator_default_ctor) {
  container::iterator i;
  container::const_iterator j;
  container s;
  mass_insert(s, {4, 1, 8, 6, 3, 2, 6});

  i = s.begin();
  j = s.begin();
  EXPECT_EQ(1, *i);
  EXPECT_EQ(1, *j);
}

TEST_F(correctness_test, iterator_decrement_end) {
  container s;
  container::const_iterator i = s.end();
  s.insert(42);
  --i;
  EXPECT_EQ(42, *i);
}

TEST_F(correctness_test, insert_simple) {
  container c;
  mass_insert(c, {8, 4, 2, 10, 5});
  expect_eq(c, {2, 4, 5, 8, 10});
}

TEST_F(correctness_test, insert_duplicates) {
  container c;
  mass_insert(c, {8, 4, 2, 4, 4, 4});
  expect_eq(c, {2, 4, 8});
}

TEST_F(correctness_test, reinsert) {
  container c;
  mass_insert(c, {6, 2, 3, 1, 9, 8});
  c.erase(c.find(6));
  c.insert(6);
  expect_eq(c, {1, 2, 3, 6, 8, 9});
}

TEST_F(correctness_test, erase_begin) {
  container c;
  mass_insert(c, {1, 2, 3, 4});
  c.erase(c.begin());
  expect_eq(c, {2, 3, 4});
}

TEST_F(correctness_test, erase_middle) {
  container c;
  mass_insert(c, {1, 2, 3, 4});
  c.erase(std::next(c.begin(), 2));
  expect_eq(c, {1, 2, 4});
}

TEST_F(correctness_test, erase_close_to_end) {
  container c;
  mass_insert(c, {6, 1, 4, 3, 2, 5});
  c.erase(std::next(c.begin(), 4));
  expect_eq(c, {1, 2, 3, 4, 6});
}

TEST_F(correctness_test, erase_end) {
  container c;
  mass_insert(c, {1, 2, 3, 4});
  c.erase(std::prev(c.end()));
  expect_eq(c, {1, 2, 3});
}

TEST_F(correctness_test, erase_root) {
  container c;
  mass_insert(c, {5, 3, 8, 1, 2});
  c.erase(c.find(5));
  expect_eq(c, {1, 2, 3, 8});
}

TEST_F(correctness_test, erase_1) {
  container c;
  mass_insert(c, {5, 3, 8, 1, 2, 7, 9, 10, 11, 12});
  c.erase(c.find(8));
  expect_eq(c, {1, 2, 3, 5, 7, 9, 10, 11, 12});
}

TEST_F(correctness_test, erase_2) {
  container c;
  mass_insert(c, {5, 3, 17, 15, 20, 19, 18});
  c.erase(c.find(17));
  expect_eq(c, {3, 5, 15, 18, 19, 20});
}

TEST_F(correctness_test, erase_3) {
  container c;
  mass_insert(c, {10, 5, 15, 14, 13});
  c.erase(c.find(15));
  expect_eq(c, {5, 10, 13, 14});
}

TEST_F(correctness_test, erase_4) {
  container c;
  mass_insert(c, {10, 5, 15, 3, 4});
  c.erase(c.find(5));
  expect_eq(c, {3, 4, 10, 15});
}

TEST_F(correctness_test, erase_5) {
  container c;
  mass_insert(c, {5, 2, 10, 6, 14, 7, 8});
  c.erase(c.find(5));
  expect_eq(c, {2, 6, 7, 8, 10, 14});
}

TEST_F(correctness_test, erase_6) {
  container c;
  mass_insert(c, {7, 3, 2, 6, 10, 9});
  c.erase(c.find(3));
  c.erase(c.find(6));
  c.erase(c.find(7));
  c.erase(c.find(10));
  c.erase(c.find(2));
  c.erase(c.find(9));
  EXPECT_TRUE(c.empty());
}

TEST_F(correctness_test, erase_7) {
  container c;
  mass_insert(c, {5, 3, 8});
  c.erase(c.find(5));
  expect_eq(c, {3, 8});
  EXPECT_FALSE(c.empty());
}

TEST_F(correctness_test, erase_8) {
  container c;
  mass_insert(c, {5, 3});
  c.erase(c.find(5));
  expect_eq(c, {3});
}

TEST_F(correctness_test, erase_iterator_invalidation) {
  container c;
  mass_insert(c, {8, 2, 6, 10, 3, 1, 9, 7});
  container::iterator i = c.find(8);
  container::iterator j = std::next(i);
  c.erase(i);
  EXPECT_EQ(9, *j);
}

TEST_F(correctness_test, erase_return_value) {
  container c;
  mass_insert(c, {7, 4, 10, 1, 8, 12});
  container::iterator i = c.find(7);
  i = c.erase(i);
  EXPECT_EQ(8, *i);
}

TEST_F(correctness_test, clear) {
  container c;
  mass_insert(c, {1, 2, 3, 4, 5, 6});
  EXPECT_FALSE(c.empty());
  c.clear();
  EXPECT_TRUE(c.empty());
  EXPECT_TRUE(c.empty());
  EXPECT_EQ(c.end(), c.begin());
}

TEST_F(correctness_test, iterator_deref_1) {
  container c;
  mass_insert(c, {1, 2, 3, 4, 5, 6});
  const container::iterator i = c.find(4);
  EXPECT_EQ(4, *i);
}

TEST_F(correctness_test, iterator_deref_2) {
  container c;
  mass_insert(c, {1, 2, 3, 4, 5, 6});
  container::iterator i = c.find(4);
  EXPECT_EQ(4, *i);
  magic(*i);
  expect_eq(c, {1, 2, 3, 4, 5, 6});
}

TEST_F(correctness_test, iterator_deref_3) {
  container c;
  mass_insert(c, {1, 2, 3, 4, 5, 6});
  const container::iterator i = c.find(4);
  magic(*i.operator->());
  expect_eq(c, {1, 2, 3, 4, 5, 6});
}

TEST_F(correctness_test, swap) {
  container c1, c2;
  mass_insert(c1, {1, 2, 3, 4});
  mass_insert(c2, {5, 6, 7, 8});
  swap(c1, c2);
  expect_eq(c1, {5, 6, 7, 8});
  expect_eq(c2, {1, 2, 3, 4});
}

TEST_F(correctness_test, swap_self) {
  container c1;
  mass_insert(c1, {1, 2, 3, 4});
  swap(c1, c1);
}

TEST_F(correctness_test, swap_empty) {
  container c1, c2;
  mass_insert(c1, {1, 2, 3, 4});
  swap(c1, c2);
  EXPECT_TRUE(c1.empty());
  expect_eq(c2, {1, 2, 3, 4});
  swap(c1, c2);
  expect_eq(c1, {1, 2, 3, 4});
  EXPECT_TRUE(c2.empty());
}

TEST_F(correctness_test, swap_empty_empty) {
  container c1, c2;
  swap(c1, c2);
}

TEST_F(correctness_test, swap_empty_self) {
  container c1;
  swap(c1, c1);
}

TEST_F(correctness_test, swap_iterator_validity) {
  container c1, c2;
  mass_insert(c1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  c2.insert(11);

  container::const_iterator c1_begin = c1.begin();
  // container::const_iterator c1_end = c1.end();

  container::const_iterator c2_begin = c2.begin();
  // container::const_iterator c2_end = c2.end();

  swap(c1, c2);

  EXPECT_EQ(1, *c1_begin++);
  EXPECT_EQ(2, *c1_begin++);
  EXPECT_EQ(3, *c1_begin++);
  std::advance(c1_begin, 7);
  // EXPECT_EQ(c1_end, c1_begin);

  EXPECT_EQ(11, *c2_begin++);
  // EXPECT_EQ(c2_end, c2_begin);
}

TEST_F(correctness_test, swap_1) {
  container c;
  mass_insert(c, {3, 2, 4, 1});
  container tmp(c);
  swap(c, tmp);
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, swap_iterators_1) {
  container c;
  mass_insert(c, {1, 2, 3});
  container c2;
  mass_insert(c2, {4, 5, 6});
  container::iterator i = c.find(2);
  container::iterator j = c2.find(5);

  {
    using std::swap;
    swap(i, j);
  }

  c2.erase(i);
  c.erase(j);
  expect_eq(c, {1, 3});
  expect_eq(c2, {4, 6});
}

TEST_F(correctness_test, find_in_empty) {
  container c;

  EXPECT_EQ(c.end(), c.find(0));
  EXPECT_EQ(c.end(), c.find(5));
  EXPECT_EQ(c.end(), c.find(42));
}

TEST_F(correctness_test, finds) {
  container c;

  mass_insert(c, {8, 3, 5, 4, 3, 1, 8, 8, 10, 9});

  EXPECT_EQ(c.end(), c.find(0));
  EXPECT_EQ(c.begin(), c.find(1));
  EXPECT_EQ(c.end(), c.find(2));
  EXPECT_EQ(std::next(c.begin(), 1), c.find(3));
  EXPECT_EQ(std::next(c.begin(), 2), c.find(4));
  EXPECT_EQ(std::next(c.begin(), 3), c.find(5));
  EXPECT_EQ(c.end(), c.find(6));
  EXPECT_EQ(c.end(), c.find(7));
  EXPECT_EQ(std::next(c.begin(), 4), c.find(8));
  EXPECT_EQ(std::next(c.begin(), 5), c.find(9));
  EXPECT_EQ(std::next(c.begin(), 6), c.find(10));
  EXPECT_EQ(c.end(), c.find(11));
}

TEST_F(correctness_test, lower_bound_empty) {
  container c;
  EXPECT_EQ(c.end(), c.lower_bound(5));
}

TEST_F(correctness_test, lower_bounds) {
  container c;

  mass_insert(c, {8, 3, 5, 4, 3, 1, 8, 8, 10, 9});

  EXPECT_EQ(c.begin(), c.lower_bound(0));
  EXPECT_EQ(c.begin(), c.lower_bound(1));
  EXPECT_EQ(std::next(c.begin()), c.lower_bound(2));
  EXPECT_EQ(std::next(c.begin()), c.lower_bound(3));
  EXPECT_EQ(std::next(c.begin(), 2), c.lower_bound(4));
  EXPECT_EQ(std::next(c.begin(), 3), c.lower_bound(5));
  EXPECT_EQ(std::next(c.begin(), 4), c.lower_bound(6));
  EXPECT_EQ(std::next(c.begin(), 4), c.lower_bound(7));
  EXPECT_EQ(std::next(c.begin(), 4), c.lower_bound(8));
  EXPECT_EQ(std::next(c.begin(), 5), c.lower_bound(9));
  EXPECT_EQ(std::next(c.begin(), 6), c.lower_bound(10));
  EXPECT_EQ(std::next(c.begin(), 7), c.lower_bound(11));
}

TEST_F(correctness_test, upper_bounds) {
  container c;

  mass_insert(c, {8, 3, 5, 4, 3, 1, 8, 8, 10, 9});

  EXPECT_EQ(c.begin(), c.upper_bound(0));
  EXPECT_EQ(std::next(c.begin()), c.upper_bound(1));
  EXPECT_EQ(std::next(c.begin()), c.upper_bound(2));
  EXPECT_EQ(std::next(c.begin(), 2), c.upper_bound(3));
  EXPECT_EQ(std::next(c.begin(), 3), c.upper_bound(4));
  EXPECT_EQ(std::next(c.begin(), 4), c.upper_bound(5));
  EXPECT_EQ(std::next(c.begin(), 4), c.upper_bound(6));
  EXPECT_EQ(std::next(c.begin(), 4), c.upper_bound(7));
  EXPECT_EQ(std::next(c.begin(), 5), c.upper_bound(8));
  EXPECT_EQ(std::next(c.begin(), 6), c.upper_bound(9));
  EXPECT_EQ(std::next(c.begin(), 7), c.upper_bound(10));
  EXPECT_EQ(std::next(c.begin(), 7), c.upper_bound(11));
}

TEST_F(correctness_test, upper_bound_empty) {
  container c;
  EXPECT_EQ(c.end(), c.upper_bound(5));
}

TEST_F(exception_safety_test, non_throwing_default_ctor) {
  faulty_run([] {
    try {
      container c;
    } catch (...) {
      fault_injection_disable dg;
      ADD_FAILURE() << "default constructor should not throw";
      throw;
    }
  });
}

TEST_F(exception_safety_test, copy_ctor) {
  faulty_run([] {
    container c;
    mass_insert(c, {3, 2, 4, 1});

    container c2 = c;
    expect_eq(c, {1, 2, 3, 4});
  });
}

TEST_F(exception_safety_test, non_throwing_clear) {
  faulty_run([] {
    container c;
    mass_insert(c, {3, 2, 4, 1});
    try {
      c.clear();
    } catch (...) {
      fault_injection_disable dg;
      ADD_FAILURE() << "clear() should not throw";
      throw;
    }
  });
}

TEST_F(exception_safety_test, copy_assignment) {
  faulty_run([] {
    container c;
    mass_insert(c, {3, 2, 4, 1});

    container c2;
    mass_insert(c2, {8, 7, 2, 14});

    strong_exception_safety_guard sg(c);
    c = c2;
    expect_eq(c, {2, 7, 8, 14});
  });
}

TEST_F(exception_safety_test, insert) {
  faulty_run([] {
    container c;
    mass_insert(c, {3, 2, 4, 1});

    strong_exception_safety_guard sg(c);
    c.insert(5);
    expect_eq(c, {1, 2, 3, 4, 5});
  });
}

TEST_F(exception_safety_test, erase) {
  faulty_run([] {
    container c;
    mass_insert(c, {6, 3, 8, 2, 5, 7, 10});
    element val = 6;

    strong_exception_safety_guard sg(c);
    c.erase(c.find(val));
    expect_eq(c, {2, 3, 5, 7, 8, 10});
  });
}
