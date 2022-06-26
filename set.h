#include <cassert>  // assert
#include <iterator> // std::reverse_iterator
#include <utility>  // std::pair, std::swap

template <typename T>
struct set {
  struct iterator;
  using const_iterator = iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  set();                            // O(1) nothrow
  set(set const& other);            // O(n) strong
  set& operator=(set const& other); // O(n) strong
  ~set();                           // O(n) nothrow

  void clear(); // O(n) nothrow
  bool empty(); // O(1) nothrow

  const_iterator begin() const; //      nothrow
  const_iterator end() const;   //      nothrow

  const_reverse_iterator rbegin() const; //      nothrow
  const_reverse_iterator rend() const;   //      nothrow

  std::pair<iterator, bool> insert(T const&); // O(h) strong
  iterator erase(iterator);                   // O(h) nothrow
  const_iterator find(T const&) const;        // O(h) strong
  const_iterator lower_bound(T const&) const; // O(h) strong
  const_iterator upper_bound(T const&) const; // O(h) strong

  void swap(set& other); // O(1) nothrow
};

template <typename T>
struct set<T>::iterator {
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T const;
  using pointer = T const*;
  using reference = T const&;

  iterator() = default;

  reference operator*() const; // O(1) nothrow
  pointer operator->() const;  // O(1) nothrow

  iterator& operator++() &;   //      nothrow
  iterator operator++(int) &; //      nothrow

  iterator& operator--() &;   //      nothrow
  iterator operator--(int) &; //      nothrow
};
