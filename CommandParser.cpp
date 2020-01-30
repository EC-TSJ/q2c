/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%X% 29/12/2019 ) Impresión con qInfo()              */
/*  (%M% 21/01/2020 ) Añadido el segundo TP_Callback     */
/*  (%M% 21/01/2020 ) Añadido runOption                  */
/*  (%M% 21/01/2020 ) Añadido runOptionEx                */
/*  (%M% 21/01/2020 ) Añadido execEx                     */
/*  <$Válido$>                                           */
/*********************************************************/

/*
 * Es una clase (dos, el parser y la línea de opciones) para gestionar las opciones
 * de la línea de comandos.
 *
 * Se distingue dos tipos de opciones: posicionales y no posicionales.
 *
 * P. Ej:  Formato.exe mandarina --valor potatoe -h frijol
 *
 * los no posicionales son: --valor potatoe -h frijol
 * y los posicionales es: mandarina
 *
 */

#include "main.h"
#include "_defs.h"
#include <QCoreApplication>
#include <iostream>
#include "generic.h"
#include "CommandParser.h"
#include "logs.h"

#if V__PRINT__
# include <QtDebug>
#else
# ifdef Q_OS_WIN
#  include <stdio.h>
# endif
#endif

QT_BEGIN_NAMESPACE


#define Q(string) (string).toStdString().c_str()


#if V__QPAIR__



/**
 * @brief _printVersion. Imprime la forma de versión
 */
APX_SETTINGS(_printVersion, parser, params) {
  Q_UNUSED(params)
  Q_UNUSED(parser)

#if !V__PRINT__
   fprintf(stdout, "%s Versión %s", qPrintable(QCoreApplication::applicationName()), OST_VERSION_STR);
#else
  qInfo().noquote() << QCoreApplication::applicationName() << " Versión " << OST_VERSION_STR; // << std::endl;
#endif

  return TP_RESULT_SHUT;
}

/**
 * @brief _printHelp. Imprime la forma de ayuda
 */
APX_SETTINGS(_printHelp, parser, params) {
    Q_UNUSED(params)

    QString names, sz  =  parser->getItems().size() > 0 ? " [options] " : QSTRING() ;
    if (parser->getPositionals().size() > 0)
      for(QPair<CommandLine, QChar> i: parser->getItems()) {
          if(i.second == V_SI_POS)
            names += V_SPACE + i.first.getLongName();
        }
#if !V__PRINT__
    fprintf(stdout, "Uso: %s %s %s\n", qPrintable(QCoreApplication::applicationName()), qPrintable(sz), qPrintable(names));
    fprintf(stdout, "%s versión %s\n\nOpciones:\n", qPrintable(QCoreApplication::applicationName()), OST_VERSION_STR);
#else
    qInfo().noquote() << "Uso: " << QCoreApplication::applicationName() << sz << names;
    qInfo().noquote() << QCoreApplication::applicationName() << " versión " << OST_VERSION_STR << endl
              << endl
              << "Opciones:" ;
#endif

    // first we analyse the list of options and get the size of longest one
    unsigned int longest = 10;

    for(QPair<CommandLine, QChar> i: parser->getItems())  {
        unsigned int is = (unsigned int)i.first.getLongName().size() + 4;
        if (is > longest)
            longest = is;
    }

    unsigned int longest_line = 20;
    QStringList lines_param, lines_help, lines_extra;

    // monta los parámetros
    for(QPair<CommandLine, QChar> i: parser->getItems())  {
        if(i.second == V_SI_POS) // si es posicional -> pasa de el
          continue;

        QString parameters = i.first.getLongName();
        if (!parameters.isEmpty())
          parameters = Generic::Strings::expandedString("--" + parameters, longest, longest + 10);
        else
          parameters = Generic::Strings::expandedString(parameters, longest, longest + 10);

        QString parameters_short;
        if (i.first.getShortName() != '\0')
            parameters_short = "-" + QString(QChar(i.first.getShortName()));
        if (!parameters.isEmpty() && !parameters_short.isEmpty())
            parameters += " | " + parameters_short;
        else if (!parameters_short.isEmpty())
            parameters += parameters_short;
        if (i.first.getParameters() > 0)  {
            if (i.first.getParameters() == 1)
                parameters += " <parámetro requerido>";
            else
                parameters += " <" + QString::number(i.first.getParameters()) + " parámetros requeridos>";
        }

        lines_extra << i.first.getExtra();
        lines_help << i.first.getDescription();
        lines_param << parameters;

        int size = parameters.size() + 2;
        if (size > longest_line)
            longest_line = size;
    }

    // Imprime sucesivamente
    int item = 0;
    while (item < lines_param.size()) {
#if !V__PRINT__
        fprintf(stdout, V_INDENT "%s:" V_SPACE "%s" V_SPACE "%s\n", qPrintable(Generic::expandedString(lines_param[item], longest_line, longest_line + 10)), qPrintable(lines_extra[item]), qPrintable(lines_help[item]));
#else
        qInfo().noquote() << V_SPACE << Generic::Strings::expandedString(lines_param[item], longest_line,
                                 longest_line + 10) << ":" << lines_extra[item] << lines_help[item];
#endif
        item++;
    }

    // averigua e imprime los parámetros posicionales
    if(parser->getItems().size() > 0) {
      //std::cout << "\nPosicionales:\n";
      int n = 0;
      for(QPair<CommandLine, QChar> i: parser->getItems()) {
          if (i.second == V_SI_POS) {
            if (n == 0) {
#if !V__PRINT__
              fprintf(stdout, "\nPosicionales:\n");
#else
              qInfo().noquote() << "\nArgumentos:";
#endif
              n++;
              }
#if !V__PRINT__
            fprintf(stdout, V_INDENT "%s:" V_SPACE "%s" V_SPACE "%s\n", qPrintable(Generic::expandedString(i.first.getLongName(), longest_line, longest_line + 10)), qPrintable(i.first.getExtra()), qPrintable(i.first.getDescription()));
#else
            qInfo().noquote() << V_SPACE << Generic::Strings::expandedString(i.first.getLongName(), longest_line,
                               longest_line + 10) << ":" << i.first.getExtra() << i.first.getDescription();
#endif
            }
        }
    }

    // final
#if !V__PRINT__
    fprintf(stdout, "\n");
    //fprintf(stdout, "This software is open source, contribute at http://github.com/benapetr/q2c\n");
#else
    qInfo().noquote() << endl;
    //qInfo().noquote() << "This software is open source, contribute at http://github.com/benapetr/q2c" << std::endl;
#endif
    return TP_RESULT_SHUT;
}

