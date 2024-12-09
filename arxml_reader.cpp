#include "arxml_reader.h"
#include "configurationparameteraggregation.h"
#include "configurationscontainers.h"
int containercount=1;
QString ECUC_UUID;
QString MODULE_SHORTNAME;
int NameMaxLength=0;

bool MODULE_PreCompile_Supported=false ;
bool MODULE_LinkTime_Supported=false ;
bool MODULE_PostBuild_Supported=false ;
QString AR_VERSION;
QString AR_MARJOR;
QString AR_MINOR;
QString AR_PATCH;
void space(int spaces)
{
    for(int i{};i<spaces;i++)
    {
        qDebug()<<"";
    }
}

QDomNodeList ARXML_Reader::StartRead(QString tagname)
{
    QDomNodeList items= this->root.elementsByTagName(tagname);
    qDebug()<<items.at(0).nodeName();
    qDebug()<<"Total items= "<<items.count();
    return items;
}

ARXML_Reader::ARXML_Reader()
{

}
int ConfigurationParameters{};
QVector<QStringList>*Types;
int ARXML_Reader::ARXML_ReadDocument(QString FileName)
{
    file.setFileName(FileName);
    if(!this->file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"failed to open";
        return -1;
    }
    else
    {
        if(!this->document.setContent(&(this->file)))
        {
            qDebug()<<"failed to load document";
            return -1;
        }
    }
    this->root=this->document.firstChildElement();





    QDomElement root = document.documentElement();

    QDomNodeList moduleDefList = root.elementsByTagName("ECUC-MODULE-DEF");
    if (moduleDefList.isEmpty()) {
        qDebug() << "ECUC-MODULE-DEF element not found.";
        return 1;
    }

    // Get the first ECUC-MODULE-DEF element
    QDomElement moduleDefElement = moduleDefList.at(0).toElement();
    QDomNodeList moduleDefnameList=moduleDefElement.elementsByTagName("SHORT-NAME");
    QDomElement moduleDefnameElement = moduleDefnameList.at(0).toElement();
    // Get the value of the UUID attribute
    QString uuidValue = moduleDefElement.attribute("UUID");
    QString shortname=moduleDefnameElement.text();
    QDomNodeList SUPPORTED_CONFIG_VARIANTS=this->root.elementsByTagName("SUPPORTED-CONFIG-VARIANT");
    int SUPPORTED_CONFIG_VARIANTS_COUNT=SUPPORTED_CONFIG_VARIANTS.count();
    for(int i{};i<SUPPORTED_CONFIG_VARIANTS_COUNT;i++)
    {
        QDomElement SUPPORTED_CONFIG_VARIANTS_ELEMENT=SUPPORTED_CONFIG_VARIANTS.at(i).toElement();
        QString value=SUPPORTED_CONFIG_VARIANTS_ELEMENT.text();
        if(value=="VARIANT-POST-BUILD")
        {
            MODULE_PostBuild_Supported=true;
        }
        if(value=="VARIANT-PRE-COMPILE")
        {
            MODULE_PreCompile_Supported=true;
        }
        if(value=="VARIANT-LINK-TIME")
        {
            MODULE_LinkTime_Supported=true;
        }

    }
    QDomNodeList ARversion=moduleDefElement.elementsByTagName("REVISION-LABEL");
    QDomElement ARversionElement = ARversion.at(0).toElement();
    AR_VERSION=ARversionElement.text();

    QStringList parts = AR_VERSION.split(".");
    AR_MARJOR=parts[0];
    AR_MINOR=parts[1];
    AR_PATCH=parts[2];
    qDebug() << "UUID attribute value:" << uuidValue;
    qDebug() << "SHORTNAME:" << shortname;
    qDebug() << "REVISION-LABEL:" << AR_VERSION;
    ECUC_UUID=uuidValue;
    MODULE_SHORTNAME=shortname;
    return 0;
}

