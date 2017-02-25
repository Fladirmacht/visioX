#ifndef IPFSDATADIALOG_H
#define IPFSDATADIALOG_H

#include <QDialog>

namespace Ui {
class IpfsDataDialog;
}
class WalletModel;

class IpfsDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IpfsDataDialog(QWidget *parent = 0);
    ~IpfsDataDialog();

    void setModel(WalletModel *model);
    bool validate();


private:
    Ui::IpfsDataDialog *ui;
    WalletModel *model;
    bool fNewRecipientAllowed;

private slots:

    void on_pushButton_clicked();
};


#endif // IPFSDATADIALOG_H
