#include "green_window.h"

#include <QApplication>
#include <QMetaObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QSizePolicy>
#include <QDateTime>
#include <QImage>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QProgressBar>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <opencv2/imgproc.hpp>
#include <cstdio>
#include <cstdlib>

// ════════════════════════════════════════════════════════════════════════════
//  全局样式表
// ════════════════════════════════════════════════════════════════════════════
static const char *APP_STYLE_GREEN = R"(
* { font-family: 'Segoe UI', Arial, sans-serif; font-size: 12px; }
QMainWindow, QWidget { background-color: #dee2e6; }
QLabel { background: transparent; }

QWidget#header { background-color: #e9ecef; border-bottom: 1px solid #adb5bd; }

QWidget#sidebar { background-color: #f8f9fa; border-right: 1px solid #ced4da; }
QLabel#secHeader {
    background-color: #dee2e6; color: #343a40;
    font-weight: bold; font-size: 11px;
    padding: 0 10px; border-bottom: 1px solid #ced4da;
}
QLabel#paramLbl { color: #495057; font-weight: bold; font-size: 11px; }
QLabel#subLbl   { color: #6c757d; font-size: 11px; }
QLabel#valueLbl { color: #3b82f6; font-size: 11px; font-weight: bold; font-family: monospace; }

QWidget#imageArea   { background-color: #dee2e6; }
QWidget#imgTitleBar { background-color: #dee2e6; border-bottom: 1px solid #adb5bd; }
QLabel#imgTitleLbl  { color: #343a40; font-size: 11px; font-weight: bold; padding: 0 8px; }
QLabel#imgFmtLbl    { color: #6c757d; font-size: 11px; font-family: monospace; padding: 0 8px; }

QWidget#analyticsPanel    { background-color: #f8f9fa; border-left: 1px solid #ced4da; }
QWidget#analyticsTitleBar { background-color: #dee2e6; border-bottom: 1px solid #ced4da; }
QLabel#analyticsTitleLbl  { color: #343a40; font-size: 11px; font-weight: bold; padding: 0 8px; }
QWidget#kpiCard           { background: white; border: 1px solid #ced4da; }