void ARXML_Reader::ARXML_ReadAUTOSARVersion(QString tagname)
{
    QDomNodeList items= StartRead(tagname);
    {
        for(int i=0;i<items.count();i++)
        {
            QDomNode itemnode=items.at(i);
            if(itemnode.isElement())
            {
                QDomElement itemele=itemnode.toElement();
                qDebug()<<itemele.tagName()<<" "<<itemele.text();
            }
        }
    }
}
void ARXML_Reader::ARXML_ReadContainers(QString Root)
{
    QDomNodeList items= StartRead(Root);
    Container->setStartTag(items.at(0).toElement());
    QDomElement ContainerHead=items.at(0).firstChildElement();
    QList<ConfigurationsContainers*> *temp =new QList<ConfigurationsContainers*> ;
    while(ContainerHead.isElement())
    {
        /*Calculate number of containers*/
        containercount++;
        /*Temp object for the container*/
        ConfigurationsContainers * SUB=new ConfigurationsContainers();
        /*10-DEVELOPMENT SPECIFIC: StartTag*/
        SUB->setStartTag(ContainerHead);
        QString debugger=ContainerHead.text();

        /****************************************************
         *              related to UUID(1)                  *
         ***************************************************/
        QDomNode NODE_ContainerDef = root.firstChildElement("ECUC-PARAM-CONF-CONTAINER-DEF");
        QDomElement ELEMEMT_ContainerDef=NODE_ContainerDef.toElement();
        QString UUID=ELEMEMT_ContainerDef.attribute("UUID");
        /****************************************************
         *           related to  Container Name(2)          *
         ***************************************************/
        QDomElement HeadTemp=ContainerHead.firstChildElement();
        /****************************************************
         *              related to DESC (3)                 *
         ***************************************************/
        QDomNode DESC=ContainerHead.firstChildElement("DESC");
        QDomNode DESSCL2=DESC.firstChildElement("L-2");
        QDomElement DESCelement=DESSCL2.toElement();
        QString Desc=DESCelement.text();
        /****************************************************
         *              related to TRACE (4)                *
         ***************************************************/
        QDomNode NODE_TRACE=ContainerHead.firstChildElement("RELATED-TRACE-ITEM-REF");
        QDomElement ELEMENT_TRACE=NODE_TRACE.toElement();
        QString Trace=ELEMENT_TRACE.text();
        /****************************************************
         *              related to Lower Mul (5)            *
         ***************************************************/
        QDomNode MULTIPLICITY=ContainerHead.firstChildElement("LOWER-MULTIPLICITY");
        QDomElement multiplicity=MULTIPLICITY.toElement();
        QString Mul=multiplicity.text();
        int lowermul=Mul.toInt();
        /****************************************************
         *              related to Upper Mul (6)            *
         ***************************************************/
        int uppermul{};
        MULTIPLICITY=ContainerHead.firstChildElement("UPPER-MULTIPLICITY");
        multiplicity=MULTIPLICITY.toElement();
        if(multiplicity.isElement())
        {
            Mul=multiplicity.text();
            uppermul=Mul.toInt();
        }
        else
        {
            uppermul=INT32_MAX;
        }
        /*1-<ECUC-PARAM-CONF-CONTAINER-DEF UUID="UUID">*/
        SUB->UUID=UUID;
        /*2-<SHORT-NAME>ContainerName</SHORT-NAME>*/
        SUB->setContainerName(HeadTemp.text());
        /*3-<DESC>*/
        SUB->Description=Desc;
        /*4-<RELATED-TRACE-ITEM-REF BASE="ArTrace" DEST="TRACEABLE">ECUC_Dio_00152</RELATED-TRACE-ITEM-REF>*/
        SUB->Trace=Trace;
        /*5-<LOWER-MULTIPLICITY>1</LOWER-MULTIPLICITY>*/
        SUB->lowerMul=lowermul;
        /*6-<UPPER-MULTIPLICITY>1</UPPER-MULTIPLICITY>*/
        SUB->upperMul=uppermul;
        /*8-<PARAMETERS>*/
        ARXML_ReadElements("PARAMETERS",SUB,Uint);
        ARXML_ReadElements("PARAMETERS",SUB,Float);
        ARXML_ReadElements("PARAMETERS",SUB,Function);
        ARXML_ReadElements("PARAMETERS",SUB,enumuration);
        ARXML_ReadElements("PARAMETERS",SUB,boolean);
        /*9-DEVELOPMENT SPECIFIC: is array or not
        * if upper multiplicity is more than lower by more than one then it's array
        * if upper multiplicity is one and lower is zero so this container may exist by one or not exist
        * if upper=lower (1,1) so singelton obligatory
        */
        if((uppermul-1)>lowermul)
        {
            SUB->Container_bool_IsArray=true;
        }
        else
        {
            SUB->Container_bool_IsArray=false;
        }
        /*16-DEVELOPMENT SPECIFIC: Base or instance*/
        SUB->Container_bool_IsBase=true;
        temp->push_back(SUB);
        ContainerHead=ContainerHead.nextSiblingElement();

    }
    Container->setSubContainer(temp);
}


