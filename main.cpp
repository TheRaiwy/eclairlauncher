#include <QApplication>
#include "eclair_launcher.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EclairLauncher launcher;
    launcher.show();
    return app.exec();
}
