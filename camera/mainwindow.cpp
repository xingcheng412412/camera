#include "mainwindow.h"

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
#include <QWheelEvent>
#include <QMouseEvent>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QProgressBar>
#include <QPainter>
#include <QPen>
#include <QScrollArea>
#include <QTimer>
#include <QInputDialog>
#include <QDir>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <cstdio>

// ════════════════════════════════════════════════════════════════════════════
//  全局样式表  ── 不在此设置任何 min/max 尺寸，全部交给 C++ 代码管控
// ════════════════════════════════════════════════════════════════════════════
static const char *APP_STYLE = R"(
* { font-family: 'Segoe UI', Arial, sans-serif; font-size: 12px; }
QMainWindow, QWidget { background-color: #dee2e6; }
QLabel { background: transparent; }

/* ── Header ── */
QWidget#header { background-color: #e9ecef; border-bottom: 1px solid #adb5bd; }

/* ── Sidebar ── */
QWidget#sidebar { background-color: #f8f9fa; border-right: 1px solid #ced4da; }
QLabel#secHeader {
    background-color: #dee2e6; color: #343a40;
    font-weight: bold; font-size: 11px;
    padding: 0 10px;
    border-bottom: 1px solid #ced4da;
}
QLabel#paramLbl { color: #495057; font-weight: bold; font-size: 11px; }
QLabel#subLbl   { color: #6c757d; font-size: 11px; }
QLabel#valueLbl { color: #3b82f6; font-size: 11px; font-weight: bold; font-family: monospace; }

/* ── Image area ── */
QWidget#imageArea   { background-color: #dee2e6; }
QWidget#imgTitleBar { background-color: #dee2e6; border-bottom: 1px solid #adb5bd; }
QLabel#imgTitleLbl  { color: #343a40; font-size: 11px; font-weight: bold; padding: 0 8px; }
QLabel#imgFmtLbl    { color: #6c757d; font-size: 11px;
                       font-family: monospace; padding: 0 8px; }

/* ── Analytics panel ── */
QWidget#analyticsPanel   { background-color: #f8f9fa; border-left: 1px solid #ced4da; }
QWidget#analyticsTitleBar{ background-color: #dee2e6; border-bottom: 1px solid #ced4da; }
QLabel#analyticsTitleLbl { color: #343a40; font-size: 11px; font-weight: bold; padding: 0 8px; }
QWidget#kpiCard          { background: white; border: 1px solid #ced4da; }

