#include "qrcodedialog.h"
#include "ui_qrcodedialog.h"
#include <QClipboard>
#include <QApplication>

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
}

QRCodeDialog::~QRCodeDialog()
{
    delete ui;
}

// 将二维码文本复制到剪贴板的槽函数
void QRCodeDialog::copyToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->qrCodeTextLabel->text());
}
