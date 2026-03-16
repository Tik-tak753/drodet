#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "controller/detectioncontroller.h"

#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , controller(new DetectionController(this))
{
    ui->setupUi(this);

    connect(controller, &DetectionController::statusChanged, this, [this](const QString &status) {
        statusBar()->showMessage(status);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