QWidget#resultsPanel   { background: white; border-top: 1px solid #ced4da; }
QLabel#resultsTitleLbl {
    background-color: #dee2e6; color: #343a40;
    font-size: 11px; font-weight: bold;
    padding: 0 8px; border-bottom: 1px solid #ced4da;
}
QTableWidget {
    border: none; background: white;
    gridline-color: #e9ecef; font-family: monospace; font-size: 11px;
}
QHeaderView::section {
    background: #f8f9fa; border: none;
    border-right: 1px solid #dee2e6; border-bottom: 1px solid #dee2e6;
    padding: 3px 6px; font-weight: bold; font-size: 10px; color: #495057;
}
QTableWidget::item { padding: 2px 6px; border-right: 1px solid #f0f0f0; }
QTableWidget::item:selected { background: #e9ecef; color: #212529; }

QWidget#statusBar { background-color: #dee2e6; border-top: 1px solid #adb5bd; }

QSlider::groove:horizontal { height: 4px; background: #ced4da; border-radius: 2px; }
QSlider::handle:horizontal {
    background: white; width: 14px; height: 14px;
    border-radius: 7px; margin: -5px 0; border: 2px solid #6c757d;
}
QSlider::sub-page:horizontal { background: #3b82f6; border-radius: 2px; }
QSlider::handle:horizontal:hover    { border-color: #343a40; }
QSlider::handle:horizontal:disabled { background: #dee2e6; border-color: #ced4da; }

QSpinBox, QLineEdit, QComboBox {
    border: 1px solid #ced4da; border-radius: 2px;
    padding: 2px 4px; background: white;
}

QProgressBar { border: none; background: #dee2e6; border-radius: 0; }
QProgressBar::chunk { border-radius: 0; }

QPushButton {
    border: 1px solid #adb5bd;
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #f8f9fa, stop:1 #e9ecef);
    border-radius: 2px; padding: 4px 12px; color: #212529;
}
QPushButton:hover   { background: #dee2e6; }
QPushButton:pressed { background: #ced4da; }
QPushButton:disabled{ color: #adb5bd; border-color: #e9ecef; }

QPushButton#beanActive {
    background: #dbeafe; color: #1d4ed8;
    border-color: #93c5fd; font-size: 11px; padding: 2px 8px;
}
QPushButton#beanInactive {
    background: #f8f9fa; color: #6c757d;
    border-color: #ced4da; font-size: 11px; padding: 2px 8px;
}
QPushButton#startBtn {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #3b82f6, stop:1 #2563eb);
    color: white; border-color: #1d4ed8;
    font-weight: bold; padding: 7px 10px; letter-spacing: 1px;
}
QPushButton#startBtn:hover    { background: #2563eb; }
QPushButton#startBtn:disabled { background: #93c5fd; border-color: #bfdbfe; }
QPushButton#stopBtn {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #ef4444, stop:1 #dc2626);
    color: white; border-color: #b91c1c;
    font-weight: bold; padding: 7px 10px; letter-spacing: 1px;
}
QPushButton#stopBtn:hover    { background: #dc2626; }
QPushButton#stopBtn:disabled { background: #fca5a5; border-color: #fecaca; }

QCheckBox { color: #343a40; font-size: 11px; font-weight: bold; }

QPushButton#tabActive {
    background: white; color: #1d4ed8;
    border: none; border-bottom: 2px solid #3b82f6;
    font-size: 10px; font-weight: bold; padding: 0 6px; border-radius: 0;
}
QPushButton#tabInactive {
    background: transparent; color: #6c757d;
    border: none; font-size: 10px; font-weight: bold; padding: 0 6px; border-radius: 0;
}
QPushButton#tabInactive:hover { background: #f0f0f0; }
)";

// ════════════════════════════════════════════════════════════════════════════
//  Helpers
// ════════════════════════════════════════════════════════════════════════════
static QFrame *hLine()
{
    auto *f = new QFrame;
    f->setFixedHeight(1);
    f->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    f->setStyleSheet("QFrame { background-color: #ced4da; border: none; }");
    return f;
}
static QFrame *vLine()
{
    auto *f = new QFrame;
    f->setFixedWidth(1);
    f->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    f->setStyleSheet("QFrame { background-color: #ced4da; border: none; }");
    return f;
}
static QLabel *secHeader(const QString &t)
{
    auto *l = new QLabel(t.toUpper());
    l->setObjectName("secHeader");
    l->setFixedHeight(28);
    return l;
}
static QLabel *dot(const char *color, int size = 10)
{
    auto *l = new QLabel("●");
    l->setStyleSheet(QString("color: %1; font-size: %2px;").arg(color).arg(size));
    l->setFixedWidth(14);
    return l;
}

// ════════════════════════════════════════════════════════════════════════════
//  GreenDefectBarChart — 6 bars: Insect / Mold / Broken / Foreign / Under. / Other
// ════════════════════════════════════════════════════════════════════════════
class GreenDefectBarChart : public QWidget
{
public:
    explicit GreenDefectBarChart(QWidget *p = nullptr) : QWidget(p)
    {
        setFixedHeight(100);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.fillRect(rect(), QColor("#f8f9fa"));
        p.setPen(QColor("#ced4da"));
        p.drawRect(rect().adjusted(0, 0, -1, -1));

        const struct { QColor c; float h; } bars[] = {
            { QColor("#f87171"), 0.20f },
            { QColor("#a78bfa"), 0.10f },
            { QColor("#fb923c"), 0.15f },
            { QColor("#60a5fa"), 0.05f },
            { QColor("#34d399"), 0.08f },
            { QColor("#94a3b8"), 0.12f },
        };
        const int n = 6, padT = 6, padB = 2;
        int maxH = height() - padT - padB;
        int barW = 12;
        int gap  = (width() - n * barW) / (n + 1);
        for (int i = 0; i < n; i++) {
            int bh = int(bars[i].h * maxH);
            int bx = gap + i * (barW + gap);
            p.fillRect(bx, padT + maxH - bh, barW, bh, bars[i].c);
        }
        p.setPen(QColor("#ced4da"));
        p.drawLine(0, height() - padB, width(), height() - padB);
    }
};

// ════════════════════════════════════════════════════════════════════════════
//  GreenCameraView — 图像显示 + 检测框叠层 + 迷你工具栏
// ════════════════════════════════════════════════════════════════════════════
class GreenCameraView : public QWidget
{
public:
    explicit GreenCameraView(QWidget *parent = nullptr);
    void setPixmap(const QPixmap &px);
    void setError(const QString &msg);
    QSize sizeHint() const override { return { 800, 500 }; }

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *ev) override;
    void showEvent(QShowEvent *ev) override;

private:
    void reposition();

    QPixmap  m_pixmap;
    QString  m_errorMsg;
    QRect    m_imgRect;
    QLabel  *m_coordLabel  = nullptr;
    QWidget *m_miniToolbar = nullptr;
};

GreenCameraView::GreenCameraView(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background: black;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_coordLabel = new QLabel("X: 204.22  Y: 852.19 | Value: 128", this);
    m_coordLabel->setStyleSheet(
        "QLabel { color: #4ade80; font-family: monospace; font-size: 9px;"
        "         background-color: rgba(0,0,0,160); padding: 2px 6px; }");
    m_coordLabel->adjustSize();
    m_coordLabel->hide();

    m_miniToolbar = new QWidget(this);
    m_miniToolbar->setStyleSheet(
        "QWidget { background-color: rgba(52,58,64,210); border-radius: 2px; }");
    auto *tl = new QVBoxLayout(m_miniToolbar);
    tl->setContentsMargins(2, 4, 2, 4);
    tl->setSpacing(2);

    auto mkBtn = [](const char *txt) {
        auto *b = new QPushButton(txt);
        b->setFixedSize(22, 22);
        b->setStyleSheet(
            "QPushButton { background: transparent; color: white;"
            "  font-size: 13px; border: none; border-radius: 2px; padding:0; }"
            "QPushButton:hover { background: rgba(255,255,255,40); }");
        return b;
    };
    tl->addWidget(mkBtn("⊕"));
    tl->addWidget(mkBtn("⊖"));
    auto *sep = new QFrame;
    sep->setFixedHeight(1);
    sep->setStyleSheet("QFrame { background: rgba(255,255,255,60); }");
    tl->addWidget(sep);
    tl->addWidget(mkBtn("⊙"));
    m_miniToolbar->adjustSize();
    m_miniToolbar->hide();
}

void GreenCameraView::setPixmap(const QPixmap &px)
{
    m_pixmap = px;
    m_errorMsg.clear();
    update();
}

void GreenCameraView::setError(const QString &msg)
{
    m_pixmap = QPixmap();
    m_errorMsg = msg;
    update();
}

void GreenCameraView::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev);
    QTimer::singleShot(0, this, [this] {
        reposition();
        m_coordLabel->show();
        m_miniToolbar->show();
        m_coordLabel->raise();
        m_miniToolbar->raise();
    });
}

void GreenCameraView::resizeEvent(QResizeEvent *ev)
{
    QWidget::resizeEvent(ev);
    reposition();
}

void GreenCameraView::reposition()
{
    if (m_miniToolbar) {
        m_miniToolbar->adjustSize();
        m_miniToolbar->move(width() - m_miniToolbar->width() - 6, 6);
    }
    if (m_coordLabel) {
        m_coordLabel->adjustSize();
        m_coordLabel->move(6, height() - m_coordLabel->height() - 6);
    }
}

void GreenCameraView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.fillRect(rect(), Qt::black);

    // ── Error state ───────────────────────────────────────────────────────
    if (!m_errorMsg.isEmpty()) {
        int pw = 380, ph = 90;
        int px2 = (width() - pw) / 2;
        int py2 = (height() - ph) / 2;
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(80, 20, 20, 200));
        p.drawRoundedRect(QRect(px2, py2, pw, ph), 8, 8);
        p.setFont(QFont("Arial", 12, QFont::Bold));
        p.setPen(QColor("#ef4444"));
        p.drawText(QRect(px2, py2 + 14, pw, 28), Qt::AlignCenter, "Camera Error");
        p.setFont(QFont("Arial", 9));
        p.setPen(QColor("#fca5a5"));
        p.drawText(QRect(px2, py2 + 46, pw, 28), Qt::AlignCenter, m_errorMsg);
        return;
    }

    // ── No signal placeholder ─────────────────────────────────────────────
    if (m_pixmap.isNull()) {
        int pw = 320, ph = 120;
        int px2 = (width()  - pw) / 2;
        int py2 = (height() - ph) / 2;
        QRect panel(px2, py2, pw, ph);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(40, 44, 52, 200));
        p.drawRoundedRect(panel, 8, 8);
        p.setPen(QColor("#6b7280"));
        p.drawRoundedRect(panel.adjusted(0, 0, -1, -1), 8, 8);
        p.setFont(QFont("Arial", 12, QFont::Bold));
        p.setPen(QColor("#d1d5db"));
        p.drawText(QRect(px2, py2 + 40, pw, 22), Qt::AlignCenter, "No Camera Signal");
        p.setFont(QFont("Arial", 9));
        p.setPen(QColor("#6b7280"));
        p.drawText(QRect(px2, py2 + 68, pw, 18), Qt::AlignCenter,
                   "Click  \"Connect Camera\"  to begin");
        return;
    }

    // ── Live frame ────────────────────────────────────────────────────────
    QSize scaled = m_pixmap.size().scaled(size(), Qt::KeepAspectRatio);
    int ix = (width()  - scaled.width())  / 2;
    int iy = (height() - scaled.height()) / 2;
    m_imgRect = QRect(ix, iy, scaled.width(), scaled.height());
    p.drawPixmap(m_imgRect, m_pixmap);
}

