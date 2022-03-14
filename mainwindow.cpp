#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include/Image.hpp"
#include "include/Kernel.hpp"
#include "include/Input.hpp"

#include <string>
#include <regex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addImage_action_button_clicked()
{

    Local::Image *show_image = new Local::Image(ui->addImage_file_path->text().toStdString());
    this->images[image_index_counter] = *show_image;

    ui->imageList->addItem(QString::fromStdString(std::to_string(image_index_counter++) + " _ " + show_image->getFilePath()));

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsView *view = new QGraphicsView(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(show_image->underlyingContainer()));

    scene->addItem(item);

    view->setWindowTitle(QString::fromStdString("File path: " +show_image->getFilePath()));
    view->show();
}


void MainWindow::on_pushButton_8_clicked()
{
    Local::Image image = Local::Image("C:\\Users\\garren\\OneDrive\\Documentos\\INF01046-Assignment_II\\img\\test_images\\grumpy.jpg");

//    image.rotate90Deg(1);
//    image.toGrayscale();

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsView *view = new QGraphicsView(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image.underlyingContainer()));

    scene->addItem(item);

    view->setWindowTitle(QString::fromStdString("File path: " +image.getFilePath()));
    view->show();

    auto renderHistogramChart = [](Local::Image &image){
        QChart *chart = &image.getHistogramChart();
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *window = new QMainWindow;
        window->setCentralWidget(chartView);
        window->resize(800, 600);
        window->setWindowTitle(QString::fromStdString("Grayscale Histogram " + image.getFilePath()));
        window->show();
    };

    auto renderCumulativeHistogramChart = [](Local::Image &image){
        QChart *chart = &image.getCumulativeHistogramChart();
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *window = new QMainWindow;
        window->setCentralWidget(chartView);
        window->resize(800, 600);
        window->setWindowTitle(QString::fromStdString("Grayscale Cumulative Histogram " + image.getFilePath()));
        window->show();
    };

    Local::Image test_image2 = Local::Image("C:\\Users\\garren\\OneDrive\\Documentos\\INF01046-Assignment_II\\img\\test_images\\grumpy.jpg");

    float gaussian[3][3] = {{0.0625, 0.125, 0.0625}, {0.125, 0.25, 0.125},  {0.0625, 0.125, 0.0625}};
    float laplacian[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    float high_pass[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
    float prewitt_hx[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    float prewitt_hy[3][3] = {{-1, -1, -1}, {0, 0, 0},  {1, 1, 1}};
    float sobel_hx[3][3] = {{-1, 0, 1}, {-2, 0, 2},  {-1, 0, 1}};
    float sobel_hy[3][3] = {{-1, -2, -1}, {0, 0, 0},  {1, 2, 1}};

    Local::Kernel<3,3,float> kernel(sobel_hx);
//    kernel.rotate180Degrees();
//    test_image2.toGrayscale();
//    test_image2.matchHistogram(image);
//    test_image2.apply2DConv(kernel, 127.0f);
    test_image2.adjustBrightness(32);

    QGraphicsScene *scene1 = new QGraphicsScene;
    QGraphicsView *view1 = new QGraphicsView(scene1);
    QGraphicsPixmapItem *item1 = new QGraphicsPixmapItem(QPixmap::fromImage(test_image2.underlyingContainer()));

    scene1->addItem(item1);

    view1->setWindowTitle(QString::fromStdString("File path: " +test_image2.getFilePath()));
    view1->show();
//    if(ui->imageList->selectedItems().size()>0){
//        QListWidgetItem* item = ui->imageList->selectedItems().at(0);
//        ui->imageList->removeItemWidget(item);
//        ui->imageList->takeItem(item->listWidget()->row(item));
//        this->images.erase(std::stoi(item->text().toStdString()));
//    }
}


void MainWindow::on_addFilter_action_button_clicked()
{
    std::regex rx(R"((?:^|\s)([+-]?[[:digit:]]+(?:\.[[:digit:]]+)?)(?=$|\s))");
    std::smatch m;
    std::string str = ui->addFilter_text_field->text().toStdString();
    std::vector<float> values;

    while (regex_search(str, m, rx)) {
        values.push_back(std::stof(m[1]));
        str = m.suffix().str();
    }

    if(values.size() == 9){
        Local::Kernel<3,3,float> filter;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                filter._data[i][j] = values[i * 3 + j];
            }
        }
        ui->filterList->addItem(QString::fromStdString(std::to_string(filter_index_counter++) + " - " + filter.getInfo()));
    }
}


