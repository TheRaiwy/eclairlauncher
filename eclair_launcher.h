#ifndef ECLAIR_LAUNCHER_H
#define ECLAIR_LAUNCHER_H

#include <QWidget>
#include <QStackedLayout>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

class EclairLauncher : public QWidget
{
    Q_OBJECT

public:
    EclairLauncher(QWidget *parent = nullptr);
    ~EclairLauncher();

private slots:

private:
    void setupFirstScreen();
    void setupLaunchScreen();
    QWidget *createHeader();
    void startOpacityAnimation();
    void stopOpacityAnimation();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;

    QStackedLayout *stackedLayout;
    QLineEdit *keyInput;
    QPushButton *enterButton;
    QPropertyAnimation *m_opacityAnimation;
    QPoint oldPos;
    bool m_isWindowMoving;
};

#endif
