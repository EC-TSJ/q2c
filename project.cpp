/*********************************************************/
/*  (%T% %S%), %J% <$A$> <$2.125$>                       */
/*  (%W% 02/12/2019 ) Escrito                            */
/*  (%X% 28/12/2019 ) Extendido extra en parámetros      */
/*  (%M% 28/12/2019 )                                    */
/*  <$Válido$>                                           */
/*********************************************************/

#include "project.h"
#include "generic.h"
#include <QStringList>
#include "logs.h"
#include "CommandParser.h"

QT_BEGIN_NAMESPACE

/**
 * @brief Project::parseQMake Parsea el fichero .pro
 * @param text
 * @return bool
 */
bool Project::parseQMake(QString text) {
    // Process the file line by line
    QStringList lines = text.split("\n");

    if (Generic::Config::verbosity)
      Logs::debugLog("\nVerbosity: parseQMake" );

    for (QString line: lines)  {
      QString another = line;
      if (line.startsWith("requires", Qt::CaseInsensitive) ||
          line.startsWith("target", Qt::CaseInsensitive) ||
          line.startsWith("rc_", Qt::CaseInsensitive) ||
          line.startsWith("INSTALL") ||
          line.startsWith("CONFIG") ||
          line.startsWith("DEST") ||
          line.startsWith("include", Qt::CaseInsensitive) ||
          line.startsWith("LIB") ||
          line.startsWith("PKG") ||
          line.startsWith("{") ||
          line.startsWith("}") ||
          line.startsWith("message") ||
          line.startsWith("qnx") ||
          line.startsWith("else") ||
          line.startsWith("DEFINES") ||
          line.startsWith("LEX") ||
          line.startsWith("YACC") ||
          line.startsWith("DEPEND") ||
          line.startsWith("VPATH") ||
          line.startsWith("DEF") ||
          line.startsWith("TEMPLATE") ||
          line.startsWith("TRANSLATIONS") ||
          line.startsWith("#") ||
          line.contains("isEmpty") || // !isEmpty
          line.contains("win32") ||  // !win32
          line.contains("unix") ||  // !unix
          line.contains("exists") || // !exists
          line.isEmpty() )
          continue;
      if (line.startsWith("QT") ||
          line.startsWith("FORMS") ||
          line.startsWith("SOURCES") ||
          line.startsWith("HEADERS") ||
          line.startsWith("RESOURCES") )
          this->_vanDerBrook.append("*");
      // Quita espacios comenzando
      while (line.startsWith(V_WHITE))
          line = line.mid(1);
      // If line starts with hash we can ignore it
      int first = 0;
      int second = 0;
      // buscamos una palabra
      QString keyword = line;
      while( line!= V_EMPTY ) {
        // si la linea contiene espacio o '\r'
        if (line.contains(V_WHITE))
          second = line.indexOf(V_WHITE);
        else
          second = line.indexOf(V_CR);
        // la palabra es la definición entre ambos simbolos
        keyword = line.mid(first, second);
        // la linea restante
        line = second == -1 ? QString() : line.mid(second);
        // si la palabra no es estos valores
        if ( keyword != V_EMPTY && keyword != V_CR && keyword != V_PLUSEQUAL
             && keyword != V_MINUSEQUAL && keyword != V_LEFT && keyword != V_EQUAL)
            this->_vanDerBrook.append(keyword);
        // borra los espacios
        while (line.startsWith(V_WHITE))
            line = line.mid(1);
        // borra los '\r'
        while (line.startsWith(V_CR))
          line = line.mid(1);
#if V__QPAIR__
        if (Generic::Config::verbosity)
          Logs::debugLog("  Verbosity: --" + keyword);
#endif
        }
    }

    lines.clear();

    for(QString str: _vanDerBrook) {
         if(str == "*")
          continue;
        if(str == "QT"){
          pSet = &Project::_updateQt;
          continue;
          }
        if(str == "FORMS") {
          pSet = &Project::_updateForms;
          continue;
          }
        if(str == "SOURCES") {
          pSet = &Project::_updateSources;
          continue;
          }
        if(str == "HEADERS") {
          pSet = &Project::_updateHeaders;
          continue;
          }
        if(str == "RESOURCES") {
          pSet = &Project::_updateResources;
          continue;
          }

        // llama al puntero
#if !V__QPAIR__
        if (Generic::Config::verbosity)
          Logs::debugLog("  Verbosity: --" + str);
#endif
        (this->*pSet)(str);
      }

  this->_vanDerBrook.clear();
  return true;
}

/**
 * @brief Project::_updateResources Actualiza el fichero _resources
 * @param str Valor a actualizar
 */
void Project::_updateResources(QString str) {
  this->_resources.append(str);
}

/**
 * @brief Project::_updateSources Actualiza el fichero _sources
 * @param str Valor a actualizar
 */
void Project::_updateSources(QString str) {
  this->_sources.append(str);
}

/**
 * @brief Project::_updateHeaders Actualiza el fichero _headers
 * @param str Valor a actualizar
 */
void Project::_updateHeaders(QString str) {
  this->_headers.append(str);
}

