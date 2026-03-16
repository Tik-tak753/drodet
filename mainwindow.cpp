#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "controller/detectioncontroller.h"

#include <QAction>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QStatusBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , controller(new DetectionController(this))
    , imageLabel(new QLabel(this))
{
    ui->setupUi(this);

    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText(QStringLiteral("Open an image to start"));

    auto *layout = new QVBoxLayout(ui->centralwidget);
    layout->addWidget(imageLabel);
    ui->centralwidget->setLayout(layout);

    auto *fileMenu = menuBar()->addMenu(QStringLiteral("File"));
    auto *openImageAction = fileMenu->addAction(QStringLiteral("Open Image..."));
    connect(openImageAction, &QAction::triggered, this, &MainWindow::onOpenImageRequested);

    connect(controller, &DetectionController::statusChanged, this, [this](const QString &status) {
        statusBar()->showMessage(status);
    });
    connect(controller, &DetectionController::frameReady, this, &MainWindow::displayFrame);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenImageRequested()
{
    const QString imagePath = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Select image"),
        QString(),
        QStringLiteral("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (imagePath.isEmpty()) {
        return;
    }

    controller->openImage(imagePath);
}

void MainWindow::displayFrame(const QImage &frame)
{
    if (frame.isNull()) {
        imageLabel->setText(QStringLiteral("Failed to display image"));
        return;
    }

    const QPixmap pixmap = QPixmap::fromImage(frame);
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
