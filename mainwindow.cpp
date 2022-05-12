#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    A.Browser=ui->A_Browser;
    A.Browser2=ui->A_Browser2;

    B.Browser=ui->B_Browser;
    B.Browser2=ui->B_Browser2;

    C.Browser=ui->C_Browser;
    C.Browser2=ui->C_Browser2;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    A.OPT();
    B.OPT();
    C.OPT();
}

void MainWindow::on_pushButton_2_clicked()
{
    A.FIFO();
    B.FIFO();
}


void MainWindow::on_pushButton_3_clicked()
{
    A.LRU();
    B.LRU();
    C.LRU();
}


void MainWindow::on_pushButton_4_clicked()
{
    A.NUR();
    B.NUR();
    C.NUR();
}


void MainWindow::on_pushButton_5_clicked()
{
    if(ui->A_lineEdit->text()!=0&&ui->B_lineEdit->text()!=0&&ui->C_lineEdit->text()!=0){
        QMessageBox::information(this,"提示","输入成功");
        QString strA=ui->A_lineEdit->text();
        QString strB=ui->B_lineEdit->text();
        QString strC=ui->C_lineEdit->text();
        A.write_interviewArr(strA);
        B.write_interviewArr(strB);
        C.write_interviewArr(strC);
    }
    else{
        QMessageBox::information(this,"提示","输入不能为空");
    }
}

