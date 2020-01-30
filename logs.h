/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%M% 28/12/2019 )                                    */
/*  <$Válido$>                                           */
/*********************************************************/

#ifndef LOGS_H
# define LOGS_H

#include <iostream>
#include "Configuration.h"


namespace Logs {
  void log(QString text);
  void errorLog(QString text);
  void warningLog(QString text);
  void debugLog(QString text, int verbosity = 1);
}

#endif // LOGS_H
