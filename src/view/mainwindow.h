#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graph.h"

#include <QMainWindow>

#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  s21::Controller controller;

 private:
  Ui::MainWindow* ui;
  Graph* graph;

 private slots:
  void key_number();
  void key_func();
  void key_clear();
  void delChar();
  void calc();
  //    void deposit_plot(int* res, double* accrued, double* tax_amount,
  //                        double* deposit);
  void key_graph();

 signals:
  void signal(QString);
  void signal_min(double);
  void signal_max(double);
};
#endif  // MAINWINDOW_H