/* ── Results table ── */
QWidget#resultsPanel   { background: white; border-top: 1px solid #ced4da; }
QLabel#resultsTitleLbl {
    background-color: #dee2e6; color: #343a40;
    font-size: 11px; font-weight: bold;
    padding: 0 8px; border-bottom: 1px solid #ced4da;
}
QTableWidget {
    border: none; background: white;
    gridline-color: #e9ecef;
    font-family: monospace; font-size: 11px;
}
QHeaderView::section {
    background: #f8f9fa; border: none;
    border-right: 1px solid #dee2e6;
    border-bottom: 1px solid #dee2e6;
    padding: 3px 6px; font-weight: bold;
    font-size: 10px; color: #495057;
}
QTableWidget::item { padding: 2px 6px; border-right: 1px solid #f0f0f0; }
QTableWidget::item:selected { background: #e9ecef; color: #212529; }

/* ── Status bar ── */
QWidget#statusBar { background-color: #dee2e6; border-top: 1px solid #adb5bd; }

/* ── Sliders ── */
QSlider::groove:horizontal {
    height: 4px; background: #ced4da; border-radius: 2px;
}
QSlider::handle:horizontal {
    background: white; width: 14px; height: 14px;
    border-radius: 7px; margin: -5px 0; border: 2px solid #6c757d;
}
QSlider::sub-page:horizontal { background: #3b82f6; border-radius: 2px; }
QSlider::handle:horizontal:hover    { border-color: #343a40; }
QSlider::handle:horizontal:disabled { background: #dee2e6; border-color: #ced4da; }

/* ── SpinBox / LineEdit / ComboBox ── */
QSpinBox, QLineEdit, QComboBox {
    border: 1px solid #ced4da; border-radius: 2px;
    padding: 2px 4px; background: white;
}

/* ── Progress bars ── */
QProgressBar { border: none; background: #dee2e6; border-radius: 0; }
QProgressBar::chunk { border-radius: 0; }
QProgressBar#lightBar::chunk  { background: #ca8a04; }
QProgressBar#mediumBar::chunk { background: #92400e; }
QProgressBar#darkBar::chunk   { background: #1f2937; }

/* ── Buttons ── */
QPushButton {
    border: 1px solid #adb5bd;
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #f8f9fa, stop:1 #e9ecef);
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
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #3b82f6, stop:1 #2563eb);
    color: white; border-color: #1d4ed8;
    font-weight: bold; padding: 7px 10px; letter-spacing: 1px;
}
QPushButton#startBtn:hover    { background: #2563eb; }
QPushButton#startBtn:disabled { background: #93c5fd; border-color: #bfdbfe; }
QPushButton#stopBtn {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #ef4444, stop:1 #dc2626);
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
    border: none; font-size: 10px; font-weight: bold;
    padding: 0 6px; border-radius: 0;
}
QPushButton#tabInactive:hover { background: #f0f0f0; }
)";

// APP_STYLE 中所有显式 font-size 进行等比例缩放
// 使用两步替换，避免不同基准尺寸四舍五入后相等时发生级联替换
static QString makeStyle(double s)
{
    auto scaled = [s](int base) -> QString {
        return QString::number(qMax(8, qRound(base * s))) + "px";
    };
    QString st = QLatin1String(APP_STYLE);
    st.replace("font-size: 12px", "font-size: __SZ12__");
    st.replace("font-size: 11px", "font-size: __SZ11__");
    st.replace("font-size: 10px", "font-size: __SZ10__");
    st.replace("__SZ12__", scaled(12));
    st.replace("__SZ11__", scaled(11));
    st.replace("__SZ10__", scaled(10));
    return st;
}

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
//  DefectBarChart
// ════════════════════════════════════════════════════════════════════════════
class DefectBarChart : public QWidget
{
public:
    explicit DefectBarChart(QWidget *p = nullptr) : QWidget(p)
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
            { QColor("#f87171"), 0.60f },
            { QColor("#fb923c"), 0.30f },
            { QColor("#fbbf24"), 0.85f },
            { QColor("#94a3b8"), 0.15f },
        };
        const int n = 4, padT = 6, padB = 2;
        int maxH = height() - padT - padB;
        int barW = 14;
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
//  CameraView — 图像显示 + 检测框叠层 + 迷你工具栏
// ════════════════════════════════════════════════════════════════════════════
class CameraView : public QWidget
{
public:
    explicit CameraView(QWidget *parent = nullptr);
    void setPixmap(const QPixmap &px);
    QSize sizeHint() const override { return { 800, 500 }; }

    void zoomIn();
    void zoomOut();
    void zoomFit();

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *ev) override;
    void showEvent(QShowEvent *ev) override;
    void wheelEvent(QWheelEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    void reposition();
    void updateZoomLabel();
    void clampOffset();

    QPixmap  m_pixmap;
    QRect    m_imgRect;
    double   m_zoomFactor  = 1.0;
    QPoint   m_offset;           // 平移偏移
    QPoint   m_dragStart;
    bool     m_dragging    = false;
    QLabel  *m_coordLabel  = nullptr;
    QWidget *m_miniToolbar = nullptr;
    QLabel  *m_zoomLabel   = nullptr;
};

CameraView::CameraView(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background: black;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // ── 坐标显示 ─────────────────────────────────────────────────────────
    m_coordLabel = new QLabel("X: 204.22  Y: 852.19 | Value: 128", this);
    m_coordLabel->setStyleSheet(
        "QLabel { color: #4ade80; font-family: monospace; font-size: 9px;"
        "         background-color: rgba(0,0,0,160); padding: 2px 6px; }");
    m_coordLabel->adjustSize();
    m_coordLabel->hide(); // 初始隐藏，等 showEvent 后定位再显示

    // ── 迷你工具栏 ────────────────────────────────────────────────────────
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
    auto *btnIn  = mkBtn("⊕");
    auto *btnOut = mkBtn("⊖");
    auto *btnFit = mkBtn("⊙");
    connect(btnIn,  &QPushButton::clicked, this, [this]{ zoomIn(); });
    connect(btnOut, &QPushButton::clicked, this, [this]{ zoomOut(); });
    connect(btnFit, &QPushButton::clicked, this, [this]{ zoomFit(); });
    tl->addWidget(btnIn);
    tl->addWidget(btnOut);
    auto *sep = new QFrame;
    sep->setFixedHeight(1);
    sep->setStyleSheet("QFrame { background: rgba(255,255,255,60); }");
    tl->addWidget(sep);
    tl->addWidget(btnFit);
    m_miniToolbar->adjustSize();
    m_miniToolbar->hide();

    // ── 缩放比例标签 ──────────────────────────────────────────────────────
    m_zoomLabel = new QLabel("1:1 Fit", this);
    m_zoomLabel->setStyleSheet(
        "QLabel { color: #facc15; font-family: monospace; font-size: 9px;"
        "         background-color: rgba(0,0,0,160); padding: 2px 5px; }");
    m_zoomLabel->adjustSize();
    m_zoomLabel->hide();
}

void CameraView::setPixmap(const QPixmap &px)
{
    m_pixmap = px;
    update();
}

void CameraView::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev);
    // 延迟到布局完成后再定位，避免 showEvent 时 size() 还未确定
    QTimer::singleShot(0, this, [this] {
        reposition();
        m_coordLabel->show();
        m_miniToolbar->show();
        m_zoomLabel->show();
        m_coordLabel->raise();
        m_miniToolbar->raise();
        m_zoomLabel->raise();
    });
}

void CameraView::resizeEvent(QResizeEvent *ev)
{
    QWidget::resizeEvent(ev);
    reposition();
}

void CameraView::reposition()
{
    if (m_miniToolbar) {
        m_miniToolbar->adjustSize();
        m_miniToolbar->move(width() - m_miniToolbar->width() - 6, 6);
    }
    if (m_coordLabel) {
        m_coordLabel->adjustSize();
        m_coordLabel->move(6, height() - m_coordLabel->height() - 6);
    }
    if (m_zoomLabel) {
        m_zoomLabel->adjustSize();
        // 缩放标签放在右下角，miniToolbar 正下方
        m_zoomLabel->move(width() - m_zoomLabel->width() - 6,
                          height() - m_zoomLabel->height() - 6);
    }
}

void CameraView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.fillRect(rect(), Qt::black);

    if (m_pixmap.isNull()) {
        // 画一个居中的占位面板
        int pw = 320, ph = 120;
        int px2 = (width()  - pw) / 2;
        int py2 = (height() - ph) / 2;
        QRect panel(px2, py2, pw, ph);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(40, 44, 52, 200));
        p.drawRoundedRect(panel, 8, 8);
        p.setPen(QColor("#6b7280"));
        p.drawRoundedRect(panel.adjusted(0, 0, -1, -1), 8, 8);

        p.setPen(QColor("#9ca3af"));
        p.setFont(QFont("Arial", 22));
        p.drawText(QRect(px2, py2 + 14, pw, 36), Qt::AlignCenter, "⬛");
        p.setFont(QFont("Arial", 12, QFont::Bold));
        p.setPen(QColor("#d1d5db"));
        p.drawText(QRect(px2, py2 + 52, pw, 22), Qt::AlignCenter, "No Camera Signal");
        p.setFont(QFont("Arial", 9));
        p.setPen(QColor("#6b7280"));
        p.drawText(QRect(px2, py2 + 78, pw, 18), Qt::AlignCenter,
                   "Click  \"Connect Camera\"  to begin");
        return;
    }

    // 以适应窗口为基准（1x），再乘以缩放因子，始终保持等比例
    QSize fitSize = m_pixmap.size().scaled(size(), Qt::KeepAspectRatio);
    int zw = int(fitSize.width()  * m_zoomFactor);
    int zh = int(fitSize.height() * m_zoomFactor);
    int ix = (width()  - zw) / 2 + m_offset.x();
    int iy = (height() - zh) / 2 + m_offset.y();
    m_imgRect = QRect(ix, iy, zw, zh);
    p.drawPixmap(m_imgRect, m_pixmap);

}

