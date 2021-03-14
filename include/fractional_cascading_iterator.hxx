#pragma once

template<typename KeyType>
static inline ElementIt<KeyType> get_nearest_lower_upper_promoted(
  ElementIt<KeyType> begin, ElementIt<KeyType> end,
  ElementIt<KeyType> lower)
{
  // elements in [first, last[ == key, but they are not necessarily promoted

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

template<typename KeyType>
class FractionalCascadingLowerBound
{
public:
  using underlying_reference = const FractionalCascading<KeyType>&;

  FractionalCascadingLowerBound(underlying_reference fractionalCascading, KeyType key)
    : _fractionalCascading(fractionalCascading)
    , _key(key)
  {
  }

  class iterator
  {
  public:

    iterator& operator++()
    {
      if (!_cascade[_i].empty())
        _lower = get_nearest_lower_upper_promoted<KeyType>(_cascade[_i].begin(), _cascade[_i].end(), _lower);

      _i++;
      if (!_cascade[_i].empty())
        if (_i < _cascade.size())
        {
          // From equal_range of i-1, deduce equal_range of i
          if (_lower == _cascade[_i-1].end())
            _lower = _cascade[_i].end();
          else
            _lower = _cascade[_i].begin() + (_lower->out - &_cascade[_i][0]);

          while (_lower != _cascade[_i].begin() && (_lower-1)->value >= _key)
            _lower--;
          while (_lower != _cascade[_i].end() && _lower->value < _key)
            _lower++;
        }

      return *this;
    }

    iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }

    bool operator==(iterator other) const { return _i == other._i; }

    bool operator!=(iterator other) const { return !(*this == other); }

    const KeyType* operator*() const
    {
      if (_cascade[_i].empty())
        return nullptr;

      if (_lower != _cascade[_i].end())
      {
        if (!_lower->isPromoted())
          return &_lower->value;
        else if (_lower->next)
          return &_lower->next->value;
      }

      return nullptr;
    }

  private:
    friend class FractionalCascadingLowerBound<KeyType>;

    iterator(const FractionalCascadingLowerBound& fractionalCascadingLowerBound, std::size_t i = 0)
      : _cascade(fractionalCascadingLowerBound._fractionalCascading._cascade)
      , _key(fractionalCascadingLowerBound._key)
      , _i(i)
    {
      if (!_cascade.empty() && !_cascade[0].empty())
        _lower = std::lower_bound(_cascade[0].begin(), _cascade[0].end(), _key);
    }

    const std::vector<std::vector<Element<KeyType>>>& _cascade;
    KeyType _key;
    std::size_t _i = 0;
    ElementIt<KeyType> _lower;
    ElementIt<KeyType> _upper;
  };

  iterator begin() const { return *this; }
  iterator end() const { return {*this, _fractionalCascading._cascade.size()}; }

private:
  underlying_reference _fractionalCascading;
  KeyType _key;
};
