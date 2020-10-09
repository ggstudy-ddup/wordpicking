#include "header/docxbuffer.h"
using namespace wordlist;

DocxBuffer::DocxBuffer(const std::string& f):
    FileName(f), pNum(0), Lines(nullptr), paragraphs()
{
    if (f.empty())
        this->DocxFile = nullptr;
    else
    {
        this->DocxFile = new duckx::Document(f);
        this->DocxFile->open();
    }
}

DocxBuffer::DocxBuffer(const DocxBuffer& obj):
    FileName(obj.FileName), pNum(obj.pNum), paragraphs(obj.paragraphs)
{
    // copy of Document object
    this->DocxFile = new duckx::Document();
    memcpy(this->DocxFile, obj.DocxFile, sizeof(duckx::Document));
    // copy of lines
    this->Lines = new std::string[this->pNum];
    memcpy(this->Lines, obj.Lines, sizeof(std::string) * obj.pNum);
    // open the docx file
    this->DocxFile->open();
}

DocxBuffer::~DocxBuffer()
{
    if (this->DocxFile)
        delete this->DocxFile;
    if (this->Lines)
        delete[] this->Lines;
}

/// Private Members ///

// get paragraphs into this->paragraph (std::list<duckx::Paragraph)
void
DocxBuffer::getParagraphs()
{
    for (auto p = this->DocxFile->paragraphs(); p.has_next(); p.next())
    {
        this->paragraphs.push_back(p);
        this->pNum++;
    }
}

// get all Runs from paragraphs into this->Lines
void
DocxBuffer::getRuns(void)
{
    for (unsigned int i = 0; i < this->pNum; i++)
    {
        duckx::Run &current = this->paragraphs.front().runs();
        for (; current.has_next(); current.next())
        {
            this->Lines[i].append(current.get_text());
        }
        this->paragraphs.pop_front();
    }
}

/// ///

/// Members ///

size_t
DocxBuffer::size() const noexcept
{
    return pNum;
}

void
DocxBuffer::open(const std::string& f)
{
    if (this->DocxFile)
        delete this->DocxFile;
    this->DocxFile = new duckx::Document(f);
    this->DocxFile->open();
    this->FileName = f;
    if (this->Lines)
        delete[] this->Lines;
}

void
DocxBuffer::close()
{
    if (this->DocxFile)
        delete this->DocxFile;
    this->FileName.clear();
    if (this->Lines)
        delete[] this->Lines;
}

void
DocxBuffer::read()
{
    this->getParagraphs();
    this->Lines = new std::string[this->pNum];
    this->getRuns();
}

std::string
DocxBuffer::get_Line(size_t i) const
{
    if (i >= this->pNum) throw std::out_of_range("Line accessing out of range");
    return this->Lines[i];
}