#if V__QPAIR__

/**
 * @brief CommandParser::addHelpOption
 */
void CommandParser::addHelpOption() {
  Generic::Config::help_option = true;
}

/**
 * @brief CommandParser::addVersionOption
 */
void CommandParser::addVersionOption() {
  Generic::Config::version_option = true;
}

#endif

/**
 * @brief CommandParser::CommandParser. Constructor
 */
CommandParser::CommandParser() {
  Generic::Config *cfg = new Generic::Config();
  if (cfg->help_option == true)
    this->addOption('h', "help", "Información de Ayuda", QSTRING(), 0, (TP_Callback)_printHelp, VN);
  if (cfg->version_option == true)
    this->addOption(0, "version", "Información de Versión", QSTRING(), 0, (TP_Callback)_printVersion, VN);
}

#if V__QPAIR__
/**
 * @brief CommandParser::parserBis. Averigua si existe --verbose ó -v
 * @param argc Nª de Argumentos
 * @param argv Argumentos
 * @return --verbose ó nullptr
 */
char *CommandParser::parseBis(int argc, char **argv ) {
  char *call = nullptr;
  int x = 0;
  // averigua si existe --verbose, si es asi lo hace call
  while (x < argc) {
    QString parm = QString(argv[x]);
    if (parm == "--verbose" || parm == "-v") {
        call = const_cast<char *>(parm.toStdString().c_str());
        break;
      }
      x++;
    }
  return call;
}
#endif

/**
 * @brief CommandParser::parse. Procesa los valores. Posicionales o no
 * @param argc Nº de argumentos
 * @param argv Argumentos
 * @return bool
 */
