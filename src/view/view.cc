#include "view.h"

#include "graph.h"
#include "ui_view.h"
namespace s21 {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);

  connect(ui_->pushButton_0, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_1, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_2, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_3, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_4, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_5, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_6, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_7, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_8, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_9, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_brace_open, SIGNAL(clicked()), this,
          SLOT(KeyNumber()));
  connect(ui_->pushButton_brace_close, SIGNAL(clicked()), this,
          SLOT(KeyNumber()));
  connect(ui_->pushButton_dot, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui_->pushButton_x, SIGNAL(clicked()), this, SLOT(KeyNumber()));

  connect(ui_->pushButton_sin, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_cos, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_tan, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_asin, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_acos, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_atan, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_ln, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_log, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_pow, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_plus, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_minus, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_mult, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_div, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui_->pushButton_mod, SIGNAL(clicked()), this, SLOT(KeyFunc()));

  connect(ui_->pushButton_clear, SIGNAL(clicked()), this, SLOT(KeyClear()));
  connect(ui_->pushButton_delChar, SIGNAL(clicked()), this, SLOT(DelChar()));
  connect(ui_->pushButton_equalTo, SIGNAL(clicked()), this, SLOT(Calc()));

  connect(ui_->pushButton_graph, SIGNAL(clicked()), this, SLOT(KeyGraph()));
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::KeyNumber() {
  QPushButton *button = (QPushButton *)sender();
  ui_->textEdit_input->setText(ui_->textEdit_input->toPlainText() +
                               button->text());
}

void MainWindow::KeyFunc() {
  QPushButton *button = (QPushButton *)sender();
  ui_->textEdit_input->setText(ui_->textEdit_input->toPlainText() +
                               button->text());
}

void MainWindow::KeyClear() { ui_->textEdit_input->setText(""); }

void MainWindow::DelChar() {
  QString text = ui_->textEdit_input->toPlainText();
  text.chop(1);
  ui_->textEdit_input->setText(text);
}

void MainWindow::Calc() {
  QString tmp = ui_->textEdit_input->toPlainText();
  std::string input = tmp.toStdString();
  controller_.SetX(ui_->doubleSpinBox_x_max->value());
  controller_.SetInput(input);
  std::string res;
  res = controller_.GetResult();

  QString str_result = QString::fromUtf8(res.c_str());
  ui_->textBrowser_output->setText(str_result);
}

void MainWindow::KeyGraph() {
  int validation_return = 0;
  QString tmp = ui_->textEdit_input->toPlainText();
  double x_max = ui_->doubleSpinBox_x_max->value();
  double x_min = ui_->doubleSpinBox_x_min->value();

  std::string ba = tmp.toStdString();
  controller_.SetInput(ba);
  Graph *graph = new Graph(controller_);
  graph->setAttribute(Qt::WA_DeleteOnClose);
  graph->show();
}

}  // namespace s21
