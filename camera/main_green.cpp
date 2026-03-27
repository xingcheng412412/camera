#include <QApplication>
#include "green_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("GigE Line-Scan Inspector — Green Beans");

    GreenWindow w;
    w.show();

    return app.exec();
}
