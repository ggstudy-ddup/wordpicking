#ifndef DEFINE_H
#define DEFINE_H

#define _WINDOWS_ 1

#include <stdexcept>
#include <stdlib.h>
#include <string.h>

// some error string
const char FILE_EXIST[2] = { -1, 0 };
const char FILE_NOTDOCX[2] = { -2, 0 };
#define FEXIST_INT (-1)
#define FNOTDOCX_INT (-1)
namespace wordlist
{
#define DEFAULT_WORDLIST__IO_ERR__CONTENT    "File IO error!"
#define DEFAULT_WORDLIST__IO_ERR__CONTENTLEN 15
class io_err : public std::exception
{
private:
    char *Content;
public:
    io_err(const char *content = DEFAULT_WORDLIST__IO_ERR__CONTENT,
           unsigned int strlenth = DEFAULT_WORDLIST__IO_ERR__CONTENTLEN)
    {
        this->Content = (char*)malloc(sizeof(char) * strlenth);
        memcpy(this->Content, content, strlenth);
    }
    ~io_err()
    {
        free(this->Content);
    }
    const char * what() const noexcept override
    {
        return Content;
    }
};
} // wordlist

/* Line Break */
#if _UNIX_
#define LINEBREAK "\n"
#elif _WINDOWS_
#define LINEBREAK "\n"
#endif /* _UNIX_ || _WINDOWS_ */

typedef unsigned char byte;

#endif /* !DEFINE_H */

