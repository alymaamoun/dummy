#include "form.h"
#include "arxml_reader.h"
#include "ui_form.h"
#include "configurationscontainers.h"
#include <QStyledItemDelegate>
#include <iostream>


QString con;
QString CFG_H_CONTENT="";
QFile CFGfile_h(MODULE_SHORTNAME+"_cfg.h");
QFile CFGfile_c(MODULE_SHORTNAME+"_cfg.c");
QFile LTfile_c(MODULE_SHORTNAME+"_Lcfg.c");
QFile LTfile_h(MODULE_SHORTNAME+"_Lcfg.h");
QFile PBfile_h(MODULE_SHORTNAME+"_PBcfg.h");
QFile PBfile_c(MODULE_SHORTNAME+"_PBcfg.c");




int lastRowAccessed=0;
using std::vector;
vector<int> Indexing;
QString IndeciesLines="";
bool FirstExpansion=true;
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    Indexing.resize(CurrentMultiplicity.size());
    Indexing.assign(Indexing.size(),0);
    CurrentContainer=new ConfigurationsContainers;
    ui->setupUi(this);
    //ParametersWindow=new QGridLayout(ui->frame);
    CFGfile_h.setFileName(MODULE_SHORTNAME+"_cfg.h");
    PBfile_h.setFileName(MODULE_SHORTNAME+"_PBcfg.h");
    PBfile_c.setFileName(MODULE_SHORTNAME+"_PBcfg.c");
    CFGfile_c.setFileName(MODULE_SHORTNAME+"_cfg.c");
    LTfile_c.setFileName(MODULE_SHORTNAME+"_Lcfg.c");
    LTfile_h.setFileName(MODULE_SHORTNAME+"_Lcfg.h");
    ui->LINEDIT_SHRTNAME->setVisible(false);
    ui->BTN_DONE->setVisible(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    //ui->pushButton_3->setEnabled(false);
    ui->treeWidget->setColumnWidth(0,(ui->treeWidget->columnWidth(1)+ui->treeWidget->columnWidth(0))/2);
    FillTree();


}

void Form::SetCurrentContainer(ConfigurationsContainers *CurrentContainer)
{
    this->CurrentContainer=CurrentContainer;
}


