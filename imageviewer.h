#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();

private:
    Ui::ImageViewer *ui;
};

#endif // IMAGEVIEWER_H
