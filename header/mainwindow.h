#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QFileDialog>

#include "header/wordlist_stringarray.h"
#include "header/docxbuffer.h"
#include "txtoutput_wordlist.h"
#include "header/define.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Captions
constexpr auto BROWSER_CAPTION_INPUTFILE = "Select Input File";
// Missing warnings
constexpr auto MISSINGWARN_INPUTFILE = "You must tell the input file";
// Hints
constexpr auto HINT_OUTPUTFILE       = "Default input file name";
constexpr auto HINT_SEPARATION       = "\\n";

using namespace wordlist;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // open a new file
    void open();

private:
    Ui::MainWindow *ui;
    QString filename;
    QString fileFullPath;
    QFileDialog *browser;

    // word picking job
    byte operation;
    unsigned int wordnum;
    WordList_StringArray *wordlist;
    WordList_StringArray *outputlist;
    DocxBuffer *docxfile;
    txtOutput_WordList *output;
    // method
    void getWords(std::list<std::string> &words);
//    void exportTextFile(const QString &filename)

    // static
    static QString desktopdir;
    static QString str2escapestr(const QString &str);

private slots:
    void syncIOfile(const QString &text);
    void browseDialog_InputFile();
    void customizeSeparation();
    void customizeOutputFile();
    void exporting();

};
#endif // MAINWINDOW_H