Form::~Form()
{
    delete ui;
}
int Form::getAbsoluteRow(QTreeWidgetItem *item)
{
    int topLevelIndex= ui->treeWidget->indexOfTopLevelItem(item);
    if (topLevelIndex != -1) {
        //qDebug() << "Top level index:" << topLevelIndex;
        return-1;
    }
    // For child items, get the index relative to their parent
    if (QTreeWidgetItem* parent = item->parent()) {
        int childIndex = parent->indexOfChild(item);
        //qDebug() << "Child index within parent:" << childIndex;
    }
    int absoluteIndex = 0;
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it && *it != item) {
        QWidget* widget=ui->treeWidget->itemWidget(*it,1);
        if(!widget)
        {
            ++absoluteIndex;
        }
        ++it;


    }
    qDebug() << "Absolute index:" << absoluteIndex;
    return absoluteIndex;
}
QTreeWidgetItem* Form::getItemAtRow(int targetRow)
{
    int currentRow=0;
    QTreeWidgetItemIterator it(ui->treeWidget);
    while(*it)
    {
        QWidget* widget=ui->treeWidget->itemWidget(*it,1);
        if(!widget)
        {
            if(currentRow==targetRow)
            {
                return *it;
            }
            ++currentRow;
        }

        ++it;
    }
    return nullptr;
}
void Form::FillTree()
{
    /*RESET STATE OF THE FORM*/
    ui->treeWidget->clear();
    ui->LINEDIT_SHRTNAME->setVisible(false);
    ui->BTN_DONE->setVisible(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);

    /*The head to carry the top item which is dio for example*/
    QTreeWidgetItem * Head=new QTreeWidgetItem;

    Head->setText(0,Container->ContainerName);
    /*the tree will now carry the head */
    ui->treeWidget->addTopLevelItem(Head);
    /*Loop to fill the subcontainers*/
    /*sub containers has two types, base sub container and child*/
    for(int i{};i<Container->SubContainer->size();i++)
    {
        addItemsToTreeWidget(ui->treeWidget,Head,Container->SubContainer->at(i));
    }
    // ui->frame->setLayout(ParametersWindow);

    ui->treeWidget->expandAll();





}
void Form::addItemsToTreeWidget(QTreeWidget *TreeWidget, QTreeWidgetItem *ParentItem, ConfigurationsContainers *container)
{
    QTreeWidgetItem* item=new QTreeWidgetItem(ParentItem);
    /*Case 1: base container
     * base container is the type of containers in which it will not carry values
     * it's type usage is schema that's carrying types
     * also it's counting the number of the occurance of this container
     * this types is used when this container can have multiple instances
     * ex: Dio channel
     */
    if(container->Container_bool_IsArray&&container->Container_bool_IsBase)
    {
        /*to make sure that the shortname is reseted*/
        container->ContainerName=container->ContainerName.replace(container->ShortName,"");
        /*remove old occurance number*/
        container->ShortName=container->ShortName.replace("("+QString::number(container->Instances->size()-1)+")","");
        container->ShortName=container->ShortName.replace("("+QString::number(container->Instances->size())+")","");
        container->ShortName=container->ShortName.replace("("+QString::number(container->Instances->size()+1)+")","");
        container->ShortName+="("+QString::number(container->Instances->size())+")";

        container->ContainerName+=container->ShortName;
        item->setText(0,container->ContainerName);
        item->setForeground(0,QBrush(QColorConstants::Svg::lightskyblue));
        QFont font;
        font.setBold(true);
        font.setPointSize(14);

        // Apply the font to the item
        item->setFont(0, font);

    }
    else
    {
        item->setText(0,container->ContainerName);
        QFont font;
        font.setPointSize(12);

        // Apply the font to the item
        item->setFont(0, font);
    }




    if(container->Container_bool_IsArray&&container->Container_bool_IsBase)
    {
        for(int i{};i<container->Instances->size();i++)
        {
            addItemsToTreeWidget(TreeWidget,item,container->Instances->at(i));
        }
    }

    else
    {
        for (const auto& parameter : *container->parameters) {
            QTreeWidgetItem *paramItem = new QTreeWidgetItem(item);
            paramItem->setText(0, parameter->parameterDef->ConfigurableName);

            // Depending on parameter type, handle differently
            switch (parameter->parameterDef->parametertype) {
            case Function:
            case Float:
            case Uint: {
                parameter->paramGui->LineEdit= new QLineEdit(ui->treeWidget);
                parameter->paramGui->LineEdit->setText(parameter->parameterDef->LineEditValue);
                ui->treeWidget->setItemWidget(paramItem, 1, parameter->paramGui->LineEdit);
                break;
            }
            case boolean: {
                parameter->paramGui->CheckBox = new QCheckBox(ui->treeWidget);
                // Set checkBox->setChecked() according to parameter value if needed
                if(parameter->parameterDef->CheckBoxValue=="TRUE")
                {
                    parameter->paramGui->CheckBox->setChecked(true);
                }
                else
                {
                    parameter->paramGui->CheckBox->setChecked(false);
                }
                ui->treeWidget->setItemWidget(paramItem, 1, parameter->paramGui->CheckBox);
                break;
            }
            case enumuration: {
                parameter->paramGui->ComboBox = new QComboBox(ui->treeWidget);
                // Populate comboBox with choices from parameter
                parameter->paramGui->ComboBox->addItems(*parameter->paramGui->choosables);
                parameter->paramGui->ComboBox->setCurrentIndex(parameter->parameterDef->ComboBoxIndex);
                ui->treeWidget->setItemWidget(paramItem, 1, parameter->paramGui->ComboBox);
                break;
            }
            default:
                break;
            }
        }
        for(int i{};i<container->SubContainer->size();i++)
        {
            addItemsToTreeWidget(TreeWidget,item,container->SubContainer->at(i));
        }
    }
}
void Form::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column){}
void Form::on_pushButton_clicked()
{
    //qDebug()<<ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn());

    QString con=ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn());
    for(int i=0;i<CurrentMultiplicity.size();i++)
    {
        std::cout<<CurrentMultiplicity.at(i)<<" ";
    }
    std::cout<<std::endl;
    Remove(con,Container);
    for(int i=0;i<CurrentMultiplicity.size();i++)
    {
        std::cout<<CurrentMultiplicity.at(i)<<" ";
    }
    std::cout<<std::endl;
    FillTree();
}


