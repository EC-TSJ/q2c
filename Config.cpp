/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%M% 28/12/2019 )                                    */
/*  <$Válido$>                                           */
/*********************************************************/

#include "Configuration.h"
#include "CommandParser.h"

QT_BEGIN_NAMESPACE

int Config::verbosity = 0;
QString inputFile = QSTRING();
QString outputFile = QSTRING();
bool q2c = true;
bool only_qt5 = false;
QString name = QSTRING();

bool help_option = true;
bool version_option = true;


QT_END_NAMESPACE
