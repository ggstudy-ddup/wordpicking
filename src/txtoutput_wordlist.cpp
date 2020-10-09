#include "header/txtoutput_wordlist.h"
#include "header/define.h"
#include <string>
#include <stdio.h>
#if _WINDOWS_
#include <io.h>
#elif _UNiX_
#include <unistd.h>
#endif // _WINDOWS_ || _UNIX_
using namespace wordlist;

txtOutput_WordList::txtOutput_WordList(std::string filename, byte operation):
    fname(filename), currentoperation(operation), words(nullptr), lenth(0), separate("\n")
{
    if (!filename.empty())
        txtOutput_WordList::openfile(this->filebuff, filename, operation);
    else
        this->filebuff = nullptr;
}

txtOutput_WordList::txtOutput_WordList(const txtOutput_WordList &obj) :
    fname(obj.fname), currentoperation(obj.currentoperation),
    words(obj.words), lenth(obj.lenth), separate(obj.separate)
{
    if (!obj.fname.empty())
        txtOutput_WordList::openfile(this->filebuff, this->fname, this->currentoperation);
}

txtOutput_WordList::~txtOutput_WordList() { fclose(this->filebuff); }

/// static functions ///

inline
void
txtOutput_WordList::openfile(FILE *&fp, const std::string& filename, byte &operation)
{
    if (0 == access(filename.c_str(), F_OK) && Operations::Write == operation)
        throw io_err(FILE_EXIST);
    /*
     * if isEmpty(file)
     * # empty means only '\20', '\n', '\r', '\t', '\w', or other characters
     * # that don't show
     *
     * ... some codes ...
     *
     */
    fp = fopen(filename.c_str(), (Operations::Write_Append == operation) ? "a" : "w");
    if (!fp) throw std::bad_alloc();
//    if (!access(filename.c_str(), W_OK)) throw io_err(FILE_WRITE_ERR);
}

/// ///

/// members ///

void
txtOutput_WordList::open(const std::string &filename, byte operation)
{
    if (this->filebuff)
        fclose(this->filebuff);
    openfile(this->filebuff, filename, operation);
}

void
txtOutput_WordList::set_wordlist(std::string *&wdls, unsigned int len, std::string separation)
{
    if (-1 == access(this->fname.c_str(), W_OK)) throw io_err("Write file error");
    this->words = wdls;
    this->lenth = len;
    this->separate = separation;
    // write a separation string if is appending to an existed file
    if (Operations::Write_Append == this->currentoperation)
        fwrite(this->separate.c_str(), sizeof(char), this->separate.size(), this->filebuff);
    for (unsigned int i = 0; i < this->lenth; i++)
    {
        fwrite(this->words[i].c_str(), sizeof(char), this->words[i].size(), this->filebuff);
        fwrite(separation.c_str(), sizeof(char), separation.size(), this->filebuff);
    }
}

void
txtOutput_WordList::close()
{ if (-1 == fflush(this->filebuff)) throw io_err("Close file error"); }

/// ///
