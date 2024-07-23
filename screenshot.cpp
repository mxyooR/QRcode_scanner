#include "screenshot.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>


Screenshot::Screenshot(QWidget *parent)
    : QWidget(parent), selecting(false), devicePixelRatio(1.0)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::CrossCursor);

    if (QGuiApplication::primaryScreen()) {
        devicePixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();
    }
}

void Screenshot::startScreenshot()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qDebug() << "No primary screen found!";
        return;
    }

    // 获取物理屏幕尺寸
    QSize physicalSize = screen->size() * devicePixelRatio;
    qDebug() << "Physical screen size:" << physicalSize;

    // 获取逻辑屏幕尺寸
    QSize logicalSize = screen->size();
    qDebug() << "Logical screen size:" << logicalSize;

    // 打印其他有用的信息
    qDebug() << "Device pixel ratio:" << devicePixelRatio;
    qDebug() << "Logical DPI:" << screen->logicalDotsPerInch();
    qDebug() << "Physical DPI:" << screen->physicalDotsPerInch();

    // 使用物理尺寸抓取屏幕
    originalPixmap = screen->grabWindow(0);

    // 使用逻辑尺寸设置窗口大小
    resize(logicalSize);
    showFullScreen();
}

void Screenshot::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        selecting = true;
        selectionStart = event->pos() * devicePixelRatio;
        selectionEnd = selectionStart;
        update();
    }
}

void Screenshot::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting) {
        selectionEnd = event->pos() * devicePixelRatio;
        update();
    }
}

void Screenshot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        selecting = false;
        QRect selectedRect = QRect(selectionStart, selectionEnd).normalized();
        if (!selectedRect.isEmpty()) {
            QPixmap selectedPixmap = originalPixmap.copy(selectedRect);
            emit screenshotTaken(selectedPixmap);
        }
        this->close();  // 确保窗口关闭
    }
}


void Screenshot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // 绘制原始截图，填充整个窗口
    painter.drawPixmap(rect(), originalPixmap);

    if (selecting) {
        QRect selectedRect = QRect(selectionStart / devicePixelRatio, selectionEnd / devicePixelRatio).normalized();
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(selectedRect);
    }
}