void Form::on_pushButton_2_clicked()
{
    //ui->frame->update();
    //qDebug()<<Container->SubContainer->at(0)->SubContainer->at(0)->SubContainer->at(0)->parameters->at(3)->paramGui->ComboBox->currentIndex();
    QString con=ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn());
    AddNew(con,Container);
    FillTree();
}
void Form::GetParameters(ConfigurationsContainers *current)
{}
void Form::GetContainerIndex(const QString& containerName, ConfigurationsContainers* current, int op)
{
    SaveNewValues(Container);

    if (containerName == current->ContainerName && !(current->Container_bool_IsArray && current->Container_bool_IsBase))
    {
        if (op == 0)
        {
            // GetParameters(current);
        }
        else if (op == 1)
        {
            SaveNewValues(current);
        }
        return;
    }

    for (int i = 0; i < current->SubContainer->count(); ++i)
    {
        auto subContainer = current->SubContainer->at(i);
        if (containerName == subContainer->ContainerName)
        {
            if (op == 0)
            {
                // GetParameters(subContainer);
            }
            else if (op == 1)
            {
                SaveNewValues(subContainer);
            }
            break;
        }
    }

    if (current->Container_bool_IsArray && current->Container_bool_IsBase)
    {
        for (int i = 0; i < current->Instances->count(); ++i)
        {
            GetContainerIndex(containerName, current->Instances->at(i), op);
        }
    }
    else
    {
        for (int i = 0; i < current->SubContainer->count(); ++i)
        {
            GetContainerIndex(containerName, current->SubContainer->at(i), op);
        }
    }

    for (auto subContainer : *current->getSubContainer())
    {
        if (containerName == subContainer->ContainerName)
        {
            if ((CurrentMultiplicity.at(subContainer->id) + 1) <= subContainer->upperMul &&
                (subContainer->Container_bool_IsArray && subContainer->Container_bool_IsBase))
            {
                ui->pushButton_2->setEnabled(true);
            }
            if ((CurrentMultiplicity.at(subContainer->id) - 1) >= subContainer->lowerMul &&
                (subContainer->Container_bool_IsArray && !subContainer->Container_bool_IsBase))
            {
                ui->pushButton->setEnabled(true);
            }
        }
    }

    for (int i = 0; i < current->Instances->count(); ++i)
    {
        auto instance = current->Instances->at(i);
        if (containerName == instance->ContainerName)
        {
            if ((CurrentMultiplicity.at(instance->id) + 1) <= instance->upperMul &&
                (instance->Container_bool_IsArray && instance->Container_bool_IsBase))
            {
                ui->pushButton_2->setEnabled(true);
            }
            if ((CurrentMultiplicity.at(instance->id) - 1) >= instance->lowerMul &&
                (instance->Container_bool_IsArray && !instance->Container_bool_IsBase))
            {
                ui->pushButton->setEnabled(true);
            }
        }
    }

    if (current->Container_bool_IsArray && current->Container_bool_IsBase)
    {
        for (int i = 0; i < current->Instances->count(); ++i)
        {
            GetContainerIndex(containerName, current->Instances->at(i), 0);
        }
    }
    else
    {
        for (int i = 0; i < current->SubContainer->count(); ++i)
        {
            GetContainerIndex(containerName, current->SubContainer->at(i), 0);
        }
    }
}

