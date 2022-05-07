#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ctrl 16777249

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QTranslator>
#include <QApplication>
#include <QKeyEvent>
#include <QVector>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QMenu>
#include "hotkeys.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_checkBox_stateChanged(int arg1);

    void clickInfo();

    void clickHotKeys();

    void clickENG();

    void clickRUS();

    void clickLight();

    void clickDark();

    void clickPrint();


private:
    Ui::MainWindow *ui;
    HotKeys *dialogForHotkeys;

    QFile file;
    QString filename;

    QTranslator translater;

    QWidget *forInfo;
    QLabel *labelForInfo;

    QVector<quint32> keys; // open,save,new,exit

    QMenu *mainMenu;

    bool is_ctrl;

private:
    void keyPressEvent(QKeyEvent *myKey);

    void keyReleaseEvent(QKeyEvent *myKey);

    void createNewFile();

    void retranslateUi();

    void mainMenuRetranslateAndCreate();

    void setPrinterImage(QPrinter&);

};
#endif // MAINWINDOW_H
