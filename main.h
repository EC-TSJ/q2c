/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%M% 28/12/2019 )                                    */
/*  <$Válido$>                                           */
/*********************************************************/

#ifndef V_MAIN_H
# define V_MAIN_H

class CommandParser;
class QStringList;

#define APX_SETTINGS(name, _name, _list)   static int name(CommandParser *_name, QStringList _list)

APX_SETTINGS(_printHelp, parser, params);
APX_SETTINGS(_printVersion, parser, params);
APX_SETTINGS(_parserInput, parser, params);
APX_SETTINGS(_parserOutput, parser, params);
APX_SETTINGS(_parserQt5, parser, params);
APX_SETTINGS(_parserVerbosity, parser, params);
APX_SETTINGS(_parserName, parser, params);

bool DetectInput();
int runQ2c(int, char *[]);

#endif // MAIN_H
