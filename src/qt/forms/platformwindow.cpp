#include "platformwindow.h"
#include "ui_platformwindow.h"

PlatformWindow::PlatformWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlatformWindow)
{
    ui->setupUi(this);
}

PlatformWindow::~PlatformWindow()
{
    delete ui;
}

void PlatformWindow::on_PlatformWindow_destroyed()
{
    this->destroy(true);
}
