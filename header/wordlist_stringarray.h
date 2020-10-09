#ifndef WORDLIST_STRINGARRAY_H
#define WORDLIST_STRINGARRAY_H

#include "wordlist_base.h"
#include "define.h"
#include <string>

using namespace wordlist;

namespace wordlist
{
class WordList_StringArray : public WordList_base<std::string>
{
private:
    // Lenth of the Array
    size_t lenth;
    // Total lenth
    size_t reallen;
    // pointer to the Array
    std::string *array;
    // used table
    unsigned int *arrtable;

public:
    // constructors
    WordList_StringArray(size_t len = 0);
    WordList_StringArray(const WordList_StringArray&);
    // deconstructor
    virtual ~WordList_StringArray();
    // resize the array
    void resize(size_t);
    // Returns the list size
    size_t size(void) const noexcept override;
    // Returns the elemnets by the index
    std::string at(size_t index) const override;
    std::string operator[](size_t) const;
    // remove element
    void remove(unsigned int index);
    // Set the value of an element
    void setElements(size_t index, const std::string& value);
};
} // wordlist

#endif // WORDLIST_STRINGARRAY_H