// ════════════════════════════════════════════════════════════════════════════
//  GreenWindow — 构造 / 析构
// ════════════════════════════════════════════════════════════════════════════
GreenWindow::GreenWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("GigE Line-Scan Inspector — Green Beans Mode");
    resize(1440, 860);
    setMinimumSize(1100, 680);
    setStyleSheet(APP_STYLE_GREEN);

    CameraSdkInit(1);
    setupUI();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GreenWindow::grabFrame);
}

GreenWindow::~GreenWindow()
{
    m_timer->stop();
    disconnectCamera();
}

// ════════════════════════════════════════════════════════════════════════════
//  setupUI
// ════════════════════════════════════════════════════════════════════════════
void GreenWindow::setupUI()
{
    auto *central    = new QWidget(this);
    auto *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    setCentralWidget(central);

    auto *mainArea   = new QWidget;
    auto *mainLayout = new QHBoxLayout(mainArea);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *workspace       = new QWidget;
    auto *workspaceLayout = new QVBoxLayout(workspace);
    workspaceLayout->setContentsMargins(4, 4, 4, 4);
    workspaceLayout->setSpacing(4);

    auto *topRow       = new QWidget;
    auto *topRowLayout = new QHBoxLayout(topRow);
    topRowLayout->setContentsMargins(0, 0, 0, 0);
    topRowLayout->setSpacing(4);

    // 图像区域
    auto *imageArea       = new QWidget;
    imageArea->setObjectName("imageArea");
    auto *imageAreaLayout = new QVBoxLayout(imageArea);
    imageAreaLayout->setContentsMargins(0, 0, 0, 0);
    imageAreaLayout->setSpacing(0);

    auto *imgTitleBar    = new QWidget;
    imgTitleBar->setObjectName("imgTitleBar");
    imgTitleBar->setFixedHeight(28);
    auto *imgTitleLayout = new QHBoxLayout(imgTitleBar);
    imgTitleLayout->setContentsMargins(0, 0, 0, 0);
    auto *imgTitleLbl = new QLabel("LIVE IMAGE: CAMERA_01_LINE_SCAN");
    imgTitleLbl->setObjectName("imgTitleLbl");
    m_imageFmtLabel = new QLabel("-- × -- BUFFER");
    m_imageFmtLabel->setObjectName("imgFmtLbl");
    imgTitleLayout->addWidget(imgTitleLbl);
    imgTitleLayout->addStretch();
    imgTitleLayout->addWidget(m_imageFmtLabel);

    m_cameraView = new GreenCameraView;
    imageAreaLayout->addWidget(imgTitleBar);
    imageAreaLayout->addWidget(m_cameraView, 1);

    topRowLayout->addWidget(imageArea, 3);
    topRowLayout->addWidget(buildAnalytics());

    workspaceLayout->addWidget(topRow, 1);
    workspaceLayout->addWidget(buildResultsTable());

    mainLayout->addWidget(buildSidebar());
    mainLayout->addWidget(workspace, 1);

    rootLayout->addWidget(buildHeader());
    rootLayout->addWidget(mainArea, 1);
    rootLayout->addWidget(buildStatusBar());
}

// ════════════════════════════════════════════════════════════════════════════
//  buildHeader
// ════════════════════════════════════════════════════════════════════════════
QWidget *GreenWindow::buildHeader()
{
    auto *header = new QWidget;
    header->setObjectName("header");
    header->setFixedHeight(48);

    auto *l = new QHBoxLayout(header);
    l->setContentsMargins(14, 0, 14, 0);
    l->setSpacing(6);

    auto *icon = new QLabel("📷");
    icon->setStyleSheet("font-size: 16px;");
    icon->setFixedWidth(24);
    auto *title = new QLabel("GigE Line-Scan Inspector");
    title->setStyleSheet("font-weight: bold; font-size: 14px; color: #212529; letter-spacing: 1px;");

    l->addWidget(icon);
    l->addWidget(title);
    l->addSpacing(8);
    l->addWidget(vLine());
    l->addSpacing(8);

    // 连接状态 — 点 + 文字（与烤豆界面一致）
    m_connDotLabel = new QLabel("●");
    m_connDotLabel->setStyleSheet("color: #dc3545; font-size: 14px;");
    m_connDotLabel->setFixedWidth(18);
    m_connTextLabel = new QLabel("Disconnected");
    m_connTextLabel->setStyleSheet("color: #6c757d; font-size: 11px;");

    m_fpsLabel = new QLabel("-- FPS");
    m_fpsLabel->setStyleSheet(
        "color: #495057; font-family: monospace; font-size: 11px;"
        "background: #e9ecef; border: 1px solid #ced4da;"
        "padding: 1px 6px; border-radius: 2px;");

    l->addWidget(m_connDotLabel);
    l->addWidget(m_connTextLabel);
    l->addSpacing(10);
    l->addWidget(m_fpsLabel);
    l->addStretch();

    // Bean mode — Green Beans 激活
    auto *beanLabel = new QLabel("Bean Mode");
    beanLabel->setStyleSheet("color: #6c757d; font-size: 10px; font-weight: bold;");
    auto *beanGreen   = new QPushButton("Green Beans ◎");
    beanGreen->setObjectName("beanActive");
    beanGreen->setFixedHeight(24);
    auto *beanRoasted = new QPushButton("Roasted Beans");
    beanRoasted->setObjectName("beanInactive");
    beanRoasted->setFixedHeight(24);

    auto *beanFrame  = new QFrame;
    beanFrame->setStyleSheet("QFrame { border: 1px solid #ced4da; background: transparent; }");
    auto *beanLayout = new QHBoxLayout(beanFrame);
    beanLayout->setContentsMargins(0, 0, 0, 0);
    beanLayout->setSpacing(0);
    beanLayout->addWidget(beanGreen);
    beanLayout->addWidget(beanRoasted);

    l->addWidget(beanLabel);
    l->addSpacing(4);
    l->addWidget(beanFrame);
    l->addSpacing(8);

    m_connectBtn = new QPushButton("Connect Camera");
    m_connectBtn->setFixedHeight(26);
    connect(m_connectBtn, &QPushButton::clicked, this, &GreenWindow::onConnectCamera);
    auto *settingsBtn = new QPushButton("Settings");
    settingsBtn->setFixedHeight(26);

    l->addWidget(m_connectBtn);
    l->addWidget(settingsBtn);
    l->addSpacing(8);
    l->addWidget(vLine());
    l->addSpacing(8);

    auto *opLbl = new QLabel("Operator:");
    opLbl->setStyleSheet("color: #6c757d; font-size: 10px; font-weight: bold;");
    auto *opName = new QLabel("QC_Eng_04");
    opName->setStyleSheet("font-size: 12px; font-weight: bold; color: #212529;");
    l->addWidget(opLbl);
    l->addWidget(opName);

    return header;
}