// ════════════════════════════════════════════════════════════════════════════
//  CameraView — 等比例缩放
//  1x = fit-to-window；每档 ×1.25，最大 8x，最小 1x（不小于适应窗口）
// ════════════════════════════════════════════════════════════════════════════
void CameraView::zoomIn()
{
    m_zoomFactor = qMin(m_zoomFactor * 1.25, 16.0);
    clampOffset();
    updateZoomLabel();
    update();
}

void CameraView::zoomOut()
{
    m_zoomFactor = qMax(m_zoomFactor / 1.25, 1.0);
    clampOffset();
    updateZoomLabel();
    update();
}

void CameraView::zoomFit()
{
    m_zoomFactor = 1.0;
    m_offset = {};
    updateZoomLabel();
    update();
}

void CameraView::clampOffset()
{
    if (m_pixmap.isNull()) { m_offset = {}; return; }
    QSize fitSize = m_pixmap.size().scaled(size(), Qt::KeepAspectRatio);
    int zw = int(fitSize.width()  * m_zoomFactor);
    int zh = int(fitSize.height() * m_zoomFactor);
    int maxX = qMax(0, (zw - width())  / 2);
    int maxY = qMax(0, (zh - height()) / 2);
    m_offset.setX(qBound(-maxX, m_offset.x(), maxX));
    m_offset.setY(qBound(-maxY, m_offset.y(), maxY));
}

void CameraView::wheelEvent(QWheelEvent *ev)
{
    if (ev->angleDelta().y() > 0)
        zoomIn();
    else
        zoomOut();
    ev->accept();
}

void CameraView::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        m_dragStart = ev->pos() - m_offset;
        m_dragging  = true;
        setCursor(Qt::ClosedHandCursor);
    }
}

void CameraView::mouseMoveEvent(QMouseEvent *ev)
{
    if (m_dragging) {
        m_offset = ev->pos() - m_dragStart;
        clampOffset();
        update();
    }
}

void CameraView::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        m_dragging = false;
        setCursor(Qt::ArrowCursor);
    }
}

void CameraView::updateZoomLabel()
{
    if (!m_zoomLabel) return;
    if (m_zoomFactor <= 1.0 + 1e-6)
        m_zoomLabel->setText("Fit");
    else
        m_zoomLabel->setText(QString("%1x").arg(m_zoomFactor, 0, 'f', 2));
    m_zoomLabel->adjustSize();
    reposition();
}

// ════════════════════════════════════════════════════════════════════════════
//  MainWindow — 构造 / 析构
// ════════════════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("GigE Line-Scan Inspector");
    resize(1440, 860);
    setMinimumSize(1100, 680);
    setStyleSheet(makeStyle(1.0));

    CameraSdkInit(1);
    setupUI();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::grabFrame);
}

MainWindow::~MainWindow()
{
    m_timer->stop();
    disconnectCamera();
}

// ════════════════════════════════════════════════════════════════════════════
//  setupUI
// ════════════════════════════════════════════════════════════════════════════
void MainWindow::setupUI()
{
    auto *central    = new QWidget(this);
    auto *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    setCentralWidget(central);

    // 主区域：侧边栏 + 工作区
    auto *mainArea   = new QWidget;
    auto *mainLayout = new QHBoxLayout(mainArea);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 工作区：上排（图像 + 分析面板） + 下排（结果表格）
    auto *workspace      = new QWidget;
    auto *workspaceLayout= new QVBoxLayout(workspace);
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
    imgTitleLayout->addSpacing(6);
    m_fullscreenBtn = new QPushButton("⛶");
    m_fullscreenBtn->setFixedSize(22, 22);
    m_fullscreenBtn->setToolTip("Fullscreen  ( F11 / Esc )");
    m_fullscreenBtn->setStyleSheet(
        "QPushButton { border: none; background: transparent; color: #6c757d;"
        "  font-size: 13px; border-radius: 2px; padding: 0; }"
        "QPushButton:hover { background: #dee2e6; color: #212529; }");
    connect(m_fullscreenBtn, &QPushButton::clicked, this, &MainWindow::onToggleFullscreen);
    imgTitleLayout->addWidget(m_fullscreenBtn);
    imgTitleLayout->setContentsMargins(0, 0, 4, 0);

    m_cameraView = new CameraView;
    imageAreaLayout->addWidget(imgTitleBar);
    imageAreaLayout->addWidget(m_cameraView, 1);

    topRowLayout->addWidget(imageArea, 4);          // 图像区 ≈ 80%
    m_analyticsWidget = buildAnalytics();
    topRowLayout->addWidget(m_analyticsWidget, 1);  // 分析面板 ≈ 20%

    workspaceLayout->addWidget(topRow, 1);
    m_resultsWidget = buildResultsTable();
    workspaceLayout->addWidget(m_resultsWidget);

    m_sidebarWidget = buildSidebar();
    mainLayout->addWidget(m_sidebarWidget, 1);   // 侧边栏 ≈ 17%
    mainLayout->addWidget(workspace, 5);         // 工作区 ≈ 83%

    m_headerWidget = buildHeader();
    rootLayout->addWidget(m_headerWidget);
    rootLayout->addWidget(mainArea, 1);
    m_statusWidget = buildStatusBar();
    rootLayout->addWidget(m_statusWidget);
}

