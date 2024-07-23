#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QWidget>
#include <QPixmap>

class Screenshot : public QWidget
{
    Q_OBJECT

public:
    explicit Screenshot(QWidget *parent = nullptr);
    void startScreenshot();

signals:
    void screenshotTaken(const QPixmap &pixmap);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap originalPixmap;
    bool selecting;
    QPoint selectionStart;
    QPoint selectionEnd;
    qreal devicePixelRatio;
};

#endif // SCREENSHOT_H
