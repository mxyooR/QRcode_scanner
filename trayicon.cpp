#include "trayicon.h"
#include "mainwindow.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>

TrayIcon::TrayIcon(MainWindow *mainWindow, QObject *parent)
    : QObject(parent), mainWindow(mainWindow)
{
    qDebug() << "Creating tray icon";

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/tray_icon.png"));
    trayIcon->setToolTip("QR Code Scanner");

    trayIconMenu = new QMenu();

    createActions();
    createTrayIcon();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayIcon::onActivate);

    trayIcon->show();
    qDebug() << "Tray icon created and shown";
}

void TrayIcon::createActions()
{
    showAction = new QAction(tr("显示主窗口"), this);
    quitAction = new QAction(tr("退出"), this);

    connect(showAction, &QAction::triggered, this, &TrayIcon::showMainWindow);
    connect(quitAction, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);

    trayIconMenu->addAction(showAction);
    trayIconMenu->addAction(quitAction);
}

void TrayIcon::createTrayIcon()
{
    trayIcon->setContextMenu(trayIconMenu);
}

void TrayIcon::showMainWindow()
{
    if (mainWindow) {
        mainWindow->show();
        mainWindow->raise();
        mainWindow->activateWindow();
    }
}

void TrayIcon::quitApp()
{
    QCoreApplication::quit();
}

void TrayIcon::onActivate(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        mainWindow->show();
        mainWindow->raise();
        mainWindow->activateWindow();
    }
}

void TrayIcon::showMessage(const QString &title, const QString &message)
{
    trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 3000);
}