void Form::SaveNewValues(ConfigurationsContainers *current)
{
    if(current->Container_bool_IsArray&&current->Container_bool_IsBase)
    {

    }
    else
    {
        for(int i{};i<current->parameters->count();i++)
        {
            //qDebug()<<current->parameters->at(i)->parameterDef->ConfigurableName;

            if(current->parameters->at(i)->parameterDef->parametertype==Uint)
            {
                current->parameters->at(i)->parameterDef->LineEditValue=current->parameters->at(i)->paramGui->LineEdit->text();
                current->parameters->at(i)->parameterDef->ChoosenValue=current->parameters->at(i)->parameterDef->LineEditValue;
                current->parameters->at(i)->paramGui->LineEdit->setText(current->parameters->at(i)->parameterDef->LineEditValue);
                //qDebug()<<current->parameters->at(i)->parameterDef->LineEditValue;
            }
            else if(current->parameters->at(i)->parameterDef->parametertype==enumuration)
            {
                current->parameters->at(i)->parameterDef->ComboBoxValue=current->parameters->at(i)->paramGui->ComboBox->currentText();
                current->parameters->at(i)->parameterDef->ChoosenValue=current->parameters->at(i)->parameterDef->ComboBoxValue;

                current->parameters->at(i)->parameterDef->ComboBoxIndex=current->parameters->at(i)->paramGui->ComboBox->currentIndex();
                //qDebug()<<current->parameters->at(i)->parameterDef->ComboBoxValue;
                //qDebug()<<current->parameters->at(i)->parameterDef->ComboBoxIndex;

            }
            else
            {
                if(current->parameters->at(i)->paramGui->CheckBox->checkState()==0)
                {
                    current->parameters->at(i)->parameterDef->CheckBoxValue="(STD_OFF)";
                    current->parameters->at(i)->parameterDef->ChoosenValue="(STD_OFF)";
                }
                else
                {
                    current->parameters->at(i)->parameterDef->CheckBoxValue="(STD_ON)";
                    current->parameters->at(i)->parameterDef->ChoosenValue="(STD_ON)";
                }
                //qDebug()<<current->parameters->at(i)->parameterDef->CheckBoxValue;
            }
        }
    }
    if(current->Container_bool_IsArray&&current->Container_bool_IsBase)
    {
        for(int i{};i<current->Instances->count();i++)
        {
            SaveNewValues(current->Instances->at(i));
        }
    }
    else
    {
        for(int i{};i<current->SubContainer->size();i++)
        {
            SaveNewValues(current->SubContainer->at(i));
        }
    }
}
void Form::AddNew(QString ContainerName,ConfigurationsContainers *current)
{

    SaveNewValues(Container);
    //qDebug()<<"caution "<<ContainerName<<" "<<current->ContainerName;
    if(ContainerName==current->ContainerName)
    {
        if((CurrentMultiplicity.at(current->id)+1)<=current->upperMul)
        {
            CurrentMultiplicity[current->id]+=1;
            ConfigurationsContainers *temp=new ConfigurationsContainers{*current} ;
            temp->ContainerName=temp->ContainerName;
            temp->Container_bool_IsBase=false;
            current->Instances->push_back(temp);
            ui->treeWidget->clear();
            this->FillTree();
        }
    }
    if(current->Container_bool_IsArray&&current->Container_bool_IsBase)
    {
        for(int i=0;i<current->Instances->size();i++)
        {
            AddNew(ContainerName,current->Instances->at(i));
        }
    }
    else
    {
        for(int i=0;i<current->getSubContainer()->size();i++)
        {
            AddNew(ContainerName,current->SubContainer->at(i));
        }
    }
}
int Form::Remove(QString ContainerName, ConfigurationsContainers *current)
{
    SaveNewValues(Container);
    if(ContainerName==current->ContainerName)
    {
        for(int subcontainer=0;subcontainer<current->getSubContainer()->size();subcontainer++)
        {
            if(current->SubContainer->at(subcontainer)->Container_bool_IsBase&&current->SubContainer->at(subcontainer)->Container_bool_IsArray)
            {
                for(int instance=0;instance<current->SubContainer->at(subcontainer)->Instances->size();instance++)
                {
                    Remove(current->SubContainer->at(subcontainer)->Instances->at(instance)->ContainerName,current->SubContainer->at(subcontainer)->Instances->at(instance));
                    current->SubContainer->at(subcontainer)->Instances->removeAt(instance);
                    instance--;
                }

            }
            else
            {
                Remove(current->SubContainer->at(subcontainer)->ContainerName,current->SubContainer->at(subcontainer));
                current->SubContainer->removeAt(subcontainer);
                subcontainer--;
            }

        }
        CurrentMultiplicity[current->id]-=1;
        ui->treeWidget->clear();
        this->FillTree();
        return 1;

    }
    else if(current->Container_bool_IsArray&&current->Container_bool_IsBase)
    {
        for(int instance=0;instance<current->Instances->size();instance++)
        {
            if(Remove(ContainerName,current->Instances->at(instance)))
            {
                current->Instances->removeAt(instance);
                instance--;
            }
        }
    }
    else
    {
        for(int subcontainer=0;subcontainer<current->getSubContainer()->size();subcontainer++)
        {
            if(Remove(ContainerName,current->SubContainer->at(subcontainer)))
            {
                current->Instances->removeAt(subcontainer);
                subcontainer--;
            }


        }

    }


}
void Form::on_pushButton_3_clicked()
{

    //ui->frame->update();
    GetContainerIndex(con,Container,1);
}