// ════════════════════════════════════════════════════════════════════════════
//  buildHeader
// ════════════════════════════════════════════════════════════════════════════
QWidget *MainWindow::buildHeader()
{
    auto *header = new QWidget;
    header->setObjectName("header");
    header->setFixedHeight(48);  // ← 用代码控制高度，不依赖样式表

    auto *l = new QHBoxLayout(header);
    l->setContentsMargins(14, 0, 14, 0);
    l->setSpacing(6);

    // 图标 + 标题
    auto *icon = new QLabel("📷");
    icon->setStyleSheet("font-size: 16px;");
    icon->setFixedWidth(24);
    auto *title = new QLabel("GigE Line-Scan Inspector");
    title->setStyleSheet(
        "font-weight: bold; font-size: 14px; color: #212529; letter-spacing: 1px;");

    l->addWidget(icon);
    l->addWidget(title);
    l->addSpacing(8);
    l->addWidget(vLine());
    l->addSpacing(8);

    // 连接状态
    m_connDotLabel = new QLabel("●");
    m_connDotLabel->setStyleSheet("color: #dc3545; font-size: 14px;");
    m_connDotLabel->setFixedWidth(18);
    m_connTextLabel = new QLabel("Disconnected");
    m_connTextLabel->setStyleSheet("color: #6c757d; font-size: 11px;");

    // FPS
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

    // Bean mode toggle（视觉占位）
    auto *beanLabel = new QLabel("Bean Mode");
    beanLabel->setStyleSheet(
        "color: #6c757d; font-size: 10px; font-weight: bold;");
    auto *beanGreen   = new QPushButton("Green Beans");
    beanGreen->setObjectName("beanInactive");
    beanGreen->setFixedHeight(24);
    auto *beanRoasted = new QPushButton("Roasted Beans ◎");
    beanRoasted->setObjectName("beanActive");
    beanRoasted->setFixedHeight(24);

    auto *beanFrame  = new QFrame;
    beanFrame->setStyleSheet(
        "QFrame { border: 1px solid #ced4da; background: transparent; }");
    auto *beanLayout = new QHBoxLayout(beanFrame);
    beanLayout->setContentsMargins(0, 0, 0, 0);
    beanLayout->setSpacing(0);
    beanLayout->addWidget(beanGreen);
    beanLayout->addWidget(beanRoasted);

    l->addWidget(beanLabel);
    l->addSpacing(4);
    l->addWidget(beanFrame);
    l->addSpacing(8);

    // 按钮
    m_connectBtn = new QPushButton("Connect Camera");
    m_connectBtn->setFixedHeight(26);
    connect(m_connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectCamera);
    auto *settingsBtn = new QPushButton("Settings");
    settingsBtn->setFixedHeight(26);

    l->addWidget(m_connectBtn);
    l->addWidget(settingsBtn);
    l->addSpacing(8);
    l->addWidget(vLine());
    l->addSpacing(8);

    auto *opLbl = new QLabel("Operator:");
    opLbl->setStyleSheet(
        "color: #6c757d; font-size: 10px; font-weight: bold;");
    auto *opName = new QLabel("QC_Eng_04");
    opName->setStyleSheet("font-size: 12px; font-weight: bold; color: #212529;");
    l->addWidget(opLbl);
    l->addWidget(opName);

    return header;
}

// ════════════════════════════════════════════════════════════════════════════
//  buildSidebar
// ════════════════════════════════════════════════════════════════════════════
QWidget *MainWindow::buildSidebar()
{
    auto *sidebar = new QWidget;
    sidebar->setObjectName("sidebar");
    sidebar->setMinimumWidth(220);   // 最小宽度，允许等比例扩展

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
        auto *lbl = new QLabel("Resolution");
        lbl->setObjectName("paramLbl");
        m_resCombo = new QComboBox;
        m_resCombo->setEnabled(false);
        m_resCombo->setPlaceholderText("-- Connect camera --");
        contentLayout->addWidget(lbl);
        contentLayout->addWidget(m_resCombo);
        connect(m_resCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::onResolutionChanged);
    }
    contentLayout->addWidget(hLine());

    // ── 曝光 ─────────────────────────────────────────────────────────────
    {
        auto *expLbl = new QLabel("Exposure (µs)");
        expLbl->setObjectName("paramLbl");

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
                this, &MainWindow::onExposureSliderMoved);
        connect(m_exposureSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &MainWindow::onExposureSpinChanged);
    }
    contentLayout->addWidget(hLine());

    // ── 线频设置 ──────────────────────────────────────────────────────────
    {
        auto *rateLbl = new QLabel("Line Rate (Hz)");
        rateLbl->setObjectName("paramLbl");

        m_frameRateValue = new QLabel("--");
        m_frameRateValue->setObjectName("valueLbl");
        m_frameRateBtn = new QPushButton("Set");
        m_frameRateBtn->setEnabled(false);

        auto *row = new QHBoxLayout;
        row->addWidget(rateLbl);
        row->addStretch();
        row->addWidget(m_frameRateValue);
        row->addWidget(m_frameRateBtn);
        contentLayout->addLayout(row);

        connect(m_frameRateBtn, &QPushButton::clicked,
                this, &MainWindow::onFrameRateButtonClicked);
    }

    contentLayout->addWidget(hLine());

    // ── 增益控制 ──────────────────────────────────────────────────────────
    {
        auto *gainTitle = new QLabel("Gain Control");
        gainTitle->setObjectName("paramLbl");
        contentLayout->addWidget(gainTitle);

        // 模拟增益
        auto *anaRow = new QHBoxLayout;
        auto *anaLbl = new QLabel("Analog"); anaLbl->setObjectName("subLbl");
        m_gainValueLabel = new QLabel("1.00x"); m_gainValueLabel->setObjectName("valueLbl");
        anaRow->addWidget(anaLbl); anaRow->addStretch(); anaRow->addWidget(m_gainValueLabel);

        m_gainSlider = new QSlider(Qt::Horizontal);
        m_gainSlider->setRange(16, 256); m_gainSlider->setValue(16);
        m_gainSlider->setEnabled(false);

        contentLayout->addLayout(anaRow);
        contentLayout->addWidget(m_gainSlider);
        connect(m_gainSlider, &QSlider::valueChanged, this, &MainWindow::onAnalogGainChanged);

        // 数字增益（视觉占位）
        auto *digRow = new QHBoxLayout;
        auto *digLbl = new QLabel("Digital"); digLbl->setObjectName("subLbl");
        auto *digVal = new QLabel("1.0x");    digVal->setObjectName("valueLbl");
        digRow->addWidget(digLbl); digRow->addStretch(); digRow->addWidget(digVal);

        auto *digSlider = new QSlider(Qt::Horizontal);
        digSlider->setRange(0, 100); digSlider->setValue(0);
        connect(digSlider, &QSlider::valueChanged, [digVal](int v) {
            digVal->setText(QString("%1x").arg(1.0 + v * 0.01, 0, 'f', 1));
        });
        contentLayout->addLayout(digRow);
        contentLayout->addWidget(digSlider);
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

        auto *wbGrid = new QGridLayout;
        wbGrid->setSpacing(4);
        const struct { const char *lbl; const char *clr; const char *val; } ch[] = {
            {"R","#dc2626","1.02"}, {"G","#16a34a","1.00"}, {"B","#2563eb","1.15"}
        };
        for (int i = 0; i < 3; i++) {
            auto *cl = new QLabel(ch[i].lbl);
            cl->setAlignment(Qt::AlignCenter);
            cl->setStyleSheet(QString(
                "color: %1; font-weight: bold; font-size: 10px;").arg(ch[i].clr));
            auto *inp = new QLineEdit(ch[i].val);
            inp->setAlignment(Qt::AlignCenter);
            inp->setFixedHeight(22);
            wbGrid->addWidget(cl,  0, i);
            wbGrid->addWidget(inp, 1, i);
        }
        contentLayout->addLayout(wbGrid);
    }
    contentLayout->addWidget(hLine());

    // ── 采集控制 ──────────────────────────────────────────────────────────
    {
        auto *cb = new QCheckBox("Live View Overlay");
        cb->setChecked(true);
        contentLayout->addWidget(cb);

        m_startBtn = new QPushButton("Start Inspection");
        m_startBtn->setObjectName("startBtn");
        m_startBtn->setEnabled(false);
        connect(m_startBtn, &QPushButton::clicked, this, &MainWindow::onStart);

        m_stopBtn = new QPushButton("Stop Acquisition");
        m_stopBtn->setObjectName("stopBtn");
        m_stopBtn->setEnabled(false);
        connect(m_stopBtn, &QPushButton::clicked, this, &MainWindow::onStop);

        contentLayout->addWidget(m_startBtn);
        contentLayout->addWidget(m_stopBtn);
    }
    contentLayout->addStretch();

    sideLayout->addWidget(content, 1);
    return sidebar;
}