bool CommandParser::parse(int argc, char **argv) {
    int x = 0;
    int expected_parameters = 0;
    QStringList parameter_buffer;
    CommandLine *item = nullptr;

#if V__QPAIR__
    char *call = this->parseBis(argc, argv);
    if (call != nullptr)
      Generic::Config::verbosity++;

    if (Generic::Config::verbosity)
      Logs::debugLog("\nVerbosity: parse");
#endif
    while (x < argc)    {
      QString parameter = QString(argv[x++]);
      if (expected_parameters > 0)   {
#if V__QPAIR__
       if (Generic::Config::verbosity)
          Logs::debugLog("  Verbosity: " + parameter);
#endif
        // the last argument we processed has some parameters, so we are now processing them into a buffer
        parameter_buffer << parameter;
        expected_parameters--;
        if (expected_parameters == 0)    {
            // execute
            if (item->exec(this, parameter_buffer) == TP_RESULT_SHUT)
            {
              delete item;
              return false;
            }
            parameter_buffer.clear();
        }
        continue;
      }
      if (parameter.length() < 2)  {
#if !V__PRINT__
        fprintf(stderr, "ERROR: parámetro no reconocido: %s\n", qPrintable(parameter));
#else
        qCritical().noquote() << "ERROR (A): parámetro no reconocido: " << qPrintable(parameter) << endl;
#endif
        return false;
      }
      if (parameter.startsWith("--"))  {
#if V__QPAIR__
        if (Generic::Config::verbosity)
          Logs::debugLog("  Verbosity: " + parameter);
#endif
        // It's a word parameter
        delete item;
        item = this->getItem(parameter.mid(2));
        if (item == nullptr)  {
#if !V__PRINT__
          fprintf(stderr, "ERROR: parámetro no reconocido: %s\n", qPrintable(parameter));
#else
          qCritical().noquote() << "ERROR (B): parámetro no reconocido: " << qPrintable(parameter) << endl;
#endif
          delete item;
          return false;
        }
        expected_parameters = item->getParameters();
        this->setItem(parameter.mid(2), true);
        if (expected_parameters)
          continue;
        // let's process this item
        if (item->exec(this, parameter_buffer) == TP_RESULT_SHUT)  {
          delete item;
          return false;
        }
      }
      else if (parameter.startsWith("-"))  {
#if V__QPAIR__
        if (Generic::Config::verbosity)
          Logs::debugLog("  Verbosity: " + parameter);
#endif
        // It's a single character(s), let's process them recursively
        int symbol_px = 0;
        while (parameter.size() > ++symbol_px)  {
          char sx = parameter[symbol_px].toLatin1();
          delete item;
          item = this->getItem(sx);
          if (item == nullptr)  {
#if !V__PRINT__
            fprintf(stderr, "ERROR: parámetro no reconocido: %s\n", qPrintable(QString(sx)));
#else
            qCritical().noquote() << "ERROR (C): parámetro no reconocido: " << qPrintable(QString(sx)) << endl;
#endif
            delete item;
            return false;
          }
          expected_parameters = item->getParameters();
          if (expected_parameters && (symbol_px+1) < parameter.size())  {
#if !V__PRINT__
            fprintf(stderr, "ERROR: no suficientes parámetros para -%s\n", qPrintable(QString(sx)));
#else
            qCritical().noquote() << "ERROR: no suficientes parámetros para -" << qPrintable(QString(sx)) << endl;
#endif
            delete item;
            return false;
          }
          this->setItem(sx, true);
          if (expected_parameters)
            continue;
          // let's process this item
          if (item->exec(this, parameter_buffer) == TP_RESULT_SHUT)    {
            delete item;
            return false;
        }
            // let's process this second item
        }
      }
      else {
#if V__QPAIR__
        if (Generic::Config::verbosity)
          Logs::debugLog("  Verbosity: " + parameter);
#endif
        if (x <= 1)
          continue;

        delete item;
        item = this->getItem(parameter);
        if (item == nullptr)   {
#if !V__PRINT__
          fprintf(stderr, "ERROR: parámetro no reconocido: %s\n", qPrintable(parameter));
#else
          qCritical().noquote() << "ERROR (D): parámetro no reconocido: " << qPrintable(parameter) << endl;
#endif
          delete item;
          return false;
        }
        // let's process this item
        this->setItem(parameter, true);
        if (item->exec(this, parameter_buffer) == TP_RESULT_SHUT)   {
          delete item;
          return false;
        }
      }
  }
  delete item;
  if (expected_parameters)  {
#if !V__PRINT__
        fprintf(stderr, "ERROR: parámetro desconocido. \n");
#else
        qCritical().noquote() << "ERROR: parámetro desconocido. " << endl;
#endif
    return false;
  }
  return true;
}

/**
 * @brief CommandParser::addOption. Añade una opción determinada
 * @param ch. -v
 * @param name. --version
 * @param description. Descripción de la opción
 * @param extra. (Default text)
 * @param parametersRequired. Requerido parámetros
 * @param callb. Callback
 * @param callb_is. Callback
 */
