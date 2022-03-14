#ifndef PICTUREPRESENTER_H
#define PICTUREPRESENTER_H

#include <QWidget>

namespace Ui {
class PicturePresenter;
}

class PicturePresenter : public QWidget
{
    Q_OBJECT

public:
    explicit PicturePresenter(QWidget *parent = nullptr);
    ~PicturePresenter();

private:
    Ui::PicturePresenter *ui;
};

#endif // PICTUREPRESENTER_H
