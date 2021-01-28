#include "SteamH264Demo.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "ChattingDialog.h"
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    SteamH264Demo w;
    w.show();
    return a.exec();
}
