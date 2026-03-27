#pragma once
#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QImage>
#include <QComboBox>
#include <thread>
#include <atomic>
#include <mutex>
#include "CameraApi.h"

class GreenCameraView;

class GreenWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GreenWindow(QWidget *parent = nullptr);
    ~GreenWindow() override;

private slots:
    void grabFrame();
    void onExposureSliderMoved(int pos);
    void onExposureSpinChanged(int us);
    void onAnalogGainChanged(int val);
    void onDigitalGainChanged(int val);
    void onResolutionChanged(int index);
    void onConnectCamera();
    void onStart();
    void onStop();

private:
    void    setupUI();
    QWidget *buildHeader();
    QWidget *buildSidebar();
    QWidget *buildAnalytics();
    QWidget *buildResultsTable();
    QWidget *buildStatusBar();
    bool    connectCamera();
    void    disconnectCamera();
    void    captureLoop();
    void    updateAnalogGainLabel(int gain);
    void    setConnectedState(bool c);
    void    setAcquiringState(bool a);
    void    showStatus(const QString &msg);

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
    double               m_fExpLineTime    = 1.0;
    double               m_digitalGain     = 1.0;
    tSdkCameraCapbility  m_capability      = {};

    // ── UI widgets ────────────────────────────────────────────────────────────
    GreenCameraView *m_cameraView        = nullptr;
    QLabel          *m_connDotLabel      = nullptr;
    QLabel          *m_connTextLabel     = nullptr;
    QLabel          *m_fpsLabel          = nullptr;
    QPushButton     *m_connectBtn        = nullptr;
    QComboBox       *m_resCombo          = nullptr;
    QSlider         *m_exposureSlider    = nullptr;
    QSpinBox        *m_exposureSpin      = nullptr;
    QSlider         *m_analogGainSlider  = nullptr;
    QLabel          *m_analogGainLabel   = nullptr;
    QSlider         *m_digitalGainSlider = nullptr;
    QLabel          *m_digitalGainLabel  = nullptr;
    QPushButton     *m_startBtn          = nullptr;
    QPushButton     *m_stopBtn           = nullptr;
    QLabel          *m_statusMsgLabel    = nullptr;
    QLabel          *m_camDotLabel       = nullptr;
    QLabel          *m_imageFmtLabel     = nullptr;

    QTimer        *m_timer      = nullptr;
    QElapsedTimer  m_fpsTimer;
    int            m_frameCount = 0;
    bool           m_syncBlock  = false;

    // ── 后台采集线程 ──────────────────────────────────────────────────────────
    std::thread        m_captureThread;
    std::atomic<bool>  m_captureRunning{false};
    std::mutex         m_frameMutex;
    QImage             m_latestFrame;
    bool               m_frameReady = false;
};
