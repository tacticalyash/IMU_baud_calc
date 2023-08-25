#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <iomanip>
#include <sstream>
#include <QFileDialog>
#include <QSerialPortInfo>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    comboBox = new QComboBox(this);
    QStringList comPorts = getAvailableComPorts();
    comboBox->addItems(comPorts);

    // Connect the ComboBox's signal to a slot
    connect(comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onComPortSelected(int)));

    // Add the ComboBox to your UI's layout
    ui->verticalLayout->addWidget(comboBox);


    //serialPort.setPortName(portName);


    serialPort.setBaudRate(921600);


    connect(&serialPort, &QSerialPort::readyRead, this, &MainWindow::handleReadyRead);



    sendTimer = new QTimer(this);
    connect(sendTimer, &QTimer::timeout, this, &MainWindow::sendContinuously);

    //connect(ui.pushButton, &QPushButton::clicked, this, &MainWindow::on_TESTT_clicked);



    // Set up the table view with the model

  //  connect(ui->plainTextEdit_2, &QPlainTextEdit::textChanged, this, &MainWindow::onPlainTextChanged);



}

MainWindow::~MainWindow()
{
    if (serialPort.isOpen())
        serialPort.close();
    delete ui;
}




void MainWindow::onComPortSelected(int index)
{
    //QString portName = ui->combo->itemText(index);

    //serialPort.setPortName(portName);

    // Open the serial port and perform other actions...
    QString portName = comboBox->itemText(index);

        serialPort.setPortName(portName);
        if (!serialPort.open(QIODevice::ReadWrite))
        {
            qWarning("Failed to open serial port!");
        }
}

void MainWindow::on_Send_data_clicked()
{
    if (sendTimer->isActive())
    {
        sendTimer->stop();
    }

    sendTimer->start(100);
}
void MainWindow::sendContinuously()
{



    QByteArray msg = "FAFF361B102002EAC410600400049C4040400CBB7F46003AEBA0003BA98B00FC";
    serialPort.write(msg);
    qDebug()<<"msegade" <<msg<<endl;





}


void MainWindow::handleReadyRead()
{



//   QByteArray dataReceived = serialPort.read(32);
//            if(dataReceived.size() == 32)
//            {
//                QString hexString = dataReceived.toHex().toUpper();
//                ui->plainTextEdit_2->appendPlainText(hexString);




//            }

    QByteArray data = serialPort.readAll();
    QString hexString = data.toHex().toUpper();
   //  ui->plainTextEdit_2->moveCursor(QTextCursor::End); // Move cursor to the end
    ui->plainTextEdit_2->insertPlainText(hexString);


//format();



//    //github code below

//    if(serialPort.waitForReadyRead(1))
//    {
//        QByteArray hexstring = serialPort.readAll();
//        QString hex = hexstring.toHex().toUpper();
//        ui->plainTextEdit_2->appendPlainText(hex);
//    }









}
/*void MainWindow::format()
{
    QString inputData = ui->plainTextEdit_2->toPlainText();
    QString patternHeader = "FAFF361B102002";
    static int patternLength = 64;
    QStringList patterns;

    // Remove all spaces from inputData
   // inputData.remove(' ');

     int index = inputData.indexOf(patternHeader);
    while (index != -1 && index + patternLength <= inputData.size()) {
        QString pattern = inputData.mid(index, patternLength);
        patterns.append(pattern);
        index = inputData.indexOf(patternHeader, index + 1);
    }

    QString formattedOutput = patterns.join("\n");
    ui->plainTextEdit->setPlainText(formattedOutput);
}
*/

