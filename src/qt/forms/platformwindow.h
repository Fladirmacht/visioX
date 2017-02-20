#ifndef PLATFORMWINDOW_H
#define PLATFORMWINDOW_H

#include <QWidget>

namespace Ui {
class PlatformWindow;
}

class PlatformWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlatformWindow(QWidget *parent = 0);
    ~PlatformWindow();

private slots:
    void on_PlatformWindow_destroyed();

private:
    Ui::PlatformWindow *ui;
};

#endif // PLATFORMWINDOW_H