/**
 * @brief Project::_updateForms Actualiza el fichero _forms
 * @param str Valor a actualizar
 */
void Project::_updateForms(QString str) {
  this->_forms.append(str);
}

/**
 * @brief Project::_updateQt Actualiza el fichero _modules
 * @param str Valor a actualizar
 */
void Project::_updateQt(QString str) {
  this->_modules.append(str);
}

/**
 * @brief Project::toCMake Genera el fichero CMakeLists.txt
 * @return QString
 */
QString Project::toCMake() {
  if (Generic::Config::verbosity)
    Logs::debugLog("\nVerbosity: toMake" );

  QString source =  "cmake_minimum_required (VERSION 3.0)\n";
  source += "set(TARGET_NAME " + Generic::Config::name + ")\n\n";
  source += "project( ${TARGET_NAME})\n\n";
  source += "# Check the build type and ask the user to set concrete one\n";
  source += "if(NOT CMAKE_BUILD_TYPE)\n";
  source += "   set(CMAKE_BUILD_TYPE RelWithDebInfo)\n";
  source += "   message(WARNING \"CMAKE_BUILD_TYPE is not set, forcing to RelWithDebInfo\")\n";
  source += "endif()\n\n";
  source += "# Set compiler flags\n";
  source += "if(${CMAKE_CXX_COMPILER_ID} MATCHES \"GNU\" OR ${CMAKE_CXX_COMPILER_ID} MATCHES \"Clang\")\n";
  source += "   set(CMAKE_CXX_FLAGS \"-std=c++11 -Wall -Wextra\")\n";
  source += "   set(CMAKE_CXX_FLAGS_DEBUG \"-O0 -g3\")\n";
  source += "   set(CMAKE_CXX_FLAGS_RELEASE \"-O3\")\n";
  source += "   set(CMAKE_CXX_FLAGS_RELWITHDEBINFO \"-O3 -g3\")\n";
  source += "   set(CMAKE_CXX_FLAGS_MINSIZEREL \"-Os\")\n";
  source += "endif()\n\n";
  if (!this->_modules.isEmpty())  {
      source += "\n# Module files\n";
      foreach (QString src, this->_modules)   {
          source += "find_package(Qt5 COMPONENTS ";
          source += " " + Generic::Strings::capitalFirst(src) ;
          source += " REQUIRED)\n";
      }
  }

  source += "\n";
  source += "include_directories(\n";
  source += "	${CMAKE_CURRENT_BINARY_DIR}\n";
  source += "	${CMAKE_CURRENT_SOURCE_DIR}\n";
  source += ")\n\n";
  source += "# Instruct CMake to run moc automatically when needed\n";
  source += "set(CMAKE_AUTOMOC ON)\nset(CMAKE_AUTORCC ON)\nset(CMAKE_AUTOUIC ON)\n\n";

  if (!this->_sources.isEmpty())   {
      source += "\n# Source files\n";
      source += "set(SOURCES";
      foreach (QString src, this->_sources)   {
          source += " " + src ;
      }
      source += ")\n\n";
  }

  if (!this->_headers.isEmpty())    {
      source += "# Headers files\n";
      source += "set(HEADERS";
      foreach (QString src, this->_headers)     {
          source += " " + src ;
      }
      source += ")\n\n";
  }

  if (!this->_forms.isEmpty())   {
      source += "# User Interfaces files\n";
      source += "set(FORMS";
      foreach (QString src, this->_forms)    {
          source += " " + src ;
      }
      source += ")\n\n";
  }

  if (!this->_resources.isEmpty())   {
      source += "# Resource files\n";
      source += "set(RESOURCES ";
      foreach (QString src, this->_resources)    {
          source += " " + src ;
      }
      source += ")\n\n";
  }

  //source += "set(RESOURCES resources.qrc )\n\n";
  if (!this->_modules.isEmpty())   {
      source += "# Shared libraries\n";
      source += "set(LIBRARIES ";
      foreach (QString src, this->_modules)    {
          source += " Qt5::" + Generic::Strings::capitalFirst(src) ;
      }
      source += " )\n\n";
  }

  //source += "set(LIBRARIES Qt5::Core Qt5::Widgets Qt5::Network)\n\n";
  source += "# Generate additional sources with MOC and UIC\n";
  source += "qt5_wrap_ui(UIC_SOURCES ${FORMS})\n";
  source += "qt5_add_resources(RCC_SOURCES ${RESOURCES})\n\n";
  source += "# Set target\n";
  source += "add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS} ${UIC_SOURCES} ${RCC_SOURCES})\n\n";
  source += "# Link with libraries\n";
  source += "target_link_libraries(${TARGET_NAME} ${LIBRARIES})\n\n";
  source += "# Installation\n";
  source += "install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION bin)\n";
  source += "install(FILES resources/${TARGET_NAME}.png DESTINATION share/icons/hicolor/48x48/apps)\n";
  source += "install(FILES ${TARGET_NAME}.desktop DESTINATION share/applications)\n\n";
#if V__QPAIR__
  if (Generic::Config::verbosity)
    Logs::debugLog("  Verbosity: --" + source);
#endif

  return source;
}

QT_END_NAMESPACE
