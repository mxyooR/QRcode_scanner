#ifndef GLOBALSHORTCUT_H
#define GLOBALSHORTCUT_H

#include <QObject>
#include <QTimer>

class GlobalShortcut : public QObject
{
    Q_OBJECT

public:
    GlobalShortcut(QObject *parent = nullptr);
    ~GlobalShortcut();

signals:
    void shortcutActivated();

private slots:
    void checkShortcut();

private:
    QTimer *timer;
    bool isCtrlAltDPressed();
};

#endif // GLOBALSHORTCUT_H
