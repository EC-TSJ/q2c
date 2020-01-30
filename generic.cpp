/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%M% 28/12/2019 )                                    */
/*  <$Válido$>                                           */
/*********************************************************/

#include "Generic.h"
#include <QStringList>
#include "CommandParser.h"

QT_BEGIN_NAMESPACE

/**
 * @brief Generic::Strings::expandedString
 * @param string
 * @param minimum_size
 * @param maximum_size
 * @return
 */
QString Generic::Strings::expandedString(QString string, unsigned int minimum_size, unsigned int maximum_size) {
    if (maximum_size > 0 && static_cast<unsigned int>(string.size()) > maximum_size)    {
        if (maximum_size < 4)  {
            string = string.mid(0, maximum_size);
        }
        else  {
            string = string.mid(0, maximum_size - 3);
            string += "...";
        }
        return string;
    }

    while (static_cast<unsigned int>(string.size()) < minimum_size)
        string += " ";

    return string;
}

/**
 * @brief Generic::Strings::indent
 * @param input
 * @param indentation
 * @return
 */
QString Generic::Strings::indent(QString input, unsigned int indentation) {
    QStringList lines = input.split("\n");
    QString indent = "";
    while (indentation-- > 0)
        indent += " ";
    QString result;
    foreach (QString line, lines)
    {
        if (!line.trimmed().isEmpty())
            result += indent + line + "\n";
        else
            result += "\n";
    }
    if (result.endsWith("\n"))
        result = result.mid(0, result.length() - 1);

    return result;
}

/**
 * @brief Generic::Strings::capitalFirst
 * @param text
 * @return
 */
QString Generic::Strings::capitalFirst(QString text) {
    if (!text.isEmpty())
    {
        text[0] = text[0].toUpper();
    }
    return text;
}

/**
 * @brief Generic::Config::verbosity
 */
int Generic::Config::verbosity = 0;
/**
 * @brief Generic::Config::inputFile
 */
QString Generic::Config::inputFile = QSTRING();
/**
 * @brief Generic::Config::outputFile
 */
QString Generic::Config::outputFile = QSTRING();
/**
 * @brief Generic::Config::q2c
 */
bool Generic::Config::q2c = true;
/**
 * @brief Generic::Config::only_qt5
 */
bool Generic::Config::only_qt5 = false;
/**
 * @brief Generic::Config::name
 */
QString Generic::Config::name = QSTRING();
/**
 * @brief Generic::Config::mustName
 */
bool Generic::Config::mustName = false;
/**
 * @brief Generic::Config::help_option
 */
bool Generic::Config::help_option = true;
/**
 * @brief Generic::Config::version_option
 */
bool Generic::Config::version_option = true;

QT_END_NAMESPACE
