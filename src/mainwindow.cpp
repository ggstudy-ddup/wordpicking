#include "header/mainwindow.h"
#include "ui_mainwindow.h"
#include <random>
#include <time.h>
#include <QMessageBox>
using namespace wordlist;
QString MainWindow::desktopdir = QString("");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      operation(Operations::Write),
      wordnum(0),
      wordlist(nullptr),
      outputlist(nullptr),
      docxfile(nullptr),
      output(nullptr)
{
    ui->setupUi(this);

    // set title
    this->setWindowTitle("TOEFL Word Selector");

    // set buddies
    ui->label_InputFile->setBuddy(ui->lineEdit_InputFile);
    ui->label_OutputWordNum->setBuddy(ui->spinBox_OutputWordNum);
    ui->label_Separation->setBuddy(ui->lineEdit_Separation);
    ui->label_OutputFile->setBuddy(ui->lineEdit_OutputFile);

    // set place holder
    ui->lineEdit_InputFile->setPlaceholderText(MISSINGWARN_INPUTFILE);
    ui->lineEdit_OutputFile->setPlaceholderText(HINT_OUTPUTFILE);

    // set default for Speration and OutputFile
    ui->lineEdit_Separation->setText(HINT_SEPARATION);
    ui->lineEdit_Separation->setReadOnly(true);
    ui->lineEdit_OutputFile->setReadOnly(true);
    ui->spinBox_OutputWordNum->setValue(40);
    ui->spinBox_OutputWordNum->setSingleStep(5);

    // set checkbox
    ui->checkBox_CustomSeparation->setCheckState(Qt::CheckState::Unchecked);
    ui->checkBox_CustomOutputFileName->setCheckState(Qt::CheckState::Unchecked);

    // set clear button
    ui->lineEdit_InputFile->setClearButtonEnabled(true);
    ui->lineEdit_Separation->setClearButtonEnabled(true);
    ui->lineEdit_OutputFile->setClearButtonEnabled(true);

    // set max lenth
    ui->lineEdit_InputFile->setMaxLength(200);
    ui->lineEdit_Separation->setMaxLength(50);
    ui->lineEdit_OutputFile->setMaxLength(200);

    // set drag enable
//    ui->lineEdit_InputFile->setDragEnabled(true);
//    ui->lineEdit_Separation->setDragEnabled(false);
//    ui->lineEdit_OutputFile->setDragEnabled(true);

    // Validator
    QRegExp regexp_IOFile("[^?*\"<>|]+");
    ui->lineEdit_InputFile->setValidator(new QRegExpValidator(regexp_IOFile, this));

    // set Desktop DIR
    if (!desktopdir.size())
    {
        desktopdir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        desktopdir.append("/");
    }

    // init - set parent, set caption, set directory
    browser = new QFileDialog(this, BROWSER_CAPTION_INPUTFILE, desktopdir);
    browser->setNameFilter("*.docx");

    // Connect Signals and Slots
    //
    connect(ui->pushButton_Browse, SIGNAL(clicked()),
            this, SLOT(browseDialog_InputFile()));
    connect(ui->checkBox_CustomSeparation, SIGNAL(clicked()),
            this, SLOT(customizeSeparation()));
    connect(ui->checkBox_CustomOutputFileName, SIGNAL(clicked()),
            this, SLOT(customizeOutputFile()));
    connect(ui->pushButton_Export, SIGNAL(clicked()),
            this, SLOT(exporting()));
    // sync
    connect(ui->lineEdit_InputFile, SIGNAL(textChanged(const QString&)),
            this, SLOT(syncIOfile(const QString&)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete browser;
}

/// members ///

void
MainWindow::syncIOfile(const QString &text)
{
    if (Qt::CheckState::Checked == ui->checkBox_CustomOutputFileName->checkState())
        return;
    QString input_pn = text;
    this->filename.clear();
    while (0 < input_pn.size() &&
           !('\\' == input_pn.back() ||
             '/' == input_pn.back()  ||
             ':' == input_pn.back())
           )
    {
        this->filename.push_front(input_pn.back());
        input_pn.remove(input_pn.size() - 1, 1);
    }
    fileFullPath = text;
    if (-1 == this->filename.indexOf('.', 0))
        this->filename.append(".txt");
    else
    {
        while('.' != filename.back())
        {
            filename.remove(filename.size() - 1, 1);
        }
        filename.append("txt");
    }
    ui->lineEdit_OutputFile->setText(this->filename);
}

void
MainWindow::browseDialog_InputFile()
{
    if (browser->exec())
        ui->lineEdit_InputFile->setText(browser->selectedFiles().at(0));
}

void
MainWindow::customizeSeparation()
{
    if (Qt::CheckState::Checked == ui->checkBox_CustomSeparation->checkState())
        ui->lineEdit_Separation->setReadOnly(false);
    else ui->lineEdit_Separation->setReadOnly(true);
}

void
MainWindow::customizeOutputFile()
{
    if (Qt::CheckState::Checked == ui->checkBox_CustomOutputFileName->checkState())
        ui->lineEdit_OutputFile->setReadOnly(false);
    else ui->lineEdit_OutputFile->setReadOnly(true);
}

void
MainWindow::exporting()
{
    // get and confirm the extend name
    QString extendname;
    if (-1 == fileFullPath) throw io_err(FILE_NOTDOCX, 2);
    for (int i = fileFullPath.size() - 1;
             '.' != fileFullPath.at(i)  && '/' != fileFullPath.at(i) &&
             '\\' != fileFullPath.at(i) && ':' != fileFullPath.at(i);
         i--)
    {
        extendname.push_front(fileFullPath.at(i));
    }
    try {
        if ("docx" != extendname) throw io_err(FILE_NOTDOCX, 2);
        // read from docx file
        docxfile = new DocxBuffer(fileFullPath.toLocal8Bit().operator const char *());
        docxfile->read();
        // get words from lines
        std::list<std::string> words;
        this->getWords(words);
        // rewrite into wordlist
        wordlist = new WordList_StringArray(wordnum);
        for (unsigned int i = 0; i < wordnum; i++)
        {
            wordlist->setElements(i, words.back());
            words.pop_back();
        }
        // pick words randomly
        unsigned int wordnum = (unsigned int)ui->spinBox_OutputWordNum->value();
        wordnum = wordnum > wordlist->size() ? wordlist->size() : wordnum;
        outputlist = new WordList_StringArray(wordnum);
        for (unsigned int i = 0; i < wordnum; i++)
        {
            srand((unsigned int)time(NULL));
            int randnum = rand();
            outputlist->setElements(i, wordlist->at(randnum % (int)wordlist->size()));
            wordlist->remove(randnum % wordlist->size());
        }
        // exporting
        output = new txtOutput_WordList(
                    desktopdir.append(ui->lineEdit_OutputFile->text()).toLocal8Bit().operator const char *(),
                    operation);
        std::string *list = new std::string[outputlist->size()];
        for (unsigned int i = 0; i < outputlist->size(); i++)
        {
            list[i] = outputlist->at(i);
        }
        output->set_wordlist(list, outputlist->size(),
                             str2escapestr(ui->lineEdit_Separation->text()).toLocal8Bit().operator const char *());
        output->close();
    } catch (const wordlist::io_err &e) {
        delete docxfile;
        delete wordlist;
        delete outputlist;
        delete output;
        if (FEXIST_INT == e.what()[0])
        {
            QMessageBox mgb;
            mgb.setDefaultButton(QMessageBox::Button::No);
            int ret = mgb.question(this, "File Aready Exist!", "File aready exist\nDo you want to cover it?");
            if (QMessageBox::Button::No == ret) return;
            else if (QMessageBox::Button::Yes == ret)
            {
                operation = wordlist::Operations::StrongWrite;
                exporting();
            }
        }
        else if (FNOTDOCX_INT == e.what()[0])
        {
            QMessageBox mgb;
            mgb.critical(this,
                         "File Type Not Matching",
                         "Input file is not a Docx file,\nplease chose a \".docx\" file");
            return;
        }
    } catch (const std::exception &e) {
        delete docxfile;
        delete wordlist;
        delete outputlist;
        delete output;
        throw e;
    }
        delete docxfile;
        delete wordlist;
        delete outputlist;
        delete output;
}

void
MainWindow::getWords(std::list<std::string> &words)
{
    for (unsigned int i = 0; i < docxfile->size(); i++)
    {
        std::string line = docxfile->get_Line(i);
        std::string word;
        while (!line.empty())
        {
            if (' '  != line[0] &&
                '\n' != line[0] &&
                '\r' != line[0] &&
                '\t' != line[0] &&
                '\v' != line[0]
                )
                word.push_back(line[0]);
            else
            {
                if (!word.empty())
                {
                    words.push_back(word);
                    wordnum++;
                    word.clear();
                }
            }
            line.erase(0, 1);
        }
        if (!word.empty())
        {
            wordnum++;
            words.push_back(word);
        }
    }
}

QString
MainWindow::str2escapestr(const QString &str)
{
    QString a = str;
    a.replace("\\n", "\n");
    a.replace("\\t", "\t");
    a.replace("\\r", "\r");
    a.replace("\\v", "\v");
    return a;
}
