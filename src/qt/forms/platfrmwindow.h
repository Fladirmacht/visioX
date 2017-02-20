#ifndef PLATFRMWINDOW_H
#define PLATFRMWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class PlatfrmWindow;
}

class PlatfrmWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlatfrmWindow(QWidget *parent = 0);
    ~PlatfrmWindow();

private slots:
    void on_PlatfrmWindow_destroyed();

private:
    Ui::PlatfrmWindow *ui;
};

#endif // PLATFRMWINDOW_H
