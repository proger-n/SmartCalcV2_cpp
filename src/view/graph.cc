#include "graph.h"

#include "ui_graph.h"

Graph::Graph(s21::Controller controller, QWidget *parent)
    : QDialog(parent), ui_(new Ui::Graph), controller_(controller) {
  ui_->setupUi(this);
  double x_min = ui_->doubleSpinBox_min_x->value();
  double x_max = ui_->doubleSpinBox_max_x->value();
  if ((x_max - x_min) > 0) {
    double h = (x_max - x_min) / 10000;
    double y_min = 0;
    double y_max = 0;
    QVector<double> x, y;
    double Y;

    ui_->widget->xAxis->setRange(x_min, x_max);
    for (double X = x_min; X <= x_max; X += h) {
      controller_.SetX(X);
      std::string res = controller_.GetResult();
      if (res != "ERROR") {
        x.push_back(X);
        Y = std::stod(res);
        y.push_back(Y);
        if (Y > y_max || y_max != y_max) y_max = Y;
        if (Y < y_min || y_min != y_min) y_min = Y;
      }
    }

    ui_->widget->yAxis->setRange(y_min, y_max);
    ui_->widget->addGraph();
    ui_->widget->graph(0)->addData(x, y);
    ui_->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui_->widget->graph(0)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    ui_->widget->replot();
  }
}

Graph::~Graph() { delete ui_; }