void MainWindow::format()
{
    union ulf {
        unsigned long ul;
        float rec, rec1, rec2;
    };

    ulf uus, uu1s, uu2s, uu, uu1, uu2;
    QString formattedG, formattedG1, formattedG2, formattedF, formattedF1, formattedF2;



    QString inputData = ui->plainTextEdit_2->toPlainText();
    QString patternHeader = "FAFF361B102002";
    static int patternLength = 65;
    QStringList patterns;

    int index = inputData.indexOf(patternHeader);
    while (index != -1 && index + patternLength <= inputData.size()) {
        QString pattern = inputData.mid(index, patternLength);
        patterns.append(pattern);
        index = inputData.indexOf(patternHeader, index + 1);
    }

    // Create a model for the TableView
    QStandardItemModel *model = new QStandardItemModel(patterns.size(), 2, this); // Change column count to 2
    model->setHorizontalHeaderItem(0, new QStandardItem("Packet"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Packet Counter")); // Add new header
    model->setHorizontalHeaderItem(2, new QStandardItem("Sample Time"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Sensor Type"));
    model->setHorizontalHeaderItem(4, new QStandardItem("Size of Sensor"));
    model->setHorizontalHeaderItem(5,new QStandardItem("Ax"));
     model->setHorizontalHeaderItem(6,new QStandardItem("Ay"));
      model->setHorizontalHeaderItem(7,new QStandardItem("Az"));
       model->setHorizontalHeaderItem(8,new QStandardItem("Gx"));
        model->setHorizontalHeaderItem(9,new QStandardItem("Gy"));
         model->setHorizontalHeaderItem(10,new QStandardItem("Gz"));
          model->setHorizontalHeaderItem(11,new QStandardItem("Checksum"));

    // Populate the model with patterns
    for (int row = 0; row < patterns.size(); ++row) {
        QStandardItem *patternItem = new QStandardItem(patterns.at(row));
        model->setItem(row, 0, patternItem);
        QString packetCounter = patternItem->text().mid(14, 4);
        int pacDec = packetCounter.toInt(nullptr, 16);
        QString sampleTime = patternItem->text().mid(24,8);

        int sns = sampleTime.toInt(nullptr, 16);
        QString sensorType = patternItem->text().mid(32, 4);

        if (sensorType == "8040") {
            QString Gx= patternItem->text().mid(38, 8);
            QString Gy = patternItem->text().mid(46, 8);
            QString Gz = patternItem->text().mid(54, 8);

            std::string str = Gx.toStdString();
            std::string str2 = Gy.toStdString();
            std::string str3 = Gz.toStdString();

            std::stringstream ss(str);
            std::stringstream ss1(str2);
            std::stringstream ss2(str3);

            ss >> std::hex >> uus.ul;
            ss1 >> std::hex >> uu1s.ul;
            ss2 >> std::hex >> uu2s.ul;

            float f = uus.rec;
            float f1 = uu1s.rec1;
            float f2 = uu2s.rec2;

            formattedF = QString::number(f, 'f', 7);
            formattedF1 = QString::number(f1, 'f', 7);
            formattedF2 = QString::number(f2, 'f', 7);
            QStandardItem *Gx_col = new QStandardItem(formattedF);
              QStandardItem *Gy_col = new QStandardItem(formattedF1);
              QStandardItem *Gz_col = new QStandardItem(formattedF2);

              model->setItem(row, 8, Gx_col);
              model->setItem(row, 9, Gy_col);
              model->setItem(row, 10, Gz_col);
        } else {
            QString Ax = patternItem->text().mid(38, 8);
            QString Ay = patternItem->text().mid(46, 8);
            QString Az = patternItem->text().mid(54, 8);
            std::string str = Ax.toStdString();
            std::string str2 = Ay.toStdString();
            std::string str3 = Az.toStdString();

            std::stringstream ss(str);
            std::stringstream ss1(str2);
            std::stringstream ss2(str3);

            ss >> std::hex >> uu.ul;
            ss1 >> std::hex >> uu1.ul;
            ss2 >> std::hex >> uu2.ul;

            float g = uu.rec;
            float g1 = uu1.rec1;
            float g2 = uu2.rec2;

            formattedG = QString::number(g, 'f', 7);
            formattedG1 = QString::number(g1, 'f', 7);
            formattedG2 = QString::number(g2, 'f', 7);
            QStandardItem *Ax_col = new QStandardItem(formattedG);
                QStandardItem *Ay_col = new QStandardItem(formattedG1);
                QStandardItem *Az_col = new QStandardItem(formattedG2);

                model->setItem(row, 5, Ax_col);
                model->setItem(row, 6, Ay_col);
                model->setItem(row, 7, Az_col);
        }
        QString checksumNumber = patternItem->text().mid(63, 2);
        QString sizee_sens = patternItem->text().mid(36,2);
        QStandardItem *counterItem = new QStandardItem(QString::number(pacDec));
        model->setItem(row, 1, counterItem); // Add packet counter item
        QStandardItem *sensort = new QStandardItem(QString::number(sns));
        model->setItem(row,2,sensort);
        QStandardItem *sens = new QStandardItem(sensorType);
        model->setItem(row,3,sens);
        QStandardItem *size_sensor = new QStandardItem(sizee_sens);
        model->setItem(row,4,size_sensor);
//        QStandardItem *Ax_col = new QStandardItem(formattedG);
//        model->setItem(row,5,Ax_col);
//        QStandardItem *Ay_col = new QStandardItem(formattedG1);
//        model->setItem(row,6,Ay_col);
//        QStandardItem *Az_col = new QStandardItem(formattedG2);
//        model->setItem(row,7,Az_col);
//        QStandardItem *Gx_col = new QStandardItem(formattedF);
//        model->setItem(row,8,Gx_col);
//        QStandardItem *Gy_col = new QStandardItem(formattedF1);
//        model->setItem(row,9,Gy_col);
//        QStandardItem *Gz_col = new QStandardItem(formattedF2);
//        model->setItem(row,10,Gz_col);
        QStandardItem *checksum = new QStandardItem(checksumNumber);
        model->setItem(row,11,checksum);
    }

    // Set the model to the TableView
    ui->tableView_3->setModel(model);



    QStandardItemModel *uniqueModel = new QStandardItemModel(0, 11, this);
     uniqueModel->setHorizontalHeaderItem(0, new QStandardItem("Packet Data"));
    uniqueModel->setHorizontalHeaderItem(1, new QStandardItem("Packet Counter"));
    uniqueModel-> setHorizontalHeaderItem(2, new QStandardItem("Sample Time"));
     uniqueModel->setHorizontalHeaderItem(3, new QStandardItem("Sensor Type"));
    uniqueModel->setHorizontalHeaderItem(4, new QStandardItem("Size of Sensor"));
    uniqueModel->setHorizontalHeaderItem(5,new QStandardItem("Ax"));
    uniqueModel->setHorizontalHeaderItem(6,new QStandardItem("Ay"));
    uniqueModel->setHorizontalHeaderItem(7,new QStandardItem("Az"));
    uniqueModel->setHorizontalHeaderItem(8,new QStandardItem("Gx"));
    uniqueModel->setHorizontalHeaderItem(9,new QStandardItem("Gy"));
    uniqueModel->setHorizontalHeaderItem(10,new QStandardItem("Gz"));
    uniqueModel->setHorizontalHeaderItem(11,new QStandardItem("Checksum"));

    QSet<QString> uniqueRows;

    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < model->columnCount(); ++col) {
            rowData << model->index(row, col).data().toString();
        }

        QString rowStr = rowData.join(",");
        if (!uniqueRows.contains(rowStr)) {
            uniqueRows.insert(rowStr);

            QList<QStandardItem*> items;
            for (const QString& itemText : rowData) {
                items.append(new QStandardItem(itemText));
            }
            uniqueModel->appendRow(items);
        }
    }

    ui->tableView_unique->setModel(uniqueModel);
    serialPort.flush();
   // serialPort.clear();
}


/*void MainWindow::format()
{
    QString inputData = ui->plainTextEdit_2->toPlainText();
      int patternLength = 64;
      QString patternHeader = "FAFF361B102002";
      QStringList patterns;

      int index = inputData.indexOf(patternHeader);
      while (index != -1 && index + patternLength <= inputData.size()) {
          QString pattern = inputData.mid(index, patternLength);
          patterns.append(pattern);
          index = inputData.indexOf(patternHeader, index + 1);
      }

      QString formattedOutput = patterns.join("\n");
      ui->plainTextEdit->setPlainText(formattedOutput);

      QRegularExpression pattern("FAFF[0-9A-F]{60}");
      QRegularExpressionMatchIterator it = pattern.globalMatch(formattedOutput);
      QString Ax, Ay, Az, Gx, Gy, Gz;
      float f, f1, f2, g, g1, g2;
      QSet<int> uniquePacketCounters; // Set to store unique packet counters
      QMap<int, QMultiMap<QString, QString>> uniqueRows; // Map to store unique rows

      union ulf {
          unsigned long ul;
          float rec, rec1, rec2;
      };

      while (it.hasNext()) {
          QRegularExpressionMatch match = it.next();
          QString patternString = match.captured();
          QString packetNumber = patternString.mid(14, 4);
          int ZC = packetNumber.toInt(nullptr, 16);

          // Check if the current packet counter is unique
          if (!uniquePacketCounters.contains(ZC)) {
              uniquePacketCounters.insert(ZC); // Add to the set of unique packet counters

              QString sampleTime = patternString.mid(24, 8);
              QString sensorType = patternString.mid(32, 4);
              QString sizeOfSensor = patternString.mid(36, 2);
              QString dataCenter = patternString.mid(38, 24);
              QString checksumNumber = patternString.mid(62, 2);



              int VB = sampleTime.toInt(nullptr, 16);

              ulf uus, uu1s, uu2s, uu, uu1, uu2;
              QString formattedG, formattedG1, formattedG2, formattedF, formattedF1, formattedF2;

              // Extract and format values


              if (sensorType == "8040") {
                  Gx = dataCenter.mid(0, 8);
                  Gy = dataCenter.mid(8, 8);
                  Gz = dataCenter.mid(16, 8);

                  std::string str = Gx.toStdString();
                  std::string str2 = Gy.toStdString();
                  std::string str3 = Gz.toStdString();

                  std::stringstream ss(str);
                  std::stringstream ss1(str2);
                  std::stringstream ss2(str3);

                  ss >> std::hex >> uus.ul;
                  ss1 >> std::hex >> uu1s.ul;
                  ss2 >> std::hex >> uu2s.ul;

                  f = uus.rec;
                  f1 = uu1s.rec1;
                  f2 = uu2s.rec2;

                  formattedF = QString::number(f, 'f', 7);
                  formattedF1 = QString::number(f1, 'f', 7);
                  formattedF2 = QString::number(f2, 'f', 7);

              } else {
                  Ax = dataCenter.mid(0, 8);
                  Ay = dataCenter.mid(8, 8);
                  Az = dataCenter.mid(16, 8);

                  std::string str = Ax.toStdString();
                  std::string str2 = Ay.toStdString();
                  std::string str3 = Az.toStdString();

                  std::stringstream ss(str);
                  std::stringstream ss1(str2);
                  std::stringstream ss2(str3);

                  ss >> std::hex >> uu.ul;
                  ss1 >> std::hex >> uu1.ul;
                  ss2 >> std::hex >> uu2.ul;

                  g = uu.rec;
                  g1 = uu1.rec1;
                  g2 = uu2.rec2;

                  formattedG = QString::number(g, 'f', 7);
                  formattedG1 = QString::number(g1, 'f', 7);
                  formattedG2 = QString::number(g2, 'f', 7);

              }



              // Create a map for the row data
              QMultiMap<QString, QString> rowData;
              rowData.insert("ZC", QString::number(ZC));
              rowData.insert("VB", QString::number(VB));
              rowData.insert("SensorType", sensorType);
              rowData.insert("SizeOfSensor", sizeOfSensor);
              rowData.insert("Gx", sensorType == "8040" ? formattedG : formattedG1);
              rowData.insert("Gy", sensorType == "8040" ? formattedG1 : formattedG);
              rowData.insert("Gz", sensorType == "8040" ? formattedG2 : formattedG2);
              rowData.insert("F", sensorType == "8040" ? formattedF : formattedF1);
              rowData.insert("F1", sensorType == "8040" ? formattedF1 : formattedF);
              rowData.insert("F2", sensorType == "8040" ? formattedF2 : formattedF2);
              rowData.insert("Checksum", checksumNumber);
              rowData.insert("PatternString", patternString);

              // Insert the row data into the uniqueRows map
              uniqueRows.insert(ZC, rowData);
          }
      }

      // Clear the current table model
      tableModel->clear();

      // Iterate through the uniqueRows map and construct rows for the table view
      QMap<int, QMultiMap<QString, QString>>::const_iterator itRows = uniqueRows.constBegin();
      while (itRows != uniqueRows.constEnd()) {
          QMultiMap<QString, QString> rowData = itRows.value();
          QList<QStandardItem *> rowItems;

          // Construct row items based on rowData
          rowItems << new QStandardItem(rowData.value("ZC"))
                   << new QStandardItem(rowData.value("VB"))
                   << new QStandardItem(rowData.value("SensorType"))
                   << new QStandardItem(rowData.value("SizeOfSensor"))
                   << new QStandardItem(rowData.value("Gx"))
                   << new QStandardItem(rowData.value("Gy"))
                   << new QStandardItem(rowData.value("Gz"))
                   << new QStandardItem(rowData.value("F"))
                   << new QStandardItem(rowData.value("F1"))
                   << new QStandardItem(rowData.value("F2"))
                   << new QStandardItem(rowData.value("Checksum"))
                   << new QStandardItem(rowData.value("PatternString"));

          // Append the row to the tableModel
          tableModel->appendRow(rowItems);

          ++itRows;
      }*/





/*void MainWindow::handleReadyRead()
{
    QByteArray dataReceived = serialPort.readAll(); // Read available data
    buffer.append(dataReceived); // Append received data to the buffer

    const int packetLength = 64; // Assuming each packet is 64 bytes long
    const QString targetPattern = "FAFF";
    QString hexString = buffer.toHex().toUpper();

    int index = 0;
    int totalLength = hexString.length();
    int packetCount = 0;

    while (totalLength - index >= packetLength)
    {
        QString packetHex = hexString.mid(index, packetLength);
        QByteArray packetData = QByteArray::fromHex(packetHex.toUtf8());

        if (packetData.size() == packetLength / 2)
        {
            ++packetCount;

            if (packetCount <= 189)
            {
                ui->plainTextEdit_2->appendPlainText(packetHex);
            }
        }

        index += packetLength;
    }

    if (index > 0)
    {
        // Remove processed data up to the last complete packet
        buffer.remove(0, index / 2);
    }

    printFaffPatterns();
}

*/


void MainWindow::on_pushButton_clicked()
{

    if (sendTimer->isActive())
    {
        sendTimer->stop();
    }

    close();

}



void MainWindow::on_TESTT_clicked()
{
    //QString plainText = ui->plainTextEdit_2->toPlainText();




}

void MainWindow::onPlainTextChanged()
   {
       //printFaffPatterns();
   }






void MainWindow::printFaffPatterns() {
    QString data = ui->plainTextEdit_2->toPlainText();
    QRegularExpression pattern("FAFF[0-9A-F]{60}");
    QRegularExpressionMatchIterator it = pattern.globalMatch(data);
    QString Ax, Ay, Az, Gx, Gy, Gz;
    float f, f1, f2, g, g1, g2;
    QSet<int> uniquePacketCounters; // Set to store unique packet counters
    QMap<int, QMultiMap<QString, QString>> uniqueRows; // Map to store unique rows

    union ulf {
        unsigned long ul;
        float rec, rec1, rec2;
    };

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString patternString = match.captured();
        QString packetNumber = patternString.mid(14, 4);
        int ZC = packetNumber.toInt(nullptr, 16);

        // Check if the current packet counter is unique
        if (!uniquePacketCounters.contains(ZC)) {
            uniquePacketCounters.insert(ZC); // Add to the set of unique packet counters

            QString sampleTime = patternString.mid(24, 8);
            QString sensorType = patternString.mid(32, 4);
            QString sizeOfSensor = patternString.mid(36, 2);
            QString dataCenter = patternString.mid(38, 24);
            QString checksumNumber = patternString.mid(62, 2);

            int VB = sampleTime.toInt(nullptr, 16);

            ulf uus, uu1s, uu2s, uu, uu1, uu2;
            QString formattedG, formattedG1, formattedG2, formattedF, formattedF1, formattedF2;

            // Extract and format values


            if (sensorType == "8040") {
                Gx = dataCenter.mid(0, 8);
                Gy = dataCenter.mid(8, 8);
                Gz = dataCenter.mid(16, 8);

                std::string str = Gx.toStdString();
                std::string str2 = Gy.toStdString();
                std::string str3 = Gz.toStdString();

                std::stringstream ss(str);
                std::stringstream ss1(str2);
                std::stringstream ss2(str3);

                ss >> std::hex >> uus.ul;
                ss1 >> std::hex >> uu1s.ul;
                ss2 >> std::hex >> uu2s.ul;

                f = uus.rec;
                f1 = uu1s.rec1;
                f2 = uu2s.rec2;

                formattedF = QString::number(f, 'f', 7);
                formattedF1 = QString::number(f1, 'f', 7);
                formattedF2 = QString::number(f2, 'f', 7);

            } else {
                Ax = dataCenter.mid(0, 8);
                Ay = dataCenter.mid(8, 8);
                Az = dataCenter.mid(16, 8);

                std::string str = Ax.toStdString();
                std::string str2 = Ay.toStdString();
                std::string str3 = Az.toStdString();

                std::stringstream ss(str);
                std::stringstream ss1(str2);
                std::stringstream ss2(str3);

                ss >> std::hex >> uu.ul;
                ss1 >> std::hex >> uu1.ul;
                ss2 >> std::hex >> uu2.ul;

                g = uu.rec;
                g1 = uu1.rec1;
                g2 = uu2.rec2;

                formattedG = QString::number(g, 'f', 7);
                formattedG1 = QString::number(g1, 'f', 7);
                formattedG2 = QString::number(g2, 'f', 7);

            }



            // Create a map for the row data
            QMultiMap<QString, QString> rowData;
            rowData.insert("ZC", QString::number(ZC));
            rowData.insert("VB", QString::number(VB));
            rowData.insert("SensorType", sensorType);
            rowData.insert("SizeOfSensor", sizeOfSensor);
            rowData.insert("Gx", sensorType == "8040" ? formattedG : formattedG1);
            rowData.insert("Gy", sensorType == "8040" ? formattedG1 : formattedG);
            rowData.insert("Gz", sensorType == "8040" ? formattedG2 : formattedG2);
            rowData.insert("F", sensorType == "8040" ? formattedF : formattedF1);
            rowData.insert("F1", sensorType == "8040" ? formattedF1 : formattedF);
            rowData.insert("F2", sensorType == "8040" ? formattedF2 : formattedF2);
            rowData.insert("Checksum", checksumNumber);
            rowData.insert("PatternString", patternString);

            // Insert the row data into the uniqueRows map
            uniqueRows.insert(ZC, rowData);
        }
    }

    // Clear the current table model
    tableModel->clear();

    // Iterate through the uniqueRows map and construct rows for the table view
    QMap<int, QMultiMap<QString, QString>>::const_iterator itRows = uniqueRows.constBegin();
    while (itRows != uniqueRows.constEnd()) {
        QMultiMap<QString, QString> rowData = itRows.value();
        QList<QStandardItem *> rowItems;

        // Construct row items based on rowData
        rowItems << new QStandardItem(rowData.value("ZC"))
                 << new QStandardItem(rowData.value("VB"))
                 << new QStandardItem(rowData.value("SensorType"))
                 << new QStandardItem(rowData.value("SizeOfSensor"))
                 << new QStandardItem(rowData.value("Gx"))
                 << new QStandardItem(rowData.value("Gy"))
                 << new QStandardItem(rowData.value("Gz"))
                 << new QStandardItem(rowData.value("F"))
                 << new QStandardItem(rowData.value("F1"))
                 << new QStandardItem(rowData.value("F2"))
                 << new QStandardItem(rowData.value("Checksum"))
                 << new QStandardItem(rowData.value("PatternString"));

        // Append the row to the tableModel
        tableModel->appendRow(rowItems);

        ++itRows;
    }
}








QStringList MainWindow::getAvailableComPorts()
{
    QStringList ports;
    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts())
    {
        ports.append(portInfo.portName());
    }
    return ports;
}