// ════════════════════════════════════════════════════════════════════════════
//  buildSidebar
// ════════════════════════════════════════════════════════════════════════════
QWidget *GreenWindow::buildSidebar()
{
    auto *sidebar = new QWidget;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(256);

    auto *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(0, 0, 0, 0);
    sideLayout->setSpacing(0);
    sideLayout->addWidget(secHeader("Camera Control"));

    auto *content       = new QWidget;
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(12, 12, 12, 12);
    contentLayout->setSpacing(12);

    // ── 分辨率 ──────────────────────────────────────────────────────────
    {
        auto *lbl = new QLabel("Resolution"); lbl->setObjectName("paramLbl");
        m_resCombo = new QComboBox;
        m_resCombo->setEnabled(false);
        m_resCombo->setPlaceholderText("-- Connect camera --");
        contentLayout->addWidget(lbl);
        contentLayout->addWidget(m_resCombo);
        connect(m_resCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &GreenWindow::onResolutionChanged);
    }
    contentLayout->addWidget(hLine());

    // ── 曝光 ─────────────────────────────────────────────────────────────
    {
        auto *expLbl = new QLabel("Exposure (µs)"); expLbl->setObjectName("paramLbl");

        m_exposureSpin = new QSpinBox;
        m_exposureSpin->setRange(100, 1000000);
        m_exposureSpin->setValue(800);
        m_exposureSpin->setSingleStep(100);
        m_exposureSpin->setSuffix(" µs");
        m_exposureSpin->setFixedWidth(90);
        m_exposureSpin->setEnabled(false);

        auto *row = new QHBoxLayout;
        row->addWidget(expLbl);
        row->addStretch();
        row->addWidget(m_exposureSpin);

        m_exposureSlider = new QSlider(Qt::Horizontal);
        m_exposureSlider->setRange(1, 10000);
        m_exposureSlider->setValue(8);
        m_exposureSlider->setSingleStep(1);
        m_exposureSlider->setPageStep(10);
        m_exposureSlider->setEnabled(false);

        contentLayout->addLayout(row);
        contentLayout->addWidget(m_exposureSlider);

        connect(m_exposureSlider, &QSlider::valueChanged,
                this, &GreenWindow::onExposureSliderMoved);
        connect(m_exposureSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &GreenWindow::onExposureSpinChanged);
    }
    contentLayout->addWidget(hLine());

    // ── 增益控制 ──────────────────────────────────────────────────────────
    {
        auto *gainTitle = new QLabel("Gain Control"); gainTitle->setObjectName("paramLbl");
        contentLayout->addWidget(gainTitle);

        // 模拟增益（对接硬件）
        auto *anaRow = new QHBoxLayout;
        auto *anaLbl = new QLabel("Analog"); anaLbl->setObjectName("subLbl");
        m_analogGainLabel = new QLabel("1.00x"); m_analogGainLabel->setObjectName("valueLbl");
        anaRow->addWidget(anaLbl); anaRow->addStretch(); anaRow->addWidget(m_analogGainLabel);

        m_analogGainSlider = new QSlider(Qt::Horizontal);
        m_analogGainSlider->setRange(16, 256);
        m_analogGainSlider->setValue(16);
        m_analogGainSlider->setEnabled(false);

        contentLayout->addLayout(anaRow);
        contentLayout->addWidget(m_analogGainSlider);
        connect(m_analogGainSlider, &QSlider::valueChanged,
                this, &GreenWindow::onAnalogGainChanged);

        // 数字增益（软件后处理）
        auto *digRow = new QHBoxLayout;
        auto *digLbl = new QLabel("Digital"); digLbl->setObjectName("subLbl");
        m_digitalGainLabel = new QLabel("1.0x"); m_digitalGainLabel->setObjectName("valueLbl");
        digRow->addWidget(digLbl); digRow->addStretch(); digRow->addWidget(m_digitalGainLabel);

        m_digitalGainSlider = new QSlider(Qt::Horizontal);
        m_digitalGainSlider->setRange(0, 100);  // 1.0x–2.0x
        m_digitalGainSlider->setValue(0);

        contentLayout->addLayout(digRow);
        contentLayout->addWidget(m_digitalGainSlider);
        connect(m_digitalGainSlider, &QSlider::valueChanged,
                this, &GreenWindow::onDigitalGainChanged);
    }
    contentLayout->addWidget(hLine());

    // ── 白平衡 ────────────────────────────────────────────────────────────
    {
        auto *wbRow = new QHBoxLayout;
        auto *wbLbl = new QLabel("White Balance"); wbLbl->setObjectName("paramLbl");
        auto *autoBtn = new QPushButton("Auto");
        autoBtn->setFixedSize(38, 20);
        autoBtn->setStyleSheet(
            "QPushButton { font-size: 10px; padding: 0 4px;"
            "  border: 1px solid #ced4da; background: white; border-radius: 2px; }"
            "QPushButton:hover { background: #f0f0f0; }");
        wbRow->addWidget(wbLbl); wbRow->addStretch(); wbRow->addWidget(autoBtn);
        contentLayout->addLayout(wbRow);

        auto *wbGrid = new QGridLayout; wbGrid->setSpacing(4);
        const struct { const char *lbl, *clr, *val; } ch[] = {
            {"R","#dc2626","1.02"}, {"G","#16a34a","1.00"}, {"B","#2563eb","1.15"}
        };
        for (int i = 0; i < 3; i++) {
            auto *cl = new QLabel(ch[i].lbl);
            cl->setAlignment(Qt::AlignCenter);
            cl->setStyleSheet(
                QString("color: %1; font-weight: bold; font-size: 10px;").arg(ch[i].clr));
            auto *inp = new QLineEdit(ch[i].val);
            inp->setAlignment(Qt::AlignCenter); inp->setFixedHeight(22);
            wbGrid->addWidget(cl, 0, i); wbGrid->addWidget(inp, 1, i);
        }
        contentLayout->addLayout(wbGrid);
    }
    contentLayout->addWidget(hLine());

    // ── 采集控制 ──────────────────────────────────────────────────────────
    {
        auto *cb = new QCheckBox("Live View Overlay"); cb->setChecked(true);
        contentLayout->addWidget(cb);

        m_startBtn = new QPushButton("Start Inspection");
        m_startBtn->setObjectName("startBtn");
        m_startBtn->setEnabled(false);
        connect(m_startBtn, &QPushButton::clicked, this, &GreenWindow::onStart);

        m_stopBtn = new QPushButton("Stop Acquisition");
        m_stopBtn->setObjectName("stopBtn");
        m_stopBtn->setEnabled(false);
        connect(m_stopBtn, &QPushButton::clicked, this, &GreenWindow::onStop);

        contentLayout->addWidget(m_startBtn);
        contentLayout->addWidget(m_stopBtn);
    }
    contentLayout->addStretch();

    sideLayout->addWidget(content, 1);
    return sidebar;
}