void CommandParser::addOption(QChar ch, QString name, QString description, QString extra, int parametersRequired, TP_Callback callb, TP_Callback callb_is) {
    this->_items.append(qMakePair(CommandLine(ch, name, description, extra, parametersRequired, callb, callb_is), V_NO_POS));
}

/**
 * @brief CommandParser::getOptions. Devuelve una lista de opciones
 * @return QStringList
 */
QStringList CommandParser::getOptions() const {
  QStringList p;
  int n = 0;
  while(n < _items.size()) {
     if(_items[n].second == V_NO_POS) {
         p.append( _items[n].first.getLongName() );
         p.append( _items[n].first.getShortName() );
       }
     n++;
  }

  return p;
}

/**
 * @brief CommandParser::addPositional. Añade un valor posicional
 * @param name. Nombre
 * @param description. Descripción de la opción
 * @param extra. (Default text)
 * @param callback. Callback
 * @param callb_is. Callback
 * @return bool
 */
bool CommandParser::addPositional(QString name, QString description, QString extra, TP_Callback callback, TP_Callback callb_is) {
  this->_items.append(qMakePair(CommandLine(0, name, description, extra, 0, callback, (TP_Callback)callb_is), V_SI_POS));
  return true;
}

/**
 * @brief CommandParser::getPositionals, Devuelve una lista de valores posionales
 * @return QStringList
 */
QStringList CommandParser::getPositionals() const {
  QStringList p;
  int n = 0;
  while(n < _items.size()) {
     if(_items[n].second == V_SI_POS)
       p.append(_items[n].first.getLongName() );
     n++;
  }
  return p;
}

/**
 * @brief CommandParser::getItems. Devuelve una lista de opciones y posicionales
 * @return QList
 */
QList<QPair<CommandLine, QChar>> CommandParser::getItems() const {
    return this->_items;
}

/**
 * @brief CommandParser::setItem. Pone un valor posicional. El _set
 * @param name. Nombre
 * @param value. Valor
 */
void CommandParser::setItem(QString name, bool value) {
  int n = 0;
  for(QPair<CommandLine , QChar> x: this->_items)  {
      if (x.first.getLongName() == name) {
        x.first.setSet(value);
        this->_items.replace(n, x);
      }
      n++;
    }
}

/**
 * @brief CommandParser::setItem. Pone un valor a value. El _set
 * @param name. Nombre
 * @param value. Valor
 */
void CommandParser::setItem(QChar name, bool value) {
  int n = 0;
  for(QPair<CommandLine, QChar> x: this->_items)  {
      if (x.first.getShortName() == name) {
        x.first.setSet(value);
      this->_items.replace(n, x);
      }
      n++;
    }
}

/**
 * @brief CommandParser::getItem. Obtiene un valor estandar
 * @param name. Nombre
 * @return CommandLine
 */
CommandLine *CommandParser::getItem(QString name) const {
  for(QPair<CommandLine, QChar> x: this->_items)  {
      if (x.first.getLongName() == name)
          return new CommandLine(x.first.getShortName(), x.first.getLongName(),
                                 x.first.getDescription(), x.first.getExtra(), x.first.getParameters(), x.first.getCallback(), x.first.getCallbackIs());
  }
  return nullptr;
}

/**
 * @brief CommandParser::getItem. Devuelve un item no posicional
 * @param name. Nombre
 * @return CommandLine
 */
CommandLine *CommandParser::getItem(QChar name) const {
    for(QPair<CommandLine, QChar> x: this->_items)  {
        if (x.first.getShortName() == name)
            return new CommandLine(x.first.getShortName(), x.first.getLongName(),
                                   x.first.getDescription(), x.first.getExtra(), x.first.getParameters(), x.first.getCallback(), x.first.getCallbackIs());
    }
    return nullptr;
}

/**
 * @brief CommandParser::run, Corre automáticamente por todos los parámetros que
 * están actualizados, que tienen su _set a true
 *
 * Firma de calling:
 *   void TP_Callback_is(bool, QString, TP_Callback);
 *
 * @param calling. Callback for run
 */
void CommandParser::run(TP_Callback_is calling) {
  for(QPair<CommandLine, QChar> x: this->_items) {
      // short name, no posicional
      if (x.first.getSet() && x.second == V_NO_POS && x.first.getShortName() != V_NULL)
        calling(false, QString(x.first.getShortName()),  x.first.getCallback());
      // long name. no posicional
      if (x.first.getSet() && x.second == V_NO_POS && x.first.getLongName() != QSTRING())
        calling(false, x.first.getLongName(), x.first.getCallback());
      // posicional
      if (x.first.getSet() && x.second == V_SI_POS && x.first.getLongName() != QSTRING())
        calling(true, x.first.getLongName(), x.first.getCallback());
    }
}

