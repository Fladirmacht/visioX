#include "ipfsdatadialog.h"
#include "ui_ipfsdatadialog.h"
#include "walletmodel.h"
#include <iostream>
#include "bitcoingui.h"
#include <QMessageBox>
#include "addressbookpage.h"
#include "walletmodel.h"
#include "optionsmodel.h"
#include "addresstablemodel.h"
#include "bitcoinunits.h"

#include "base58.h"
//#include "coincontrol.h"
//#include "coincontroldialog.h"

IpfsDataDialog::IpfsDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpfsDataDialog),
    model(0),
    fNewRecipientAllowed(true)

{
    ui->setupUi(this);
#if QT_VERSION >= 0x040700
    /* Do not move this to the XML file, Qt before 4.7 will choke on it */
    ui->datatextEdit->setPlaceholderText(tr("Enter data which will be save in blockchain.."));
    ui->payTo->setPlaceholderText(tr("Enter a Visio address (e.g. VjBj1QvJvsAkU5EBKggdZ8gWc4oK2F5AMY)"));
#endif
}

IpfsDataDialog::~IpfsDataDialog()
{
    delete ui;
}

void IpfsDataDialog::setModel(WalletModel *mode)
{
    this->model = mode;
}


void IpfsDataDialog::on_pushButton_clicked()
{
    QString str = ui->datatextEdit->toPlainText();
    if (str.length()>0 && str.length()<2048 && this->validate()) {

        if(!model || !model->getOptionsModel())
            return;

        QList<SendCoinsRecipient> recipients;
        SendCoinsRecipient rec;
        QString addr = ui->payTo->text();
        rec.address = addr;
        rec.amount = TX_DATAMESSAGE_FEE;
        rec.label =model->getAddressTableModel()->labelForAddress(addr);
        rec.datamessage = ui->datatextEdit->toPlainText();
        rec.txtype = TX_TYPE_DATAMESAGE;
        recipients.insert(recipients.end(),rec);


        // Format confirmation message
        QStringList formatted;
        foreach(const SendCoinsRecipient &rcp, recipients)
        {
            formatted.append(tr("<b>%1</b> to %2 (%3)").arg(BitcoinUnits::formatWithUnit(model->getOptionsModel()->getDisplayUnit(), rcp.amount), Qt::escape(rcp.label), rcp.address));
        }

        fNewRecipientAllowed = false;

        QMessageBox::StandardButton retval = QMessageBox::question(this, tr("Confirm send coins"),
                              tr("Are you sure you want to send %1?").arg(formatted.join(tr(" and "))),
              QMessageBox::Yes|QMessageBox::Cancel,
              QMessageBox::Cancel);

        if(retval != QMessageBox::Yes)
        {
            fNewRecipientAllowed = true;
            return;
        }

        WalletModel::UnlockContext ctx(model->requestUnlock());
        if(!ctx.isValid())
        {
            // Unlock wallet was cancelled
            fNewRecipientAllowed = true;
            return;
        }

        WalletModel::SendCoinsReturn sendstatus;

//        if (!model->getOptionsModel() || !model->getOptionsModel()->getCoinControlFeatures())
            sendstatus = model->sendCoins(recipients);
//        else
//            sendstatus = model->sendCoins(recipients, CoinControlDialog::coinControl);

        switch(sendstatus.status)
        {
        case WalletModel::InvalidAddress:
            QMessageBox::warning(this, tr("Send Coins"),
                tr("The recipient address is not valid, please recheck."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case WalletModel::InvalidAmount:
            QMessageBox::warning(this, tr("Send Coins"),
                tr("The amount to pay must be larger than 0."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case WalletModel::AmountExceedsBalance:
            QMessageBox::warning(this, tr("Send Coins"),
                tr("The amount exceeds your balance."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case WalletModel::AmountWithFeeExceedsBalance:
            QMessageBox::warning(this, tr("Send Coins"),
                tr("The total exceeds your balance when the %1 transaction fee is included.").
                arg(BitcoinUnits::formatWithUnit(model->getOptionsModel()->getDisplayUnit(), sendstatus.fee)),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case WalletModel::DuplicateAddress:
            QMessageBox::warning(this, tr("Send Coins"),
                tr("Duplicate address found, can only send to each address once per send operation."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case WalletModel::TransactionCreationFailed:
            QMessageBox::warning(this, tr("Send Coins"),
                tr("Error: Transaction creation failed!"),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case WalletModel::TransactionCommitFailed:
            QMessageBox::warning(this, tr("Send Coins"),
                tr("Error: The transaction was rejected. This might happen if some of the coins in your wallet were already spent, such as if you used a copy of wallet.dat and coins were spent in the copy but not marked as spent here."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case WalletModel::Aborted: // User aborted, nothing to do
            break;
        case WalletModel::OK:
            accept();
//            CoinControlDialog::coinControl->UnSelectAll();
//            coinControlUpdateLabels();
            break;
        }
        fNewRecipientAllowed = true;







    close();
    } else {
        QMessageBox *qm = new QMessageBox();
        qm->setText("Enter valid VISIO address and some COOL data");
        qm->setModal(true);
        qm->exec();
    }
}
bool IpfsDataDialog::validate()
{
    // Check input validity
    bool retval = true;

    if(!ui->payTo->hasAcceptableInput() || (model && !model->validateAddress(ui->payTo->text())))
    {
        retval = false;
    }

    return retval;
}
