#pragma once

#include <cassert>
#include <iterator>
#include <utility>

template <typename T>
class set {
public:
  using value_type = T;

  using reference = T&;
  using const_reference = const T&;

  using pointer = T*;
  using const_pointer = const T*;

  using iterator = void;
  using const_iterator = void;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
  // O(1) nothrow
  set() noexcept;

  // O(n) strong
  set(const set& other);

  // O(n) strong
  set& operator=(const set& other);

  // O(n) nothrow
  ~set() noexcept;

  // O(n) nothrow
  void clear() noexcept;

  // O(1) nothrow
  size_t size() const noexcept;

  // O(1) nothrow
  bool empty() const noexcept;

  // nothrow
  const_iterator begin() const noexcept;

  // nothrow
  const_iterator end() const noexcept;

  // nothrow
  const_reverse_iterator rbegin() const noexcept;

  // nothrow
  const_reverse_iterator rend() const noexcept;

  // O(h) strong
  std::pair<iterator, bool> insert(const T&);

  // O(h) nothrow
  iterator erase(const_iterator pos);

  // O(h) strong
  size_t erase(const T&);

  // O(h) strong
  const_iterator lower_bound(const T&) const;

  // O(h) strong
  const_iterator upper_bound(const T&) const;

  // O(h) strong
  const_iterator find(const T&) const;

  // O(1) nothrow
  friend void swap(set&, set&) noexcept;
};
