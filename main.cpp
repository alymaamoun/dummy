#include "arxml_reader.h"
#include "configurationparameteraggregation.h"
#include <QApplication>
#include <QGridLayout>
#include <QTime>
#include "configurationscontainers.h"
#include "form.h"
#include <QTreeWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include "mainwindow.h"
#include <QThread>
void delay( int millisecondsToWait );

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    /***********************************************************************************************
    *************************************instantiate the vectors************************************
    ************************************************************************************************/
    Class_ConfigurationParameterAggregation_Instance = new QList<ConfigurationParameterAggregation>;
    ARXML_Reader *Class_ARXML_Reader_Instance= new ARXML_Reader();
    Types =new QVector<QStringList>;
    Container=new ConfigurationsContainers();



    MainWindow startwindow;
    startwindow.show();
    delay(3000);
    startwindow.close();
    // Open file dialog to select a file
    QString filePath = QFileDialog::getOpenFileName(
        nullptr,                            // Parent widget (nullptr for no parent)
        "Select a File",                    // Dialog title
        QDir::homePath(),                   // Initial directory
        "Text Files (*.txt);;All Files (*)" // File filters
        );

    // Check if a file was selected
    if (!filePath.isEmpty()) {
        qDebug() << "Selected file:" << filePath;
        QMessageBox::information(nullptr, "File Selected", "Selected File: " + filePath);
    } else {
        qDebug() << "No file selected";
    }



    Class_ARXML_Reader_Instance->ARXML_ReadDocument("D:/Configurators all/AUTOSAR Configurator milestone 1/PortDefinition.arxml");
    Container->setContainerName(MODULE_SHORTNAME);
    Class_ARXML_Reader_Instance->ARXML_ReadContainers("CONTAINERS");
    for(int i{};i<Container->getSubContainer()->size();i++)
    {
            Class_ARXML_Reader_Instance->ARXML_FillContainer("SUB-CONTAINERS",Container->getSubContainer()->at(i));
    }



    Form form;

    form.show();


    // Specify the file name




    return a.exec();
}
void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}








//#include <QApplication>
//#include <QWidget>
//#include <QVBoxLayout>
//#include <QLineEdit>
//#include <QPushButton>
//#include <QDebug>

//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    // Create the main window
//    QWidget window;
//    window.setWindowTitle("Line Edit Example");

//    // Create layout for the window
//    QVBoxLayout *layout = new QVBoxLayout(&window);

//    // Create Line Edits
//    QLineEdit *lineEdit1 = new QLineEdit(&window);
//    QLineEdit *lineEdit2 = new QLineEdit(&window);

//    // Add Line Edits to layout
//    layout->addWidget(lineEdit1);
//    layout->addWidget(lineEdit2);

//    // Create Push Button
//    QPushButton *button = new QPushButton("Print Text", &window);

//    // Add button to layout
//    layout->addWidget(button);

//    // Connect button clicked signal to a lambda function
//    QObject::connect(button, &QPushButton::clicked, [&]() {
//        // Get text from Line Edits and print
//        qDebug() << "Text from Line Edit 1: " << lineEdit1->text();
//        qDebug() << "Text from Line Edit 2: " << lineEdit2->text();
//    });

//    // Set the layout for the window
//    window.setLayout(layout);

//    // Resize and show the window
//    window.resize(300, 200);
//    window.show();

//    return app.exec();
//}
