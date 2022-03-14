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
    std::unordered_map<int, Local::Kernel<3,3,float>> filters;
    std::unordered_map<int, Local::Image> images;

    int image_index_counter = 0;
    int filter_index_counter = 0;

    Local::Image test_image;

private slots:
    void on_addImage_action_button_clicked();
    void on_pushButton_8_clicked();
    void on_addFilter_action_button_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_clicked();
    void on_imageList_itemDoubleClicked(QListWidgetItem *item);
};
#endif // MAINWINDOW_H
