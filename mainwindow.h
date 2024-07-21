#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GlobalShortcut;
class TrayIcon;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onShortcutActivated();
    void recognizeQRCode(const QPixmap &pixmap);
    void onAutoRunCheckBoxToggled(bool checked);

private:
    Ui::MainWindow *ui;
    GlobalShortcut *globalShortcut;
    TrayIcon *trayIcon;  // 声明 TrayIcon 成员变量
    void updateAutoRunSetting();
};

#endif // MAINWINDOW_H
