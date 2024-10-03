#include "eclair_launcher.h"
#include <QApplication>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QMouseEvent>

EclairLauncher::EclairLauncher(QWidget *parent)
    : QWidget(parent),
    stackedLayout(new QStackedLayout(this)),
    m_isWindowMoving(false)
{
    setFixedSize(400, 200);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color: #21242B;");

    QFont pixelFont("Courier New", 10);
    QApplication::setFont(pixelFont);

    m_opacityAnimation = new QPropertyAnimation(this, "windowOpacity");
    m_opacityAnimation->setDuration(500);
    m_opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);

    setupFirstScreen();
    setupLaunchScreen();
}

QWidget *EclairLauncher::createHeader()
{
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(25);
    headerWidget->setStyleSheet("background-color: #811342;");

    QLabel *titleLabel = new QLabel("ECLAIR LAUNCHER", headerWidget);
    titleLabel->setStyleSheet("color: white; font-size: 14px; text-align: center;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QPushButton *closeButton = new QPushButton("X", headerWidget);
    closeButton->setFixedSize(25, 25);
    closeButton->setStyleSheet("background-color: #971342; color: white; font-size: 14px; border: none;");
    connect(closeButton, &QPushButton::clicked, this, &EclairLauncher::close);

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->addStretch();
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(closeButton);

    return headerWidget;
}

void EclairLauncher::setupFirstScreen()
{
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *headerWidget = createHeader();
    mainLayout->addWidget(headerWidget);

    QComboBox *combo = new QComboBox(this);
    combo->addItem("SPOOFER");
    combo->addItem("ECLAIR PVE");
    combo->addItem("PERKS");
    combo->setFixedSize(120, 25);
    combo->setStyleSheet("background-color: #2b3c46; color: white; font-size: 10px; border: none; border-radius: 4px;"
                         "QComboBox::drop-down { background-color: #323941; }");
    mainLayout->addWidget(combo, 0, Qt::AlignCenter);

    QLabel *keyLabel = new QLabel("KEY:", this);
    keyLabel->setStyleSheet("color: white; font-size: 12px;");

    QLineEdit *keyInput = new QLineEdit(this);
    keyInput->setPlaceholderText("Enter Your Key");
    keyInput->setFixedHeight(19);
    keyInput->setStyleSheet("background-color: #50545F; color: white; font-size: 10px; border: none; border-radius: 4px;");

    QHBoxLayout *keyLayout = new QHBoxLayout();
    keyLayout->setContentsMargins(0, 0, 0, 0);
    keyLayout->addWidget(keyLabel);
    keyLayout->addWidget(keyInput);

    QPushButton *enterButton = new QPushButton("ENTER", this);
    enterButton->setFixedSize(57, 19);
    enterButton->setStyleSheet("background-color: #2b3c46; color: white; font-size: 10px; border: none; border-radius: 4px;");
    keyLayout->addWidget(enterButton);

    mainLayout->addLayout(keyLayout);

    QVBoxLayout *statusLayout = new QVBoxLayout();
    QLabel *statusLabel = new QLabel("Status: Invalid", this);
    statusLabel->setStyleSheet("color: white; font-size: 12px;");

    QLabel *licenseLabel = new QLabel("License Time: Unknown", this);
    licenseLabel->setStyleSheet("color: white; font-size: 12px;");

    statusLayout->addWidget(statusLabel);
    statusLayout->addWidget(licenseLabel);

    QVBoxLayout *linkLayout = new QVBoxLayout();
    QLabel *linkLabel1 = new QLabel("Example Link 1", this);
    linkLabel1->setStyleSheet("color: white;");

    QLabel *linkLabel2 = new QLabel("Example Link 2", this);
    linkLabel2->setStyleSheet("color: white;");

    linkLayout->addWidget(linkLabel1);
    linkLayout->addWidget(linkLabel2);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addLayout(statusLayout);
    bottomLayout->addStretch();
    bottomLayout->addLayout(linkLayout);

    mainLayout->addLayout(bottomLayout);
    mainLayout->setAlignment(bottomLayout, Qt::AlignLeft | Qt::AlignBottom);

    stackedLayout->addWidget(mainWidget);
    connect(enterButton, &QPushButton::clicked, this, [keyInput, enterButton, keyLabel, mainLayout]() {
        keyLabel->setVisible(false);
        keyInput->setVisible(false);
        enterButton->setVisible(false);

        QPushButton *launchButton = new QPushButton("LAUNCH", mainLayout->parentWidget());
        launchButton->setFixedSize(100, 40);
        launchButton->setStyleSheet("background-color: #2b3c46; color: white; font-size: 14px; border: none;");
        mainLayout->insertWidget(mainLayout->count() - 1, launchButton, 0, Qt::AlignCenter);
    });
}

void EclairLauncher::setupLaunchScreen()
{
    QWidget *launchWidget = new QWidget(this);
    QVBoxLayout *launchLayout = new QVBoxLayout(launchWidget);
    launchLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *launchHeaderWidget = createHeader();
    launchLayout->addWidget(launchHeaderWidget);

    QPushButton *launchButton = new QPushButton("LAUNCH", this);
    launchButton->setFixedSize(100, 40);
    launchButton->setStyleSheet("background-color: #2b3c46; color: white; font-size: 14px; border: none;");
    launchLayout->addWidget(launchButton, 0, Qt::AlignCenter);

    stackedLayout->addWidget(launchWidget);
}

void EclairLauncher::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (event->pos().y() < 25 || !childAt(event->pos())) {
            oldPos = event->globalPosition().toPoint();
            event->accept();
        }
    }
}

void EclairLauncher::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (oldPos != QPoint() && (event->pos().y() < 25 || !childAt(event->pos()))) {
            QPoint delta = event->globalPosition().toPoint() - oldPos;
            move(x() + delta.x(), y() + delta.y());
            startOpacityAnimation();
            oldPos = event->globalPosition().toPoint();
        }
    }
}

void EclairLauncher::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        stopOpacityAnimation();
        QWidget::mouseReleaseEvent(event);
    }
}

bool EclairLauncher::event(QEvent *event)
{
    if (event->type() == QEvent::NonClientAreaMouseButtonRelease) {
        stopOpacityAnimation();
        return true;
    }
    return QWidget::event(event);
}

void EclairLauncher::startOpacityAnimation()
{
    if (!m_isWindowMoving) {
        m_opacityAnimation->stop();
        m_opacityAnimation->setStartValue(windowOpacity());
        m_opacityAnimation->setEndValue(0.5);
        m_opacityAnimation->start();
        m_isWindowMoving = true;
    }
}

void EclairLauncher::stopOpacityAnimation()
{
    if (m_isWindowMoving) {
        m_opacityAnimation->stop();
        m_opacityAnimation->setStartValue(windowOpacity());
        m_opacityAnimation->setEndValue(1.0);
        m_opacityAnimation->start();
        m_isWindowMoving = false;
    }
}

EclairLauncher::~EclairLauncher() {}
