#ifndef BANSHEE_H
#define BANSHEE_H

#include <QMainWindow>

namespace Ui {
class banshee;
}

class banshee : public QMainWindow
{
    Q_OBJECT

public:
    explicit banshee(QWidget *parent = 0);
    ~banshee();

private slots:
    void on_write_pushButton_clicked();

private:
    Ui::banshee *ui;
};

#endif // BANSHEE_H
