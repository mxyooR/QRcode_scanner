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
    , trayIcon(new TrayIcon(this, this))
{
    ui->setupUi(this);

    // 设置全局快捷键
    connect(globalShortcut, &GlobalShortcut::shortcutActivated, this, &MainWindow::onShortcutActivated);
    // 连接 AutoRunCheckBox 的信号和槽
    connect(ui->AutoRuncheckBox, &QCheckBox::toggled, this, &MainWindow::onAutoRunCheckBoxToggled);
    // 连接截图信号
    connect(screenshot, &Screenshot::screenshotTaken, this, &MainWindow::onScreenshotTaken);
    connect(ui->closeButton, &QPushButton::clicked, this, &QMainWindow::close);
    // 初始化 AutoRunCheckBox 的状态
    updateAutoRunSetting();

    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #f0f0f0;
            border: 1px solid #cccccc;
            border-radius: 10px;
        }
        QWidget#centralwidget {
            background-color: transparent;
        }
        QLabel#titleLabel {
            font-family: "黑体";
            font-size: 24px;
            font-weight: bold;
            color: #266ebc;
            margin-bottom: 20px;
        }
        QCheckBox {
            font-family: "黑体";
            font-size: 16px;
            color: #333;
        }
        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border-radius: 10px;
        }
        QCheckBox::indicator:unchecked {
            border: 2px solid #266ebc;
            background-color: white;
        }
        QCheckBox::indicator:checked {
            background-color: #266ebc;
            border: 2px solid #266ebc;
        }
        QCheckBox::indicator:checked::after {
            content: "✓";
            color: white;
            font-size: 16px;
            position: absolute;
            top: -2px;
            left: 3px;
        }
        QPushButton#closeButton {
            background-color: #266ebc;
            border: none;
            color: white;
            font-weight: bold;
            border-radius: 15px;
            min-width: 30px;
            max-width: 30px;
            min-height: 30px;
            max-height: 30px;
        }
        QPushButton#closeButton:hover {
            background-color: #ff4d4d;
        }
    )");

    // 移除窗口边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 允许自定义窗口移动
    this->setAttribute(Qt::WA_TranslucentBackground);
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
    QString appPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
    QString value = settings.value(appName).toString();

    // Check if the appPath contains the --startup argument
    bool isAutoRun = value.contains(QString("\"%1\" --startup").arg(appPath), Qt::CaseInsensitive);

    ui->AutoRuncheckBox->setChecked(isAutoRun);
}

void MainWindow::onAutoRunCheckBoxToggled(bool checked)
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString appName = QApplication::applicationName();
    QString appPath = QDir::toNativeSeparators(QApplication::applicationFilePath());

    if (checked) {
        // Set the value with --startup argument
        QString value = QString("\"%1\" --startup").arg(appPath);
        settings.setValue(appName, value);
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

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

