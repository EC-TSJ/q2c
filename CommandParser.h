/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%M% 21/01/2020 ) Añadido el segundo TP_Callback     */
/*  (%M% 21/01/2020 ) Añadido runOption                  */
/*  (%M% 21/01/2020 ) Añadido runOptionEx                */
/*  (%M% 21/01/2020 ) Añadido execEx                     */
/*  <$Válido$>                                           */
/*********************************************************/

#ifndef __COMMANDPARSER_H
# define __COMMANDPARSER_H

#define TP_RESULT_OK    0
#define TP_RESULT_SHUT  1
#define VN              (TP_Callback)0
#define QAddPositional(obj, name, desc)   obj->addPositional(name, desc, nullptr)
#define OST_VERSION_STR "0.252.62"
#define STRING()        ""
#define QSTRING()       QString()
#define V_NO_POS        'N'
#define V_SI_POS        'P'
#define V_NULL          '\0'
#define V_SPACE         " "
#define V_INDENT        "  "

#include <QStringList>
#include <QList>
#include <QString>
#include "main.h"
#include "_defs.h"


class CommandParser;
class CommandLine;

typedef       int     (*TP_Callback)      (CommandParser*, QStringList);
typedef       void    (*TP_Callback_is)   (bool, QString, TP_Callback);


//#define V__QPAIR__ 1

class CommandLine {
public:
#if V__QPAIR__
  CommandLine(QChar symbol, QString name, QString description, QString extra, int parametersRequired, TP_Callback callback, TP_Callback callb_is);
#else
  CommandLine(QChar symbol, QString name, QString description, int parametersRequired, TP_Callback callback);
#endif
  QString getDescription() const;
#if V__QPAIR__
  QString getExtra() const;
#endif
  QChar getShortName() const;
  QString getLongName() const;
  int getParameters() const;
  TP_Callback getCallback() const;
  TP_Callback getCallbackIs() const;
  bool getSet() const;
  int exec(CommandParser *parser, QStringList parameters) const;
#if V__QPAIR__
  int execEx(CommandParser *parser, QStringList parameters) const;
#endif
  //-------
  void setSet(bool value);

private:
  QChar _ch;
  QString _name;
  QString _description;
#if V__QPAIR__
  QString _extra;
  TP_Callback _callb_is;
#endif
  int _parameters_required;
  TP_Callback _callb;
  bool _set = false;
};

/*
 * The TerminalParser class processes the arguments passed in terminal
 */
class CommandParser {
#if V__QPAIR__
  char *parseBis(int, char **);
#endif
public:
  CommandParser();
#if V__QPAIR__
  void addHelpOption();
  void addVersionOption();
#endif
  bool parse(int argc, char **argv);
#if V__QPAIR__
  QStringList getOptions() const;
  void addOption(QChar ch, QString name, QString description, QString extra,  int parametersRequired, TP_Callback callb, TP_Callback callb_is);
#endif
#if !V__QPAIR__
  void addOption(QChar ch, QString name, QString description, int parametersRequired, TP_Callback callb);
  QList<CommandLine> getItems() const;  //nonpositionals
  QList<CommandLine> getPItems() const; //positionals
#else
  QList<QPair<CommandLine, QChar>> getItems() const;  //nonpositionals
  int runOption(QString, QStringList) const;
  int runOption(QChar, QStringList) const;
  int runOptionEx(QString, QStringList) const;
  int runOptionEx(QChar, QStringList) const;
#endif
  CommandLine *getItem(QChar name) const;    // -v
  CommandLine *getItem(QString name) const;  // --verbose
#if !V__QPAIR__
  CommandLine *getPItem(QString name) const; //
#endif
  void setItem(QChar name, bool value);    //
  void setItem(QString name, bool value);  //
#if !V__QPAIR__
  void setPItem(QString name, bool value); //
#endif
  QStringList getPositionals() const;
#if V__QPAIR__
  bool addPositional(QString name, QString description, QString extra, TP_Callback callback, TP_Callback callb_is);
#else
  bool addPositional(QString name, QString description, TP_Callback callback);
#endif
  void run(TP_Callback_is callback);
  bool exist(QChar name) const;
  bool exist(QString name) const;

private:
#if V__QPAIR__
  QList<QPair<CommandLine, QChar>> _items;
#else
  QList<CommandLine> _items;
  QList<CommandLine> _pitems; // positionals
#endif
};

#endif // TERMINALPARSER_H
