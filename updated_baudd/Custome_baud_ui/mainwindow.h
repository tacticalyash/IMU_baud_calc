#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QStandardItemModel>
#include <QRegularExpression>
#include <QDebug>
#include <QTableView>
#include <QComboBox>





namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);


    void onPlainTextChanged();

    ~MainWindow();




private slots:
     void on_Send_data_clicked();
       void sendContinuously();


       void on_pushButton_clicked();

       void on_TESTT_clicked();
       void printFaffPatterns();
       void onComPortSelected(int index);
       QStringList getAvailableComPorts();

       void on_EXCELL_clicked();
       void handleReadyRead();
       void format();
      // float hexStringToFloat(const QString &hexString);

       void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
       QSerialPort serialPort;
       QTimer *sendTimer;
       QStandardItemModel model; // Create the model
       QTableView *tableView;
          QStandardItemModel *tableModel;
          QComboBox *comboBox;
           QByteArray buffer;
           QString pattern ;



};

#endif // MAINWINDOW_H
