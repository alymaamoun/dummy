#include "configurationscontainers.h"

ConfigurationsContainers * Container;
int ConfigurationsContainers::ID=0;
QVector<int> CurrentMultiplicity;
QVector<int> instance;
QList<ConfigurationsContainers*> *ConfigurationsContainers::getSubContainer()
{
    return SubContainer;
}

void ConfigurationsContainers::setSubContainer(QList<ConfigurationsContainers*> *newSubContainer)
{
    SubContainer = newSubContainer;
}

QList<ConfigurationParameterAggregation*> *ConfigurationsContainers::getParameters()
{
    return parameters;
}

void ConfigurationsContainers::setParameters(QList<ConfigurationParameterAggregation*> *newParameters)
{
    parameters = newParameters;
}

QString ConfigurationsContainers::getContainerName()
{
    return ContainerName;
}

void ConfigurationsContainers::setContainerName(const QString &newContainerName)
{
    ShortName="";
    if(upperMul>1)
        ShortName=QString::number(rand());
    ContainerName = newContainerName+ShortName;
}
QDomElement ConfigurationsContainers::getStartTag()
{
    return StartTag;
}

void ConfigurationsContainers::setStartTag(const QDomElement &newStartTag)
{
    StartTag = newStartTag;
}

void ConfigurationsContainers::SetMuls(int lower, int upper)
{
    this->lowerMul=lower;
    this->upperMul=upper;
}

int ConfigurationsContainers::GetLowerMul()
{
    return this->lowerMul;
}

int ConfigurationsContainers::GetUpperMul()
{
    return this->upperMul;
}

int ConfigurationsContainers::GetCount()
{
    return this->count;
}

void ConfigurationsContainers::SetCount(int count)
{
    this->count=count;
}



ConfigurationsContainers::ConfigurationsContainers()
{
    this->id=ID;
    CurrentMultiplicity.push_back(1);
    instance.push_back(1);
    ID++;
    this->parameters=new QList<ConfigurationParameterAggregation*>;
    this->SubContainer=new QList<ConfigurationsContainers*>;
    this->Instances=new QList<ConfigurationsContainers*>;
    this->count=1;
    //this->ShortName=QString::number(id)+"_"+QString::number(instance.at(id));
    this->ContainerName+=this->ShortName;



}

ConfigurationsContainers::ConfigurationsContainers(const ConfigurationsContainers &inst)
{

    this->SubContainer = new QList<ConfigurationsContainers*>;
    for (auto sub : *inst.SubContainer) {
        this->SubContainer->append(new ConfigurationsContainers(*sub));
    }
    this->Container_bool_IsArray=inst.Container_bool_IsArray;
    this->Container_bool_IsBase=inst.Container_bool_IsBase;
    this->Description=inst.Description;
    this->Trace=inst.UUID;
    this->Trace=inst.Trace;
    this->id=inst.id;
    this->IsPostBuilt=inst.IsPostBuilt;
    this->parameters = new QList<ConfigurationParameterAggregation*>;
    for (auto param : *inst.parameters) {
        this->parameters->append(new ConfigurationParameterAggregation(*param));
    }
    this->Instances=new QList<ConfigurationsContainers*>;
    this->upperMul=inst.upperMul;
    this->lowerMul=inst.lowerMul;
    //this->ShortName=QString::number(this->id)+"_"+QString::number(instance.at(this->id));




    this->ContainerName=inst.ContainerName;
    this->ContainerName.replace(inst.ShortName,"");
    this->ContainerName+=this->ShortName;
    this->StartTag=inst.StartTag;


    instance[this->id]++;
    this->count=inst.count+1;





}


