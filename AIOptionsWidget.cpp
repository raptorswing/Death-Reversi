#include "AIOptionsWidget.h"
#include "ui_AIOptionsWidget.h"

AIOptionsWidget::AIOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AIOptionsWidget)
{
    ui->setupUi(this);
}

AIOptionsWidget::~AIOptionsWidget()
{
    delete ui;
}

void AIOptionsWidget::setBlackSearchDepth(quint8 depth)
{
    this->ui->blackAIDepthSpinbox->setValue(depth);
    this->options.blackSearchDepth = depth;
}

void AIOptionsWidget::setWhiteSearchDepth(quint8 depth)
{
    this->ui->whiteAIDepthSpinbox->setValue(depth);
    this->options.whiteSearchDepth = depth;
}

quint8 AIOptionsWidget::getBlackSearchDepth() const
{
    quint8 toRet = this->ui->blackAIDepthSpinbox->value();
    return toRet;
}

quint8 AIOptionsWidget::getWhiteSearchDepth() const
{
    quint8 toRet = this->ui->whiteAIDepthSpinbox->value();
    return toRet;
}

//private slot
void AIOptionsWidget::on_blackAIDepthSpinbox_valueChanged(int arg1)
{
    this->blackSearchDepthChanged(arg1);
    this->options.blackSearchDepth = arg1;
    this->AIOptionsChanged(this->options);
}

//private slot
void AIOptionsWidget::on_whiteAIDepthSpinbox_valueChanged(int arg1)
{
    this->whiteSearchDepthChanged(arg1);
    this->options.whiteSearchDepth = arg1;
    this->AIOptionsChanged(this->options);
}
