#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class DetectionController;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenImageRequested();
    void displayFrame(const QImage &frame);

private:
    Ui::MainWindow *ui;
    DetectionController *controller;
    QLabel *imageLabel;
};
#endif // MAINWINDOW_H
