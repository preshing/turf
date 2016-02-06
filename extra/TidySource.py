#!/usr/bin/env python

# FIXME: This file probably needs a license

import optparse
import os
import re
import collections
import difflib
import sys

DESIRED_PREAMBLE = """/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.  
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

""".splitlines()

# Parse options
parser = optparse.OptionParser(usage='Usage: %prog <options> path [path2 ...]')
parser.add_option('-p', '--preview', action='store_true', dest='preview', default=False,
                  help='preview changes as a diff')
parser.add_option('-w', '--write', action='store_true', dest='write', default=False,
                  help='write changes to files, destroying previous contents')
parser.add_option('--unix', action='store_const', const='unix', dest='lineEndings',
                  help="expect Unix line endings '\\n' (autodetect otherwise)")
parser.add_option('--windows', action='store_const', const='windows', dest='lineEndings',
                  help="expect Windows line endings '\\r\\n'")
options, roots = parser.parse_args()
if len(roots) == 0:
    print('You must specify at least one path.\n')
    parser.print_help()

RE_LINEENDING = re.compile('[\n\r]*$')

SCRIPT_NAME = os.path.split(sys.argv[0])[1]

class ConsistencyGroup:
    """ This class is used to help identify files that have inconsistent line endings
        or inconsistent preamble. """
    
    def __init__(self):
        self.groups = collections.defaultdict(list)
        self.expected = None
        self.pairs = []
        self.allOddBalls = []
        
    def add(self, value, path):
        assert self.expected is None
        self.groups[value].append(path)

    def finalize(self):
        if self.expected is None:
            pairs = sorted(self.groups.items(), key=lambda pair: len(pair[1]), reverse=True)
            if len(pairs) > 0:
                self.expected = pairs[0][0]
                self.pairs = pairs
                for value, paths in pairs[1:]:
                    for path in paths:
                        self.allOddBalls.append((value, path))

    def getValues(self):
        self.finalize()
        return [v for v, p in self.pairs]
    
    def getPairs(self):
        self.finalize()
        return self.pairs

    def getAllOddBalls(self):
        self.finalize()
        return self.allOddBalls

    def getExpected(self):
        self.finalize()
        return self.expected

class SortedSet:
    def __init__(self):
        self.set = set()
        self.list = []

    def add(self, item):
        if item not in self.set:
            self.set.add(item)
            self.list.append(item)
            