/**
 * @brief CommandParser::exist. Dice si existe una opción. QChar
 * @param name Nombre
 * @return bool
 */
bool CommandParser::exist(QChar name) const {
  for(QPair<CommandLine, QChar> x: this->_items) {
      if(x.first.getSet() && (x.first.getShortName() == name))
          return true;
    }

  return false;
}

/**
 * @brief CommandParser::exist Dice si existe una opción. QString
 * @param name Nombre
 * @return bool
 */
bool CommandParser::exist(QString name) const {
  for(QPair<CommandLine, QChar> x: this->_items) {
      if(x.first.getSet() && (x.first.getLongName() == name))
          return true;
    }

  return false;
}

/**
 * @brief CommandParser::runOption Ejecuta un valor estandar
 * @param name Nombre
 * @param qsl Parámetros
 * @return 0 ó 1
 */
int CommandParser::runOption(QString name, QStringList qsl) const {
  for(QPair<CommandLine, QChar> x: this->_items)  {
    if (x.first.getLongName() == name)
      return x.first.exec(const_cast<CommandParser*>(this), qsl);
  }

  return TP_RESULT_OK;
}

/**
 * @brief CommandParser::runOption Ejecuta un valor estandar
 * @param name Nombre
 * @param qsl Parámetros
 * @return 0 ó 1
 */
int CommandParser::runOption(QChar name, QStringList qsl) const {
  for(QPair<CommandLine, QChar> x: this->_items)  {
    if (x.first.getShortName() == name)
      return x.first.exec(const_cast<CommandParser*>(this), qsl);
  }

  return TP_RESULT_OK;
}

/**
 * @brief CommandParser::runOptionEx Ejecuta un valor estandar, la función extendida
 * @param name Nombre
 * @param qsl Argumentos
 * @return 0 ó 1
 */
int CommandParser::runOptionEx(QString name, QStringList qsl) const {
  for(QPair<CommandLine, QChar> x: this->_items)  {
    if (x.first.getLongName() == name)
      return x.first.execEx(const_cast<CommandParser*>(this), qsl);
  }

  return TP_RESULT_OK;
}

/**
 * @brief CommandParser::runOptionEx Ejecuta un valor estandar, la función extendida
 * @param name Nombre
 * @param qsl Argumentos
 * @return 0 ó 1
 */
int CommandParser::runOptionEx(QChar name, QStringList qsl) const {
  for(QPair<CommandLine, QChar> x: this->_items)  {
    if (x.first.getShortName() == name)
      return x.first.execEx(const_cast<CommandParser*>(this), qsl);
  }

  return TP_RESULT_OK;
}

/**
 * @brief CommandLine::exec Ejecuta la opción
 * @param parser Parser
 * @param parameters
 * @return 0 ó 1
 */
int CommandLine::exec(CommandParser *parser, QStringList parameters) const {
  if (this->_callb != nullptr)
    this->_callb(parser, parameters);

  return TP_RESULT_OK;
}

/**
 * @brief CommandLine::execEx Ejecuta la opción
 * @param parser Parser
 * @param parameters
 * @return 0 ó 1
 */
int CommandLine::execEx(CommandParser *parser, QStringList parameters) const {
  if (this->_callb_is != nullptr)
    this->_callb_is(parser, parameters);

  return TP_RESULT_OK;
}

/**
 * @brief CommandLine::setSet Pone un valor a _set
 * @param value
 */
void CommandLine::setSet(bool value) {
  this->_set = value;
}

/**
 * @brief CommandLine::CommandLine Constructor
 * @param symbol -v
 * @param name --verbose
 * @param description Descripción
 * @param extra (Default text)
 * @param parametersRequired Parámetros
 * @param callback Callback
 * @param callb_is Callback
 */
CommandLine::CommandLine(QChar symbol, QString name, QString description, QString extra, int parametersRequired, TP_Callback callback, TP_Callback callb_is) {
    this->_callb = callback;
    this->_parameters_required = parametersRequired;
    this->_ch = symbol;
    this->_name = name;
    this->_extra = extra;
    this->_description = description;
    this->_callb_is = callb_is;
    this->_set = false;
}

/**
 * @brief CommandLine::getDescription Devuelve la descripción de la opción
 * @return QString
 */
