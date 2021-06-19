#pragma once
#include <cassert>
#include <iterator>

template <typename T>
class list {
private:
  struct node;
  struct data_node;

  node* begin_;
  node end_;

public:
  // bidirectional iterator
  struct iterator;
  // bidirectional iterator
  struct const_iterator;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // O(1)
  list() noexcept;

  // O(n), strong
  list(list const&);

  // O(n), strong
  list& operator=(list const&);

  // O(n)
  ~list();

  // O(1)
  bool empty() const noexcept;

  // O(1)
  T& front() noexcept;
  // O(1)
  T const& front() const noexcept;

  // O(1), strong
  void push_front(T const&);
  // O(1)
  void pop_front() noexcept;

  // O(1)
  T& back() noexcept;
  // O(1)
  T const& back() const noexcept;

  // O(1), strong
  void push_back(T const&);
  // O(1)
  void pop_back() noexcept;

  // O(1)
  iterator begin() noexcept;
  // O(1)
  const_iterator begin() const noexcept;

  // O(1)
  iterator end() noexcept;
  // O(1)
  const_iterator end() const noexcept;

  // O(1)
  reverse_iterator rbegin() noexcept;
  // O(1)
  const_reverse_iterator rbegin() const noexcept;

  // O(1)
  reverse_iterator rend() noexcept;
  // O(1)
  const_reverse_iterator rend() const noexcept;

  // O(n)
  void clear() noexcept;

  // O(1), strong
  iterator insert(const_iterator pos, T const& val);
  // O(1)
  iterator erase(const_iterator pos) noexcept;
  // O(n)
  iterator erase(const_iterator first, const_iterator last) noexcept;
  // O(1)
  void splice(const_iterator pos, list& other, const_iterator first,
              const_iterator last) noexcept;

  friend void swap(list& a, list& b) noexcept;

private:
  node* copy_node(node* right, node* orig);
  node* get_begin();

  void swap(list& other);

  static void destruct_list(node* cur);
};

template <typename T>
struct list<T>::iterator {
private:
  node* ptr_{nullptr};

public:
  using pointer = T*;
  using reference = T&;

  iterator() = default;

  reference operator*() const; // O(1) nothrow
  pointer operator->() const;  // O(1) nothrow

  iterator& operator++() &;   //      nothrow
  iterator operator++(int) &; //      nothrow

  iterator& operator--() &;   //      nothrow
  iterator operator--(int) &; //      nothrow

private:
  explicit iterator(node* ptr);

  friend bool operator==(iterator const& a, iterator const& b) {
    return a.ptr_ == b.ptr_;
  }

  friend bool operator!=(iterator const& a, iterator const& b) {
    return a.ptr_ != b.ptr_;
  }

  friend list;
};

template <typename T>
struct list<T>::const_iterator : iterator {
public:
  using const_pointer = T const*;
  using const_reference = T const&;

  const_iterator() = default;
  const_iterator(iterator const& other) = default;

//  const_reference operator*() const; // O(1) nothrow
//  const_pointer operator->() const;  // O(1) nothrow
//
//  const_iterator& operator++() &;   //      nothrow
//  const_iterator operator++(int) &; //      nothrow
//
//  const_iterator& operator--() &;   //      nothrow
//  const_iterator operator--(int) &; //      nothrow
};

template <typename T>
struct list<T>::node {
  node() = default;

  T& value();
  node*& right();

private:
  node* left_{nullptr};

  friend list;
};

template <typename T>
struct list<T>::data_node : node {
  data_node(T const& value, node* right);

private:
  T value_;
  node* right_;

  friend node;
};

template <typename T>
list<T>::list() noexcept : end_(), begin_(&end_) {}

template <typename T>
list<T>::list(const list<T>& other) : list() {
  end_.left_ = copy_node(&end_, other.end_.left);
  begin_ = get_begin();
}

template <typename T>
list<T>& list<T>::operator=(list const& other) {
  list(other).swap(*this);
}

template <typename T>
list<T>::~list() {
  destruct_list(end_.left_);
}

template <typename T>
bool list<T>::empty() const noexcept {
  return end_.left_ != nullptr;
}

template <typename T>
T& list<T>::front() noexcept {
  return begin_->value();
}

template <typename T>
T const& list<T>::front() const noexcept {
  return begin_->value();
}

template <typename T>
void list<T>::push_front(T const& val) {
  insert(begin(), val);
}

template <typename T>
void list<T>::pop_front() noexcept {
  erase(begin());
}

template <typename T>
T& list<T>::back() noexcept {
  return end_.left_->value();
}

template <typename T>
T const& list<T>::back() const noexcept {
  return end_.left_->value();
}

template <typename T>
void list<T>::push_back(T const& val) {
  insert(end(), val);
}

template <typename T>
void list<T>::pop_back() noexcept {
  erase(std::prev(end()));
}

template <typename T>
typename list<T>::iterator list<T>::begin() noexcept {
  return iterator(begin_);
}

