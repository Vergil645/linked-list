#pragma once
#include <cassert>
#include <iterator>

template <typename T>
class list {
private:
  template <typename VALUE_TYPE>
  struct list_iterator;

  struct node;
  struct data_node;

  node end_;

public:
  // bidirectional iterator
  using iterator = list_iterator<T>;
  // bidirectional iterator
  using const_iterator = list_iterator<T const>;

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

  friend void swap(list& a, list& b) noexcept {
    a.swap(b);
  }

private:
  node* copy_node(node* right, node* orig, node const* end);

  void swap(list& other);

  static void destruct_list(node* cur);

  template <typename VALUE_TYPE>
  struct list_iterator {
  private:
    node* ptr_{nullptr};

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = VALUE_TYPE;
    using pointer = VALUE_TYPE*;
    using reference = VALUE_TYPE&;

    list_iterator() = default;

    list_iterator(iterator const& other) : ptr_(other.ptr_) {}

    reference operator*() const {
      return ptr_->value();
    }
    pointer operator->() const {
      return &ptr_->value();
    }

    list_iterator& operator++() & {
      ptr_ = ptr_->right_;
      return *this;
    }

    list_iterator operator++(int) & {
      list_iterator old = *this;
      ++(*this);
      return old;
    }

    list_iterator& operator--() & {
      ptr_ = ptr_->left_;
      return *this;
    }

    list_iterator operator--(int) & {
      list_iterator old = *this;
      --(*this);
      return old;
    }

    bool operator==(const_iterator const& other) const {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const_iterator const& other) const {
      return ptr_ != other.ptr_;
    }

  private:
    explicit list_iterator(node* ptr) : ptr_(ptr) {}

    friend list;
  };
};

template <typename T>
struct list<T>::node {
  node() = default;

  T& value();

private:
  node* left_{nullptr};
  node* right_{nullptr};

  friend list;
};

template <typename T>
struct list<T>::data_node : node {
  data_node(T const& value, node* left, node* right);

private:
  T value_;

  friend node;
};

template <typename T>
list<T>::list() noexcept : end_() {
  end_.left_ = &end_;
  end_.right_ = &end_;
}

template <typename T>
list<T>::list(const list<T>& other) : list() {
  end_.left_ = copy_node(&end_, other.end_.left_, &other.end_);
}

template <typename T>
list<T>& list<T>::operator=(list const& other) {
  if (this != &other) {
    list(other).swap(*this);
  }
  return *this;
}

template <typename T>
list<T>::~list() {
  end_.right_->left_ = nullptr;
  destruct_list(end_.left_);
}

template <typename T>
bool list<T>::empty() const noexcept {
  return end_.left_ == &end_;
}

template <typename T>
T& list<T>::front() noexcept {
  return end_.right_->value();
}

template <typename T>
T const& list<T>::front() const noexcept {
  return end_.right_->value();
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
  return iterator(end_.right_);
}

template <typename T>
typename list<T>::const_iterator list<T>::begin() const noexcept {
  return const_iterator(end_.right_);
}

template <typename T>
typename list<T>::iterator list<T>::end() noexcept {
  return iterator(&end_);
}

template <typename T>
typename list<T>::const_iterator list<T>::end() const noexcept {
  return const_iterator(const_cast<node*>(&end_));
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rend() noexcept {
  return reverse_iterator(begin());
}

template <typename T>
typename list<T>::const_reverse_iterator list<T>::rend() const noexcept {
  return const_reverse_iterator(begin());
}

template <typename T>
void list<T>::clear() noexcept {
  end_.right_->left_ = nullptr;
  destruct_list(end_.left_);
  end_.left_ = &end_;
  end_.right_ = &end_;
}

template <typename T>
typename list<T>::iterator list<T>::insert(const_iterator pos, T const& val) {
  node* cur = pos.ptr_;
  node* new_node = new data_node(val, cur->left_, cur);
  cur->left_->right_ = new_node;
  cur->left_ = new_node;
  return iterator(new_node);
}

template <typename T>
typename list<T>::iterator list<T>::erase(const_iterator pos) noexcept {
  return erase(pos, std::next(pos));
}

template <typename T>
typename list<T>::iterator list<T>::erase(const_iterator first,
                                          const_iterator last) noexcept {
  if (first != last) {
    node* cur1 = first.ptr_;
    node* cur2 = last.ptr_->left_;

    cur2->right_->left_ = cur1->left_;
    cur1->left_->right_ = cur2->right_;
    cur1->left_ = nullptr;
    cur2->right_ = nullptr;

    destruct_list(cur2);
  }
  return iterator(last.ptr_);
}

template <typename T>
void list<T>::splice(const_iterator pos, list<T>& other, const_iterator first,
                     const_iterator last) noexcept {
  if (first == last) {
    return;
  }
  node* cur1 = first.ptr_;
  node* cur2 = last.ptr_->left_;
  node* cur_pos = pos.ptr_;

  cur2->right_->left_ = cur1->left_;
  cur1->left_->right_ = cur2->right_;

  cur2->right_ = cur_pos;
  cur1->left_ = cur_pos->left_;

  cur_pos->left_->right_ = cur1;
  cur_pos->left_ = cur2;
}

template <typename T>
typename list<T>::node* list<T>::copy_node(node* right, node* orig,
                                           node const* end) {
  if (orig == end) {
    end_.right_ = right;
    return &end_;
  }
  node* res = new data_node(orig->value(), nullptr, right);
  try {
    res->left_ = copy_node(res, orig->left_, end);
    return res;
  } catch (...) {
    delete static_cast<data_node*>(res);
    throw;
  }
}

template <typename T>
void list<T>::swap(list<T>& other) {
  if (empty()) {
    end_.left_ = &other.end_;
    end_.right_ = &other.end_;
  } else {
    end_.left_->right_ = &other.end_;
    end_.right_->left_ = &other.end_;
  }
  if (other.empty()) {
    other.end_.left_ = &end_;
    other.end_.right_ = &end_;
  } else {
    other.end_.left_->right_ = &end_;
    other.end_.right_->left_ = &end_;
  }
  std::swap(end_, other.end_);
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
T& list<T>::node::value() {
  return static_cast<data_node*>(this)->value_;
}

template <typename T>
list<T>::data_node::data_node(const T& value, node* left, node* right)
    : value_(value) {
  node::left_ = left;
  node::right_ = right;
}
