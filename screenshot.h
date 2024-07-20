#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QObject>
#include <QPixmap>

class Screenshot : public QObject
{
    Q_OBJECT

public:
    Screenshot(QObject *parent = nullptr);
    QPixmap takeScreenshot();

signals:
    void screenshotTaken(const QPixmap &pixmap);
};

#endif // SCREENSHOT_H
