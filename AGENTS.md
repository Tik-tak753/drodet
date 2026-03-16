Project overview

This repository contains a C++ desktop application built with Qt Widgets.

The purpose of the project is to create a drone detection demo application using a YOLO-based model.
The project must keep a clean and modular architecture so the detection model can be integrated later.

Development environment:

OS: Windows

IDE: Qt Creator

Build system: qmake

Language: C++

GUI: Qt Widgets

Computer vision: OpenCV (planned integration)

Architecture

The project must follow a layered architecture where the UI is separated from application logic.

Main components:

UI layer

MainWindow

Controller layer

DetectionController

Detection layer

YoloDetector

DetectionResult

Frame sources

IFrameSource

CameraSource

VideoFileSource

ImageSource

Utilities

FrameProcessor

Responsibilities
MainWindow

MainWindow must only handle UI logic.

MainWindow responsibilities:

reacting to user actions

forwarding commands to DetectionController

displaying frames

displaying FPS

displaying number of detections

displaying status messages

MainWindow must NOT:

load models

perform detection

implement camera logic

implement inference logic

DetectionController

DetectionController is the central coordinator of the application.

Responsibilities:

receive commands from UI

manage frame sources

call the detector

process results

send frames and statistics back to UI

Use Qt signals/slots for communication.

Expected signals:

frameReady(const QImage&)

statsReady(double fps, int detections)

statusChanged(const QString&)

YoloDetector

YoloDetector encapsulates model interaction.

Responsibilities:

loading model files

running detection

returning DetectionResult

The detector must be independent from UI.

At early stages it is acceptable to implement a stub detector.

Example stub comment:

// TODO: implement real YOLO inference here
Detection structures

Detection data should not be passed as raw values.

Use structured data types.

Example structure:

struct DetectionBox
{
    QRect rect;
    QString className;
    float confidence;
};

struct DetectionResult
{
    QVector<DetectionBox> boxes;
    int detectionsCount = 0;
    double inferenceMs = 0.0;
};
Frame sources

Frame acquisition must be abstracted.

Create interface:

class IFrameSource
{
public:
    virtual ~IFrameSource() = default;

    virtual bool open(const QString& source) = 0;
    virtual bool read(cv::Mat& frame) = 0;
    virtual void close() = 0;
    virtual bool isOpened() const = 0;
};

Implementations:

CameraSource

VideoFileSource

ImageSource

DetectionController must interact with them through the interface.

Utilities

Create helper utilities for frame operations.

FrameProcessor should contain:

cv::Mat ? QImage conversion

drawing detection boxes

drawing overlay information (FPS, detections)

Project structure

The repository should maintain this layout:

project_root
?
??? main.cpp
??? mainwindow.h
??? mainwindow.cpp
??? mainwindow.ui
?
??? controller
?   ??? detectioncontroller.h
?   ??? detectioncontroller.cpp
?
??? detection
?   ??? yolodetector.h
?   ??? yolodetector.cpp
?   ??? detectionresult.h
?
??? sources
?   ??? iframesource.h
?   ??? camerasource.h
?   ??? camerasource.cpp
?   ??? videofilesource.h
?   ??? videofilesource.cpp
?   ??? imagesource.h
?   ??? imagesource.cpp
?
??? utils
    ??? frameprocessor.h
    ??? frameprocessor.cpp

Do not collapse the architecture into a single folder or class.

Coding guidelines

Follow these principles:

keep code simple and readable

avoid unnecessary abstractions

avoid magic numbers when possible

maintain clear class responsibilities

use Qt signal/slot communication

avoid global variables

avoid static global state

Prefer adding new code instead of rewriting large portions of the project.

UI guidelines

UI should stay lightweight.

UI must only:

display data

forward commands

update visual state

All business logic belongs outside the UI.

Compilation requirements

The agent must ensure:

the project compiles

headers are correctly included

file structure matches the architecture

stubs are clearly marked

Testing policy

Do NOT create automated tests.

Specifically:

do NOT add unit tests

do NOT add integration tests

do NOT introduce testing frameworks

do NOT create test directories

do NOT generate mock test code

Testing will be performed manually offline by the project author.

The agent should focus only on:

architecture

implementation

structure

compilation correctness

Restrictions

The agent must NOT:

create testing frameworks

generate test code

add CI pipelines for tests

create GitHub Actions workflows

generate artificial test harnesses

Agent expectations

When modifying the repository:

keep the architecture intact

avoid unnecessary refactoring

keep new code consistent with project structure

mark stub implementations clearly

ensure the project compiles

summarize created or modified files