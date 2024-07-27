#include "qrcodedialog.h"
#include "ui_qrcodedialog.h"
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>

QRCodeDialog::QRCodeDialog(const QString &qrCodeText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRCodeDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);// 使窗口保持在最顶部
    ui->qrCodeTextLabel->setText(qrCodeText);

    // 连接关闭按钮的clicked信号到对话框的accept槽，关闭对话框
    connect(ui->closeButton, &QPushButton::clicked, this, &QRCodeDialog::accept);
    // 连接复制按钮的clicked信号到copyToClipboard槽，复制文本到剪贴板
    connect(ui->copyButton, &QPushButton::clicked, this, &QRCodeDialog::copyToClipboard);
    // 设置固定宽度为400
    this->setFixedWidth(400);
    // 设置QLabel的最大宽度和大小策略
    ui->qrCodeTextLabel->setMinimumWidth(360);
    ui->qrCodeTextLabel->setMaximumWidth(360);
    ui->qrCodeTextLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    // 设置滚动区域的大小策略
    ui->scrollArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    // 设置样式
    this->setStyleSheet(R"(
        QDialog {
            background-color: #f0f0f0;
        }
        QLabel {
            background-color: white;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 10px;
        }
        QPushButton {
            background-color: #266ebc;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #1652ab;
        }
    )");
}

QRCodeDialog::~QRCodeDialog()
{
    delete ui;
}

void QRCodeDialog::copyToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->qrCodeTextLabel->text());

    QMessageBox::information(this, "复制成功", "二维码内容已成功复制到剪贴板。", QMessageBox::Ok);
}
