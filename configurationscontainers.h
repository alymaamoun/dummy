#ifndef CONFIGURATIONSCONTAINERS_H
#define CONFIGURATIONSCONTAINERS_H
#include "configurationparameteraggregation.h"
#include <QPushButton>
extern QVector<int> CurrentMultiplicity;
extern QVector<int> instance;
class ConfigurationsContainers
{

    /*1-<ECUC-PARAM-CONF-CONTAINER-DEF UUID="UUID">*/
    QString UUID;
    /*2-<SHORT-NAME>ContainerName</SHORT-NAME>*/
    QString ContainerName;
    /*3-<DESC>*/
    QString Description;
    /*4-<RELATED-TRACE-ITEM-REF BASE="ArTrace" DEST="TRACEABLE">ECUC_Dio_00152</RELATED-TRACE-ITEM-REF>*/
    QString Trace;
    /*5-<LOWER-MULTIPLICITY>1</LOWER-MULTIPLICITY>*/
    int lowerMul{};
    /*6-<UPPER-MULTIPLICITY>1</UPPER-MULTIPLICITY>*/
    int upperMul{};
    /*7-<SUB-CONTAINERS>*/
    QList<ConfigurationsContainers*> *SubContainer;
    /*8-<PARAMETERS>*/
    QList<ConfigurationParameterAggregation*> *parameters;
    /*9-DEVELOPMENT SPECIFIC: is array or not
    * if upper multiplicity is more than lower by more than one then it's array
    * if upper multiplicity is one and lower is zero so this container may exist by one or not exist
    * if upper=lower (1,1) so singelton obligatory
    */
    bool Container_bool_IsArray=false;
    /*10-DEVELOPMENT SPECIFIC: StartTag*/
    QDomElement StartTag;
    /*11-DEVELOPMENT SPECIFIC: id of this container*/
    int id;
    /*12-DEVELOPMENT SPECIFIC: Global ID */
    static int ID;
    /*13-DEVELOPMENT SPECIFIC: count of this container maybe there's 5 instances of the same container*/
    int count{};
    /*14-DEVELOPMENT SPECIFIC: this container has any postbuild parameter or not*/
    bool IsPostBuilt=false;
    /*15-DEVELOPMENT SPECIFIC: short name of the container will be used if it has parameter has symbolic name*/
    QString ShortName;
    /*16-DEVELOPMENT SPECIFIC: Base or instance*/
    bool Container_bool_IsBase=false;
    /*17-INSTANCES-CONTAINERS*/
    QList<ConfigurationsContainers*> *Instances;
public:




    ConfigurationsContainers();
    ConfigurationsContainers(const ConfigurationsContainers&inst);

    QList<ConfigurationsContainers*> *getSubContainer();
    void setSubContainer(QList<ConfigurationsContainers*> *newSubContainer);
    QList<ConfigurationParameterAggregation*> *getParameters() ;
    void setParameters(QList<ConfigurationParameterAggregation*> *newParameters);
    QString getContainerName() ;
    void setContainerName(const QString &newContainerName);
    QDomElement getStartTag() ;
    void setStartTag(const QDomElement &newStartTag);
    void SetMuls(int lower,int upper);
    int GetLowerMul();
    int GetUpperMul();
    int GetCount();
    void SetCount(int count);
    friend class Form;
    friend class ARXML_Reader;

};
extern ConfigurationsContainers * Container;
#endif // CONFIGURATIONSCONTAINERS_H