QString CommandLine::getDescription() const {
    return this->_description;
}

/**
 * @brief CommandLine::getExtra Devuelve la descripción extra de la opción
 * @return QString
 */
QString CommandLine::getExtra() const {
    return this->_extra;
}

/**
 * @brief CommandLine::getShortName Devuelve el nombre corto de la opción
 * @return QChar
 */
QChar CommandLine::getShortName() const {
  if ( this->_ch == ((char)0) )
    return 0;
  else
    return this->_ch;
}

/**
 * @brief CommandLine::getLongName Devuelve el nombre largo de la opción
 * @return QString
 */
QString CommandLine::getLongName() const {
    return this->_name;
}

/**
 * @brief CommandLine::getParameters Devuelve el número de parámetros requeridos
 * @return Nº
 */
int CommandLine::getParameters() const {
    return this->_parameters_required;
}

/**
 * @brief CommandLine::getSet  Devuelve el _set de la opción
 * @return bool
 */
bool CommandLine::getSet() const {
    return this->_set;
}

/**
 * @brief CommandLine::getCallback Devuelve el callback del item
 * @return Callback
 */
TP_Callback CommandLine::getCallback() const {
  return this->_callb;
}

/**
 * @brief CommandLine::getCallbackIs Devuelve el callback extendido del item
 * @return Callback
 */
TP_Callback CommandLine::getCallbackIs() const {
  return this->_callb_is;
}


#else

/*
 * Imprime la forma de versión
 */
static int PrintVersion(CommandParser *parser, QStringList params) {
  Q_UNUSED(params)
  Q_UNUSED(parser)

  std::string app  = QCoreApplication::applicationName().toStdString();
  std::cout << app << " Version " << OST_VERSION_STR << std::endl;
  return TP_RESULT_SHUT;
}

/*
 * Imprime la forma de ayuda
 */
static int PrintHelp(CommandParser *parser, QStringList params) {
    Q_UNUSED(params)

    std::cout << "Uso: " << QCoreApplication::applicationName().toStdString();
    std::string s  =  parser->getItems().size() > 0 ? " [options] " : STRING() ;
    std::cout  << s;
    if (parser->getPositionals().size() > 0)
      foreach (CommandLine i, parser->getPItems())  {
          std::cout << " " << i.getLongName().toStdString();
      }
    std::cout << std::endl;

    std::cout << QCoreApplication::applicationName().toStdString() << " versión " << OST_VERSION_STR << std::endl
              << std::endl
              << "Opciones:" << std::endl;

    // first we analyse the list of options and get the size of longest one
    int longest = 10;

    foreach (CommandLine i, parser->getItems())
    {
        int is = i.getLongName().size() + 4;
        if (is > longest)
            longest = is;
    }

    int longest_line = 20;
    QStringList lines_param, lines_help;

    foreach (CommandLine i, parser->getItems())
    {
        QString parameters = i.getLongName();
        if (!parameters.isEmpty())
          parameters = Generic::expandedString("--" + parameters, longest, longest + 10);
        else
          parameters = Generic::expandedString(parameters, longest, longest + 10);

        QString parameters_short;
        if (i.getShortName() != 0)
            parameters_short = "-" + QString(QChar(i.getShortName()));
        if (!parameters.isEmpty() && !parameters_short.isEmpty())
            parameters += " | " + parameters_short;
        else if (!parameters_short.isEmpty())
            parameters += parameters_short;
        if (i.getParameters() > 0)
        {
            if (i.getParameters() == 1)
                parameters += " <parametros requeridos>";
            else
                parameters += " <" + QString::number(i.getParameters()) + " parametros requeridos>";
        }
        lines_help << i.getDescription();
        lines_param << parameters;

        int size = parameters.size() + 2;
        if (size > longest_line)
            longest_line = size;
    }

    int item = 0;
    while (item < lines_param.size())
    {
        std::cout << "  " << Generic::expandedString(lines_param[item], longest_line, longest_line + 10).toStdString() << ": " << lines_help[item].toStdString() << std::endl;
        // let's go next
        item++;
    }

    if(parser->getPItems().size() > 0) {
      std::cout << "\nPosicionales:\n";
      foreach(CommandLine i, parser->getPItems()) {
          //std::cout << "  " << i.getLong().toStdString() ;
          std::cout << "  " << Generic::expandedString(i.getLongName(), longest_line, longest_line + 10).toStdString() << ": " << i.getDescription().toStdString() << std::endl;
        }
    }

    std::cout << std::endl;
    //std::cout << "This software is open source, contribute at http://github.com/benapetr/q2c" << std::endl;

    return TP_RESULT_SHUT;
}

