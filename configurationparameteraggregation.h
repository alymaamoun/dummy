

#ifndef CONFIGURATIONPARAMETERAGGREGATION_H
#define CONFIGURATIONPARAMETERAGGREGATION_H
#include "configurationparameterdefinition.h"
#include "configurationparametergui.h"


class ConfigurationParameterAggregation
{
private:
    ConfigurationParameterDefinition *parameterDef;
    ConfigurationParameterGUI *paramGui;
public:


    ConfigurationParameterAggregation(const ConfigurationParameterAggregation&inst);
    ConfigurationParameterAggregation(const ConfigurationParameterDefinition &parameterDef, const ConfigurationParameterGUI &paramGui);
    ConfigurationParameterDefinition getParameterDef() const;
    void setParameterDef( ConfigurationParameterDefinition &newParameterDef);
    ConfigurationParameterGUI getParamGui() const;
    void setParamGui( ConfigurationParameterGUI &newParamGui);

    ConfigurationParameterAggregation& operator=(const ConfigurationParameterAggregation& val);
    //move assignment operator
    ConfigurationParameterAggregation& operator=(ConfigurationParameterAggregation&& val)noexcept;

    friend class ARXML_Reader;
    friend class ConfigurationsContainers;
    friend class Form;

};
extern QList<ConfigurationParameterAggregation>*Class_ConfigurationParameterAggregation_Instance;
#endif // CONFIGURATIONPARAMETERAGGREGATION_H

