/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_EXTRA_OPTIONS_H
#define TURF_EXTRA_OPTIONS_H

#include <turf/Core.h>
#include <vector>
#include <map>
#include <string>

namespace turf {
namespace extra {

struct Option {
    const char* longName; // required
    char shortName;
    bool takesParam;
    const char* description;
};

class Options {
private:
    const char* m_programName;
    const char* m_argString;
    const Option* m_options;
    Option m_helpOption;
    ureg m_numOptions;
    std::vector<const char*> m_positionalArgs;
    typedef std::map<std::string, const char*> KeywordArgs;
    KeywordArgs m_keywordArgs;

public:
    Options(const Option* options, ureg numOptions, const char* argString = NULL);
    void parse(int argc, const char** argv);
    sreg getInteger(const char* longName, sreg defaultValue);
    double getDouble(const char* longName, double defaultValue);
    bool hasKeywordArg(const char* longName) const {
        return m_keywordArgs.find(longName) != m_keywordArgs.end();
    }
    const std::vector<const char*> getPositionalArgs() const {
        return m_positionalArgs;
    }
    const KeywordArgs& getKeywordArgs() const {
        return m_keywordArgs;
    }
    void printUsageAndExit(int code);
    void printErrorAndExit(const char* format, ...);
};

} // namespace extra
} // namespace turf

#endif // TURF_EXTRA_OPTIONS_H