void MainWindow::on_EXCELL_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save CSV File", "", "CSV Files (*.csv);;All Files (*)");
    if (filePath.isEmpty()) {
        return; // User canceled the save dialog
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Handle error if file cannot be opened
        return;
    }

    QTextStream out(&file);

    QSet<QString> uniquePacketCounters;
    int packetCounterColumn = 0; // Adjust this index based on your column position

    // Write the CSV header
    QStringList header;
    for (int col = 0; col < ui->tableView_3->model()->columnCount(); ++col) {
        header << ui->tableView_3->model()->headerData(col, Qt::Horizontal).toString();
    }
    out << header.join(",") << endl;

    // Write the rows with unique "Packet Counter" values to the CSV file
    for (int row = 0; row < ui->tableView_3->model()->rowCount(); ++row) {
        QString packetCounter = ui->tableView_3->model()->index(row, packetCounterColumn).data().toString();
        if (!uniquePacketCounters.contains(packetCounter)) {
            QStringList rowData;
            for (int col = 0; col < ui->tableView_3->model()->columnCount(); ++col) {
                rowData << ui->tableView_3->model()->index(row, col).data().toString();
            }
            out << rowData.join(",") << endl;
            uniquePacketCounters.insert(packetCounter);
        }
    }

    file.close();

}

void MainWindow::on_pushButton_2_clicked()
{
     //printFaffPatterns();
}