void MainWindow::on_pushButton_14_clicked()
{
    if(ui->filterList->selectedItems().size()>0){
        QListWidgetItem* item = ui->filterList->selectedItems().at(0);
        ui->filterList->removeItemWidget(item);
        ui->filterList->takeItem(item->listWidget()->row(item));
    }
}

void MainWindow::on_pushButton_clicked()
{
    Local::Image image = Local::Image("C:\\Users\\garren\\OneDrive\\Documentos\\INF01046-Assignment_II\\img\\test_images\\grumpy.jpg.jpg");

//    image.rotate90Deg(1);
    image.toGrayscale();

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsView *view = new QGraphicsView(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image.underlyingContainer()));

    scene->addItem(item);

    view->setWindowTitle(QString::fromStdString("File path: " +image.getFilePath()));
    view->show();

    auto renderHistogramChart = [](Local::Image &image){
        QChart *chart = &image.getHistogramChart();
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *window = new QMainWindow;
        window->setCentralWidget(chartView);
        window->resize(800, 600);
        window->setWindowTitle(QString::fromStdString("Grayscale Histogram " + image.getFilePath()));
        window->show();
    };

    auto renderCumulativeHistogramChart = [](Local::Image &image){
        QChart *chart = &image.getCumulativeHistogramChart();
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *window = new QMainWindow;
        window->setCentralWidget(chartView);
        window->resize(800, 600);
        window->setWindowTitle(QString::fromStdString("Grayscale Cumulative Histogram " + image.getFilePath()));
        window->show();
    };

    Local::Image test_image2 = Local::Image("C:\\Users\\garren\\OneDrive\\Documentos\\INF01046-Assignment_II\\img\\test_images\\grumpy.jpg.jpg");

    float gaussian[3][3] = {{0.0625, 0.125, 0.0625}, {0.125, 0.25, 0.125},  {0.0625, 0.125, 0.0625}};
    float laplacian[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    float high_pass[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
    float prewitt_hx[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    float prewitt_hy[3][3] = {{-1, -1, -1}, {0, 0, 0},  {1, 1, 1}};
    float sobel_hx[3][3] = {{-1, 0, 1}, {-2, 0, 2},  {-1, 0, 1}};
    float sobel_hy[3][3] = {{-1, -2, -1}, {0, 0, 0},  {1, 2, 1}};

    Local::Kernel<3,3,float> kernel(sobel_hx);
//    kernel.rotate180Degrees();
//    test_image2.toGrayscale();
//    test_image2.matchHistogram(image);
//    test_image2.apply2DConv(kernel, 127.0f);
    test_image2.adjustBrightness(25);

    QGraphicsScene *scene1 = new QGraphicsScene;
    QGraphicsView *view1 = new QGraphicsView(scene1);
    QGraphicsPixmapItem *item1 = new QGraphicsPixmapItem(QPixmap::fromImage(test_image2.underlyingContainer()));

    scene1->addItem(item1);

    view1->setWindowTitle(QString::fromStdString("File path: " +test_image2.getFilePath()));
    view1->show();

//    renderHistogramChart(image);
//    renderHistogramChart(test_image2);

//    renderCumulativeHistogramChart(image);
//    renderCumulativeHistogramChart(test_image2);
}

void MainWindow::on_imageList_itemDoubleClicked(QListWidgetItem *item)
{

}

