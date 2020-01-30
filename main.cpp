/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%X% 29/12/2019 ) Impresión con qInfo                */
/*  (%M% 28/12/2019 )                                    */
/*  <$Válido$>                                           */
/*********************************************************/



#include "_defs.h"
#include "main.h"
#include "generic.h"
#include <QCoreApplication>
#include <iostream>
#include <QDir>
#include "Config.h"
#include "project.h"
#include "logs.h"
#include "CommandParser.h"

#if V__MFUNC__
# if V__PRINT__
#   include <QtDebug>
# else
#   ifdef Q_OS_WIN
#     include <stdio.h>
#   endif
# endif
#endif


QT_BEGIN_NAMESPACE


/**
 * @brief DetectInput. Scan for input file and return true if it finds some
 * @return true ó false
 */
bool DetectInput() {
  QDir d(QDir::currentPath());
  QStringList files = d.entryList();
  int x = 0;
  bool found = false;
  while (x < files.count()) {
      QString filename = files.at(x);
      filename.toLower();
      if (filename.endsWith(".pro")) {
          if (found) {
              // more pro files exist, let's print error and quit
              Logs::errorLog("Hay varios fichero .pro en este directorio, necesitas proporcionar uno explícitamente ");
              Logs::errorLog("el proyecto que quieres convertir. mira --help para más.");
              return false;
          }
          found = true;
          Generic::Config::inputFile = files.at(x);
      }
      x++;
  }
  return found;
}

/**
 * @brief _parserInput.
 */
APX_SETTINGS(_parserInput, parser, params) {
  Q_UNUSED(parser)
  Generic::Config::inputFile = params.at(0);
  return 0;
}

/**
 * @brief _parserOutput
 */
APX_SETTINGS(_parserOutput, parser, params) {
  Q_UNUSED(parser)
  Generic::Config::outputFile = params.at(0);
  return 0;
}

/**
 * @brief _parserQt5
 */
APX_SETTINGS(_parserQt5, parser, params) {
  Q_UNUSED(params)
  Q_UNUSED(parser)
  Generic::Config::only_qt5 = true;
  return 0;
}

/**
 * @brief _parserVerbosity
 */
APX_SETTINGS(_parserVerbosity, parser, params) {
  Q_UNUSED(params)
  Q_UNUSED(parser)
  Generic::Config::verbosity++;
  return 0;
}

/**
 * @brief _parserName
 */
APX_SETTINGS(_parserName, parser, params) {
  Q_UNUSED(params)
  Q_UNUSED(parser)
  Generic::Config::name = params.at(0);
  Generic::Config::mustName = true;
  return 0;
}

#if V__MFUNC__

APX_SETTINGS(_parserClown, parser, params) {
  Q_UNUSED(params)
  Q_UNUSED(parser)
}


void myFunction(bool f, QString sz, TP_Callback cb) {
#if !V__PRINT__
  fprintf(stdout, "%b | %s | %p\n",f, sz.toStdString().c_str(), cb);
#else
  qInfo().noquote() << f << " | " << sz.toStdString().c_str() << " | " << cb  << endl;
#endif
 }

#endif
/**
 * @brief runQ2c. Ejecuta el programa
 * @param argc Nº de argumentos
 * @param argv. Argumentos
 * @return 0
 */
