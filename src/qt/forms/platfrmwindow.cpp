#include "platfrmwindow.h"
#include "ui_platfrmwindow.h"
#include <QWebView>
#include <QWebFrame>
PlatfrmWindow::PlatfrmWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlatfrmWindow)
{
    ui->setupUi(this);
    this->setStatusBar(0);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
        ui->webView->load(QUrl("http://visio.wtf"));
//        ui->webView->load(QUrl("http://ipfs2.purevidz.net:8080/ipfs/QmP3BxwEt96gNc3GKxS2sr7iBJAHE6edyjM8kuXSJAt4P5"));
        ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);
}

PlatfrmWindow::~PlatfrmWindow()
{
     delete ui;
}

void PlatfrmWindow::on_PlatfrmWindow_destroyed()
{
    this->destroy(true,true);
}

