#include "configurationparameteraggregation.h"

ConfigurationParameterDefinition ConfigurationParameterAggregation::getParameterDef() const
{
    return *parameterDef;
}

void ConfigurationParameterAggregation::setParameterDef( ConfigurationParameterDefinition &newParameterDef)
{
    parameterDef = &newParameterDef;
}

ConfigurationParameterGUI ConfigurationParameterAggregation::getParamGui() const
{
    return *paramGui;
}

void ConfigurationParameterAggregation::setParamGui( ConfigurationParameterGUI &newParamGui)
{
    paramGui = &newParamGui;
}

ConfigurationParameterAggregation &ConfigurationParameterAggregation::operator=(const ConfigurationParameterAggregation &inst)
{
    this->paramGui=new ConfigurationParameterGUI{*inst.paramGui};
    this->parameterDef=new ConfigurationParameterDefinition{*inst.parameterDef};
    return *this;
}
ConfigurationParameterAggregation&ConfigurationParameterAggregation:: operator=(ConfigurationParameterAggregation&& inst)noexcept
{
    this->paramGui=new ConfigurationParameterGUI{*inst.paramGui};
    this->parameterDef=new ConfigurationParameterDefinition{*inst.parameterDef};
    return *this;
}



QList<ConfigurationParameterAggregation>* Class_ConfigurationParameterAggregation_Instance;

ConfigurationParameterAggregation::ConfigurationParameterAggregation(const ConfigurationParameterAggregation &inst)
{
    this->paramGui=new ConfigurationParameterGUI{*inst.paramGui};
    this->parameterDef=new ConfigurationParameterDefinition{*inst.parameterDef};
}

ConfigurationParameterAggregation::ConfigurationParameterAggregation(const ConfigurationParameterDefinition &parameterDef, const ConfigurationParameterGUI &paramGui)
{
    this->paramGui=new ConfigurationParameterGUI{paramGui};
    this->parameterDef=new ConfigurationParameterDefinition{parameterDef};
}
