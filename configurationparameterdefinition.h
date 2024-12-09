#ifndef CONFIGURATIONPARAMETERDEFINITION_H
#define CONFIGURATIONPARAMETERDEFINITION_H
#include <QtXml>
#include <QList>


typedef enum
{
    Uint,
    boolean,
    enumuration,
    Float,
    Function
}ParameterType;

typedef enum
{
    PreCompileConfig,
    LinkTimeConfig,
    PostBuildConfig
}ConfigType;

class ConfigurationParameterDefinition
{
private:

    QString ConfigurableName;
    ParameterType parametertype;
    QStringList choosable;
    ConfigType configtype;
    int LowerMultiplicity;
    int UpperMultiplicity;
    QString Description;


    QString LineEditValue;
    int ComboBoxIndex;
    QString ComboBoxValue;
    QString CheckBoxValue;
    QString ChoosenValue;
    bool is_Symbolic;
    QString SymbolicValue;
public:


    ConfigurationParameterDefinition();
    ConfigurationParameterDefinition(const ConfigurationParameterDefinition&inst);
    ConfigurationParameterDefinition(const QString &ConfigurableName, ParameterType parametertype, const QStringList &choosable, ConfigType configtype,int Lower,int Upper,bool is_Symbolic);
    QString getConfigurableName() const;
    void setConfigurableName(const QString &newConfigurableName);
    ParameterType getParametertype() const;
    void setParametertype(ParameterType newParametertype);
    QStringList getChoosable() const;
    void setChoosable(const QStringList &newChoosable);
    ConfigType getConfigtype() const;
    void setConfigtype(ConfigType newConfigtype);

    ConfigurationParameterDefinition& operator=(const ConfigurationParameterDefinition& val);
    ConfigurationParameterDefinition& operator=(ConfigurationParameterDefinition&& val) noexcept;

    friend class ConfigurationParameterAggregation;
    friend class Form;
    friend class ARXML_Reader;

};

#endif // CONFIGURATIONPARAMETERDEFINITION_H