// ════════════════════════════════════════════════════════════════════════════
//  buildAnalytics — 绿豆版本
// ════════════════════════════════════════════════════════════════════════════
QWidget *GreenWindow::buildAnalytics()
{
    auto *panel = new QWidget;
    panel->setObjectName("analyticsPanel");
    panel->setFixedWidth(240);

    auto *l = new QVBoxLayout(panel);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    // 标题栏
    auto *titleBar = new QWidget;
    titleBar->setObjectName("analyticsTitleBar");
    titleBar->setFixedHeight(28);
    auto *tl = new QHBoxLayout(titleBar);
    tl->setContentsMargins(0, 0, 0, 0);
    auto *titleLbl = new QLabel("Real-time Analytics");
    titleLbl->setObjectName("analyticsTitleLbl");
    tl->addWidget(titleLbl);
    l->addWidget(titleBar);

    // Tab 行 — GREEN BEANS 激活
    auto *tabContainer = new QWidget;
    tabContainer->setStyleSheet("QWidget { background: #f0f0f0; }");
    auto *tabVBox = new QVBoxLayout(tabContainer);
    tabVBox->setContentsMargins(0, 0, 0, 0); tabVBox->setSpacing(0);

    auto *tabRow    = new QWidget;
    auto *tabLayout = new QHBoxLayout(tabRow);
    tabLayout->setContentsMargins(0, 0, 0, 0); tabLayout->setSpacing(0);

    auto *tabG = new QPushButton("GREEN BEANS");
    tabG->setObjectName("tabActive"); tabG->setFixedHeight(26);
    auto *tabR = new QPushButton("ROASTED BEANS");
    tabR->setObjectName("tabInactive"); tabR->setFixedHeight(26);

    tabLayout->addWidget(tabG);
    tabLayout->addWidget(vLine());
    tabLayout->addWidget(tabR);
    tabVBox->addWidget(tabRow);
    tabVBox->addWidget(hLine());
    l->addWidget(tabContainer);

    // 内容
    auto *content = new QWidget;
    auto *cl      = new QVBoxLayout(content);
    cl->setContentsMargins(8, 8, 8, 8);
    cl->setSpacing(10);

    // KPI 卡片 — 绿豆指标
    auto *kpiGrid = new QGridLayout; kpiGrid->setSpacing(6);
    auto mkCard = [](const char *name, const char *val,
                     const char *valClr, const char *sub, const char *subClr) {
        auto *card = new QWidget; card->setObjectName("kpiCard");
        card->setFixedHeight(64);
        auto *cl2 = new QVBoxLayout(card);
        cl2->setContentsMargins(6, 4, 6, 4); cl2->setSpacing(0);
        auto *n = new QLabel(name);
        n->setStyleSheet("font-size: 9px; color: #6c757d; font-weight: bold;");
        auto *v = new QLabel(val);
        v->setStyleSheet(QString("font-size: 18px; font-weight: bold; color: %1;").arg(valClr));
        auto *s = new QLabel(sub);
        s->setStyleSheet(QString("font-size: 9px; color: %1;").arg(subClr));
        cl2->addWidget(n); cl2->addWidget(v); cl2->addWidget(s);
        return card;
    };
    kpiGrid->addWidget(mkCard("Avg Mesh Size", "18.4 mm", "#212529", "▲ 0.2",  "#16a34a"), 0, 0);
    kpiGrid->addWidget(mkCard("Throughput",    "1.2k kg/h","#212529","▲ 2.5%","#16a34a"), 0, 1);
    cl->addLayout(kpiGrid);

    // 缺陷统计 — 6 类
    auto *defTitle = new QLabel("Defect Statistics");
    defTitle->setStyleSheet(
        "font-size: 10px; font-weight: bold; color: #495057;");
    cl->addWidget(defTitle);
    cl->addWidget(new GreenDefectBarChart);

    auto *barLabels = new QHBoxLayout;
    for (const char *lbl : {"Insect","Mold","Broken","Foreign","Under.","Other"}) {
        auto *l2 = new QLabel(lbl);
        l2->setAlignment(Qt::AlignCenter);
        l2->setStyleSheet("font-size: 7px; color: #6c757d; font-weight: bold;");
        barLabels->addWidget(l2);
    }
    cl->addLayout(barLabels);
    cl->addStretch();

    l->addWidget(content, 1);
    return panel;
}

