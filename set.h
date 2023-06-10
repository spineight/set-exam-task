#include <cassert>  // assert
#include <iterator> // std::reverse_iterator
#include <utility>  // std::pair, std::swap

template <typename T>
struct set {
  struct iterator;
  using const_iterator = iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // O(1) nothrow
  set();

  // O(n) strong
  set(const set& other);

  // O(n) strong
  set& operator=(const set& other);

  // O(n) nothrow
  ~set();

  // O(n) nothrow
  void clear();

  // O(1) nothrow
  bool empty();

  // nothrow
  const_iterator begin() const;

  // nothrow
  const_iterator end() const;

  // nothrow
  const_reverse_iterator rbegin() const;

  // nothrow
  const_reverse_iterator rend() const;

  // O(h) strong
  std::pair<iterator, bool> insert(const T&);

  // O(h) nothrow
  iterator erase(iterator);

  // O(h) strong
  const_iterator find(const T&) const;

  // O(h) strong
  const_iterator lower_bound(const T&) const;

  // O(h) strong
  const_iterator upper_bound(const T&) const;

  // O(1) nothrow
  void swap(set& other);
};

template <typename T>
struct set<T>::iterator {
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T const;
  using pointer = const T*;
  using reference = const T&;

  iterator() = default;

  // O(1) nothrow
  reference operator*() const;

  // O(1) nothrow
  pointer operator->() const;

  // nothrow
  iterator& operator++() &;

  // nothrow
  iterator operator++(int) &;

  // nothrow
  iterator& operator--() &;

  // nothrow
  iterator operator--(int) &;
};
