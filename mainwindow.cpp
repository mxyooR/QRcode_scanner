#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qrcodedialog.h"
#include "trayicon.h"
#include "globalshortcut.h"

#include <QApplication>
#include <QClipboard>
#include <QGuiApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QScreen>
#include <QTimer>
#include <QSettings>
#include <QDir>


#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/objdetect.hpp>

cv::Mat QPixmapToCvMat(const QPixmap &pixmap)
{
    QImage image = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
    cv::Mat mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    return mat.clone();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , globalShortcut(new GlobalShortcut(this))
    , trayIcon(new TrayIcon(this, this))  // 初始化 TrayIcon 成员变量
{
    ui->setupUi(this);

    // 设置全局快捷键
    connect(globalShortcut, &GlobalShortcut::shortcutActivated, this, &MainWindow::onShortcutActivated);
    // 连接 AutoRunCheckBox 的信号和槽
    connect(ui->AutoRuncheckBox, &QCheckBox::toggled, this, &MainWindow::onAutoRunCheckBoxToggled);

    // 初始化 AutoRunCheckBox 的状态
    updateAutoRunSetting();

    this->setStyleSheet("QMainWindow {"
                        "    background-color: #f0f0f0;"
                        "    border: 1px solid #ccc;"
                        "}"
                        "QPushButton {"
                        "    background-color: #4CAF50;"
                        "    color: white;"
                        "    border: none;"
                        "    padding: 10px 24px;"
                        "    text-align: center;"
                        "    text-decoration: none;"
                        "    display: inline-block;"
                        "    font-size: 16px;"
                        "    margin: 4px 2px;"
                        "    border-radius: 8px;"
                        "}"
                        "QPushButton:hover {"
                        "    background-color: #45a049;"
                        "}"
                        "QLabel {"
                        "    font-size: 18px;"
                        "    color: #333;"
                        "}");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete trayIcon;  // 删除 TrayIcon 成员变量
}

void MainWindow::onShortcutActivated()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QPixmap pixmap = screen->grabWindow(0);
        recognizeQRCode(pixmap);
    }
}

void MainWindow::recognizeQRCode(const QPixmap &pixmap)
{
    cv::Mat mat = QPixmapToCvMat(pixmap);
    std::vector<cv::Point> points;
    std::string decoded_info;

    cv::QRCodeDetector qr_decoder;
    decoded_info = qr_decoder.detectAndDecode(mat, points);

    if (!decoded_info.empty()) {
        QRCodeDialog dialog(QString::fromStdString(decoded_info), this);
        dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowStaysOnTopHint);
        dialog.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("No QR Code detected!"));
        msgBox.setWindowTitle(tr("QR Code Content"));
        msgBox.exec();
    }
}

void MainWindow::updateAutoRunSetting()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString appName = QApplication::applicationName();
    bool isAutoRun = settings.contains(appName);

    ui->AutoRuncheckBox->setChecked(isAutoRun);
}

void MainWindow::onAutoRunCheckBoxToggled(bool checked)
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString appName = QApplication::applicationName();
    QString appPath = QDir::toNativeSeparators(QApplication::applicationFilePath());

    if (checked) {
        settings.setValue(appName, appPath);
    } else {
        settings.remove(appName);
    }
}