// ════════════════════════════════════════════════════════════════════════════
//  buildResultsTable — 绿豆版本（7 列）
// ════════════════════════════════════════════════════════════════════════════
QWidget *GreenWindow::buildResultsTable()
{
    auto *panel = new QWidget;
    panel->setObjectName("resultsPanel");
    panel->setFixedHeight(190);

    auto *l = new QVBoxLayout(panel);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    auto *titleLbl = new QLabel("Detection Results");
    titleLbl->setObjectName("resultsTitleLbl");
    titleLbl->setFixedHeight(28);
    l->addWidget(titleLbl);

    auto *table = new QTableWidget(4, 7);
    table->setHorizontalHeaderLabels({
        "ID", "Timestamp", "Type", "Mesh", "Size Grade", "Defect Type", "Confidence"
    });
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->verticalHeader()->setDefaultSectionSize(28);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setShowGrid(false);

    const struct {
        const char *id, *ts, *type, *mesh, *grade, *defect, *conf, *defClr;
    } rows[] = {
        { "#89030","14:23:10.012","Green","19","Large",  "NONE",       "0.998","#16a34a" },
        { "#89031","14:23:10.045","Green","17","Medium", "INSECT",     "0.965","#dc2626" },
        { "#89032","14:23:10.088","Green","16","Small",  "BROKEN",     "0.942","#ca8a04" },
        { "#89033","14:23:10.120","Green","18","Medium", "UNDERSIZED", "0.978","#ea580c" },
    };
    for (int r = 0; r < 4; r++) {
        const char *cols[] = {
            rows[r].id, rows[r].ts, rows[r].type,
            rows[r].mesh, rows[r].grade, rows[r].defect, rows[r].conf
        };
        for (int c = 0; c < 7; c++) {
            auto *item = new QTableWidgetItem(cols[c]);
            item->setTextAlignment(
                c == 6 ? Qt::AlignRight | Qt::AlignVCenter
                       : Qt::AlignLeft  | Qt::AlignVCenter);
            if (c == 5) {
                item->setForeground(QColor(rows[r].defClr));
                QFont f = item->font(); f.setBold(true); item->setFont(f);
            }
            table->setItem(r, c, item);
        }
        if (r == 1) {
            for (int c = 0; c < 7; c++)
                table->item(r, c)->setBackground(QColor("#eff6ff"));
        }
    }
    l->addWidget(table, 1);
    return panel;
}

// ════════════════════════════════════════════════════════════════════════════
//  buildStatusBar
// ════════════════════════════════════════════════════════════════════════════
QWidget *GreenWindow::buildStatusBar()
{
    auto *bar = new QWidget;
    bar->setObjectName("statusBar");
    bar->setFixedHeight(30);

    auto *l = new QHBoxLayout(bar);
    l->setContentsMargins(8, 0, 8, 0);
    l->setSpacing(0);

    m_camDotLabel = new QLabel("●");
    m_camDotLabel->setStyleSheet("color: #dc3545; font-size: 10px;");
    m_camDotLabel->setFixedWidth(14);
    auto mkTxt = [](const char *t) {
        auto *l2 = new QLabel(t);
        l2->setStyleSheet("color: #343a40; font-size: 10px; font-weight: bold;");
        return l2;
    };
    l->addWidget(m_camDotLabel);
    l->addWidget(mkTxt("Cam 1"));
    l->addSpacing(8); l->addWidget(vLine()); l->addSpacing(8);
    l->addWidget(dot("#3b82f6")); l->addWidget(mkTxt("YOLO_v8_Coffee"));
    l->addSpacing(8); l->addWidget(vLine()); l->addSpacing(8);
    l->addWidget(dot("#3b82f6")); l->addWidget(mkTxt("DISK: 4.2TB FREE"));
    l->addSpacing(8); l->addWidget(vLine()); l->addSpacing(8);

    m_statusMsgLabel = new QLabel("[Ready] Waiting for connection...");
    m_statusMsgLabel->setStyleSheet(
        "color: #495057; font-size: 10px; font-style: italic;");
    m_statusMsgLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    l->addWidget(m_statusMsgLabel, 1);

    auto *recLbl = new QLabel("Recording:");
    recLbl->setStyleSheet("color: #495057; font-weight: bold; font-size: 10px;");
    l->addWidget(recLbl);
    l->addSpacing(4);

    auto *recFrame  = new QFrame;
    recFrame->setStyleSheet("QFrame { border: 1px solid #ced4da; background: transparent; }");
    auto *recLayout = new QHBoxLayout(recFrame);
    recLayout->setContentsMargins(0, 0, 0, 0); recLayout->setSpacing(0);
    auto mkRecBtn = [](const char *t, bool active) {
        auto *b = new QPushButton(t);
        b->setFixedHeight(20);
        b->setStyleSheet(active
            ? "QPushButton { background: #dee2e6; color: #212529;"
              " font-size: 10px; border: none; padding: 0 6px; border-radius: 0; }"
              "QPushButton:hover { background: #ced4da; }"
            : "QPushButton { background: #e9ecef; color: #adb5bd;"
              " font-size: 10px; border: none; padding: 0 6px; border-radius: 0; }");
        return b;
    };
    recLayout->addWidget(mkRecBtn("● START", true));
    recLayout->addWidget(mkRecBtn("■ STOP",  false));
    l->addWidget(recFrame);
    l->addSpacing(4);

    auto *pathFrame  = new QFrame;
    pathFrame->setStyleSheet("QFrame { border: 1px solid #ced4da; background: white; }");
    pathFrame->setFixedWidth(200);
    auto *pathLayout = new QHBoxLayout(pathFrame);
    pathLayout->setContentsMargins(4, 0, 4, 0); pathLayout->setSpacing(4);
    auto *pathLbl = new QLabel("D:/Data/Inspection_20231024/");
    pathLbl->setStyleSheet("color: #6c757d; font-size: 10px;");
    pathLbl->setMaximumWidth(134);
    pathLbl->setWordWrap(false);
    auto *browseBtn = new QPushButton("Browse");
    browseBtn->setStyleSheet(
        "QPushButton { color: #3b82f6; font-size: 10px; border: none;"
        " background: transparent; padding: 0; font-weight: bold; }");
    pathLayout->addWidget(pathLbl); pathLayout->addWidget(browseBtn);
    l->addWidget(pathFrame);

    return bar;
}

