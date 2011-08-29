#ifndef AIOPTIONSWIDGET_H
#define AIOPTIONSWIDGET_H

#include <QWidget>

namespace Ui {
    class AIOptionsWidget;
}

struct AIOptions
{
    quint8 blackSearchDepth;
    quint8 whiteSearchDepth;
};

class AIOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AIOptionsWidget(QWidget *parent = 0);
    ~AIOptionsWidget();

    void setBlackSearchDepth(quint8 depth);
    void setWhiteSearchDepth(quint8 depth);

    quint8 getBlackSearchDepth() const;
    quint8 getWhiteSearchDepth() const;

signals:
    void blackSearchDepthChanged(quint8 depth);
    void whiteSearchDepthChanged(quint8 depth);
    void AIOptionsChanged(AIOptions newOptions);

private slots:
    void on_blackAIDepthSpinbox_valueChanged(int arg1);
    void on_whiteAIDepthSpinbox_valueChanged(int arg1);
    //void AIOptionsChanged(AIOptions newOptions);

private:
    Ui::AIOptionsWidget *ui;
    AIOptions options;
};

#endif // AIOPTIONSWIDGET_H
