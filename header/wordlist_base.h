#ifndef WORDLIST_BASE_H
#define WORDLIST_BASE_H

#include <stddef.h>

template <class Element>
class WordList_base
{
public:
    virtual size_t size(void) const noexcept = 0;
    virtual Element at(size_t index) const = 0;
};

#endif // WORDLIST_BASE_H
