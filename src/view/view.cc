#include "view.h"

#include "graph.h"
#include "ui_mainwindow.h"
// #include "deligate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_brace_open, SIGNAL(clicked()), this,
          SLOT(KeyNumber()));
  connect(ui->pushButton_brace_close, SIGNAL(clicked()), this,
          SLOT(KeyNumber()));
  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(KeyNumber()));
  connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(KeyNumber()));

  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_mult, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(KeyFunc()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(KeyFunc()));

  connect(ui->pushButton_clear, SIGNAL(clicked()), this, SLOT(KeyClear()));
  connect(ui->pushButton_delChar, SIGNAL(clicked()), this, SLOT(DelChar()));
  connect(ui->pushButton_equalTo, SIGNAL(clicked()), this, SLOT(Calc()));

  connect(ui->pushButton_graph, SIGNAL(clicked()), this, SLOT(KeyGraph()));
}

MainWindow::~MainWindow() {
  delete ui;
  delete graph;
}

void MainWindow::KeyNumber() {
  QPushButton *button = (QPushButton *)sender();
  ui->textEdit_input->setText(ui->textEdit_input->toPlainText() +
                              button->text());
}

void MainWindow::KeyFunc() {
  QPushButton *button = (QPushButton *)sender();
  ui->textEdit_input->setText(ui->textEdit_input->toPlainText() +
                              button->text());
}

void MainWindow::KeyClear() { ui->textEdit_input->setText(""); }

void MainWindow::DelChar() {
  QString text = ui->textEdit_input->toPlainText();
  text.chop(1);
  ui->textEdit_input->setText(text);
}

void MainWindow::Calc() {
  QString tmp = ui->textEdit_input->toPlainText();
  // QByteArray ba = tmp.toLocal8Bit();
  std::string input = tmp.toStdString();
  controller.SetX(ui->doubleSpinBox_x_max->value());
  //  double number = ui->doubleSpinBox_x->value();
  controller.SetInput(input);
  std::string res;
  res = controller.GetResult();

  QString str_result = QString::fromUtf8(res.c_str());
  ui->textBrowser_output->setText(str_result);
}

void MainWindow::KeyGraph() {
  int validation_return = 0;
  QString tmp = ui->textEdit_input->toPlainText();
  std::string ba = tmp.toStdString();
  //  validation_return = validation_input(str);
  //  if (validation_return) {
  //    ui->textBrowser_output->setText("incorrect expression");
  //  } else {
  //    ui->textBrowser_output->setText(str);
  double x_max = ui->doubleSpinBox_x_max->value();
  double x_min = ui->doubleSpinBox_x_min->value();

  controller.SetInput(ba);
  graph = new Graph(controller);
  graph->show();
  //    connect(this, &MainWindow::signal, graph, &Graph::slot);
  //    connect(this, &MainWindow::signal_min, graph, &Graph::slot_min);
  //    connect(this, &MainWindow::signal_max, graph, &Graph::slot_max);
  //    emit signal(ui->textEdit_input->toPlainText());
  //    emit signal_min(x_min);
  //    emit signal_max(x_max);
  //  }
}

// void MainWindow::text_send(QString q_string) {
//     ui->textEdit_input->setText(q_string);
// }