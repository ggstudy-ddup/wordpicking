#ifndef DOCXBUFFER_H
#define DOCXBUFFER_H

#include "define.h"
#include <duckx.h>
#include <string>
#include <list>

using namespace wordlist;

namespace wordlist
{
class DocxBuffer
{
private:

    /// showing data ///
    // Docx file object
    duckx::Document *DocxFile;
    // File name includes its path
    std::string FileName;
    // Number of paragraphs
    size_t pNum;
    // Each Paragraph will be a Line
    std::string *Lines;
    /// ///

    std::list<duckx::Paragraph> paragraphs;
    // get all duckx::Paragraph objects
    void getParagraphs(void);
    // get all Runs (text), into *Lines*
    void getRuns(void);
public:
    // File name with its full path or relative path
    DocxBuffer(const std::string& filename = std::string());
    DocxBuffer(const DocxBuffer&);
    // Deconstructer
    ~DocxBuffer();
    size_t size() const noexcept;
    // Change the opened file
    // Opened file will be automatically closeld
    void open(const std::string&);
    // Close the file (deconstruct the docx file object)
    void close(void);
    // Read the docx file and find paragraphs
    void read(void);
    // Get a chosen Line in the docx file
    std::string get_Line(size_t) const;
};
} // wordlist

#endif // DOCXBUFFER_H
