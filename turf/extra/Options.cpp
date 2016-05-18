/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#include <turf/Core.h>
#include <turf/extra/Options.h>
#include <turf/Assert.h>
#include <turf/Util.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

namespace turf {
namespace extra {

Options::Options(const Option* options, ureg numOptions, const char* argString)
    : m_programName("???"), m_argString(argString), m_options(options), m_numOptions(numOptions) {
    m_helpOption.longName = "help";
    m_helpOption.shortName = 'h';
    m_helpOption.takesParam = false;
    m_helpOption.description = "display help then exit";
}

void Options::parse(int argc, const char** argv) {
    // Extract program name
    // FIXME: Just recognize one of the slashes depending on Windows or not.
    // Must make sure it works in Cygwin and MinGW.
    TURF_ASSERT(argc >= 1);
    const char* programName = turf::util::max(strrchr(argv[0], '/'), strrchr(argv[0], '\\'));
    if (programName)
        m_programName = programName + 1;

    // Make option tables
    typedef std::map<std::string, const Option*> LongDict;
    LongDict longDict;
    longDict[m_helpOption.longName] = &m_helpOption;
    typedef std::map<char, const Option*> ShortDict;
    ShortDict shortDict;
    shortDict[m_helpOption.shortName] = &m_helpOption;
    for (ureg i = 0; i < m_numOptions; i++) {
        const Option* opt = m_options + i;
        TURF_ASSERT(opt->longName);
        TURF_ASSERT(longDict.find(opt->longName) == longDict.end()); // duplicate argument
        longDict[opt->longName] = opt;
        if (opt->shortName) {
            TURF_ASSERT(shortDict.find(opt->shortName) == shortDict.end()); // duplicate argument
            shortDict[opt->shortName] = opt;
        }
    }

    // Scan provided arguments
    for (int a = 1; a < argc; a++) {
        const char* arg = argv[a];
        const char* param = NULL;
        if (arg[0] == '-') {
            const Option* opt = NULL;
            if (arg[1] == '-') {
                LongDict::const_iterator iter = longDict.find(arg + 2);
                if (iter == longDict.end())
                    printErrorAndExit("Unrecognized option %s", arg);
                opt = iter->second;
                if (m_keywordArgs.find(opt->longName) != m_keywordArgs.end())
                    printErrorAndExit("Option %s was specified twice", arg);
                if (opt->takesParam) {
                    if (++a >= argc)
                        printErrorAndExit("Option %s requires a parameter", arg);
                    param = argv[a];
                }
            } else {
                ShortDict::const_iterator iter = shortDict.find(arg[1]);
                if (iter == shortDict.end())
                    printErrorAndExit("Unrecognized option -%c", arg[1]);
                opt = iter->second;
                if (m_keywordArgs.find(opt->longName) != m_keywordArgs.end())
                    printErrorAndExit("Option -%c was specified twice", arg[1]);
                if (arg[2]) {
                    if (opt->takesParam)
                        param = arg + 2;
                    else
                        printErrorAndExit("Option -%c does not take a parameter; \"%s\" provided", arg[1], arg + 2);
                } else if (opt->takesParam) {
                    if (++a >= argc)
                        printErrorAndExit("Option -%c requires a parameter", arg[1]);
                    param = argv[a];
                }
            }
            TURF_ASSERT(opt);
            m_keywordArgs[opt->longName] = param;
        } else {
            m_positionalArgs.push_back(arg);
        }
    }

    // Check for help option
    if (hasKeywordArg("help"))
        printUsageAndExit(0);
}

sreg Options::getInteger(const char* longName, sreg defaultValue) {
    KeywordArgs::const_iterator iter = m_keywordArgs.find(longName);
    if (iter == m_keywordArgs.end())
        return defaultValue;
    char* endPtr = NULL;
    long int result = strtol(iter->second, &endPtr, 10);
    if (endPtr == iter->second || endPtr[0] != 0)
        printErrorAndExit("Expected integer argument for option --%s, got \"%s\"", longName, iter->second);
    return (sreg) result;
}

double Options::getDouble(const char* longName, double defaultValue) {
    KeywordArgs::const_iterator iter = m_keywordArgs.find(longName);
    if (iter == m_keywordArgs.end())
        return defaultValue;
    char* endPtr = NULL;
    double result = strtod(iter->second, &endPtr);
    if (endPtr == iter->second || endPtr[0] != 0)
        printErrorAndExit("Expected floating-point argument for option --%s, got \"%s\"", longName, iter->second);
    return result;
}

void Options::printUsageAndExit(int code) {
    fprintf(stderr, "Usage: %s %s\n\n", m_programName, m_argString ? m_argString : "<options>");
    if (m_numOptions > 0) {
        fprintf(stderr, "Options:\n");
        for (sreg i = -1; i < (sreg) m_numOptions; i++) {
            const Option* opt = i < 0 ? &m_helpOption : m_options + i;
            const char* param = opt->takesParam ? " <param>" : "";
            fprintf(stderr, "  ");
            if (opt->shortName)
                fprintf(stderr, "-%c, ", opt->shortName);
            fprintf(stderr, "--%s%s", opt->longName, param);
            if (opt->description)
                fprintf(stderr, ": %s", opt->description);
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n");
    }
    exit(code);
}

void Options::printErrorAndExit(const char* format, ...) {
    fprintf(stderr, "Error: ");
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n\n");
    printUsageAndExit(1);
}

} // namespace extra
} // namespace turf