template <typename T>
typename list<T>::const_iterator list<T>::begin() const noexcept {
  return const_iterator(begin_);
}

template <typename T>
typename list<T>::iterator list<T>::end() noexcept {
  return iterator(&end_);
}

template <typename T>
typename list<T>::const_iterator list<T>::end() const noexcept {
  return const_iterator(&end_);
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rbegin() noexcept {
  return reverse_iterator(begin());
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::rbegin() const noexcept {
  return reverse_iterator(begin());
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rend() noexcept {
  return reverse_iterator(end());
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::rend() const noexcept {
  return reverse_iterator(end());
}

template <typename T>
void list<T>::clear() noexcept {
  destruct_list(end_.left_);
  end_.left_ = nullptr;
  begin_ = &end_;
}

template <typename T>
typename list<T>::iterator list<T>::insert(const_iterator pos, T const& val) {
  node* cur = pos.ptr;
  node* new_node = new data_node(val, cur);
  new_node->left_ = cur->left_;
  if (cur->left_) {
    cur->left_->right() = new_node;
  } else {
    begin_ = new_node;
  }
  cur->left_ = new_node;
  return iterator(new_node);
}

template <typename T>
typename list<T>::iterator list<T>::erase(const_iterator pos) noexcept {
  erase(pos, pos + 1);
}

template <typename T>
typename list<T>::iterator list<T>::erase(const_iterator first,
                                          const_iterator last) noexcept {
  if (first != last) {
    node* cur1 = first.ptr;
    node* cur2 = last.ptr->left_;

    cur2->right()->left_ = cur1->left_;
    if (cur1->left_) {
      cur1->left_->right() = cur2->right();
    } else {
      begin_ = cur2->right();
    }
    cur1->left_ = nullptr;
    cur2->right() = nullptr;
    destruct_list(cur2);
  }
  return iterator(last.ptr);
}

template <typename T>
void list<T>::splice(const_iterator pos, list<T>& other, const_iterator first,
                     const_iterator last) noexcept {
  if (first == last) {
    return;
  }
  node* cur1 = first.ptr;
  node* cur2 = last.ptr->left_;
  node* cur_pos = pos.ptr;

  cur2->right()->left_ = cur1->left_;
  if (cur1->left_) {
    cur1->left_->right() = cur2->right();
  } else {
    other.begin_ = cur2->right();
  }
  cur2->right() = cur_pos;
  cur1->left_ = cur_pos->left_;
  if (cur_pos->left_) {
    cur_pos->left_->right() = cur1;
  } else {
    begin_ = cur1;
  }
  cur_pos->left_ = cur2;
}

template <typename T>
void swap(list<T>& a, list<T>& b) {
  a.swap(b);
}

template <typename T>
typename list<T>::node* list<T>::copy_node(node* right, node* orig) {
  if (!orig) {
    return nullptr;
  }
  node* res = new data_node(orig->value(), right);
  try {
    res->left_ = copy_node(res, orig->left_);
    return res;
  } catch (...) { delete static_cast<data_node*>(res); }
}

template <typename T>
typename list<T>::node* list<T>::get_begin() {
  node* res = end_.left_;
  for (;;) {
    if (!res->left_) {
      return res;
    }
    res = res->left_;
  }
}

template <typename T>
void list<T>::swap(list<T>& other) {
  std::swap(end_.left_, other.end_.left_);
  std::swap(begin_, other.begin_);
}

template <typename T>
void list<T>::destruct_list(node* cur) {
  if (!cur) {
    return;
  }
  destruct_list(cur->left_);
  delete static_cast<data_node*>(cur);
}

template <typename T>
typename list<T>::iterator::reference list<T>::iterator::operator*() const {
  return ptr_->value();
}

template <typename T>
typename list<T>::iterator::pointer list<T>::iterator::operator->() const {
  return &ptr_->value();
}

template <typename T>
typename list<T>::iterator& list<T>::iterator::operator++() & {
  ptr_ = ptr_->right();
  return *this;
}

template <typename T>
typename list<T>::iterator list<T>::iterator::operator++(int) & {
  iterator old = *this;
  operator++();
  return old;
}

template <typename T>
typename list<T>::iterator& list<T>::iterator::operator--() & {
  ptr_ = ptr_->left_;
  return *this;
}

template <typename T>
typename list<T>::iterator list<T>::iterator::operator--(int) & {
  iterator old = *this;
  operator--();
  return old;
}

template <typename T>
list<T>::iterator::iterator(node* ptr) : ptr_(ptr) {}

template <typename T>
T& list<T>::node::value() {
  return static_cast<data_node*>(this)->value_;
}

template <typename T>
typename list<T>::node*& list<T>::node::right() {
  return static_cast<data_node*>(this)->right_;
}

template <typename T>
list<T>::data_node::data_node(const T& value, node* right)
    : value_(value), right_(right) {}