void ARXML_Reader::ARXML_FillContainer(QString root,ConfigurationsContainers* container)
{

    QDomNodeList items= container->getStartTag().elementsByTagName(root);
    QDomElement ContainerHead=items.at(0).firstChildElement();
    QString debugger=ContainerHead.text();
    QList<ConfigurationsContainers*> *temp =new QList<ConfigurationsContainers*> ;
    while(ContainerHead.isElement())
    {
        /*Calculate number of containers*/
        containercount++;
        /*temp object */
        ConfigurationsContainers * SUB=new ConfigurationsContainers();
        SUB->Container_bool_IsArray=false;
        SUB->Container_bool_IsBase=false;
        /*10-DEVELOPMENT SPECIFIC: StartTag*/
        SUB->setStartTag(ContainerHead);

        /****************************************************
         *              related to UUID(1)                  *
         ***************************************************/
        QDomNode NODE_ContainerDef = ContainerHead.firstChildElement("ECUC-PARAM-CONF-CONTAINER-DEF");
        QDomElement ELEMEMT_ContainerDef=NODE_ContainerDef.toElement();
        QString UUID=ELEMEMT_ContainerDef.attribute("UUID");
        /****************************************************
         *           related to  Container Name(2)          *
         ***************************************************/
        QDomElement HeadTemp=ContainerHead.firstChildElement();
        /****************************************************
         *              related to DESC (3)                 *
         ***************************************************/
        QDomNode DESC=ContainerHead.firstChildElement("DESC");
        QDomNode DESSCL2=DESC.firstChildElement("L-2");
        QDomElement DESCelement=DESSCL2.toElement();
        QString Desc=DESCelement.text();
        /****************************************************
         *              related to TRACE (4)                *
         ***************************************************/
        QDomNode NODE_TRACE=ContainerHead.firstChildElement("RELATED-TRACE-ITEM-REF");
        QDomElement ELEMENT_TRACE=NODE_TRACE.toElement();
        QString Trace=ELEMENT_TRACE.text();
        /****************************************************
         *              related to Lower Mul (5)            *
         ***************************************************/
        QDomNode MULTIPLICITY=ContainerHead.firstChildElement("LOWER-MULTIPLICITY");
        QDomElement multiplicity=MULTIPLICITY.toElement();
        QString Mul=multiplicity.text();
        int lowermul=Mul.toInt();
        /****************************************************
         *              related to Upper Mul (6)            *
         ***************************************************/
        int uppermul{};
        MULTIPLICITY=ContainerHead.firstChildElement("UPPER-MULTIPLICITY");
        multiplicity=MULTIPLICITY.toElement();
        if(multiplicity.isElement())
        {
            Mul=multiplicity.text();
            uppermul=Mul.toInt();
        }
        else
        {
            uppermul=INT32_MAX;
        }


        /*1-<ECUC-PARAM-CONF-CONTAINER-DEF UUID="UUID">*/
        SUB->UUID=UUID;
        /*2-<SHORT-NAME>ContainerName</SHORT-NAME>*/
        SUB->setContainerName(HeadTemp.text());
        /*3-<DESC>*/
        SUB->Description=Desc;
        /*4-<RELATED-TRACE-ITEM-REF BASE="ArTrace" DEST="TRACEABLE">ECUC_Dio_00152</RELATED-TRACE-ITEM-REF>*/
        SUB->Trace=Trace;
        /*5-<LOWER-MULTIPLICITY>1</LOWER-MULTIPLICITY>*/
        SUB->lowerMul=lowermul;
        /*6-<UPPER-MULTIPLICITY>1</UPPER-MULTIPLICITY>*/
        SUB->upperMul=uppermul;
        /*8-<PARAMETERS>*/
        ARXML_FillContainer("SUB-CONTAINERS",SUB);
        ARXML_ReadElements("PARAMETERS",SUB,Uint);
        ARXML_ReadElements("PARAMETERS",SUB,Function);
        ARXML_ReadElements("PARAMETERS",SUB,enumuration);
        ARXML_ReadElements("PARAMETERS",SUB,Float);
        ARXML_ReadElements("PARAMETERS",SUB,boolean);

        /*9-DEVELOPMENT SPECIFIC: is array or not
        * if upper multiplicity is more than lower by more than one then it's array
        * if upper multiplicity is one and lower is zero so this container may exist by one or not exist
        * if upper=lower (1,1) so singelton obligatory
        */
        if((uppermul-1)>lowermul)
        {
            SUB->Container_bool_IsArray=true;
        }
        else
        {
            SUB->Container_bool_IsArray=false;
        }
        /*16-DEVELOPMENT SPECIFIC: Base or instance*/
        SUB->Container_bool_IsBase=true;
        if(SUB->Container_bool_IsArray)
        {
            for(int i{};i<SUB->lowerMul;i++)
            {
                ConfigurationsContainers * INSTANCE=new ConfigurationsContainers(*SUB);
                INSTANCE->Container_bool_IsBase=false;
                SUB->Instances->push_back(INSTANCE);
            }
        }
        temp->push_back(SUB);
        ContainerHead=ContainerHead.nextSiblingElement();
    }
    container->setSubContainer(temp);
}
//CORRECT
QStringList* ARXML_Reader::ARXML_ContainerENUMS(QDomNode* itemnode)
{
    QStringList*InternaTypes=new QStringList;
    //expected to recieve root name "Literals" at specific parameter is this parameter is from type enum param
    QDomElement parameters=itemnode->toElement();
    qDebug()<<parameters.tagName();//ECUC-ENUMERATION-PARAM-DEF


    QDomNodeList items=parameters.elementsByTagName("ECUC-ENUMERATION-LITERAL-DEF");
    for(int i=0;i<items.count();i++)
    {
        QDomNode itemnode=items.at(i);//ECUC-ENUMERATION-LITERAL-DEF
        //convert node to element
        if(itemnode.isElement())
        {
            QDomNode shortname=itemnode.firstChild();//SHORT-NAME
            QDomElement itemele=shortname.toElement();//convert to element
            qDebug()<<itemele.text();
            InternaTypes->push_back(itemele.text());
            space(4);
        }
    }
    return InternaTypes;
}


