#include "mainwindow.h"
#include "qpushbutton.h"
#include "ui_mainwindow.h"
#include "qrcodedialog.h"
#include "trayicon.h"
#include "globalshortcut.h"
#include "screenshot.h"

#include <QApplication>
#include <QClipboard>
#include <QGuiApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QScreen>
#include <QTimer>
#include <QSettings>
#include <QDir>
#include <QCloseEvent>


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
    , screenshot(new Screenshot(this))
    , globalShortcut(new GlobalShortcut(this))
    , trayIcon(new TrayIcon(this, this))  // 初始化 TrayIcon 成员变量
{
    ui->setupUi(this);

    // 设置全局快捷键
    connect(globalShortcut, &GlobalShortcut::shortcutActivated, this, &MainWindow::onShortcutActivated);

    // 连接 AutoRunCheckBox 的信号和槽
    connect(ui->AutoRuncheckBox, &QCheckBox::toggled, this, &MainWindow::onAutoRunCheckBoxToggled);

    // 连接截图信号
    connect(screenshot, &Screenshot::screenshotTaken, this, &MainWindow::onScreenshotTaken);

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
    screenshot->startScreenshot();
}

void MainWindow::recognizeQRCode(const QPixmap &pixmap)
{
    qDebug() << "Converting QPixmap to cv::Mat...";
    cv::Mat mat = QPixmapToCvMat(pixmap);
    if (mat.empty()) {
        qDebug() << "Conversion failed, empty cv::Mat.";
        return;
    }
    qDebug() << "Conversion done. Detecting QR code...";

    cv::QRCodeDetector qr_decoder;
    std::vector<cv::Point> points;
    std::string decoded_info = qr_decoder.detectAndDecode(mat);

    qDebug() << "QR code detection finished.";

    if (!decoded_info.empty()) {
        qDebug() << "QR Code content:" << QString::fromStdString(decoded_info);
        QMetaObject::invokeMethod(this, "showQRCodeDialog", Qt::QueuedConnection,
                                  Q_ARG(QString, QString::fromStdString(decoded_info)));
    } else {
        QMetaObject::invokeMethod(this, "showNoQRCodeMessage", Qt::QueuedConnection);
    }
}

// 添加这两个新的槽函数
void MainWindow::showQRCodeDialog(const QString &content)
{
    QRCodeDialog dialog(content, this);
    dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowStaysOnTopHint);
    dialog.exec();
}

void MainWindow::showNoQRCodeMessage()
{
    QMessageBox::information(this, tr("QR Code Content"), tr("没有检测到二维码"), QMessageBox::Ok);
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


void MainWindow::closeEvent(QCloseEvent *event)
{
    // 弹出对话框，询问用户选择
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("关闭程序"));
    msgBox.setText(tr("您想隐藏到托盘还是关闭程序?"));

    QPushButton *hideButton = msgBox.addButton(tr("隐藏到托盘"), QMessageBox::ActionRole);
    QPushButton *closeButton = msgBox.addButton(tr("关闭程序"), QMessageBox::ActionRole);
    msgBox.setDefaultButton(hideButton);

    msgBox.exec();

    if (msgBox.clickedButton() == hideButton) {
        // 隐藏到托盘
        this->hide();
        trayIcon->showMessage("QR Code Scanner", "程序已隐藏到托盘。");
        event->ignore();
    } else if (msgBox.clickedButton() == closeButton) {
        // 关闭程序
        event->accept();
    } else {
        // 用户取消关闭操作
        event->ignore();
    }
}

void MainWindow::onScreenshotTaken(const QPixmap &pixmap)
{
    qDebug() << "Screenshot taken, starting QR code recognition...";

    // 在新线程中执行识别
    recognitionFuture = QtConcurrent::run([this, pixmap]() {
        this->recognizeQRCode(pixmap);
    });
}