void Form::on_BTN_Generate_clicked()
{
    SaveNewValues(Container);
    Generate();
}

void Form::readandwrite(ConfigurationsContainers * root,int tabing,QString &lines)
{

    QString tab=" ";

    if(root->IsPostBuilt)
    {
        //        out<<tab.repeated(tabing+4)+"{  //"+root->ContainerName<<"\n";
        //        out<<tab.repeated(tabing+8);
        lines+="\n";
        lines+=tab.repeated(tabing+4)+"{  //"+root->ContainerName+"\n";
        lines+=tab.repeated(tabing+8);
        for(int currentParameter{};currentParameter<root->parameters->count();currentParameter++)
        {

            if(root->parameters->at(currentParameter)->parameterDef->configtype==PreCompileConfig&&root->parameters->at(currentParameter)->parameterDef->is_Symbolic==false)
            {
                continue;
            }
            //out<<root->parameters->at(currentParameter)->parameterDef->ChoosenValue<<",";
            lines+="."+root->parameters->at(currentParameter)->parameterDef->ConfigurableName+"="+root->parameters->at(currentParameter)->parameterDef->ChoosenValue+",";
        }
        lines=lines.mid(0,lines.length()-1);
        //out<<"\n";
        lines+="\n";
    }
    for(int CurrentContainer {};CurrentContainer<root->SubContainer->count();CurrentContainer++)
    {
        readandwrite(root->SubContainer->at(CurrentContainer),tabing+4,lines);
    }
    lines=lines.mid(0,lines.length()-1);
    lines+="\n";
    if(root->IsPostBuilt)
    {
        // out<<tab.repeated(tabing+4)+"},  //"+root->ContainerName<<"\n";
        lines+=tab.repeated(tabing+4)+"},";
    }
    for(int currentParameter{};currentParameter<root->parameters->count();currentParameter++)
    {

        if(root->parameters->at(currentParameter)->parameterDef->configtype==PreCompileConfig&&root->parameters->at(currentParameter)->parameterDef->is_Symbolic==false)
        {
            CFG_H_CONTENT+= "#define "+root->parameters->at(currentParameter)->parameterDef->ConfigurableName+tab.repeated(2+NameMaxLength-root->parameters->at(currentParameter)->parameterDef->ConfigurableName.length())+root->parameters->at(currentParameter)->parameterDef->ChoosenValue+"\n";
        }
    }

}

