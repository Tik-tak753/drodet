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
    void onOpenImageClicked();
    void onOpenVideoClicked();
    void onOpenCameraClicked();
    void onLoadModelClicked();
    void onStartClicked();
    void onStopClicked();
    void onThresholdChanged();

private:
    void setupConnections();
    void updateUiState(bool running);
    void setRuntimeState(const QString &stateText, const QString &color);

    Ui::MainWindow *ui;
    DetectionController *controller;
    QLabel *runtimeStateLabel;
};
#endif // MAINWINDOW_H
