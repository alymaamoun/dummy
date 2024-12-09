#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "configurationscontainers.h"
#include "qgridlayout.h"
#include "qtreewidget.h"
extern QString Shared_ShortName;
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);

    void FillTree();
    void addItemsToTreeWidget(QTreeWidget* treeWidget,QTreeWidgetItem*ParentItem,ConfigurationsContainers*container);
    void SetCurrentContainer(ConfigurationsContainers * CurrentContainer);
    ~Form();
    Ui::Form *ui;
    QGridLayout *ParametersWindow;

private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void GetParameters(ConfigurationsContainers *current);

    void GetContainerIndex(const QString& containerName,ConfigurationsContainers *current,int op);


    void SaveNewValues(ConfigurationsContainers *current);

    void AddNew(QString ContainerName,ConfigurationsContainers *current);

    int Remove(QString ContainerName,ConfigurationsContainers *current);

    void on_pushButton_3_clicked();

    void on_BTN_Generate_clicked();

    void on_BTN_SHRTNAME_clicked();

    void on_BTN_DONE_clicked();

    void on_SymbolicNamesBTN_clicked();

    void on_treeWidget_clicked(const QModelIndex &index);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void PreCompileGenerate(ConfigurationsContainers * root, QString &Data);
    std::vector<QString> LinkTimeGenerate(ConfigurationsContainers * root, int shift);
    void PostBuildGenerate(ConfigurationsContainers * root,QString &Data);

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

private:



    ConfigurationsContainers * CurrentContainer;
    QTreeWidgetItem * FillRecurser(QList<ConfigurationsContainers *> *container);
    void Generate();
    void readandwrite(ConfigurationsContainers * root, int tabing, QString& lines);
    void AddShortName(QString ContainerName,ConfigurationsContainers *current);
    int getAbsoluteRow(QTreeWidgetItem *item);
    QTreeWidgetItem* getItemAtRow(int targetRow);

};

#endif // FORM_H
