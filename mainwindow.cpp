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

void MainWindow::show_image(Local::Image* image, QString window_title)
{
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsView *view = new QGraphicsView(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image->underlyingContainer()));

    scene->addItem(item);

    view->setWindowTitle(window_title);
    view->show();
}

void MainWindow::show_chart(QChart *chart, QString window_title)
{
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow *window = new QMainWindow;
    window->setCentralWidget(chartView);
    window->resize(800, 600);
    window->setWindowTitle(window_title);
    window->show();
}


void MainWindow::on_addImage_action_button_clicked()
{

    Local::Image *image_t = new Local::Image(ui->addImage_file_path->text().toStdString());
    this->images[image_index_counter] = image_t;

    ui->imageList->addItem(QString::fromStdString(std::to_string(image_index_counter++) + " _ " + image_t->getFilePath()));

    show_image(image_t, QString::fromStdString("File path: " +image_t->getFilePath()));
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
        Local::Kernel<3,3,float>* filter = new Local::Kernel<3,3,float>;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                filter->_data[i][j] = values[i * 3 + j];
            }
        }
        ui->filterList->addItem(QString::fromStdString(std::to_string(filter_index_counter) + " - " + filter->getInfo()));
        filters[filter_index_counter] = filter;
        filter_index_counter++;
    }
}



void MainWindow::on_imageList_itemDoubleClicked(QListWidgetItem *item)
{
    Local::Image *image_to_be_shown = images[std::stoi(item->text().toStdString())];
    show_image(image_to_be_shown, QString::fromStdString("File Path: " + image_to_be_shown->getFilePath()));
}


void MainWindow::on_getHistogram_clicked()
{
    show_chart(&current_image->getHistogramChart(), QString::fromStdString("Grayscale Histogram Chart for: " + current_image->getFilePath()));
}


void MainWindow::on_imageList_itemClicked(QListWidgetItem *item)
{
    current_image = images[std::stoi(item->text().toStdString())];
}


void MainWindow::on_equalizeHistogram_clicked()
{
    show_chart(&current_image->getHistogramChart(), QString::fromStdString("Grayscale Histogram Chart for: " + current_image->getFilePath()));
    show_chart(&current_image->equalizeHistogram().getHistogramChart(), QString::fromStdString("Post equalization Grayscale Histogram Chart for: " + current_image->getFilePath()));
}


void MainWindow::on_MatchHistogram_clicked()
{
    show_chart(&current_image->getHistogramChart(), QString::fromStdString("Grayscale Histogram Chart for: " + current_image->getFilePath()));
    show_chart(&current_image->matchHistogram(*images[std::stoi(ui->matchHistogramFIeld->text().toStdString())]).getHistogramChart(), QString::fromStdString("Post matching Grayscale Histogram Chart for: " + current_image->getFilePath()));
}


void MainWindow::on_resetImage_clicked()
{
    current_image->reset();
}


void MainWindow::on_contrastAdjust_clicked()
{
    current_image->adjustContrast(std::stof(ui->contrastField->text().toStdString()));
}


void MainWindow::on_brightnessAdjust_clicked()
{
    current_image->adjustBrightness(std::stoi(ui->brightnessField->text().toStdString()));
}


void MainWindow::on_getGrayscale_clicked()
{
    current_image->toGrayscale();
}


void MainWindow::on_getNegative_clicked()
{
    current_image->toNegative();
}


void MainWindow::on_verticalMirror_clicked()
{
    current_image->mirrorV();
}


void MainWindow::on_HorizontalMirror_clicked()
{
    current_image->mirrorH();
}


void MainWindow::on_Quantize_clicked()
{
    current_image->quantize(std::stoi(ui->quantizationField->text().toStdString()));
}


void MainWindow::on_zoomIn_clicked()
{
    current_image->zoomIn(std::stoi(ui->zoomInField->text().toStdString()));
}


void MainWindow::on_zoomOut_clicked()
{
    current_image->zoomOut(std::stoi(ui->zoomOutXFIeld->text().toStdString()), std::stoi(ui->zoomOutYFIeld->text().toStdString()));
}


void MainWindow::on_rotate90_clicked()
{
    current_image->rotate90Deg(std::stoi(ui->rotate90Field->text().toStdString()));
}


void MainWindow::on_applyFilter_clicked()
{
    current_image->apply2DConv(*current_filter, std::stof(ui->pre_clamp_offset->text().toStdString()));
}


void MainWindow::on_deleteImage_clicked()
{
    if(ui->imageList->selectedItems().size()>0){
        QListWidgetItem* item = ui->imageList->selectedItems().at(0);
        ui->imageList->removeItemWidget(item);
        ui->imageList->takeItem(item->listWidget()->row(item));
        this->images.erase(std::stoi(item->text().toStdString()));
    }
}


void MainWindow::on_deleteFilter_clicked()
{
    if(ui->filterList->selectedItems().size()>0){
        QListWidgetItem* item = ui->filterList->selectedItems().at(0);
        ui->filterList->removeItemWidget(item);
        ui->filterList->takeItem(item->listWidget()->row(item));
        this->filters.erase(std::stoi(item->text().toStdString()));
    }
}


void MainWindow::on_filterList_itemClicked(QListWidgetItem *item)
{
    current_filter = filters[std::stoi(item->text().toStdString())];
}


void MainWindow::on_saveImage_clicked()
{
    current_image->saveToJPEG(ui->outImageField->text().toStdString(), 95);
}