// ════════════════════════════════════════════════════════════════════════════
//  buildAnalytics
// ════════════════════════════════════════════════════════════════════════════
QWidget *MainWindow::buildAnalytics()
{
    auto *panel = new QWidget;
    panel->setObjectName("analyticsPanel");
    panel->setMinimumWidth(180);   // 最小宽度，允许等比例扩展

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

    // Tab 行 ── 用单独 QFrame 做下边框，避免样式污染按钮
    auto *tabContainer = new QWidget;
    tabContainer->setStyleSheet("QWidget { background: #f0f0f0; }");
    auto *tabVBox = new QVBoxLayout(tabContainer);
    tabVBox->setContentsMargins(0, 0, 0, 0);
    tabVBox->setSpacing(0);

    auto *tabRow = new QWidget;
    auto *tabLayout = new QHBoxLayout(tabRow);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->setSpacing(0);

    auto *tabG = new QPushButton("GREEN BEANS");
    tabG->setObjectName("tabInactive"); tabG->setFixedHeight(26);
    auto *tabR = new QPushButton("ROASTED BEANS");
    tabR->setObjectName("tabActive"); tabR->setFixedHeight(26);

    tabLayout->addWidget(tabG);
    tabLayout->addWidget(vLine());
    tabLayout->addWidget(tabR);

    tabVBox->addWidget(tabRow);
    tabVBox->addWidget(hLine()); // 用分隔线代替 border-bottom
    l->addWidget(tabContainer);

    // 内容区域（直接布局，不使用 QScrollArea）
    auto *content = new QWidget;
    auto *cl      = new QVBoxLayout(content);
    cl->setContentsMargins(8, 8, 8, 8);
    cl->setSpacing(10);

    // KPI 卡片
    auto *kpiGrid = new QGridLayout;
    kpiGrid->setSpacing(6);
    auto mkCard = [](const char *name, const char *val,
                     const char *valClr, const char *trend, const char *trendClr) {
        auto *card = new QWidget; card->setObjectName("kpiCard");
        card->setFixedHeight(64);
        auto *cl2  = new QVBoxLayout(card);
        cl2->setContentsMargins(6, 4, 6, 4); cl2->setSpacing(0);
        auto *n = new QLabel(name);
        n->setStyleSheet("font-size: 9px; color: #6c757d; font-weight: bold;");
        auto *v = new QLabel(val);
        v->setStyleSheet(QString(
            "font-size: 20px; font-weight: bold; color: %1;").arg(valClr));
        auto *t = new QLabel(trend);
        t->setStyleSheet(QString("font-size: 9px; color: %1;").arg(trendClr));
        cl2->addWidget(n); cl2->addWidget(v); cl2->addWidget(t);
        return card;
    };
    kpiGrid->addWidget(mkCard("Throughput",  "1.2k kg/h","#212529","▲ 2.5%","#16a34a"), 0, 0);
    kpiGrid->addWidget(mkCard("Defect Rate", "0.82 %",   "#dc2626","▲ 0.1%","#dc2626"), 0, 1);
    cl->addLayout(kpiGrid);

    // 缺陷统计柱状图
    auto *defTitle = new QLabel("Defect Statistics");
    defTitle->setStyleSheet(
        "font-size: 10px; font-weight: bold; color: #495057; text-transform: uppercase;");
    cl->addWidget(defTitle);
    cl->addWidget(new DefectBarChart);

    auto *barLabels = new QHBoxLayout;
    for (const char *lbl : { "Crack", "Small", "Mesh", "Other" }) {
        auto *l2 = new QLabel(lbl);
        l2->setAlignment(Qt::AlignCenter);
        l2->setStyleSheet(
            "font-size: 8px; color: #6c757d; font-weight: bold;");
        barLabels->addWidget(l2);
    }
    cl->addLayout(barLabels);

    // 烘焙程度分布
    auto *roastTitle = new QLabel("Roast Distribution");
    roastTitle->setStyleSheet(
        "font-size: 10px; font-weight: bold; color: #495057; text-transform: uppercase;");
    cl->addWidget(roastTitle);

    auto mkRoast = [](const char *name, const char *objName,
                      int pct, const char *pctStr) {
        auto *w  = new QWidget;
        auto *wl = new QVBoxLayout(w);
        wl->setContentsMargins(0, 0, 0, 0); wl->setSpacing(2);
        auto *row = new QHBoxLayout;
        auto *nl  = new QLabel(name);
        nl->setStyleSheet("font-size: 10px; font-weight: bold;");
        auto *vl2 = new QLabel(pctStr);
        vl2->setStyleSheet("font-size: 10px; font-weight: bold;");
        row->addWidget(nl); row->addStretch(); row->addWidget(vl2);
        auto *bar = new QProgressBar;
        bar->setObjectName(objName);
        bar->setRange(0, 100); bar->setValue(pct);
        bar->setTextVisible(false); bar->setFixedHeight(6);
        wl->addLayout(row); wl->addWidget(bar);
        return w;
    };
    cl->addWidget(mkRoast("Light",  "lightBar",  12, "12%"));
    cl->addWidget(mkRoast("Medium", "mediumBar", 78, "78%"));
    cl->addWidget(mkRoast("Dark",   "darkBar",   10, "10%"));
    cl->addStretch();

    l->addWidget(content, 1);
    return panel;
}

