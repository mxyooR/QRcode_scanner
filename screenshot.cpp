#include "screenshot.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

Screenshot::Screenshot(QWidget *parent)
    : QWidget(parent), selecting(false)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::CrossCursor);
}

void Screenshot::startScreenshot()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qDebug() << "No primary screen found!";
        return;
    }
    originalPixmap = screen->grabWindow(0);
    resize(screen->size());
    //showFullScreen();
}

void Screenshot::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        selecting = true;
        selectionStart = event->pos();
        selectionEnd = selectionStart;
        update();
    }
}

void Screenshot::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting) {
        selectionEnd = event->pos();
        update();
    }
}

void Screenshot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        selecting = false;
        QRect selectedRect = QRect(selectionStart, selectionEnd).normalized();
        QPixmap selectedPixmap = originalPixmap.copy(selectedRect);
        emit screenshotTaken(selectedPixmap);
        close();
    }
}

void Screenshot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(0, 0, originalPixmap);

    if (selecting) {
        QRect selectedRect = QRect(selectionStart, selectionEnd).normalized();
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(selectedRect);
    }
}
