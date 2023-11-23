#ifndef CPP3_SMARTCALC_V2_0_VIEW_GRAPH_H_
#define CPP3_SMARTCALC_V2_0_VIEW_GRAPH_H_

#include <QDialog>
#include <QVector>

#include "../controller/controller.h"

namespace Ui {
class Graph;
}

class Graph : public QDialog {
  Q_OBJECT

 public:
  explicit Graph(s21::Controller controller, QWidget* parent = nullptr);
  ~Graph();

 private:
  Ui::Graph* ui;
  s21::Controller controller;
};

#endif  // CPP3_SMARTCALC_V2_0_VIEW_GRAPH_H_