// ════════════════════════════════════════════════════════════════════════════
//  buildResultsTable
// ════════════════════════════════════════════════════════════════════════════
QWidget *MainWindow::buildResultsTable()
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

    auto *table = new QTableWidget(4, 6);
    table->setHorizontalHeaderLabels({
        "ID", "Timestamp", "Mesh", "Roast Level", "Defects", "Confidence"
    });
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->verticalHeader()->setDefaultSectionSize(28);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setShowGrid(false);

    const struct { const char *id, *ts, *mesh, *roast, *defect, *conf, *defClr; } rows[] = {
        { "#89025","14:22:05.110","18","Medium","NONE",     "0.999","#16a34a" },
        { "#89026","14:22:05.125","17","Dark",  "SCORCH",   "0.975","#dc2626" },
        { "#89027","14:22:05.138","18","Light", "OFF-COLOR","0.921","#ca8a04" },
        { "#89028","14:22:05.150","19","Medium","CRACK",    "0.988","#dc2626" },
    };
    for (int r = 0; r < 4; r++) {
        const char *cols[] = { rows[r].id, rows[r].ts, rows[r].mesh,
                               rows[r].roast, rows[r].defect, rows[r].conf };
        for (int c = 0; c < 6; c++) {
            auto *item = new QTableWidgetItem(cols[c]);
            item->setTextAlignment(
                c == 5 ? Qt::AlignRight | Qt::AlignVCenter
                       : Qt::AlignLeft  | Qt::AlignVCenter);
            if (c == 4) {
                item->setForeground(QColor(rows[r].defClr));
                QFont f = item->font(); f.setBold(true); item->setFont(f);
            }
            table->setItem(r, c, item);
        }
        if (r == 1) {
            for (int c = 0; c < 6; c++)
                table->item(r, c)->setBackground(QColor("#eff6ff"));
        }
    }
    l->addWidget(table, 1);
    return panel;
}

