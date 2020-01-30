/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%X% 29/12/2019 ) Impresión con qInfo                */
/*  (%M% 28/12/2019 )                                    */
/*  <$Válido$>                                           */
/*********************************************************/

#include "logs.h"
#include "generic.h"
#include "main.h"
#include "_defs.h"


#if V__PRINT__
# include <QtDebug>
#else
# ifdef Q_OS_WIN
#  include <stdio.h>
# endif
#endif

QT_BEGIN_NAMESPACE

/**
 * @brief Logs::log Imprime el log
 * @param text
 */
void Logs::log(QString text) {
#if !V__PRINT__
  fprintf(stdout, "%s\n", qPrintable(text));
#else
  qInfo().noquote() << text;
#endif
}

/**
 * @brief Logs::errorLog Imprime el log para el caso de error
 * @param text
 */
void Logs::errorLog(QString text) {
#if !V__PRINT__
  fprintf(stderr, "%s\n", qPrintable(text));
#else
  qWarning().noquote() << text;
#endif
}

/**
 * @brief Logs::warningLog Imprime el log para el caso de warning
 * @param text
 */
void Logs::warningLog(QString text) {
#if !V__PRINT__
  fprintf(stderr, "%s\n", qPrintable(text));
#else
  qWarning().noquote() << text;
#endif
}

/**
 * @brief Logs::debugLog Imprime el log para el caso de debug
 * @param text
 * @param verbosity
 */
void Logs::debugLog(QString text, int verbosity) {
    if (verbosity <= Generic::Config::verbosity) {
        log(text);
    }
}


QT_END_NAMESPACE
