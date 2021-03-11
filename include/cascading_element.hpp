#pragma once

template<typename KeyType>
struct Element
{
  struct Element* prev;
  struct Element* next;
  struct Element* out;
  KeyType value;

  inline bool isPromoted() const
  {
    return out != nullptr;
  }
};

template<typename KeyType>
inline bool operator<(const Element<KeyType>& rhs, KeyType lhs)
{
  return rhs.value < lhs;
}