// ════════════════════════════════════════════════════════════════════════════
//  buildStatusBar
// ════════════════════════════════════════════════════════════════════════════
QWidget *MainWindow::buildStatusBar()
{
    auto *bar = new QWidget;
    bar->setObjectName("statusBar");
    bar->setFixedHeight(30);  // ← 用代码控制高度

    auto *l = new QHBoxLayout(bar);
    l->setContentsMargins(8, 0, 8, 0);
    l->setSpacing(0);

    // 左侧状态指示
    m_camDotLabel = new QLabel("●");
    m_camDotLabel->setStyleSheet("color: #dc3545; font-size: 10px;");
    m_camDotLabel->setFixedWidth(14);
    auto mkTxt = [](const char *t) {
        auto *l2 = new QLabel(t);
        l2->setStyleSheet(
            "color: #343a40; font-size: 10px; font-weight: bold;");
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

    // 右侧录制控件
    auto *recLbl = new QLabel("Recording:");
    recLbl->setStyleSheet(
        "color: #495057; font-weight: bold; font-size: 10px;");
    l->addWidget(recLbl);
    l->addSpacing(4);

    auto *recFrame  = new QFrame;
    recFrame->setStyleSheet(
        "QFrame { border: 1px solid #ced4da; background: transparent; }");
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

    // 路径显示（限宽+省略号）
    auto *pathFrame  = new QFrame;
    pathFrame->setStyleSheet(
        "QFrame { border: 1px solid #ced4da; background: white; }");
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
void MainWindow::captureLoop()
{
    tSdkFrameHead sFrameInfo;
    BYTE *pbyBuffer = nullptr;

    bool firstFrame = true;
    while (m_captureRunning) {
        if (CameraGetImageBuffer(m_hCamera, &sFrameInfo, &pbyBuffer, 1000)
                != CAMERA_STATUS_SUCCESS)
            continue;

        if (firstFrame) {
            printf("[Camera] First frame received! Size: %dx%d  MediaType: 0x%X\n",
                   sFrameInfo.iWidth, sFrameInfo.iHeight, sFrameInfo.uiMediaType);
            firstFrame = false;
        }

        CameraImageProcess(m_hCamera, pbyBuffer, m_pRgbBuffer, &sFrameInfo);

        // 使用 UI 选中的分辨率（m_imageW × m_imageH）作为保存和显示的尺寸
        int w = m_imageW, h = m_imageH;
        bool isMono = (sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8);

        // 保存图像到 image 目录（尺寸与 UI 选中分辨率完全一致）
        if (!m_imageDir.isEmpty()) {
            QString filename = m_imageDir + "/"
                + QString("%1.png").arg(m_saveCounter.fetch_add(1), 5, 10, QChar('0'));
            cv::Mat saveImg(h, w, isMono ? CV_8UC1 : CV_8UC3, m_pRgbBuffer);
            if (!cv::imwrite(filename.toStdString(), saveImg))
                printf("[Camera] Warning: failed to save image: %s\n",
                       filename.toUtf8().constData());
            else
                printf("[Camera] Saved %s  [%dx%d]\n",
                       filename.toUtf8().constData(), w, h);
        }

        CameraReleaseImageBuffer(m_hCamera, pbyBuffer);

        // 构建用于显示的 QImage（彩色需 BGR→RGB）
        QImage img;
        if (isMono) {
            img = QImage(m_pRgbBuffer, w, h, w, QImage::Format_Grayscale8).copy();
        } else {
            cv::Mat bgr(h, w, CV_8UC3, m_pRgbBuffer), rgb;
            cv::cvtColor(bgr, rgb, cv::COLOR_BGR2RGB);
            img = QImage(rgb.data, w, h, (int)rgb.step, QImage::Format_RGB888).copy();
        }

        {
            std::lock_guard<std::mutex> lock(m_frameMutex);
            m_latestFrame = std::move(img);
            m_frameReady  = true;
        }
        // 一旦缓冲区有新图像，立即通过 queued signal 调用 grabFrame()
        // 这样主线程会在下一次事件循环中马上刷新，而不是等待固定的
        // 30ms 定时器。可以更精确地跟随相机的采集速率。
        if (m_acquiring) {
            QMetaObject::invokeMethod(this, "grabFrame", Qt::QueuedConnection);
        }
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  Frame 显示（主线程，在 captureLoop 每次获取新帧时触发）
//  原先依赖 30ms 定时器刷新，现在由后台线程通过 queued invokeMethod
//  调用 grabFrame，刷新频率精确跟随相机采集帧率，画面更流畅。
// ════════════════════════════════════════════════════════════════════════════
void MainWindow::grabFrame()
{
    if (!m_cameraConnected || !m_acquiring) return;

    QImage img;
    {
        std::lock_guard<std::mutex> lock(m_frameMutex);
        if (!m_frameReady) return;
        img = m_latestFrame;
        m_frameReady = false;
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
//  曝光 / 增益 slots
// ════════════════════════════════════════════════════════════════════════════
// slider 单位为"行数（lines）"，参考 GTK Demo on_exposure_time_value_changed
void MainWindow::onExposureSliderMoved(int lines)
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

// spin 单位为 µs，反算行数同步 slider
void MainWindow::onExposureSpinChanged(int us)
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

void MainWindow::onAnalogGainChanged(int value)
{
    updateAnalogGainLabel(value);
    if (m_cameraConnected) {
        CameraSetAnalogGain(m_hCamera, value);
        showStatus(QString("Analog Gain → %1  (%2x)")
                       .arg(value).arg(double(value) * m_gainStep, 0, 'f', 2));
    }
}

void MainWindow::updateAnalogGainLabel(int gain)
{
    m_gainValueLabel->setText(
        QString("%1x").arg(double(gain) * m_gainStep, 0, 'f', 2));
}

void MainWindow::onFrameRateButtonClicked()
{
    if (!m_cameraConnected) return;
    bool ok;
    int current = 0;
    if (CameraGetFrameRate(m_hCamera, &current) != CAMERA_STATUS_SUCCESS)
        current = 0;
    int rate = QInputDialog::getInt(this, "Set Line Rate",
                                    "Rate (Hz, <=0 for max):", current,
                                    -1000000, 1000000, 1, &ok);
    if (!ok) return;
    if (CameraSetFrameRate(m_hCamera, rate) != CAMERA_STATUS_SUCCESS) {
        showStatus("ERROR: Failed to set line rate.");
        return;
    }
    int actual = 0;
    CameraGetFrameRate(m_hCamera, &actual);
    m_frameRateValue->setText(QString("%1 Hz").arg(actual));
    showStatus(QString("Line rate set to %1 Hz").arg(actual));
    // also output to terminal
    printf("[Camera] Frame/line rate = %d Hz\n", actual);
}

// ════════════════════════════════════════════════════════════════════════════
//  分辨率切换（参考 GTK Demo on_res_combobox_changed）
// ════════════════════════════════════════════════════════════════════════════
void MainWindow::onResolutionChanged(int index)
{
    if (!m_cameraConnected || index < 0 || index >= m_capability.iImageSizeDesc)
        return;

    CameraSetImageResolution(m_hCamera, &m_capability.pImageSizeDesc[index]);

    tSdkImageResolution res;
    CameraGetImageResolution(m_hCamera, &res);
    m_imageW = res.iWidth;
    m_imageH = res.iHeight;
    m_imageFmtLabel->setText(QString("%1 × %2 BUFFER").arg(m_imageW).arg(m_imageH));

    // 重新分配 RGB 缓冲区
    if (m_pRgbBuffer) { free(m_pRgbBuffer); }
    m_pRgbBuffer = (unsigned char *)malloc(m_imageW * m_imageH * 3);

    showStatus(QString("Resolution → %1 × %2").arg(m_imageW).arg(m_imageH));
}

// ════════════════════════════════════════════════════════════════════════════
//  全屏切换（F11 进入 / Esc 或 F11 退出）
//  全屏时隐藏侧边栏、分析面板、结果表格、顶部栏、状态栏，
//  仅保留图像区域。paintEvent 已保证等比例拉伸，无需额外处理。
// ════════════════════════════════════════════════════════════════════════════
void MainWindow::onToggleFullscreen()
{
    m_imageFullscreen = !m_imageFullscreen;

    m_sidebarWidget->setVisible(!m_imageFullscreen);
    m_analyticsWidget->setVisible(!m_imageFullscreen);
    m_resultsWidget->setVisible(!m_imageFullscreen);
    m_headerWidget->setVisible(!m_imageFullscreen);
    m_statusWidget->setVisible(!m_imageFullscreen);

    if (m_imageFullscreen) {
        m_fullscreenBtn->setToolTip("Exit Fullscreen  ( F11 / Esc )");
        showFullScreen();
    } else {
        m_fullscreenBtn->setToolTip("Fullscreen  ( F11 / Esc )");
        showNormal();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_F11 ||
        (e->key() == Qt::Key_Escape && m_imageFullscreen))
        onToggleFullscreen();
    else
        QMainWindow::keyPressEvent(e);
}

// 窗口状态变化（最大化 / 还原 / 全屏）后重新计算缩放比例并刷新字体
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    if (e->type() == QEvent::WindowStateChange) {
        // 延迟 60ms 等待窗口几何更新完成
        QTimer::singleShot(60, this, [this] {
            double scale = qBound(0.75, double(width()) / 1440.0, 1.8);
            if (qAbs(scale - m_uiScale) > 0.03) {
                m_uiScale = scale;
                setStyleSheet(makeStyle(m_uiScale));
            }
        });
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  相机连接
// ════════════════════════════════════════════════════════════════════════════
void MainWindow::onConnectCamera()
{
    if (!m_cameraConnected) {
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

bool MainWindow::connectCamera()
{
    int iCameraCounts = 1;
    tSdkCameraDevInfo devInfo;
    CameraEnumerateDevice(&devInfo, &iCameraCounts);
    printf("[Camera] Enumerate: found %d device(s)\n", iCameraCounts);
    if (iCameraCounts == 0) {
        printf("[Camera] No camera device found.\n");
        return false;
    }
    printf("[Camera] Camera signal detected! Device: %s  SN: %s\n",
           devInfo.acFriendlyName, devInfo.acSn);
    if (CameraInit(&devInfo, -1, -1, &m_hCamera) != CAMERA_STATUS_SUCCESS) {
        printf("[Camera] CameraInit failed.\n");
        return false;
    }
    printf("[Camera] CameraInit success, handle=%d\n", m_hCamera);

    CameraGetCapability(m_hCamera, &m_capability);

    // ── 分辨率 combo：动态填充（参考 GTK Demo create_gtkdemo_resolution） ──
    m_resCombo->blockSignals(true);
    m_resCombo->clear();
    for (int i = 0; i < m_capability.iImageSizeDesc; i++) {
        m_resCombo->addItem(
            QString::fromLocal8Bit(m_capability.pImageSizeDesc[i].acDescription));
    }
    // 初始化分辨率：优先选择 4096×2048，否则回退到第 4 个预设值
    {
        int targetIdx = -1;
        for (int i = 0; i < m_capability.iImageSizeDesc; i++) {
            if (m_capability.pImageSizeDesc[i].iWidth  == 4096 &&
                m_capability.pImageSizeDesc[i].iHeight == 2048) {
                targetIdx = i;
                break;
            }
        }
        if (targetIdx < 0 && m_capability.iImageSizeDesc > 3)
            targetIdx = 3;
        if (targetIdx >= 0)
            CameraSetImageResolution(m_hCamera, &m_capability.pImageSizeDesc[targetIdx]);
    }
    // 读取相机当前分辨率，并同步选中项
    tSdkImageResolution res;
    CameraGetImageResolution(m_hCamera, &res);
    m_resCombo->setCurrentIndex(res.iIndex);
    m_resCombo->setEnabled(true);
    m_resCombo->blockSignals(false);

    m_imageW = res.iWidth; m_imageH = res.iHeight;
    m_imageFmtLabel->setText(QString("%1 × %2 BUFFER").arg(m_imageW).arg(m_imageH));

    // ── 模拟增益（参考 GTK Demo Gtk_SetExposure） ──────────────────────────
    m_gainMin  = (int)m_capability.sExposeDesc.uiAnalogGainMin;
    m_gainMax  = (int)m_capability.sExposeDesc.uiAnalogGainMax;
    m_gainStep = m_capability.sExposeDesc.fAnalogGainStep;
    m_gainSlider->blockSignals(true);
    m_gainSlider->setRange(m_gainMin, m_gainMax);
    // 初始化增益为最大值
    CameraSetAnalogGain(m_hCamera, m_gainMax);
    m_gainSlider->setValue(m_gainMax);
    m_gainSlider->blockSignals(false);
    updateAnalogGainLabel(m_gainMax);

    // ── 曝光时间（参考 GTK Demo及 cope.cpp：slider 单位=行数，spin 单位=µs） ──────────
    // 初始化曝光时间为 36µs
    CameraSetExposureTime(m_hCamera, 36.0);
    
    // GTK Demo: range = [uiExposeTimeMin, uiExposeTimeMax]（行数）
    //           实际µs = 行数 × m_fExpLineTime
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

    m_pRgbBuffer = (unsigned char *)malloc(m_imageW * m_imageH * 3);
    if (m_capability.sIspCapacity.bMonoSensor) {
        m_channel = 1; CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8);
    } else {
        m_channel = 3; CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_BGR8);
    }
    CameraPlay(m_hCamera);
    setConnectedState(true);

    // 获取并显示当前帧/行频
    if (m_frameRateValue) {
        int rate = 0;
        if (CameraGetFrameRate(m_hCamera, &rate) == CAMERA_STATUS_SUCCESS) {
            m_frameRateValue->setText(QString("%1 Hz").arg(rate));
            printf("[Camera] Current frame/line rate = %d Hz\n", rate);
        }
    }

    // 确保 image 目录存在（使用可执行文件所在目录的绝对路径）
    m_imageDir = QCoreApplication::applicationDirPath() + "/image";
    QDir().mkpath(m_imageDir);
    m_saveCounter = 1;
    printf("[Camera] Image save directory: %s\n", m_imageDir.toUtf8().constData());

    // 启动后台采集线程
    m_captureRunning = true;
    m_captureThread  = std::thread(&MainWindow::captureLoop, this);

    // 连接成功后自动开始采集，图像刷新由后台线程信号驱动
    setAcquiringState(true);
    m_fpsTimer.invalidate(); m_frameCount = 0;
    //m_timer->start(30);  // 不再使用固定周期定时器
    return true;
}

void MainWindow::disconnectCamera()
{
    if (!m_cameraConnected) return;

    // 先停止后台采集线程
    m_captureRunning = false;
    if (m_captureThread.joinable())
        m_captureThread.join();

    CameraUnInit(m_hCamera);
    if (m_pRgbBuffer) { free(m_pRgbBuffer); m_pRgbBuffer = nullptr; }
    m_hCamera = -1;

    // 清空分辨率下拉框
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
//  Start / Stop
// ════════════════════════════════════════════════════════════════════════════
void MainWindow::onStart()
{
    setAcquiringState(true);
    m_fpsTimer.invalidate(); m_frameCount = 0;
    // 开启预览不需启动 Timer，captureLoop 会自行触发
    showStatus("Acquisition started.");
}

void MainWindow::onStop()
{
    m_timer->stop();
    setAcquiringState(false);
    showStatus("Acquisition stopped.");
}

// ════════════════════════════════════════════════════════════════════════════
//  状态辅助
// ════════════════════════════════════════════════════════════════════════════
void MainWindow::setConnectedState(bool c)
{
    m_cameraConnected = c;
    m_connDotLabel->setStyleSheet(
        c ? "color: #28a745; font-size: 14px;" : "color: #dc3545; font-size: 14px;");
    m_connTextLabel->setText(c ? "Connected" : "Disconnected");
    m_connTextLabel->setStyleSheet(
        c ? "color: #28a745; font-size: 11px; font-weight: bold;"
          : "color: #6c757d; font-size: 11px;");

    // enable/disable controls
    m_resCombo->setEnabled(c);
    m_exposureSlider->setEnabled(c);
    m_exposureSpin->setEnabled(c);
    m_gainSlider->setEnabled(c);
    m_startBtn->setEnabled(c);
    if (m_frameRateBtn) m_frameRateBtn->setEnabled(c);
}

void MainWindow::setAcquiringState(bool a)
{
    m_acquiring = a;
    m_startBtn->setEnabled(!a && m_cameraConnected);
    m_stopBtn->setEnabled(a);
}

void MainWindow::showStatus(const QString &msg)
{
    m_statusMsgLabel->setText(
        QString("[%1] %2")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"), msg));
}
