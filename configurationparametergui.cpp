#include "configurationparametergui.h"
QString ConfigurationParameterGUI::getLabelName() const
{
    return LabelName;
}

void ConfigurationParameterGUI::setLabelName(const QString &newLabelName)
{
    LabelName = newLabelName;
    Label=new QLabel{LabelName};
}

QLabel *ConfigurationParameterGUI::getLabel() const
{
    return Label;
}

void ConfigurationParameterGUI::setLabel(QLabel *newLabel)
{
    Label = newLabel;
}

QLineEdit *ConfigurationParameterGUI::getLineEdit() const
{
    return LineEdit;
}

void ConfigurationParameterGUI::setLineEdit(QLineEdit *newLineEdit)
{
    LineEdit = newLineEdit;
}

QComboBox *ConfigurationParameterGUI::getComboBox() const
{
    return ComboBox;
}

void ConfigurationParameterGUI::setComboBox(const QStringList &text)
{
    ComboBox->addItems(text);
}

QCheckBox *ConfigurationParameterGUI::getCheckBox() const
{
    return CheckBox;
}

void ConfigurationParameterGUI::setCheckBox(QCheckBox *newCheckBox)
{
    CheckBox = newCheckBox;
}

ConfigurationParameterGUI &ConfigurationParameterGUI::operator=(const ConfigurationParameterGUI &val)
{
    LineEdit =new QLineEdit;
    ComboBox= new QComboBox;
    CheckBox = new QCheckBox;
    LabelName=val.LabelName;
    Label=new QLabel{LabelName};
    choosables=new QStringList;
    choosables=val.choosables;
    ComboBox->addItems(*choosables);
    return *this;
}
ConfigurationParameterGUI& ConfigurationParameterGUI::operator=(ConfigurationParameterGUI&& val) noexcept
{
    LineEdit =new QLineEdit;
    ComboBox= new QComboBox;
    CheckBox = new QCheckBox;
    LabelName=val.LabelName;
    Label=new QLabel{LabelName};
    choosables=new QStringList;
    choosables=val.choosables;
    ComboBox->addItems(*choosables);
    return *this;
}

ConfigurationParameterGUI::ConfigurationParameterGUI()
{
    LabelName="N/A";
    Description="N/A";
    Label=new QLabel{LabelName};
    LineEdit =new QLineEdit;
    ComboBox= new QComboBox;
    CheckBox = new QCheckBox;
    DiscreptionLabel=new QLabel{Description};
    this->DiscreptionLabel->setStyleSheet("color: blue;");
    QFont font2 = DiscreptionLabel->font();
    font2.setPointSize(10); // Set font size to 16 points
    DiscreptionLabel->setFont(font2);
    DiscreptionLabel->setWordWrap(true);
}

ConfigurationParameterGUI::ConfigurationParameterGUI(const ConfigurationParameterGUI &inst)
{
    LineEdit =new QLineEdit;
    ComboBox= new QComboBox;
    CheckBox = new QCheckBox;
    LabelName=inst.LabelName;
    Description=inst.Description;
    DiscreptionLabel=new QLabel{Description};
    this->DiscreptionLabel->setStyleSheet("color: blue;");
    Label=new QLabel{LabelName};
    choosables=new QStringList;
    choosables=inst.choosables;
    ComboBox->addItems(*choosables);
    QFont font2 = DiscreptionLabel->font();
    font2.setPointSize(10); // Set font size to 16 points
    DiscreptionLabel->setFont(font2);
    DiscreptionLabel->setWordWrap(true);

}


ConfigurationParameterGUI::ConfigurationParameterGUI(const QString &newLabelName, const QStringList &text, const QString &DescriptionLabel)
{
    LabelName="N/A";
    Label=new QLabel{LabelName};
    LineEdit =new QLineEdit;
    ComboBox= new QComboBox;
    CheckBox = new QCheckBox;
    Description=DescriptionLabel;
    LabelName = newLabelName;
    Label=new QLabel{LabelName};
    this->DiscreptionLabel=new QLabel{Description};
    this->DiscreptionLabel->setStyleSheet("color: blue;");
    choosables=new QStringList;
    *choosables=text;
    ComboBox->addItems(*choosables);
    QFont font2 = DiscreptionLabel->font();
    font2.setPointSize(10); // Set font size to 16 points
    DiscreptionLabel->setFont(font2);
    DiscreptionLabel->setWordWrap(true);

}
