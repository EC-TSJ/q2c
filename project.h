/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%M% 28/12/2019 )                                    */
/*  <$Válido$>                                           */
/*********************************************************/

#ifndef PROJECT_H
# define PROJECT_H

#include <QString>
#include <QList>
#include <QHash>
#include <QDateTime>
#include "logs.h"

#define V_CR          '\r'
#define V_WHITE       " "
#define V_EMPTY       ""
#define V_EQUAL       "="
#define V_PLUSEQUAL   "+="
#define V_MINUSEQUAL  "-="
#define V_LEFT        "\\"

class Project {
public:
  bool parseQMake(QString text);
  QString toCMake();
private:
  QList<QString> _sources;
  QList<QString> _headers;
  QList<QString> _modules;
  QList<QString> _forms;
  QList<QString> _resources;
  QList<QString> _vanDerBrook;
  void _updateResources(QString);
  void _updateSources(QString);
  void _updateHeaders(QString);
  void _updateForms(QString);
  void _updateQt(QString);
  void (Project::*pSet)(QString);
};

#endif // PROJECT_H