/*
 * Constructor
 */
CommandParser::CommandParser() {
   this->addOption('h', "help", "Muestra Ayuda", 0, (TP_Callback)PrintHelp);
   this->addOption(0, "version", "Muestra información de versión", 0, (TP_Callback)PrintVersion);
}

/*
 * Procesa los valores. Posicionales o no
 */
bool CommandParser::parse(int argc, char **argv) {
    int x = 0;
    int expected_parameters = 0;
    QStringList parameter_buffer;
    CommandLine *item = nullptr;
    while (x < argc)
    {
        QString parameter = QString(argv[x++]);
        if (expected_parameters > 0)
        {
            // the last argument we processed has some parameters, so we are now processing them into a buffer
            parameter_buffer << parameter;
            expected_parameters--;
            if (expected_parameters == 0)
            {
                // execute
                if (item->exec(this, parameter_buffer) == TP_RESULT_SHUT)
                {
                    delete item;
                    return false;
                }
                parameter_buffer.clear();
            }
            continue;
        }
        if (parameter.length() < 2)
        {
            std::cerr << "ERROR: parámetro no reconocido: " << parameter.toStdString() << std::endl;
            return false;
        }
        if (parameter.startsWith("--"))
        {
            // It's a word parameter
            delete item;
            item = this->getItem(parameter.mid(2));
            if (item == NULL)
            {
                std::cerr << "ERROR: parámetro no reconocido: " << parameter.toStdString() << std::endl;
                delete item;
                return false;
            }
            expected_parameters = item->getParameters();
            if (expected_parameters)
                continue;
            this->setItem(parameter.mid(2), true);
            // let's process this item
            if (item->exec(this, parameter_buffer) == TP_RESULT_SHUT)
            {
                delete item;
                return false;
            }
        }
        else if (parameter.startsWith("-"))
        {
            // It's a single character(s), let's process them recursively
            int symbol_px = 0;
            while (parameter.size() > ++symbol_px)
            {
                char sx = parameter[symbol_px].toLatin1();
                delete item;
                item = this->getItem(sx);
                if (item == NULL)
                {
                    std::cerr << "ERROR: parámetro no reconocido: -" << sx << std::endl;
                    delete item;
                    return false;
                }
                expected_parameters = item->getParameters();
                if (expected_parameters && (symbol_px+1) < parameter.size())
                {
                    std::cerr << "ERROR: no suficientes parámetros para -" << sx << std::endl;
                    delete item;
                    return false;
                }
                if (expected_parameters)
                    continue;
                this->setItem(sx, true);
                // let's process this item
                if (item->exec(this, parameter_buffer) == TP_RESULT_SHUT)
                {
                    delete item;
                    return false;
                }
            }
        }
        else {
            if (x <= 1)
              continue;

            delete item;
            item = this->getPItem(parameter);
            if (item == nullptr)
            {
                std::cerr << "ERROR: parámetro no reconocido: " << parameter.toStdString() << std::endl;
                delete item;
                return false;
            }
            // let's process this item
            this->setPItem(parameter, true);
            if (item->exec(this, parameter_buffer) == TP_RESULT_SHUT)
            {
                delete item;
                return false;
            }
          }
    }
    delete item;
    if (expected_parameters)
    {
        std::cerr << "ERROR: parámetro desconocido" << std::endl;
        return false;
    }
    return true;
}

/*
 * Ejecuta la opción
 */
int CommandLine::exec(CommandParser *parser, QStringList parameters) const {
  if (this->_callb != nullptr)
    this->_callb(parser, parameters);
}

/*
 * Obtiene un valor estandar
 */
CommandLine *CommandParser::getItem(QString name) const {
    foreach(CommandLine x, this->_items)  {
        if (x.getLongName() == name)
            return new CommandLine(x);
    }
    return nullptr;
}

/*
 * Pone un valor posicional. El _set
 */
void CommandParser::setItem(QString name, bool value) {
  int n = 0;
  for(CommandLine x: this->_items) {
      if (x.getLongName() == name) {
        x.setSet(value);
        this->_items.replace(n, x);
      }
      n++;
    }
}


/*
 * Obtiene un valor posicional
 */
CommandLine *CommandParser::getPItem(QString name)  const {
    foreach(CommandLine x, this->_pitems) {
        if (x.getLongName() == name)
            return new CommandLine(x);
    }
    return nullptr;
}

