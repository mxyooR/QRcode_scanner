#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class MainWindow;

class TrayIcon : public QObject
{
    Q_OBJECT
public:
    explicit TrayIcon(MainWindow *mainWindow, QObject *parent = nullptr);

private slots:
    void showMainWindow();
    void quitApp();
    void onActivate(QSystemTrayIcon::ActivationReason reason);

private:
    void createActions();
    void createTrayIcon();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *showAction;
    QAction *quitAction;
    MainWindow *mainWindow;
};

#endif // TRAYICON_H
