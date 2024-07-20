#ifndef QRCODEDIALOG_H
#define QRCODEDIALOG_H

#include <QDialog>

namespace Ui {
class QRCodeDialog;
}

class QRCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QRCodeDialog(const QString &qrCodeText, QWidget *parent = nullptr);
    ~QRCodeDialog();

private slots:
    void copyToClipboard();

private:
    Ui::QRCodeDialog *ui;
};

#endif // QRCODEDIALOG_H