// ════════════════════════════════════════════════════════════════════════════
//  后台采集线程 — 持续从相机获取帧，存入共享缓冲
// ════════════════════════════════════════════════════════════════════════════
void GreenWindow::captureLoop()
{
    tSdkFrameHead sFrameInfo;
    BYTE *pbyBuffer = nullptr;

    while (m_captureRunning) {
        if (CameraGetImageBuffer(m_hCamera, &sFrameInfo, &pbyBuffer, 1000)
                != CAMERA_STATUS_SUCCESS)
            continue;

        CameraImageProcess(m_hCamera, pbyBuffer, m_pRgbBuffer, &sFrameInfo);

        int w = sFrameInfo.iWidth, h = sFrameInfo.iHeight;
        QImage img;
        if (sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8) {
            img = QImage(m_pRgbBuffer, w, h, w, QImage::Format_Grayscale8).copy();
        } else {
            cv::Mat bgr(h, w, CV_8UC3, m_pRgbBuffer), rgb;
            cv::cvtColor(bgr, rgb, cv::COLOR_BGR2RGB);
            img = QImage(rgb.data, w, h, (int)rgb.step, QImage::Format_RGB888).copy();
        }

        CameraReleaseImageBuffer(m_hCamera, pbyBuffer);

        {
            std::lock_guard<std::mutex> lock(m_frameMutex);
            m_latestFrame = std::move(img);
            m_frameReady  = true;
        }
        if (m_acquiring) {
            QMetaObject::invokeMethod(this, "grabFrame", Qt::QueuedConnection);
        }
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  Camera — CameraApi SDK
// ════════════════════════════════════════════════════════════════════════════
bool GreenWindow::connectCamera()
{
    int iCameraCounts = 1;
    tSdkCameraDevInfo devInfo;
    CameraEnumerateDevice(&devInfo, &iCameraCounts);
    printf("[GreenCamera] Enumerate: found %d device(s)\n", iCameraCounts);
    if (iCameraCounts == 0) {
        m_cameraView->setError("No camera device found. Check connection.");
        return false;
    }
    if (CameraInit(&devInfo, -1, -1, &m_hCamera) != CAMERA_STATUS_SUCCESS) {
        printf("[GreenCamera] CameraInit failed.\n");
        m_cameraView->setError("CameraInit failed. Check SDK / driver.");
        return false;
    }
    printf("[GreenCamera] CameraInit success, handle=%d\n", m_hCamera);

    CameraGetCapability(m_hCamera, &m_capability);

    // ── 分辨率 combo（动态填充）────────────────────────────────────────────
    m_resCombo->blockSignals(true);
    m_resCombo->clear();
    for (int i = 0; i < m_capability.iImageSizeDesc; i++) {
        m_resCombo->addItem(
            QString::fromLocal8Bit(m_capability.pImageSizeDesc[i].acDescription));
    }
    // 与 cope.cpp 保持一致：设置分辨率为第 4 个预设值
    if (m_capability.iImageSizeDesc > 3) {
        CameraSetImageResolution(m_hCamera, &m_capability.pImageSizeDesc[3]);
    }
    tSdkImageResolution res;
    CameraGetImageResolution(m_hCamera, &res);
    m_resCombo->setCurrentIndex(res.iIndex);
    m_resCombo->blockSignals(false);

    m_imageW = res.iWidth;
    m_imageH = res.iHeight;
    m_imageFmtLabel->setText(QString("%1 × %2 BUFFER").arg(m_imageW).arg(m_imageH));

    // ── 模拟增益 ──────────────────────────────────────────────────────────
    m_gainMin  = (int)m_capability.sExposeDesc.uiAnalogGainMin;
    m_gainMax  = (int)m_capability.sExposeDesc.uiAnalogGainMax;
    m_gainStep = m_capability.sExposeDesc.fAnalogGainStep;
    m_analogGainSlider->blockSignals(true);
    m_analogGainSlider->setRange(m_gainMin, m_gainMax);
    int curGain = m_gainMin;
    CameraGetAnalogGain(m_hCamera, &curGain);
    m_analogGainSlider->setValue(curGain);
    m_analogGainSlider->blockSignals(false);
    updateAnalogGainLabel(curGain);

    // ── 曝光时间 ──────────────────────────────────────────────────────────
    // 与 cope.cpp 保持一致：设置曝光时间为 800µs
    CameraSetExposureTime(m_hCamera, 800.0);
    
    CameraGetExposureLineTime(m_hCamera, &m_fExpLineTime);
    if (m_fExpLineTime <= 0) m_fExpLineTime = 1.0;

    int lineMin = (int)m_capability.sExposeDesc.uiExposeTimeMin;
    int lineMax = (int)m_capability.sExposeDesc.uiExposeTimeMax;
    int usMin   = (int)(lineMin * m_fExpLineTime);
    int usMax   = (int)(lineMax * m_fExpLineTime);

    m_exposureSlider->setRange(lineMin, lineMax);
    m_exposureSpin->setRange(usMin, usMax);

    double curExp = 0;
    CameraGetExposureTime(m_hCamera, &curExp);
    int curLines = (int)(curExp / m_fExpLineTime);
    m_syncBlock = true;
    m_exposureSlider->setValue(qBound(lineMin, curLines, lineMax));
    m_exposureSpin->setValue((int)curExp);
    m_syncBlock = false;

    // ── 缓冲区 + 输出格式 ─────────────────────────────────────────────────
    m_pRgbBuffer = (unsigned char *)malloc(m_imageW * m_imageH * 3);
    if (m_capability.sIspCapacity.bMonoSensor) {
        m_channel = 1;
        CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8);
    } else {
        m_channel = 3;
        CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_BGR8);
    }

    CameraPlay(m_hCamera);
    setConnectedState(true);

    // 启动后台采集线程，连接后自动开始采集
    m_captureRunning = true;
    m_captureThread  = std::thread(&GreenWindow::captureLoop, this);
    setAcquiringState(true);
    m_fpsTimer.invalidate();
    m_frameCount = 0;
    //m_timer->start(30);  // 改为由 captureLoop 直接触发刷新
    return true;
}

void GreenWindow::disconnectCamera()
{
    if (!m_cameraConnected) return;

    m_captureRunning = false;
    if (m_captureThread.joinable())
        m_captureThread.join();

    CameraUnInit(m_hCamera);
    if (m_pRgbBuffer) { free(m_pRgbBuffer); m_pRgbBuffer = nullptr; }
    m_hCamera = -1;

    m_resCombo->blockSignals(true);
    m_resCombo->clear();
    m_resCombo->blockSignals(false);

    setConnectedState(false);
    setAcquiringState(false);
    m_cameraView->setPixmap(QPixmap());
    m_imageFmtLabel->setText("-- × -- BUFFER");
    m_fpsLabel->setText("-- FPS");
}

