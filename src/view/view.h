#ifndef CPP3_SMARTCALC_V2_0_VIEW_VIEW_H_
#define VIEW_H

#include <QMainWindow>

#include "../controller/controller.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow* ui_;
  s21::Controller controller_;

 private slots:
  void KeyNumber();
  void KeyFunc();
  void KeyClear();
  void DelChar();
  void Calc();
  void KeyGraph();
};
}  // namespace s21
#endif  // VIEW_H
