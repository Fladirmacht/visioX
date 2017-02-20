#include "platfrmwindow.h"
#include "ui_platfrmwindow.h"

PlatfrmWindow::PlatfrmWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlatfrmWindow)
{
    ui->setupUi(this);
    this->setStatusBar(0);    
}

PlatfrmWindow::~PlatfrmWindow()
{
     delete ui;
}

void PlatfrmWindow::on_PlatfrmWindow_destroyed()
{
    this->destroy(true,true);
}

