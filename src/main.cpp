#include "header/mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "header/define.h"

int main(int argc, char *argv[])
{
    int ret = -1;
    QApplication a(argc, argv);
    try {
        MainWindow w;
        w.show();

        ret = a.exec();
    } catch (const std::exception &e) {
        std::string errstr = "Error: \"";
        errstr += e.what();
        errstr += "\" was caught! \nThe program is aborted.";
        QMessageBox mgb;
        mgb.critical(nullptr, "Error", errstr.c_str());
    } catch (...) {
        QMessageBox mgb;
        mgb.critical(nullptr, "Error", "Unkown Error");
    }
    return ret;
}
