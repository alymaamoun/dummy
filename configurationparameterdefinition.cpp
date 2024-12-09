#include "configurationparameterdefinition.h"

QString ConfigurationParameterDefinition::getConfigurableName() const
{
    return ConfigurableName;
}

void ConfigurationParameterDefinition::setConfigurableName(const QString &newConfigurableName)
{
    ConfigurableName = newConfigurableName;
}

ParameterType ConfigurationParameterDefinition::getParametertype() const
{
    return parametertype;
}

void ConfigurationParameterDefinition::setParametertype(ParameterType newParametertype)
{
    parametertype = newParametertype;
}

QStringList ConfigurationParameterDefinition::getChoosable() const
{
    return choosable;
}

void ConfigurationParameterDefinition::setChoosable(const QStringList &newChoosable)
{
    choosable = newChoosable;
}

ConfigType ConfigurationParameterDefinition::getConfigtype() const
{
    return configtype;
}

void ConfigurationParameterDefinition::setConfigtype(ConfigType newConfigtype)
{
    configtype = newConfigtype;
}

ConfigurationParameterDefinition &ConfigurationParameterDefinition::operator=(const ConfigurationParameterDefinition &inst)
{
    this->parametertype=inst.parametertype;
    this->ConfigurableName=inst.ConfigurableName;
    this->Description=inst.Description;
    this->LowerMultiplicity=inst.LowerMultiplicity;
    this->UpperMultiplicity=inst.UpperMultiplicity;
    this->choosable=inst.choosable;
    this->configtype=inst.configtype;
    this->is_Symbolic=inst.is_Symbolic;
    this->LineEditValue="";
    this->ComboBoxIndex=0;
    this->ComboBoxValue="";
    this->CheckBoxValue="FALSE";
    return *this;

}
ConfigurationParameterDefinition& ConfigurationParameterDefinition::operator=(ConfigurationParameterDefinition&& inst) noexcept
{
    this->parametertype=inst.parametertype;
    this->ConfigurableName=inst.ConfigurableName;
    this->Description=inst.Description;
    this->LowerMultiplicity=inst.LowerMultiplicity;
    this->UpperMultiplicity=inst.UpperMultiplicity;
    this->choosable=inst.choosable;
    this->configtype=inst.configtype;
    this->LineEditValue="";
    this->ComboBoxIndex=0;
    this->ComboBoxValue="";
    this->CheckBoxValue="FALSE";
    this->is_Symbolic=inst.is_Symbolic;

    if(parametertype==boolean)
    {
        ChoosenValue="FALSE";
    }
    else if(parametertype==Uint)
    {
        ChoosenValue="0";
    }
    else
    {
        ChoosenValue=choosable.at(0);
    }

    return *this;
}

ConfigurationParameterDefinition::ConfigurationParameterDefinition()
{

}

ConfigurationParameterDefinition::ConfigurationParameterDefinition(const ConfigurationParameterDefinition &inst)
{
    this->parametertype=inst.parametertype;
    this->ConfigurableName=inst.ConfigurableName;
    this->Description=inst.Description;
    this->LowerMultiplicity=inst.LowerMultiplicity;
    this->UpperMultiplicity=inst.UpperMultiplicity;
    this->choosable=inst.choosable;
    this->configtype=inst.configtype;
    this->LineEditValue="";
    this->ComboBoxIndex=0;
    this->ComboBoxValue="";
    this->CheckBoxValue="FALSE";
    this->is_Symbolic=inst.is_Symbolic;
    if(parametertype==boolean)
    {
        ChoosenValue="FALSE";
    }
    else if(parametertype==Uint||parametertype==Function||parametertype==Float)
    {
        ChoosenValue="0";
    }
    else
    {
        ChoosenValue=choosable.at(0);
    }
}

ConfigurationParameterDefinition::ConfigurationParameterDefinition(const QString &ConfigurableName, ParameterType parametertype, const QStringList &choosable, ConfigType configtype,int Lower,int Upper,bool is_Symbolic) : ConfigurableName(ConfigurableName),
    parametertype(parametertype),
    choosable(choosable),
    configtype(configtype),
    LowerMultiplicity(Lower),
    UpperMultiplicity(Upper)

{
    LineEditValue="";
    ComboBoxIndex=0;
    ComboBoxValue="";
    CheckBoxValue="FALSE";
    this->is_Symbolic=is_Symbolic;
    if(parametertype==boolean)
    {
        ChoosenValue="FALSE";
    }
    else if(parametertype==Uint||parametertype==Function||parametertype==Float)
    {
        ChoosenValue="0";
    }
    else
    {
        ChoosenValue=choosable.at(0);
    }
}
