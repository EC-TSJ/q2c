/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%M% 28/12/2019 )                                    */
/*  <$Válido$>                                           */
/*********************************************************/

#ifndef GENERIC_H
# define GENERIC_H

#include <QString>

class Generic {
public:
  class Strings {
  public:
    static QString expandedString(QString string, unsigned int minimum_size, unsigned int maximum_size = 0);
    static QString indent(QString input, unsigned int indentation = 4);
    static QString capitalFirst(QString text);
  };

  class Config {
  public:
    static int verbosity;
    static QString inputFile;
    static QString outputFile;
    static bool only_qt4;
    static bool only_qt5;
    static bool forcing;
    static bool q2c;
    static QString name;
    static bool mustName;
    static bool help_option;
    static bool version_option;
  };
};

#endif // GENERIC_H

