#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <vector>
#include <unordered_map>

#include "include/Image.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::vector<QGraphicsView*> views;
    std::unordered_map<int, Local::Kernel<3,3,float>*> filters;
    std::unordered_map<int, Local::Image*> images;
    Local::Image* current_image;
    Local::Kernel<3,3,float>* current_filter;

    int image_index_counter = 0;
    int filter_index_counter = 0;

    Local::Image test_image;

    void show_image(Local::Image* image, QString window_title);
    void show_chart(QChart *chart, QString window_title);

private slots:
    void on_addImage_action_button_clicked();
    void on_addFilter_action_button_clicked();
    void on_imageList_itemDoubleClicked(QListWidgetItem *item);
    void on_getHistogram_clicked();
    void on_imageList_itemClicked(QListWidgetItem *item);
    void on_equalizeHistogram_clicked();
    void on_MatchHistogram_clicked();
    void on_resetImage_clicked();
    void on_contrastAdjust_clicked();
    void on_brightnessAdjust_clicked();
    void on_getGrayscale_clicked();
    void on_getNegative_clicked();
    void on_verticalMirror_clicked();
    void on_HorizontalMirror_clicked();
    void on_Quantize_clicked();
    void on_zoomIn_clicked();
    void on_zoomOut_clicked();
    void on_rotate90_clicked();
    void on_applyFilter_clicked();
    void on_deleteImage_clicked();
    void on_deleteFilter_clicked();
    void on_filterList_itemClicked(QListWidgetItem *item);
    void on_saveImage_clicked();
};
#endif // MAINWINDOW_H
