

#ifndef CONFIGURATIONPARAMETERGUI_H
#define CONFIGURATIONPARAMETERGUI_H
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

class ConfigurationParameterGUI
{

    QString LabelName;
    QString Description;
    QLabel *Label;
    QLabel *DiscreptionLabel;
    QLineEdit *LineEdit;
    QComboBox *ComboBox;
    QCheckBox *CheckBox;
    QStringList *choosables;
    ConfigurationParameterGUI();
    ConfigurationParameterGUI(const ConfigurationParameterGUI&inst);
public:


    ConfigurationParameterGUI(const QString &newLabelName, const QStringList &text, const QString &DescriptionLabel);
    QString getLabelName() const;
    void setLabelName(const QString &newLabelName);
    QLabel *getLabel() const;
    void setLabel(QLabel *newLabel);
    QLineEdit *getLineEdit() const;
    void setLineEdit(QLineEdit *newLineEdit);
    QComboBox *getComboBox() const;
    void setComboBox(const QStringList &text);
    friend class ARXML_Reader;
    QCheckBox *getCheckBox() const;
    void setCheckBox(QCheckBox *newCheckBox);




    ConfigurationParameterGUI& operator=(const ConfigurationParameterGUI& val);
    ConfigurationParameterGUI& operator=(ConfigurationParameterGUI&& val) noexcept;

    friend class ConfigurationParameterAggregation;
    friend class Form;

};

#endif // CONFIGURATIONPARAMETERGUI_H
