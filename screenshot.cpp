#include "screenshot.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDateTime>
#include <QDir>

Screenshot::Screenshot(QObject *parent) : QObject(parent)
{
}

QPixmap Screenshot::takeScreenshot()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        return QPixmap();
    }

    QPixmap pixmap = screen->grabWindow(0);
    emit screenshotTaken(pixmap);
    return pixmap;
}
