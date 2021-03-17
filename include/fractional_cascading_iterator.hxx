#pragma once

template<typename KeyType>
class LowerBoundSearch
{
public:
  static inline ElementIt<KeyType> bound_search(ElementIt<KeyType> begin, ElementIt<KeyType> end, KeyType key)
  {
    return std::lower_bound(begin, end, key);
  }

  static inline ElementIt<KeyType> get_nearest_promoted(
    ElementIt<KeyType> begin, ElementIt<KeyType> end,
    ElementIt<KeyType> lower)
  {
    if (lower != end)
    {
      if (!lower->isPromoted())
      {
        if (lower->prev)
          lower = begin + (lower->prev - &*begin);
        else if (lower->next)
          lower = begin + (lower->next - &*begin);
        else
          lower = end;
      }
      // else ok
    }
    assert(lower == end || lower->isPromoted());

    return lower;
  }

  static inline ElementIt<KeyType> next_from_prev(
    ElementIt<KeyType> prev_end,
    ElementIt<KeyType> begin, ElementIt<KeyType> end,
    ElementIt<KeyType> lower,
    KeyType key)
  {
    if (lower == prev_end)
      lower = end;
    else
      lower = begin + (lower->out - &*begin);

    while (lower != begin && (lower-1)->value >= key)
      lower--;
    while (lower != end && lower->value < key)
      lower++;

    // lower: first element >= key
    if (lower != end)
      assert(lower->value >= key);

    if (begin != end && lower != begin)
      assert((lower-1)->value < key);

    return lower;
  }

  static inline const KeyType* get_value(ElementIt<KeyType> lower)
  {
    if (!lower->isPromoted())
      return &lower->value;
    if (lower->next)
      return &lower->next->value;
    return nullptr;
  }
};

template<typename KeyType>
class UpperBoundSearch
{
public:
  static inline ElementIt<KeyType> bound_search(ElementIt<KeyType> begin, ElementIt<KeyType> end, KeyType key)
  {
    return std::upper_bound(begin, end, key);
  }

  static inline ElementIt<KeyType> get_nearest_promoted(
    ElementIt<KeyType> begin, ElementIt<KeyType> end,
    ElementIt<KeyType> upper)
  {
    if (upper != end)
    {
      if (!upper->isPromoted())
      {
        if (upper->next)
          upper = begin + (upper->next - &*begin);
        else if (upper->prev)
          upper = begin + (upper->prev - &*begin);
        else
          upper = end;
      }
      // else ok
    }
    assert(upper == end || upper->isPromoted());

    return upper;
  }

  static inline ElementIt<KeyType> next_from_prev(
    ElementIt<KeyType> prev_end,
    ElementIt<KeyType> begin, ElementIt<KeyType> end,
    ElementIt<KeyType> upper,
    KeyType key)
  {
    if (upper == prev_end)
      upper = end;
    else
      upper = begin + (upper->out - &*begin);

    while (upper != end && upper->value <= key)
      upper++;
    while (upper != begin && (upper-1)->value > key)
      upper--;

    // upper: first element > key
    if (upper != end)
      assert(upper->value > key);

    if (begin != end && upper != begin)
      assert((upper-1)->value <= key);

    return upper;
  }

  static inline const KeyType* get_value(ElementIt<KeyType> upper)
  {
    if (!upper->isPromoted())
      return &upper->value;
    if (upper->next)
      return &upper->next->value;
    return nullptr;
  }
};

template<typename KeyType, typename SearchType>
class FractionalCascadingIterable
{
public:
  using underlying_reference = const FractionalCascading<KeyType>&;

  FractionalCascadingIterable(underlying_reference fractionalCascading, KeyType key)
    : _fractionalCascading(fractionalCascading)
    , _key(key)
  {
  }

  class iterator
  {
  public:

    iterator& operator++()
    {
      if (_it != _end)
      {
        if (!_it->empty())
          _bound = SearchType::get_nearest_promoted(_it->begin(), _it->end(), _bound);

        const auto prev_i = _it;
        _it++;
        if (_it != _end && !_it->empty())
        {
          // From equal_range of i-1, deduce equal_range of i
          _bound = SearchType::next_from_prev(prev_i->end(), _it->begin(), _it->end(), _bound, _key);
        }
      }

      return *this;
    }

    iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }

    bool operator==(iterator other) const { return _it == other._it; }

    bool operator!=(iterator other) const { return !(*this == other); }

    const KeyType* operator*() const
    {
      if (_it->empty())
        return nullptr;

      if (_bound != _it->end())
        return SearchType::get_value(_bound);

      return nullptr;
    }

  private:
    friend class FractionalCascadingIterable<KeyType, LowerBoundSearch<KeyType>>;
    friend class FractionalCascadingIterable<KeyType, UpperBoundSearch<KeyType>>;

    iterator(
      typename std::vector<std::vector<Element<KeyType>>>::const_iterator it,
      typename std::vector<std::vector<Element<KeyType>>>::const_iterator end,
      KeyType key
    )
    : _it(it)
    , _end(end)
    , _key(key)
    {
      if (_it != _end && !_it->empty())
        _bound = SearchType::bound_search(_it->begin(), _it->end(), _key);
    }

    KeyType _key;
    typename std::vector<std::vector<Element<KeyType>>>::const_iterator _it;
    typename std::vector<std::vector<Element<KeyType>>>::const_iterator _end;
    ElementIt<KeyType> _bound;
  };

  iterator begin() const { return {_fractionalCascading._cascade.begin(), _fractionalCascading._cascade.end(), _key}; }
  iterator end() const { return {_fractionalCascading._cascade.end(), _fractionalCascading._cascade.end(), _key}; }

private:
  underlying_reference _fractionalCascading;
  KeyType _key;
};
