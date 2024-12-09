#ifndef ARXML_READER_H
#define ARXML_READER_H
#include <QtXml>
#include "configurationparameterdefinition.h"
#include "configurationscontainers.h"


class ARXML_Reader
{
    QFile file;
    QDomDocument document;
    QDomElement root;
    QDomElement roottemp;
    ConfigType ARXML_ReadPostBuildVariant(QString tagname);
    QDomNodeList StartRead(QString tagname);
    void ARXML_ReadElements(QString root,ConfigurationsContainers* container,ParameterType parametertype);
    QStringList *ARXML_ContainerENUMS(QDomNode *itemnode);
public:
    ARXML_Reader();
    int ARXML_ReadDocument(QString FileName);
    void ARXML_ReadAUTOSARVersion(QString tagname);
    void ARXML_ReadContainers(QString root);
    void ARXML_FillContainer(QString root,ConfigurationsContainers* container);

};
extern QVector<QStringList>*Types;
extern int ConfigurationParameters;
extern int containercount;
extern QString ECUC_UUID;
extern QString MODULE_SHORTNAME;
extern int NameMaxLength;
extern QString AR_VERSION;
extern QString AR_MARJOR;
extern QString AR_MINOR;
extern QString AR_PATCH;
extern bool MODULE_PreCompile_Supported ;
extern bool MODULE_LinkTime_Supported ;
extern bool MODULE_PostBuild_Supported ;


#endif // ARXML_READER_H
