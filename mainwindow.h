#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "screenshot.h"
#include "globalshortcut.h"
#include "trayicon.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GlobalShortcut;
class TrayIcon;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onShortcutActivated();
    void onScreenshotTaken(const QPixmap &pixmap);
    void recognizeQRCode(const QPixmap &pixmap);
    void onAutoRunCheckBoxToggled(bool checked);

private:
    Ui::MainWindow *ui;
    Screenshot *screenshot;
    GlobalShortcut *globalShortcut;
    TrayIcon *trayIcon;  // 声明 TrayIcon 成员变量
    void updateAutoRunSetting();
};

#endif // MAINWINDOW_H