/*
 * Pone un valor posicional. El _set
 */
void CommandParser::setPItem(QString name, bool value) {
  int n = 0;
  for(CommandLine x: this->_pitems)  {
      if (x.getLongName() == name) {
        x.setSet(value);
        this->_pitems.replace(n, x);
      }
      n++;
    }
}

/*
 * Añade una opción determinada
 */
void CommandParser::addOption(QChar ch, QString name, QString description, int parametersRequired, TP_Callback callb) {
    this->_items.append(CommandLine(ch, name, description, parametersRequired, callb));
}

/*
 * Añade un valor posicional
 */
bool CommandParser::addPositional(QString name, QString description, TP_Callback callback) {
  this->_pitems.append( CommandLine(0, name, description, 0, callback) );
  return true;
}

/*
 * Devuelve una lista de valores posionales
 */
QStringList CommandParser::getPositionals() const {
  QStringList p;
  int n = 0;
  while(n < _pitems.size())
     p.append( _pitems[n++].getLongName() );

  return p;
}

/*
 * Devuelve una lista de opciones
 */
QList<CommandLine> CommandParser::getItems() const {
    return this->_items;
}

/*
 * Devuelve una lista de valores posicionales
 */
QList<CommandLine> CommandParser::getPItems() const {
    return this->_pitems;
}

/*
 * Devuelve un item no posicional
 */
CommandLine *CommandParser::getItem(QChar name) const {
    foreach(CommandLine x, this->_items)  {
        if (x.getShortName() == name)
            return new CommandLine(x);
    }
    return nullptr;
}

/*
 * Pone un valor a value
 */
void CommandParser::setItem(QChar name, bool value) {
  int n = 0;
  for(CommandLine x: this->_items)  {
      if (x.getShortName() == name) {
        x.setSet(value);
      this->_items.replace(n, x);
      }
      n++;
    }
}
/*
 * Pone un valor a _set
 */
void CommandLine::setSet(bool value) {
  this->_set = value;
}

/*
 * Constructor de CommandLine
 */
CommandLine::CommandLine(QChar symbol, QString name, QString description, int parametersRequired, TP_Callback callback) {
    this->_callb = callback;
    this->_parameters_required = parametersRequired;
    this->_ch = symbol;
    this->_name = name;
    this->_description = description;
    this->_set = false;
}

/*
 * Devuelve la descripción de la opción
 */
QString CommandLine::getDescription() const {
    return this->_description;
}

/*
 * Devuelve el nombre corto de la opción
 */
QChar CommandLine::getShortName() const {
  if ( this->_ch == ((char)0) )
    return 0;
  else
    return this->_ch;
}

/*
 * Devuelve el nombre largo de la opción
 */
QString CommandLine::getLongName() const {
    return this->_name;
}

/*
 * Devuelve el número de parámetros requeridos
 */
int CommandLine::getParameters() const {
    return this->_parameters_required;
}
/*
 * Devuelve el _set de la opción
 */
bool CommandLine::getSet() const {
    return this->_set;
}

/*
 * Devuelve el callback del item
 */
TP_Callback CommandLine::getCallback() const {
  return this->_callb;
}

/*
 * Corre automáticamente por todos los parámetros que están actualizados,
 * que tiene su _set a true
 *
 * Firma de calling:
 *   void TP_Callback_is(bool, QString, TP_Callback);
 *
 */
void CommandParser::run(TP_Callback_is calling) {
  for(CommandLine x: this->_items) {
      if(x.getSet())
        calling(false, QString(x.getShortName()),  x.getCallback());
    }

  for(CommandLine x: this->_items) {
      if(x.getSet())
        calling(false, x.getLongName(),  x.getCallback());
    }

  for(CommandLine x: this->_pitems) {
      if(x.getSet())
        calling(true, x.getLongName(),  x.getCallback());
    }

}

/*
 * Dice si existe una opción. QChar
 */
bool CommandParser::exist(QChar name) const {
  for(CommandLine x: this->_items) {
      if(x.getSet() && (x.getShortName() == name))
          return true;
    }

  return false;
}

/*
 * Dice si existe una opción. QString
 */
bool CommandParser::exist(QString name) const {
  for(CommandLine x: this->_items) {
      if(x.getSet() && (x.getLongName() == name))
          return true;
    }

  for(CommandLine x: this->_pitems) {
      if(x.getSet() && (x.getLongName() == name))
          return true;
    }

  return false;
}
#endif

QT_END_NAMESPACE
