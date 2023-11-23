#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include <QVector>

#include "../controller/controller.h"

namespace Ui {
class Graph;
}

class Graph : public QDialog {
  Q_OBJECT

 public:
  explicit Graph(QWidget* parent = nullptr);
  ~Graph();

 private:
  Ui::Graph* ui;

 public slots:
  void slot(QString str_inp);
  void slot_min(double min_x);
  void slot_max(double max_x);
};

#endif  // GRAPH_H
