#include "qrcodedialog.h"
#include "ui_qrcodedialog.h"
#include <QClipboard>
#include <QApplication>

QRCodeDialog::QRCodeDialog(const QString &qrCodeText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRCodeDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->qrCodeTextLabel->setText(qrCodeText);

    connect(ui->closeButton, &QPushButton::clicked, this, &QRCodeDialog::accept);
    connect(ui->copyButton, &QPushButton::clicked, this, &QRCodeDialog::copyToClipboard);
}

QRCodeDialog::~QRCodeDialog()
{
    delete ui;
}

void QRCodeDialog::copyToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->qrCodeTextLabel->text());
}
