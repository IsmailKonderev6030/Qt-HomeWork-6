#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mainMenu(nullptr)
    , is_ctrl(false)
{
    ui->setupUi(this);

    dialogForHotkeys = new HotKeys;
    dialogForHotkeys->setMinimumSize(250,250);
    dialogForHotkeys->setMaximumSize(250,250);

    keys = dialogForHotkeys->getKeys();

    filename = "";
    file.setFileName(filename);

    setWindowTitle(tr("Text editer"));
    ui->pushButton   ->  setText(tr("Open file"));
    ui->pushButton_2 ->  setText(tr("Save file"));
    ui->pushButton_3 ->  setText(tr("Menu"));
    ui->checkBox     ->  setText(tr("Only read"));

    labelForInfo = new QLabel;
    labelForInfo->resize(200,120);

    QFile info("about.txt");
    if(info.open(QIODevice::ReadOnly))
        labelForInfo->setText(info.readAll());
    else
        labelForInfo->setText("Error open file");

    forInfo = new QWidget;
    forInfo->setWindowTitle(tr("Info"));
    forInfo->setMaximumSize(200,200);
    forInfo->setMinimumSize(200,200);

    QVBoxLayout test(forInfo);
    test.addWidget(labelForInfo);

    mainMenu = new QMenu(this);
    mainMenuRetranslateAndCreate();
    ui->pushButton_3->setMenu(mainMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete forInfo;
    delete labelForInfo;
    delete dialogForHotkeys;
    delete mainMenu;
}

void MainWindow::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,tr("Select file txt"),QDir::currentPath(),"Text(*.txt)");
    if(filename.size()){
        file.setFileName(filename);
        if(file.open(QIODevice::ReadWrite)){
            QByteArray text = file.readAll();
            ui->plainTextEdit->setPlainText(text);
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    filename = QFileDialog::getSaveFileName(this,tr("Save file txt"),filename,"Text(*.txt)");
    if(filename.size()){
        file.setFileName(filename);
        if(file.open(QIODevice::WriteOnly)){
            QString str = ui->plainTextEdit->toPlainText();
            QByteArray text = str.toUtf8();
            file.write(text);
            file.close();
            ui->plainTextEdit->setPlainText("");
        }
    }
}


void MainWindow::on_pushButton_3_clicked(){

}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->plainTextEdit->setReadOnly(arg1);
}


void MainWindow::keyPressEvent(QKeyEvent *myKey){
    if(!is_ctrl && myKey->key() == ctrl) is_ctrl = true;
}


void MainWindow::keyReleaseEvent(QKeyEvent *myKey){
    if(myKey->key() == ctrl) is_ctrl = false;

    if(is_ctrl){
        quint32 temp_key = myKey->key() + ctrl;

        for(int i=0;i<4;i++)
            if(keys[i] == temp_key)
                switch (i) {
                case 0: MainWindow::on_pushButton_clicked();break;
                case 1: MainWindow::on_pushButton_2_clicked();break;
                case 2: createNewFile(); break;
                case 3: MainWindow::close();
                }
    }
    is_ctrl = false;
}


void MainWindow::createNewFile(){
    filename = QFileDialog::getSaveFileName(this,tr("Create file txt"),filename,"Text(*.txt)");
    if(filename.size()){
        file.setFileName(filename);
        if(file.open(QIODevice::ReadWrite)){
            QByteArray text = file.readAll();
            ui->plainTextEdit->setPlainText(text);
        }
    }
}


void MainWindow::mainMenuRetranslateAndCreate(){
    mainMenu->clear();

    mainMenu->addAction(tr("Information"),this,SLOT(clickInfo()));
    mainMenu->addAction(tr("Hotkeys"),this,SLOT(clickHotKeys()));
    QMenu *languageMenu = mainMenu->addMenu(tr("Language"));
    languageMenu->addAction("ENG",this,SLOT(clickENG()));
    languageMenu->addAction("RUS",this,SLOT(clickRUS()));
    QMenu *themeMenu = mainMenu->addMenu(tr("Theme"));
    themeMenu->addAction(tr("Light"),this,SLOT(clickLight()));
    themeMenu->addAction(tr("Dark"),this,SLOT(clickDark()));
    mainMenu->addAction(tr("Print"),this,SLOT(clickPrint()));
}


void MainWindow::setPrinterImage(QPrinter &printer){
    QPainter painter;
    painter.begin(&printer);

    QFont font = painter.font();
    QFontMetrics fmetrics(font);

    QString printStr = ui->plainTextEdit->toPlainText();
    printStr += '\n';

    QChar *list = printStr.data();
    QStringList strlst;
    int line = 20;
    int cursor = 0;
    const quint8 MAX_SIZE_WIDTH = 150;


    while (true){
        int index = printStr.indexOf("\n", cursor);

        if(index - cursor >= MAX_SIZE_WIDTH)
            index = MAX_SIZE_WIDTH + cursor;

        QString s = "";

        s.append(&list[cursor], index - cursor);

        cursor += index - cursor;
        strlst << s;

        if (index >= printStr.size()-1) break;
    }



    int h = painter.window().height();
    int amount = strlst.count();


    for (int i = 0; i < amount; i++){
        QPointF pf;

        pf.setX(20);
        pf.setY(line);

        painter.drawText(pf, strlst.at(i));
        line += fmetrics.height();

        if (h - line <= fmetrics.height()){
            printer.newPage();
            line = 20;
        }
    }
    painter.end();
}


void MainWindow::retranslateUi(){
    setWindowTitle(tr("Text editer"));

    ui->pushButton   ->  setText(tr("Open file"));
    ui->pushButton_2 ->  setText(tr("Save file"));
    ui->pushButton_3 ->  setText(tr("Menu"));
    ui->checkBox     ->  setText(tr("Only read"));

    mainMenuRetranslateAndCreate();

    dialogForHotkeys->retranslateUi();

}


void MainWindow::clickInfo(){
    forInfo->show();
}


void MainWindow::clickHotKeys(){
    dialogForHotkeys->setModal(true);
    dialogForHotkeys->exec();
    keys = dialogForHotkeys->getKeys();
}


void MainWindow::clickENG(){
    qApp->removeTranslator(&translater);
    dialogForHotkeys->RemoveLanguage(translater);
    MainWindow::retranslateUi();
}


void MainWindow::clickRUS(){
    if(translater.load("switch_ru")){
        qApp->installTranslator(&translater);
        dialogForHotkeys->SetLanguage(translater);
    }
    MainWindow::retranslateUi();
}


void MainWindow::clickLight(){
    qApp->setStyleSheet("");
}


void MainWindow::clickDark(){
    qApp->setStyleSheet("QPushButton,QComboBox,QPlainTextEdit,QLabel,QCheckBox,QLineEdit {background-color: #2F4F4F; color: #FFFFFF; border-color: white} "
                        "QMainWindow,QDialog {background-color: black}");
}


void MainWindow::clickPrint(){
    QPrinter printer;

    QPrintDialog dlg(&printer,this);
    dlg.setWindowTitle("Print");

    if(dlg.exec()==QDialog::Accepted)
        setPrinterImage(printer); // also can use ui->plainTextEdit->print(&printer)
}
