#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "controller/detectioncontroller.h"

#include <QAction>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , controller(new DetectionController(this))
    , runtimeStateLabel(new QLabel(this))
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("Drodet - Drone Detection Demo"));

    runtimeStateLabel->setMinimumWidth(100);
    statusBar()->addPermanentWidget(runtimeStateLabel);

    setupConnections();
    updateUiState(false);
    setRuntimeState(QStringLiteral("Idle"), QStringLiteral("#6c757d"));

    statusBar()->showMessage(QStringLiteral("Ready"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenImageClicked()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Open Image"),
        QString(),
        QStringLiteral("Images (*.png *.jpg *.jpeg *.bmp)"));

    if (filePath.isEmpty()) {
        statusBar()->showMessage(QStringLiteral("Image selection canceled"), 3000);
        return;
    }

    ui->sourceValueLabel->setText(QStringLiteral("Image: %1").arg(filePath));
    ui->framePlaceholderLabel->setPixmap(QPixmap());
    ui->framePlaceholderLabel->setText(QStringLiteral("Image selected. Press Start to preview."));
    controller->openImage(filePath);
}

void MainWindow::onOpenVideoClicked()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Open Video"),
        QString(),
        QStringLiteral("Videos (*.mp4 *.avi *.mov *.mkv)"));

    if (filePath.isEmpty()) {
        statusBar()->showMessage(QStringLiteral("Video selection canceled"), 3000);
        return;
    }

    ui->sourceValueLabel->setText(QStringLiteral("Video: %1").arg(filePath));
    ui->framePlaceholderLabel->setPixmap(QPixmap());
    ui->framePlaceholderLabel->setText(QStringLiteral("Video selected. Press Start to preview."));
    controller->openVideo(filePath);
}

void MainWindow::onOpenCameraClicked()
{
    ui->sourceValueLabel->setText(QStringLiteral("Camera: default"));
    ui->framePlaceholderLabel->setPixmap(QPixmap());
    ui->framePlaceholderLabel->setText(QStringLiteral("Camera source selected (stub)."));
    statusBar()->showMessage(QStringLiteral("Camera source selected (controller wiring pending)"), 4000);
}

void MainWindow::onLoadModelClicked()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Load Model"),
        QString(),
        QStringLiteral("Model Files (*.onnx *.pt *.engine);;All Files (*)"));

    if (filePath.isEmpty()) {
        statusBar()->showMessage(QStringLiteral("Model selection canceled"), 3000);
        return;
    }

    ui->modelValueLabel->setText(QStringLiteral("Loaded: %1").arg(filePath));
    controller->loadModel(filePath);
}

void MainWindow::onStartClicked()
{
    controller->start();
    updateUiState(true);
    setRuntimeState(QStringLiteral("Running"), QStringLiteral("#198754"));
    ui->statusValueLabel->setText(QStringLiteral("Preview running"));
    statusBar()->showMessage(QStringLiteral("Started"), 3000);
}

void MainWindow::onStopClicked()
{
    controller->stop();
    updateUiState(false);
    setRuntimeState(QStringLiteral("Stopped"), QStringLiteral("#dc3545"));
    ui->statusValueLabel->setText(QStringLiteral("Stopped"));
    ui->framePlaceholderLabel->setPixmap(QPixmap());
    ui->framePlaceholderLabel->setText(QStringLiteral("Preview stopped. Select a source and press Start."));
}

void MainWindow::onThresholdChanged()
{
    const double confidence = static_cast<double>(ui->confidenceSpinBox->value());
    const double iou = static_cast<double>(ui->iouSpinBox->value());

    ui->statusValueLabel->setText(
        QStringLiteral("Thresholds updated: conf=%1 iou=%2")
            .arg(confidence, 0, 'f', 2)
            .arg(iou, 0, 'f', 2));
}

void MainWindow::setupConnections()
{
    connect(ui->openImageButton, &QPushButton::clicked, this, &MainWindow::onOpenImageClicked);
    connect(ui->openVideoButton, &QPushButton::clicked, this, &MainWindow::onOpenVideoClicked);
    connect(ui->openCameraButton, &QPushButton::clicked, this, &MainWindow::onOpenCameraClicked);
    connect(ui->loadModelButton, &QPushButton::clicked, this, &MainWindow::onLoadModelClicked);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::onStopClicked);

    connect(ui->actionOpen_Image, &QAction::triggered, this, &MainWindow::onOpenImageClicked);
    connect(ui->actionOpen_Video, &QAction::triggered, this, &MainWindow::onOpenVideoClicked);
    connect(ui->actionOpen_Camera, &QAction::triggered, this, &MainWindow::onOpenCameraClicked);
    connect(ui->actionLoad_Model, &QAction::triggered, this, &MainWindow::onLoadModelClicked);
    connect(ui->actionStart, &QAction::triggered, this, &MainWindow::onStartClicked);
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::onStopClicked);

    connect(ui->confidenceSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            [this](double) { onThresholdChanged(); });
    connect(ui->iouSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            [this](double) { onThresholdChanged(); });

    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, [this]() {
        statusBar()->showMessage(QStringLiteral("Drone detection UI shell (demo stub)"), 5000);
    });

    connect(controller, &DetectionController::statusChanged, this, [this](const QString &status) {
        ui->statusValueLabel->setText(status);
        statusBar()->showMessage(status, 3000);
    });

    connect(controller, &DetectionController::frameReady, this, [this](const QImage &frame) {
        if (frame.isNull()) {
            return;
        }

        ui->framePlaceholderLabel->setText(QString());
        const QPixmap pixmap = QPixmap::fromImage(frame).scaled(
            ui->framePlaceholderLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->framePlaceholderLabel->setPixmap(pixmap);
    });

    connect(controller, &DetectionController::statsReady, this, [this](double fps, int detections) {
        ui->fpsValueLabel->setText(QString::number(fps, 'f', 1));
        ui->detectionsValueLabel->setText(QString::number(detections));
    });
}

void MainWindow::updateUiState(bool running)
{
    ui->startButton->setEnabled(!running);
    ui->stopButton->setEnabled(running);
    ui->actionStart->setEnabled(!running);
    ui->actionStop->setEnabled(running);
}

void MainWindow::setRuntimeState(const QString &stateText, const QString &color)
{
    runtimeStateLabel->setText(stateText);
    runtimeStateLabel->setStyleSheet(
        QStringLiteral("QLabel { color: white; font-weight: 600; padding: 2px 8px; border-radius: 8px; background: %1; }")
            .arg(color));
}
