#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts>

#include "include/Image.hpp"
#include "include/Input.hpp"
#include "include/Kernel.hpp"


QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window2;
    window2.resize(320, 240);
    window2.setWindowTitle(QApplication::translate("toplevel", "Main Window"));
//    window2.show();

    Local::Image image = Local::Image("C:\\Users\\garren\\OneDrive\\Documentos\\test-3\\img\\test_images\\Space_46k.jpg");

    image.zoomOut(1, 1);

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    QGraphicsPixmapItem item(QPixmap::fromImage(image.underlyingContainer()));
    scene.addItem(&item);
    image.saveToJPEG("./file.jpeg", 95);
    view.setWindowTitle(QString::fromStdString(image.getFilePath()));
    view.show();

    return app.exec();
}
