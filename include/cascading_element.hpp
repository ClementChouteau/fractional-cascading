#pragma once

struct Element
{
  struct Element* prev;
  struct Element* next;
  struct Element* out;
  long value;

  inline bool isPromoted() const
  {
    return out != nullptr;
  }
};

inline bool operator<(const Element& rhs, long lhs)
{
  return rhs.value < lhs;
}
