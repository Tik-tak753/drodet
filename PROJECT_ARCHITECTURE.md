# PROJECT_ARCHITECTURE.md

## Purpose

This document describes the intended architecture of the project.

The project is a **Qt Widgets desktop application in C++** for demonstrating a **drone detection system** based on a YOLO-like model.

The architecture must remain:

- modular
- maintainable
- easy to extend
- suitable for step-by-step implementation

The project is developed in **Qt Creator** on **Windows** using **qmake**.

---

# High-level architecture

The application is divided into several logical layers:

1. UI layer
2. Controller layer
3. Detection layer
4. Frame source layer
5. Utility layer

The goal is to keep responsibilities separated.

---

# Layer descriptions

## 1. UI layer

### MainWindow

`MainWindow` is responsible only for the user interface.

Its responsibilities:

- display the main application window
- provide buttons and controls
- receive user input
- send user commands to the controller
- display processed frames
- display FPS
- display detection count
- display status text

`MainWindow` must NOT:

- implement detection logic
- manage camera logic
- directly read frames from OpenCV
- directly load the model
- directly perform inference

The UI layer should remain thin.

---

## 2. Controller layer

### DetectionController

`DetectionController` is the central coordinator of the application.

Its responsibilities:

- receive commands from the UI
- manage the current frame source
- own or control the detector
- request frames from the source
- send frames to the detector
- receive detection results
- prepare processed data for display
- emit signals back to the UI

Expected signals:

- `frameReady(const QImage&)`
- `statsReady(double fps, int detections)`
- `statusChanged(const QString&)`

Expected public methods or slots:

- `loadModel(const QString& path)`
- `openImage(const QString& path)`
- `openVideo(const QString& path)`
- `startCamera(int index)`
- `stop()`
- `setThresholds(float conf, float iou)`
- `setImageSize(int imgsz)`

The controller contains application flow logic, but should not become a dumping ground for every implementation detail.

---

## 3. Detection layer

### YoloDetector

`YoloDetector` encapsulates the model-specific logic.

Responsibilities:

- load the model
- track whether the model is loaded
- run detection on frames
- return structured results

Expected interface:

```cpp
class YoloDetector
{
public:
    bool loadModel(const QString& path);
    bool isLoaded() const;
    DetectionResult detect(const cv::Mat& frame, float conf, float iou, int imgsz);
};

At early development stages, YoloDetector may be implemented as a stub.

A stub is acceptable if:

it compiles

it preserves the final interface

it is clearly marked for future replacement

Example stub comment:

// TODO: implement real YOLO inference here
DetectionResult

Detection data must be represented using dedicated structures.

Example:

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

Why this exists:

avoids passing raw unrelated values

makes code easier to read

simplifies future extension

keeps detection logic separate from rendering logic

4. Frame source layer

The application should not hardcode frame acquisition logic into the controller.

Frame reading must be abstracted behind a common interface.

IFrameSource

Base interface:

class IFrameSource
{
public:
    virtual ~IFrameSource() = default;

    virtual bool open(const QString& source) = 0;
    virtual bool read(cv::Mat& frame) = 0;
    virtual void close() = 0;
    virtual bool isOpened() const = 0;
};
Implementations
CameraSource

Responsibilities:

open a camera by index

return frames from a live camera

close camera resource correctly

VideoFileSource

Responsibilities:

open a video file

return frames sequentially

close file resource correctly

ImageSource

Responsibilities:

load a single image

provide one frame

optionally return the same frame once or mark itself as finished

Why this abstraction is needed:

the controller does not need to know the exact source type

new source types can be added later

camera/video/image support remains consistent

Possible future extensions:

RTSP stream source

folder-of-images source

screen capture source

5. Utility layer
FrameProcessor

FrameProcessor contains helper logic related to frames and visualization.

Responsibilities:

convert cv::Mat to QImage

draw detection boxes

draw overlay text such as FPS and detection count

prepare a frame for display in the UI

Example responsibilities:

class FrameProcessor
{
public:
    static QImage matToQImage(const cv::Mat& frame);
    static cv::Mat drawDetections(const cv::Mat& frame, const DetectionResult& result);
    static cv::Mat drawOverlay(const cv::Mat& frame, double fps, int detections);
};

Why this should be separate:

keeps drawing logic out of the controller

keeps UI code simple

makes rendering behavior easier to change

Data flow

The expected runtime data flow is:

MainWindow
   ?
DetectionController
   ?
IFrameSource
   ?
cv::Mat frame
   ?
YoloDetector
   ?
DetectionResult
   ?
FrameProcessor
   ?
QImage
   ?
MainWindow

This flow ensures separation of concerns.

Main project structure

Expected repository structure:

project_root
?
??? AGENTS.md
??? PROJECT_ARCHITECTURE.md
??? main.cpp
??? mainwindow.h
??? mainwindow.cpp
??? mainwindow.ui
??? project.pro
?
??? controller
?   ??? detectioncontroller.h
?   ??? detectioncontroller.cpp
?
??? detection
?   ??? detectionresult.h
?   ??? yolodetector.h
?   ??? yolodetector.cpp
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

This structure should be preserved as the project grows.

Ownership and responsibilities
MainWindow owns:

UI widgets

signal connections to the controller

DetectionController owns or manages:

detector instance

current frame source

timer or update loop for frame processing

runtime configuration such as thresholds and image size

YoloDetector owns:

model-related state

model path

detector configuration that belongs specifically to the model

FrameSource implementations own:

source-specific OpenCV resources such as cv::VideoCapture

state needed for source lifecycle

Runtime behavior

Basic expected application flow:

application starts

MainWindow is shown

user selects a model file

UI sends model path to DetectionController

controller asks YoloDetector to load the model

user chooses a source:

image

video

camera

controller creates the proper frame source

controller requests frames

controller sends frames to detector

detector returns DetectionResult

controller uses FrameProcessor

controller emits processed frame and stats to UI

UI updates display

Threading approach
Initial stage

At the beginning, a simple implementation is acceptable:

use QTimer

process frames in a straightforward update loop

keep architecture ready for future threading

Future stage

Later, detection may be moved to a worker thread using:

QThread

worker object pattern

queued signal-slot communication

Important rule:

Threading should be introduced only when needed, but the architecture must not block that future step.

Error handling principles

The architecture should support clean error reporting.

Errors should be propagated as status messages where possible.

Examples:

model file not found

camera cannot be opened

video file cannot be opened

unsupported source

detector not loaded

The controller should emit user-facing status messages instead of letting low-level classes directly manipulate UI widgets.

Extension strategy

The architecture should support future additions without major rewrites.

Expected future features:

real YOLO inference integration

model switching

confidence threshold controls

IoU threshold controls

image size controls

save frame

save video

object class filtering

multiple source types

threaded inference

performance monitoring

The design should make these features incremental rather than disruptive.

Non-goals

The following are intentionally NOT required in the initial architecture stage:

full YOLO integration

advanced optimization

automated tests

CI pipelines

plugin system

networking layer

database storage

overengineered abstractions

The first goal is a clean, compilable, extensible architecture.

Architectural rules

These rules must be preserved:

MainWindow must remain thin

detection logic must not live in the UI

frame acquisition must be abstracted

detection results must use dedicated data structures

helper rendering logic must stay outside the UI

avoid global mutable state

preserve clear file and folder organization

prefer incremental extension over aggressive refactoring

Summary

This project is intended to evolve in stages.

The architecture is designed so that:

the UI remains simple

the controller coordinates application behavior

detection logic is isolated

frame sources are replaceable

utility code stays reusable

future YOLO integration can be added cleanly

This document should be used as the architectural reference when adding or modifying code.