class FileSweeper:
    """ This class is used to help identify files that have inconsistent line endings
        or inconsistent preamble. """
    
    def __init__(self, root, projectName):
        self.root = root
        self.projectName = projectName
        self.expectedLineEnding = None
        self.expectedPreamble = None
        self.previewChanges = False
        self.applyChanges = False
        self.secondPass = False
        self.logCategories = collections.defaultdict(list)            

    def scan(self):
        self.lineEndingGroups = ConsistencyGroup()
        self.preambleGroups = ConsistencyGroup()
        self.missingLineEnding = []
        self.mismatchedGuards = []
        self.missingErrorQuotes = SortedSet()
        self.idCounter = 0
        self.logIdentifiersFixed = SortedSet()
        for dirpath, dirnames, filenames in os.walk(self.root):
            name = os.path.split(dirpath)[1]
            if 'build' in name:  # Skip build folders
                del dirnames[:]
                continue
            for fn in filenames:
                ext = os.path.splitext(fn)[1].lower()
                if ext in ['.h', '.cpp']:
                    self.tidyFile(os.path.join(dirpath, fn), ext)
        if self.expectedLineEnding is None:
            self.expectedLineEnding = self.lineEndingGroups.getExpected()
        self.expectedPreamble = self.preambleGroups.getExpected()
        self.origLogCategories = dict((k, list(v)) for k, v in self.logCategories.iteritems())
        self.logCategories = collections.defaultdict(list)            
        self.secondPass = True

    def report(self):
        anyFixes = False
        if self.lineEndingGroups.getValues() != [self.expectedLineEnding]:
            anyFixes = True
            for ending, files in self.lineEndingGroups.getPairs():
                if ending != self.expectedLineEnding:
                    print
                    print 'These files have inconsistent line endings (%s should be %s):' % (repr(ending), repr(self.expectedLineEnding))
                    for f in files:
                        print f
        if len(self.missingLineEnding) > 0:
            anyFixes = True
            print
            print 'These files are missing the final carriage return:'
            for f in self.missingLineEnding:
                print f
        if len(self.preambleGroups.getAllOddBalls()) > 0:
            anyFixes = True
            print
            print 'These files have inconsistent preambles:'
            for preamble, f in self.preambleGroups.getAllOddBalls():
                print f
        if len(self.mismatchedGuards) > 0:
            anyFixes = True
            print
            print 'These files have mismatched include guards:'
            for f in self.mismatchedGuards:
                print f
        if len(self.missingErrorQuotes.list) > 0:
            anyFixes = True
            print
            print 'These files are missing quotes around #error directives:'
            for f in self.missingErrorQuotes.list:
                print f
        if len(self.logIdentifiersFixed.list) > 0:
            anyFixes = True
            print
            print 'These files need TURF_TRACE identifiers fixed:'
            for f in self.logIdentifiersFixed.list:
                print f
        if not anyFixes:
            print
            print 'Code is already tidy.'

    def tidyFile(self, path, ext):
        with open(path, 'rb') as f:
            lines = f.readlines()
        origLines = list(lines) # Make a copy
        lineEndings = set()
        for i in xrange(len(lines)):
            m = RE_LINEENDING.search(lines[i])
            lineEndings.add(m.group(0))
            if self.secondPass:     # Autofix line endings on second pass
                lines[i] = lines[i][:m.start(0)] + self.expectedLineEnding
        for ending in lineEndings:
            if ending == '':
                self.missingLineEnding.append(path)
            else:
                self.lineEndingGroups.add(ending, path)
        preambleCheckLimit = min(len(lines), 25)
        for preamble in xrange(0, preambleCheckLimit):
            if '-----*/' in lines[preamble]:
                preamble += 2
                break
        else:
            preamble = 0

        # Autofix preamble on second pass
        if 0: #self.secondPass: (disabled for now)
            lines[:preamble] = [p.rstrip() + self.expectedLineEnding for p in DESIRED_PREAMBLE]
            preamble = len(DESIRED_PREAMBLE)

        if ext == '.h':
            # Check include guards
            guard = self.getIncludeGuard(path)
            gotIfNDef = None
            gotDefine = None
            gotEndIf = None
            for s in xrange(preamble, len(lines)):
                if lines[s].strip() == '':
                    continue
                if gotIfNDef is not None and gotEndIf is not None:
                    break
                m = re.match('\\s*#\\s*(define|ifndef)\\s*(.+?)[\r\n]*$', lines[s])
                if not m:
                    break
                if m.group(1) == 'define' and gotDefine is None:
                    gotDefine = m.group(2)
                elif m.group(1) == 'ifndef' and gotIfNDef is None:
                    gotIfNDef = m.group(2)
                else:
                    break
            for e in xrange(len(lines) - 1, s, -1):
                if lines[e].strip() == '':
                    continue
                if gotEndIf is None:
                    m = re.match('\\s*#\\s*endif\\s*(.+?)[\r\n]*$', lines[e])
                    if not m:
                        break
                    gotEndIf = m.group(1)
                else:
                    break
            if guard != gotIfNDef or guard != gotDefine or ('// ' + guard != gotEndIf):
                self.mismatchedGuards.append(path)
                # Autofix include guards on second pass
                if self.secondPass:
                    if not ((gotDefine is not None) and (gotIfNDef is None)):   # Don't try to fix guards that start with a #define
                        lines = lines[:preamble] + \
                            ['#ifndef ' + guard + self.expectedLineEnding] + \
                            ['#define ' + guard + self.expectedLineEnding] + \
                            [self.expectedLineEnding] + \
                            lines[s:e + 1] + \
                            [self.expectedLineEnding] + \
                            ['#endif // ' + guard + self.expectedLineEnding]

        # Check for quotes around #error directives
        for l in xrange(len(lines)):
            line = lines[l]
            m = re.match('\\s*#\\s*error\\s*(.+?)\\s*$', line)
            if m:
                fixedMsg = msg = m.group(1)
                if not fixedMsg.startswith('"'):
                    fixedMsg = '"' + fixedMsg
                if not fixedMsg.endswith('"'):
                    fixedMsg = fixedMsg + '"'
                if msg != fixedMsg:
                    self.missingErrorQuotes.add(path)
                    # Autofix missing #error quotes on second pass
                    if self.secondPass:
                        lines[l] = line[:m.start(1)] + fixedMsg + line[m.end(1):]

        # Assign unique indices to TURF_TRACE statements
        # FIXME: Need a way to write this to the summary
        logCategory = None
        logList = None
        l = 0
        while l < len(lines):
            line = lines[l]
            m = re.match(r'\s*TURF_TRACE_DECLARE\s*\(\s*(\w+?)\s*,', line)
            if m:
                if logCategory is not None:
                    if not self.secondPass:
                        sys.stderr.write('%s(%d): File already has a TURF_TRACE category\n' % (path, l))
                else:
                    logCategory = m.group(1)
                    logList = self.logCategories[logCategory]
                    if self.secondPass:
                        origLogList = self.origLogCategories[logCategory]
                        lines[l] = 'TURF_TRACE_DECLARE(%s, %d)' % (logCategory, len(origLogList)) + self.expectedLineEnding
                l += 1
                continue
            m = re.match(r'\s*TURF_TRACE_DEFINE_BEGIN\s*\(\s*(\w+?)\s*,', line)
            if m:
                if logCategory is not None:
                    if not self.secondPass:
                        sys.stderr.write('%s(%d): File already has a TURF_TRACE category\n' % (path, l))
                    l += 1
                else:
                    logCategory = m.group(1)
                    logList = self.logCategories[logCategory]
                    endl = l + 1
                    while endl < len(lines):
                        m = re.match(r'\s*TURF_TRACE_DEFINE_END', lines[endl])
                        if m:
                            break
                        endl += 1
                    else:
                        endl = l
                    if self.secondPass:
                        origLogList = self.origLogCategories[logCategory]
                        repl = ['TURF_TRACE_DEFINE_BEGIN(%s, %d) // autogenerated by %s' % (logCategory, len(origLogList), SCRIPT_NAME) + self.expectedLineEnding]
                        for desc in origLogList:
                            repl += ['TURF_TRACE_DEFINE("%s")' % desc + self.expectedLineEnding]
                        repl += ['TURF_TRACE_DEFINE_END(%s, %d)' % (logCategory, len(origLogList)) + self.expectedLineEnding]
                        lines[l:endl+1] = repl
                        l += len(repl)
                    else:
                        l = endl + 1
                continue
            m = re.search(r'TURF_TRACE\s*\(.*?"(.*?)"', line)
            if m:
                if logList is None:
                    sys.stderr.write('%s(%d): TURF_TRACE category not declared\n' % (path, l))
                else:
                    logIndex = len(logList)
                    logList.append(m.group(1))
                    if self.secondPass:
                        lines[l] = line[:m.start()] + 'TURF_TRACE(%s, %d, "%s"' % (logCategory, logIndex, m.group(1)) + line[m.end():]
            l += 1

        # Did anything change?
        if origLines != lines:
            if self.previewChanges:
                print
                print 'Changes for: ' + path
                for dl in list(difflib.unified_diff(origLines, lines, n=0))[2:]:
                    sys.stdout.write(dl)
            if self.applyChanges:
                print 'Writing changes to: ' + path
                with open(path, 'wb') as f:
                    f.write(''.join(lines))

    def getIncludeGuard(self, path):
        # FIXME: This messes up when a trailing slash is specified on the root folder
        guard = projectName + path[len(self.root):]
        return '_'.join(re.findall('\\w+', guard)).upper()

# Run a FileSweeper on each path
for root in roots:
    projectName = os.path.split(os.path.abspath(root))[1]
    sweeper = FileSweeper(root, projectName)
    if options.lineEndings:
        sweeper.expectedLineEnding = {'windows': '\r\n', 'unix': '\n'}[options.lineEndings]
    sweeper.scan()  # First pass
    if options.preview or options.write:
        sweeper.previewChanges = options.preview
        sweeper.applyChanges = options.write
        sweeper.scan()  # Second pass
    else:
        sweeper.report()
