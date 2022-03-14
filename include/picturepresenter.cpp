#include "picturepresenter.h"
#include "ui_picturepresenter.h"

PicturePresenter::PicturePresenter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicturePresenter)
{
    ui->setupUi(this);
}

PicturePresenter::~PicturePresenter()
{
    delete ui;
}