void Form::Generate()
{
    CFG_H_CONTENT="/******************************************************************************\n"
                    " *\n"
                    " * Module: Dio\n"
                    " *\n"
                    " * File Name: Dio_Cfg.h\n"
                    " *\n"
                    " * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Dio Driver\n"
                    " *\n"
                    " * Author: Mohamed Tarek\n"
                    " ******************************************************************************/\n";
    IndeciesLines="";
    Indexing.assign(Indexing.size(),0);
    QTextStream PBfile_c_out(&PBfile_c);
    QTextStream LTfile_c_out(&LTfile_c);
    QTextStream CFGfile_h_out(&CFGfile_h);
    // Open the file in write-only mode
    if (!CFGfile_h.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //() << "Failed to create the file:" << CFGfile_h.errorString();
    }
    if (!CFGfile_c.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //qDebug() << "Failed to create the file:" << CFGfile_h.errorString();
    }
    if (!PBfile_h.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //qDebug() << "Failed to create the file:" << CFGfile_h.errorString();
    }
    if (!PBfile_c.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //qDebug() << "Failed to create the file:" << CFGfile_h.errorString();
    }

    if (!LTfile_c.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //qDebug() << "Failed to create the file:" << LTfile_c.errorString();
    }
    if (!LTfile_h.open(QIODevice::WriteOnly | QIODevice::Text)) {
       // //qDebug() << "Failed to create the file:" << LTfile_h.errorString();
    }
    CFG_H_CONTENT+="\n";
    CFG_H_CONTENT+="#ifndef "+MODULE_SHORTNAME.toUpper()+"_CFG_H\n";
    CFG_H_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_CFG_H\n";
    CFG_H_CONTENT+="\n";
    CFG_H_CONTENT+= "/*\n * Module Version 1.0.0\n */\n";
    CFG_H_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_CFG_SW_MAJOR_VERSION (1U)\n";
    CFG_H_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_CFG_SW_MINOR_VERSION (0U)\n";
    CFG_H_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_CFG_SW_PATCH_VERSION (0U)\n\n";
    CFG_H_CONTENT+="/*\n * AUTOSAR Version "+AR_MARJOR+"."+AR_MINOR+"."+AR_PATCH+"\n */\n";
    CFG_H_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_CFG_AR_RELEASE_MAJOR_VERSION ("+AR_MARJOR+"U)\n";
    CFG_H_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_CFG_AR_RELEASE_MINOR_VERSION ("+AR_MINOR+"U)\n";
    CFG_H_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_CFG_AR_RELEASE_PATCH_VERSION ("+AR_PATCH+"U)\n";
    CFG_H_CONTENT+="\n";
    CFG_H_CONTENT+="/* Number of configured Dio Channels*/\n";
    CFG_H_CONTENT+="#define DIO_CONFIGURED_CHANNLES           ("+QString::number(CurrentMultiplicity[4]-1)+"U)\n";

    QString lines;

    QString precompiler="";
    PreCompileGenerate(Container,precompiler);

    CFG_H_CONTENT+="\n/*Indecies*/\n";
    CFG_H_CONTENT+=IndeciesLines;
    CFG_H_CONTENT+="\n/*SYMBOLIC NAMES*/\n";
    CFG_H_CONTENT+=precompiler;

    //CFG_H_CONTENT+=SymbolicName;
    CFG_H_CONTENT+="#endif";
    lines=lines.mid(0,lines.length()-1);
    PBfile_c_out<<lines;
    //qDebug() <<  precompiler;

    CFGfile_h_out<<CFG_H_CONTENT;


    QString LINK_TIME_C_CONTENT="/******************************************************************************\n"
                           " *\n"
                           " * Module: Dio\n"
                           " *\n"
                           " * File Name: Dio_Lcfg.c\n"
                           " *\n"
                           " * Description: Link Time Configuration Source file for TM4C123GH6PM Microcontroller - Dio Driver\n"
                           " *\n"
                           " * Author: Mohamed Tarek\n"
                           " ******************************************************************************/\n";
    LINK_TIME_C_CONTENT+="\n#include "+MODULE_SHORTNAME+".h\n";
    LINK_TIME_C_CONTENT+= "/*\n * Module Version 1.0.0\n */\n";
    LINK_TIME_C_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_LCFG_SW_MAJOR_VERSION (1U)\n";
    LINK_TIME_C_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_LCFG_SW_MINOR_VERSION (0U)\n";
    LINK_TIME_C_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_LCFG_SW_PATCH_VERSION (0U)\n\n";

    LINK_TIME_C_CONTENT+="/*\n * AUTOSAR Version "+AR_MARJOR+"."+AR_MINOR+"."+AR_PATCH+"\n */\n";
    LINK_TIME_C_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_LCFG_AR_RELEASE_MAJOR_VERSION ("+AR_MARJOR+"U)\n";
    LINK_TIME_C_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_LCFG_AR_RELEASE_MINOR_VERSION ("+AR_MINOR+"U)\n";
    LINK_TIME_C_CONTENT+="#define "+MODULE_SHORTNAME.toUpper()+"_LCFG_AR_RELEASE_PATCH_VERSION ("+AR_PATCH+"U)\n";
    LINK_TIME_C_CONTENT+="const Dio_ConfigType Dio_Configuration  ={\n";
    vector<QString> data=LinkTimeGenerate(Container,0        );
    for(int i{};i<data.size();i++)
    {
        LINK_TIME_C_CONTENT+="    {"+data[i]+"}";
        if(i+1<data.size())
        {
            LINK_TIME_C_CONTENT+=",\n";
        }

    }
    LINK_TIME_C_CONTENT+="\n};";
    LTfile_c_out<<LINK_TIME_C_CONTENT;

    PBfile_c.close();
    PBfile_h.close();
    CFGfile_c.close();
    CFGfile_h.close();
    LTfile_c.close();
    LTfile_h.close();
}


