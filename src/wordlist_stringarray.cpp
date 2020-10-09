#include "header/wordlist_stringarray.h"
#include <stdexcept>
#include <string.h>
using namespace wordlist;

WordList_StringArray::WordList_StringArray(size_t l)
{
    this->lenth = l;
    this->reallen = l;
    if (l)
    {
        this->array = new std::string[l];
        this->arrtable = new unsigned int[l];
        for (unsigned int i = 0; i < l; i++)
        {
            arrtable[i] = i;
        }
    }
    else
    {
        this->array = nullptr;
        this->arrtable = nullptr;
    }
}

WordList_StringArray::WordList_StringArray(const WordList_StringArray& obj)
{
    this->lenth = obj.lenth;
    this->reallen = obj.reallen;
    if (obj.reallen)
    {
        this->array = new std::string[this->reallen];
        this->arrtable = new unsigned int[this->reallen];
        memcpy(this->array, obj.array, sizeof(std::string) * obj.reallen);
        memcpy(this->arrtable, obj.arrtable, sizeof(unsigned int) * obj.reallen);
    }
}

WordList_StringArray::~WordList_StringArray()
{
    delete[] this->array;
}

/// Members ///

// resize
void
WordList_StringArray::resize(size_t sz)
{
    if (this->array)
        delete [] this->array;
    if (this->arrtable)
        delete [] this->arrtable;
    this->lenth = sz;
    this->reallen = sz;
    if (sz)
    {
        this->array = new std::string[sz];
        this->arrtable = new unsigned int[sz];
        for (unsigned int i = 0; i < sz; i++)
        {
            this->arrtable[i] = i;
        }
    }
    else
    {
        this->array = nullptr;
        this->arrtable = nullptr;
    }
}

// size
size_t
WordList_StringArray::size(void) const noexcept
{
    return this->lenth;
}

// at
std::string
WordList_StringArray::at(size_t i) const
{
    if (i > this->lenth) throw std::out_of_range("List out of range");
    return this->array[this->arrtable[i]];
}

// operator[] same with at()
std::string
WordList_StringArray::operator[](size_t i) const
{
    if (i > this->lenth) throw std::out_of_range("List out of range");
    return this->array[arrtable[i]];
}

// remove elements
void
WordList_StringArray::remove(unsigned int index)
{
    if (index > this->lenth) throw std::out_of_range("List out of range");
    for (unsigned int i = index; i < this->reallen - 1; i++)
    {
        arrtable[i]++;
    }
    lenth--;
}

// set chosen element
void
WordList_StringArray::setElements(size_t i, const std::string& v)
{
    if (i > this->lenth) throw std::out_of_range("List out of range");
    this->array[arrtable[i]] = v;
}
