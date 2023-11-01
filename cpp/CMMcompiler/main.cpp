#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h" // 包含自动生成的头文件

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow mainWindow;
    Ui::MainWindow ui;
    ui.setupUi(&mainWindow);
    mainWindow.show();
    return a.exec();
}