void Form::on_BTN_SHRTNAME_clicked()
{

    QString con=ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn());
    ui->LINEDIT_SHRTNAME->setVisible(true);
    ui->BTN_DONE->setVisible(true);

}
void Form::AddShortName(QString ContainerName,ConfigurationsContainers *current)
{
    static int index=0;
    SaveNewValues(Container);

    QTreeWidgetItem * item=getItemAtRow(lastRowAccessed);
    if(item)
    {   index++;
        qDebug()<<item->text(0);
        qDebug()<<item->text(0);
        qDebug()<<"lastRowAccessed "<<lastRowAccessed;

        qDebug()<<"index "<<index;
        if(index==lastRowAccessed+1)
        {
            CurrentMultiplicity[current->id]+=1;
            ConfigurationsContainers *temp=new ConfigurationsContainers{*current} ;
            temp->ContainerName=temp->ContainerName;
            temp->Container_bool_IsBase=false;
            current->Instances->push_back(temp);
            ui->treeWidget->clear();
            this->FillTree();
            current->ContainerName.replace(current->ShortName,"");
            current->ShortName=ui->LINEDIT_SHRTNAME->text();
            current->ContainerName+=current->ShortName;
            ui->LINEDIT_SHRTNAME->setVisible(false);
            ui->BTN_DONE->setVisible(false);
            ui->treeWidget->clear();
            index=0;
            this->FillTree();


        }
        if(current->Container_bool_IsArray&&current->Container_bool_IsBase)
        {
            for(int i=0;i<current->Instances->size();i++)
            {
                AddShortName(ContainerName,current->Instances->at(i));
            }
        }
        else
        {
            for(int i=0;i<current->getSubContainer()->size();i++)
            {
                AddShortName(ContainerName,current->SubContainer->at(i));
            }
        }
    }

}
void Form::on_BTN_DONE_clicked()
{
    AddShortName(con,Container);
}


void Form::on_SymbolicNamesBTN_clicked(){}
void Form::on_treeWidget_clicked(const QModelIndex &index){}

void Form::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    con=item->text(column);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton->setDisabled(true);
    // clearGridLayout(ParametersWindow);
    GetContainerIndex(con,Container,0);
    lastRowAccessed=getAbsoluteRow(item);
    qDebug()<<item->columnCount();




}



