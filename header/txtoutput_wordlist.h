#ifndef TXTOUTPUT_WORDLIST_H
#define TXTOUTPUT_WORDLIST_H

#include <stdio.h>
#include <string>
#include <list>
#include "define.h"
using namespace wordlist;

#define DEFAULT_LINEBREAK "\n"

namespace wordlist
{
// Operations
enum Operations
{
    Write        = 0,   // Create a new file when the file is not exist
    StrongWrite  = 1,   // Write if the file is exist, overwrite the file
    Write_Append = 2    // Append the content to the file, with a *separation* at the begginning
};
// txtOutput_WordList
// all methods(functions) throw exceptions
// exceptions:
// 	wordlist::io_err
//  std::bad_alloc
class txtOutput_WordList
{
private:
    FILE *filebuff;
    std::string fname;
    byte currentoperation;
    // only references the list with this ptr
    std::string *words;
    unsigned int lenth;
    std::string separate;

    // open or create a file
    static inline void openfile(FILE *&fp, const std::string &filename, byte &operation);

public:
    // constructor
    // this will throw exceptions when:
    // - wordlist::Operations::Write
    //    - When the file is exist and it's not empty
    // - cannot IO or create a file
    // @exception: wordlist::io_error : public std::exception
    txtOutput_WordList(std::string filename = std::string(), byte operation = wordlist::Operations::Write);
    txtOutput_WordList(const txtOutput_WordList&);
    // deconstructor
    ~txtOutput_WordList();
    // Open another file
    // Close the previous file without saving
    void open(const std::string &filename, byte operation = wordlist::Operations::Write);
    // Write the word list
    // para *separation* - how to separate different string in a wordlist
    // exception: will throw if cannot write the chosen file
    void set_wordlist(std::string *&wordlist, unsigned int len, std::string separation = std::string(DEFAULT_LINEBREAK));
    // Close and save the text file
    void close();
};
} // wordlist

#endif // TXTOUTPUT_WORDLIST_H
