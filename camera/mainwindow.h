#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <QElapsedTimer>
#include <QPixmap>
#include <QImage>
#include <QKeyEvent>
#include <thread>
#include <atomic>
#include <mutex>
#include "CameraApi.h"

class CameraView;   // defined in mainwindow.cpp

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void grabFrame();
    void onExposureSliderMoved(int pos);
    void onExposureSpinChanged(int us);
    void onAnalogGainChanged(int value);
    void onConnectCamera();
    void onStart();
    void onStop();
    void onResolutionChanged(int index);
    void onFrameRateButtonClicked();
    void onToggleFullscreen();

private:
    // build helpers
    QWidget *buildHeader();
    QWidget *buildSidebar();
    QWidget *buildAnalytics();
    QWidget *buildResultsTable();
    QWidget *buildStatusBar();
    void     setupUI();
    void     keyPressEvent(QKeyEvent *e) override;
    void     changeEvent(QEvent *e) override;

    // camera helpers
    bool connectCamera();
    void disconnectCamera();
    void setConnectedState(bool c);
    void setAcquiringState(bool a);
    void showStatus(const QString &msg);
    void updateAnalogGainLabel(int gain);
    void captureLoop();   // 后台采集线程函数

    // ── camera state ─────────────────────────────────────────────────────────
    int                  m_hCamera         = -1;
    bool                 m_cameraConnected = false;
    bool                 m_acquiring       = false;
    unsigned char       *m_pRgbBuffer      = nullptr;
    int                  m_channel         = 3;
    float                m_gainStep        = 0.125f;
    int                  m_gainMin         = 16;
    int                  m_gainMax         = 256;
    int                  m_imageW          = 0;
    int                  m_imageH          = 0;
    bool                 m_syncBlock       = false;
    tSdkCameraCapbility  m_capability      = {};
    double               m_fExpLineTime    = 1.0;

    // ── UI widgets ────────────────────────────────────────────────────────────
    CameraView  *m_cameraView        = nullptr;
    QWidget     *m_sidebarWidget     = nullptr;
    QWidget     *m_analyticsWidget   = nullptr;
    QWidget     *m_resultsWidget     = nullptr;
    QWidget     *m_headerWidget      = nullptr;
    QWidget     *m_statusWidget      = nullptr;
    QPushButton *m_fullscreenBtn     = nullptr;
    bool         m_imageFullscreen   = false;
    double       m_uiScale          = 1.0;
    QComboBox   *m_resCombo          = nullptr;
    QLabel      *m_imageFmtLabel     = nullptr;
    QSlider     *m_exposureSlider    = nullptr;
    QSpinBox    *m_exposureSpin      = nullptr;
    QSlider     *m_gainSlider        = nullptr;   // analog
    QLabel      *m_gainValueLabel    = nullptr;   // analog multiplier
    // 线频设置
    QPushButton *m_frameRateBtn      = nullptr;
    QLabel      *m_frameRateValue    = nullptr;
    QPushButton *m_connectBtn        = nullptr;
    QPushButton *m_startBtn          = nullptr;
    QPushButton *m_stopBtn           = nullptr;
    QLabel      *m_connDotLabel      = nullptr;
    QLabel      *m_connTextLabel     = nullptr;
    QLabel      *m_fpsLabel          = nullptr;
    QLabel      *m_statusMsgLabel    = nullptr;
    QLabel      *m_camDotLabel       = nullptr;   // status bar cam dot

    QTimer       *m_timer      = nullptr;
    QElapsedTimer m_fpsTimer;
    int           m_frameCount = 0;

    // ── 后台采集线程 ──────────────────────────────────────────────────────────
    std::thread        m_captureThread;
    std::atomic<bool>  m_captureRunning{false};
    std::atomic<int>   m_saveCounter{1};
    std::mutex         m_frameMutex;
    QImage             m_latestFrame;
    bool               m_frameReady = false;
    QString            m_imageDir;
};