void Form::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    con=item->text(column);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton->setDisabled(true);
    // clearGridLayout(ParametersWindow);

    GetContainerIndex(con,Container,0);
}
void Form::PreCompileGenerate(ConfigurationsContainers *root, QString &Data)
{
    static int index = 0;
    static const QString tabs(" ");

    if (!(root->Container_bool_IsArray && root->Container_bool_IsBase))
    {
        for (auto parameter : *root->parameters)
        {
            if (parameter->parameterDef->configtype == PreCompileConfig)
            {
                QString DefinedName = root->Container_bool_IsArray ? root->ContainerName : parameter->parameterDef->ConfigurableName;
                QString DefinedValue = parameter->parameterDef->ChoosenValue;

                if (parameter->parameterDef->UpperMultiplicity == 1 &&
                    parameter->parameterDef->LowerMultiplicity == 1 &&
                    !parameter->parameterDef->is_Symbolic)
                {
                    Data += "\n/* " + parameter->parameterDef->Description + "*/ \n";
                }

                Data += "#define " + DefinedName + tabs.repeated(NameMaxLength + 12 - DefinedName.length());
                Data += (parameter->parameterDef->parametertype == Uint) ?
                            "(" + DefinedValue + "U)\n" :
                            DefinedValue + "\n";

                if (root->Container_bool_IsArray)
                {
                    IndeciesLines += "#define " + DefinedName + "_Index" + tabs.repeated(NameMaxLength + 12 - DefinedName.length());
                    IndeciesLines += "(" + QString::number(Indexing[root->id]) + "U)\n";
                }

                Indexing[root->id]++;
            }
        }

        for (auto subContainer : *root->SubContainer)
        {
            PreCompileGenerate(subContainer, Data);
        }
    }
    else
    {
        for (int i = 0; i < root->Instances->count(); ++i)
        {
            index = i;
            PreCompileGenerate(root->Instances->at(i), Data);
        }
    }
}

std::vector<QString> Form::LinkTimeGenerate(ConfigurationsContainers *root, int shift)
{
    vector<QString> data;
    vector<QString> subdata;
    if(!MODULE_LinkTime_Supported)
    {
        return data;
    }
    if(!(root->Container_bool_IsArray&&root->Container_bool_IsBase))
    {
        for(int i{};i<root->SubContainer->count();i++)
        {
            vector<QString> instances;
            instances=LinkTimeGenerate(root->SubContainer->at(i),shift+1);
            subdata.insert(subdata.end(), instances.begin(), instances.end());

        }

        QString CurrentParameters="";

        for(int i{};i<root->parameters->count();i++)
        {

            if(root->parameters->at(i)->parameterDef->configtype==LinkTimeConfig)
            {
                CurrentParameters+=(root->parameters->at(i)->parameterDef->ChoosenValue);
                if(i+1<root->parameters->count())
                    CurrentParameters+=",";
            }
            else
            {
                if(root->parameters->at(i)->parameterDef->is_Symbolic)
                {
                    CurrentParameters+=(root->ContainerName);
                    if(i+1<root->parameters->count())
                        CurrentParameters+=",";
                }
            }

        }
        for(int subdataindex{};subdataindex<subdata.size();subdataindex++)
        {
            if(CurrentParameters!="")
                data.push_back(CurrentParameters+","+subdata[subdataindex]);
            else
                data.push_back(subdata[subdataindex]);
        }
        if(subdata.size()==0)
        {
            if(CurrentParameters!="")
                data.push_back(CurrentParameters);
        }
    }
    else
    {
        for(int i{};i<root->Instances->count();i++)
        {
            vector<QString> instances;
            instances=LinkTimeGenerate(root->Instances->at(i),shift+1);
            subdata.insert(subdata.end(), instances.begin(), instances.end());
        }
        return subdata;
    }
    return data;

}
void Form::PostBuildGenerate(ConfigurationsContainers *root, QString &Data)
{
    if(!MODULE_PostBuild_Supported)
    {
        return;
    }
}


void Form::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{

}


void Form::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{

}