void ARXML_Reader::ARXML_ReadElements(QString root, ConfigurationsContainers *container, ParameterType parametertype)
{
    //childrens to parameters
    QDomElement parameters=container->getStartTag().firstChildElement(root);
    //check if this is a valid parameter not null (base condition)
    if(!(parameters.isElement()))
    {
        return;
    }
    QDomNodeList items;
    if(parametertype==boolean)
    {
        items=parameters.elementsByTagName("ECUC-BOOLEAN-PARAM-DEF");
    }
    else if(parametertype==enumuration)
    {
        items=parameters.elementsByTagName("ECUC-ENUMERATION-PARAM-DEF");
    }
    else if (parametertype==Uint)
    {
        items=parameters.elementsByTagName("ECUC-INTEGER-PARAM-DEF");
    }
    else if(parametertype==Float)
    {
        items=parameters.elementsByTagName("ECUC-FLOAT-PARAM-DEF");
    }
    else if(parametertype==Function)
    {
        items=parameters.elementsByTagName("ECUC-FUNCTION-NAME-DEF");
    }



    //Loop for parameters in this scope
    for(int i=0;i<items.count();i++)
    {
        //current parameter
        QDomNode itemnode=items.at(i);//ECUC-<parameter type>-PARAM-DEF

        /********************************************************************************
         *                          Get the config type                                 *
         ********************************************************************************/
        //variable that will carry the config type: post build or pre compile or link time
        ConfigType configtype;
        //get the tag of post build
        QDomNode POSTBUILD=itemnode.firstChildElement("POST-BUILD-VARIANT-VALUE");
        QDomElement postBuild=POSTBUILD.toElement();
        //if false then it's precompile
        if(postBuild.text()=="false")
        {
            configtype=PreCompileConfig;
        }
        else
        {

            configtype=PostBuildConfig;
            container->IsPostBuilt=true;
        }
        /********************************************************************************
         *                             Get the Variants                                 *
         ********************************************************************************/
        QDomElement ELEMENT_VARIANT_CLASS=itemnode.firstChildElement("VALUE-CONFIG-CLASSES");
        QDomNodeList NODE_VARIANT_CLASS=ELEMENT_VARIANT_CLASS.elementsByTagName("ECUC-VALUE-CONFIGURATION-CLASS");
        configtype=PreCompileConfig;
        for(int i{};i<NODE_VARIANT_CLASS.count();i++)
        {
            QDomNode NODE_VARIANT_CLASS_ITEM =NODE_VARIANT_CLASS.at(i);
            QDomNode NODE_CONFIG_CLASS=NODE_VARIANT_CLASS_ITEM.firstChildElement("CONFIG-CLASS");
            QDomElement ELEMENT_CONFIG_CLASS=NODE_CONFIG_CLASS.toElement();
            if(ELEMENT_CONFIG_CLASS.text()=="POST-BUILD")
            {
                configtype=PostBuildConfig;
                i=NODE_VARIANT_CLASS.count();
                break;
            }
            else if(ELEMENT_CONFIG_CLASS.text()=="LINK")
            {
                configtype=LinkTimeConfig;
            }
            else
            {
                if(configtype==LinkTimeConfig)
                    continue;
                configtype=PreCompileConfig;
            }

        }

        /********************************************************************************
         *                          Get the Multiplicities                              *
         ********************************************************************************/
        //check the upper and lower multiplicity
        QDomNode MULTIPLICITY=itemnode.firstChildElement("LOWER-MULTIPLICITY");
        QDomElement multiplicity=MULTIPLICITY.toElement();
        QString Mul=multiplicity.text();
        int lowermul=Mul.toInt();
        int uppermul{};
        MULTIPLICITY=itemnode.firstChildElement("UPPER-MULTIPLICITY");
        multiplicity=MULTIPLICITY.toElement();
        if(multiplicity.isElement())
        {
            Mul=multiplicity.text();
            uppermul=Mul.toInt();

        }
        else
        {
            uppermul=INT32_MAX;
        }
        QDomNode isSymoblic=itemnode.firstChildElement("SYMBOLIC-NAME-VALUE");
        QDomElement issymoblic=isSymoblic.toElement();
        QString symbolic=issymoblic.text();
        bool symb;
        if(symbolic=="true")
        {
            symb=true;
        }
        else
        {
            symb=false;
        }


        /*
         *
         *   <DESC>
         *      <L-2 L="EN">Port pin mode from mode list for use with Port_Init() function.</L-2>
         *   </DESC>
        */
        QDomNode DESC=itemnode.firstChildElement("DESC");
        QDomNode DESSCL2=DESC.firstChildElement("L-2");
        QDomElement DESCelement=DESSCL2.toElement();
        QString Desc=DESCelement.text();
        /********************************************************************************
         *                          finalize the parameter                              *
         ********************************************************************************/
        //convert node to element
        if(itemnode.isElement())
        {
            QDomNode shortname=itemnode.firstChild();//SHORT-NAME
            QDomElement itemele=shortname.toElement();//convert to element
            qDebug()<<itemele.text();

            if(NameMaxLength<itemele.text().length())
            {
                NameMaxLength=itemele.text().length();
            }

            ConfigurationParameterDefinition *temp2=new ConfigurationParameterDefinition(itemele.text(),parametertype,*ARXML_ContainerENUMS(&itemnode),configtype,lowermul,uppermul,symb);
            temp2->Description=Desc;
            ConfigurationParameterGUI *temp3=new ConfigurationParameterGUI(itemele.text(),temp2->getChoosable(),Desc);
            ConfigurationParameterAggregation *temp=new ConfigurationParameterAggregation(*temp2,*temp3);
            container->getParameters()->push_back(temp);

        }
    }
    ConfigurationParameters++;
}
