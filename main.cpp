#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts>

#include "include/Image.hpp"
#include "include/Input.hpp"
#include "include/Kernel.hpp"

#include <iostream>


QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    Local::UserInput userInput;
    QApplication app(argc, argv);
    QWidget window2;
    window2.resize(320, 240);
    window2.setWindowTitle(QApplication::translate("toplevel", "Main Window"));
//    window2.show();

    Local::Image image = Local::Image(userInput.getUserInput("Path to image 1: ", std::cin));
//    float filter_data[3][3] = {{0.0625, 0.125, 0.0625}, {0.125, 0.25, 0.125}, {0.0625, 0.125, 0.0625}};

//    Local::Kernel<3,3,float> filter(filter_data);
//    filter.rotate180Degrees();

    image.toGrayscale();

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    QGraphicsPixmapItem item(QPixmap::fromImage(image.underlyingContainer()));
    scene.addItem(&item);
    view.setWindowTitle("Image 1");
    view.show();


    QChart *chart = &image.getHistogramChart();
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(800, 600);
    window.setWindowTitle("Grayscale Histogram Image 1");
    window.show();

    return app.exec();
}
