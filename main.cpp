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

    float filter_data[3][3] = {{0.0625, 0.125, 0.0625}, {0.125, 0.25, 0.125}, {0.0625, 0.125, 0.0625}};

    Local::Kernel<3,3,float> filter(filter_data);
    filter.rotate180Degrees();

//    image.zoomIn(2);

    image.toGrayscale();

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    QGraphicsPixmapItem item(QPixmap::fromImage(image.underlyingContainer()));
    scene.addItem(&item);
    image.saveToJPEG("./file.jpeg", 95);
    view.setWindowTitle(QString::fromStdString(image.getFilePath()));
//    view.show();

//![6]
    QChart *chart = &image.getHistogram();
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
//![6]

//![7]
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(800, 600);
    window.setWindowTitle("Grayscale Histogram");
    window.show();

    return app.exec();
}