// ════════════════════════════════════════════════════════════════════════════
//  grabFrame — 从共享缓冲读帧并显示
//  （主线程，由 captureLoop 每帧后通过 queued invokeMethod 触发，
//   不再依赖固定 Timer）
// ════════════════════════════════════════════════════════════════════════════
void GreenWindow::grabFrame()
{
    if (!m_cameraConnected || !m_acquiring) return;

    QImage img;
    {
        std::lock_guard<std::mutex> lock(m_frameMutex);
        if (!m_frameReady) return;
        img = m_latestFrame;
        m_frameReady = false;
    }

    // 数字增益：软件后处理
    if (qAbs(m_digitalGain - 1.0) > 0.01) {
        img.detach();
        int type = (img.format() == QImage::Format_Grayscale8) ? CV_8UC1 : CV_8UC3;
        cv::Mat mat(img.height(), img.width(), type, img.bits(), (size_t)img.bytesPerLine());
        mat.convertTo(mat, -1, m_digitalGain, 0);
    }

    m_cameraView->setPixmap(QPixmap::fromImage(img));

    m_frameCount++;
    if (!m_fpsTimer.isValid()) {
        m_fpsTimer.start();
    } else if (m_fpsTimer.elapsed() >= 1000) {
        m_fpsLabel->setText(
            QString("%1 FPS").arg(m_frameCount * 1000.0 / m_fpsTimer.elapsed(), 0, 'f', 1));
        m_frameCount = 0;
        m_fpsTimer.restart();
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  Slots
// ════════════════════════════════════════════════════════════════════════════
void GreenWindow::onExposureSliderMoved(int lines)
{
    if (m_syncBlock) return;
    double us = lines * m_fExpLineTime;
    m_syncBlock = true;
    m_exposureSpin->setValue((int)us);
    m_syncBlock = false;
    if (m_cameraConnected) {
        CameraSetExposureTime(m_hCamera, us);
        showStatus(QString("Exposure → %1 µs (%2 lines)").arg((int)us).arg(lines));
    }
}

void GreenWindow::onExposureSpinChanged(int us)
{
    if (m_syncBlock) return;
    m_syncBlock = true;
    if (m_fExpLineTime > 0)
        m_exposureSlider->setValue((int)(us / m_fExpLineTime));
    m_syncBlock = false;
    if (m_cameraConnected) {
        CameraSetExposureTime(m_hCamera, (double)us);
        showStatus(QString("Exposure → %1 µs").arg(us));
    }
}

void GreenWindow::onAnalogGainChanged(int val)
{
    updateAnalogGainLabel(val);
    if (m_cameraConnected) {
        CameraSetAnalogGain(m_hCamera, val);
        showStatus(QString("Analog Gain → %1  (%2x)")
                       .arg(val).arg(double(val) * m_gainStep, 0, 'f', 2));
    }
}

void GreenWindow::updateAnalogGainLabel(int gain)
{
    m_analogGainLabel->setText(
        QString("%1x").arg(double(gain) * m_gainStep, 0, 'f', 2));
}

void GreenWindow::onDigitalGainChanged(int val)
{
    m_digitalGain = 1.0 + val * 0.01;
    m_digitalGainLabel->setText(QString("%1x").arg(m_digitalGain, 0, 'f', 2));
}

void GreenWindow::onResolutionChanged(int index)
{
    if (!m_cameraConnected || index < 0 || index >= m_capability.iImageSizeDesc)
        return;

    CameraSetImageResolution(m_hCamera, &m_capability.pImageSizeDesc[index]);

    tSdkImageResolution res;
    CameraGetImageResolution(m_hCamera, &res);
    m_imageW = res.iWidth;
    m_imageH = res.iHeight;
    m_imageFmtLabel->setText(QString("%1 × %2 BUFFER").arg(m_imageW).arg(m_imageH));

    if (m_pRgbBuffer) { free(m_pRgbBuffer); }
    m_pRgbBuffer = (unsigned char *)malloc(m_imageW * m_imageH * 3);

    showStatus(QString("Resolution → %1 × %2").arg(m_imageW).arg(m_imageH));
}

void GreenWindow::onConnectCamera()
{
    if (!m_cameraConnected) {
        showStatus("Connecting to camera...");
        if (connectCamera()) {
            m_connectBtn->setText("Disconnect");
            showStatus("Camera connected successfully.");
        } else {
            showStatus("ERROR: Failed to connect camera.");
        }
    } else {
        if (m_acquiring) onStop();
        disconnectCamera();
        m_connectBtn->setText("Connect Camera");
        showStatus("Camera disconnected.");
    }
}

void GreenWindow::onStart()
{
    setAcquiringState(true);
    m_fpsTimer.invalidate();
    m_frameCount = 0;
    // 预览更新由 captureLoop 信号触发
    showStatus("Acquisition started.");
}

void GreenWindow::onStop()
{
    m_timer->stop();
    setAcquiringState(false);
    showStatus("Acquisition stopped.");
}

void GreenWindow::setConnectedState(bool c)
{
    m_cameraConnected = c;
    m_connDotLabel->setStyleSheet(
        c ? "color: #28a745; font-size: 14px;" : "color: #dc3545; font-size: 14px;");
    m_connTextLabel->setText(c ? "Connected" : "Disconnected");
    m_connTextLabel->setStyleSheet(
        c ? "color: #28a745; font-size: 11px; font-weight: bold;"
          : "color: #6c757d; font-size: 11px;");
    m_camDotLabel->setStyleSheet(
        c ? "color: #28a745; font-size: 10px;" : "color: #dc3545; font-size: 10px;");
    m_resCombo->setEnabled(c);
    m_exposureSlider->setEnabled(c);
    m_exposureSpin->setEnabled(c);
    m_analogGainSlider->setEnabled(c);
    m_startBtn->setEnabled(c);
}

void GreenWindow::setAcquiringState(bool a)
{
    m_acquiring = a;
    m_startBtn->setEnabled(!a && m_cameraConnected);
    m_stopBtn->setEnabled(a);
}

void GreenWindow::showStatus(const QString &msg)
{
    m_statusMsgLabel->setText(
        QString("[%1] %2")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"), msg));
}