int runQ2c(int argc, char *argv[]) {
  QCoreApplication::setApplicationName("q2c");

  CommandParser *tp = new CommandParser();

  tp->addOption('v', "verbose", "Incrementa verbosidad", "(Default off, 1er lugar)", 0, (TP_Callback)_parserVerbosity,VN);
  tp->addOption('o', "out", "Especifica fichero de salida", QString(), 1, (TP_Callback)_parserOutput, VN);
  tp->addOption('i', "in", "Especifica fichero de entrada", QString(), 1, (TP_Callback)_parserInput, VN);
#if V__MFUNC__
  tp->addOption('5', "qt5", "Soporta sólo Qt5", QString(), 0, (TP_Callback)_parserQt5, (TP_Callback)_parserClown);
#else
  tp->addOption('5', "qt5", "Soporta sólo Qt5", QString(), 0, (TP_Callback)_parserQt5, VN);
#endif
  tp->addOption('n', "name", "Nombre del proyecto", "(Default name)", 1, (TP_Callback)_parserName,VN);

#if V__MFUNC__

  tp->addPositional("Gilipollas", "lo que haga", QString(), VN, VN);
  tp->addPositional("Oculto", "lo que haga", QString(), VN, VN);
  tp->addPositional("eleven", "lo que haga", "(15, 20, 25. Default 20)", VN, VN);

#endif

  if (!tp->parse(argc, argv)) {
    // Parameter require to exit (--help) etc
    delete tp;
    return 0;
  }

#if V__MFUNC__

  QStringList qsl;
  qsl.append("");
  tp->runOption('5', qsl);
  tp->runOptionEx('5', qsl);
  tp->run(myFunction);
  bool f = tp->exist('v');
  f = tp->exist("Gilipollas");
  f = tp->exist("out");

#endif

  delete tp;

  // a)
  if ( (Generic::Config::mustName) && (Generic::Config::name == QSTRING()) ) {
      Logs::debugLog("Debe ser puesto nombre en la opción ' --name NAME '");
      return 7;
     }

  // c)
  if (Generic::Config::inputFile == QSTRING())    {
    // user didn't provide any input file
    if (!DetectInput()) {
        return 2;
    }
    Logs::debugLog("Resuelto nombre de entrada a " + Generic::Config::inputFile);
  }

  // d)
  if (Generic::Config::outputFile == QSTRING())  {
    // user didn't provide output file name
    // we can simply reuse the original name
    if (!Generic::Config::inputFile.contains(".")) {
        Logs::errorLog("El fichero de entrada no pede ser convertido a fichero de salida, por favor proporciona fichero de salida");
        return 3;
    }
    Generic::Config::outputFile = Generic::Config::inputFile.mid(0, Generic::Config::inputFile.indexOf("."));
    if (Generic::Config::q2c) {
        Generic::Config::outputFile = "CMakeLists.txt";
    }
    else {
        Generic::Config::outputFile += ".pro";
    }
    Logs::debugLog("Resuelto nombre de salida a " + Generic::Config::outputFile);
  }

  // e) Load the project file
  QFile *file = new QFile(Generic::Config::inputFile);
  if (!file->open(QIODevice::ReadOnly)) {
      Logs::errorLog("Incapaz de leer: " + Generic::Config::inputFile);
      delete file;
      return 4;
  }

  // f) lee todo
  QString source = QString(file->readAll());
  delete file;

  // g) Parsea el QMake
  Project *input = new Project();
  if (!input->parseQMake( source )) {
    Logs::errorLog("Incapaz de parsear: " + Generic::Config::inputFile);
    return 5;
  }

  // h)
  file = new QFile(Generic::Config::outputFile);
  if (!file->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
    Logs::errorLog("Incapaz de abrir para escritura: " + Generic::Config::outputFile);
    delete file;
    return 6;
  }

  // i) escribe el CMake
  if (Generic::Config::q2c) {
    file->write(input->toCMake().toUtf8());
  }
  delete file;
  delete input;
  return 0;
}

QT_END_NAMESPACE

#if V__MHANDLER__
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);
void myCustomMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  // Handle the messages!
  QString txt;
  switch (type) {
  case QtDebugMsg:
      txt = QString("Debug: %1").arg(msg);
      break;
  case QtWarningMsg:
      txt = QString("Warning: %1").arg(msg);
      break;
  case QtCriticalMsg:
      txt = QString("Critical: %1").arg(msg);
      break;
  case QtFatalMsg:
      txt = QString("Fatal: %1").arg(msg);
      break;
  case QtInfoMsg:
      for(QChar x: msg)
        if(x == "\"")
            continue;
        else
          txt = txt + x;
  }
  //msg = txt;
  // Call the default handler.
  (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, txt);
}
#endif
/**
 * @brief main.Programa
 * @param argc. Nº de argumentos
 * @param argv. Argumentos
 * @return 0 ó 1
 */
int main(int argc, char *argv[]) {
#if V__MHANDLER__
  qInstallMessageHandler(myCustomMessageHandler);
#endif
  return QT_PREPEND_NAMESPACE(runQ2c)(argc, argv);
}

