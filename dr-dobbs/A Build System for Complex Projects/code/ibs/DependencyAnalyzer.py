#!/usr/bin/env python
import os, sys, re, glob
from pprint import pprint as pp

# Pick up both #include statements
# Also take care of comments following the #include statement
include_re = re.compile('\s*#include [<"](.*)[>"](.*)')


def get_file_includes(text, prefix, search_path):
  """Get the filenames from #include statements in a text

  The text usually comes from a source file. If prefix
  is not empty it will return only include statements
  whose content (following the first quote or angle bracket)
  begins with the prefix.

  The algorithm is to extract the relative filename using a regex
  and then scan the search path an try to append the relative filename
  and see if it exists.
  """
  includes = []
  lines = text.split('\n')
  for line in lines:
    m = include_re.match(line)
    if m is not None:
      includes.append((m.group(1), line, m.group(2)))

  results = []

  for i in includes:
    if not i[0].startswith(prefix):
      continue
    for d in search_path:
      full_path = os.path.join(d, i[0])
      if os.path.exists(full_path):
        results.append((full_path, i[1], i[2]))
        break

  return results


def get_dir_includes(dir_path, prefix, search_path):
  """Get all the filenames from #include statements in all files in a directory

  """
  includes = {}
  for d, subs, files  in os.walk(dir_path):
    for f in files:
      if os.path.splitext(f)[1] not in ('.cpp', '.c', '.h', '.hpp'):
        continue
      filename = os.path.join(d, f)
      text = open(filename).read()
      includes[os.path.join(d, f)] = get_file_includes(text, prefix, search_path)
    #break

  return includes

def starts_with_prefix(filename, prefix, search_path):
  """Check if the filename starts with the prefix.

  The provided prefix isw actually just part of the entireprefix of the
  filename that includes also some directory from the search path. You
  can think of the filename as being composed of these elements:

  <some dir from the search path> + <prefix> + <rest of filename>

  The function goes over each directory in the search path, adds
  the prefix to it and then checks if the absolute filename starts
  with this absolute prefix.
  """
  filename = os.path.abspath(filename)
  for p in search_path:
    if filename.startswith(os.path.join(p, prefix)):
      return True
  return False

def get_file_dependencies(filename, prefix, file_dependencies, search_path):
  """Get all the projects in the prefix dir that the target file depends on

  All the filenames it #include are extracted using get_file_includes()
  The dependencies of each dependency are extracted recursively.

  @filename: the target filename
  @prefix: the prefix of interesting dependencies
  @file_dependencies: the list of dependencies (grows as the function trundles along)
  """
  filename = os.path.abspath(filename)
  if filename in file_dependencies:
    return
  else:
    file_dependencies.append(filename)

  text = open(filename).read()
  includes = get_file_includes(text, prefix, search_path)

  includes = [i[0] for i in includes if starts_with_prefix(i[0], prefix, search_path)]
  for i in includes:
    if i not in file_dependencies:
      get_file_dependencies(i, prefix, file_dependencies, search_path)

def get_project_dependencies(project_dir, libs_dir, search_path, extensions=['.cpp', '.hpp', '.c', '.h']):
  """Get all the projects in the libs dir that the target project depends on

  The algorithm is to get the dependencies of every file in the current project and keep
  a list of all the directories the files reside in.

  @project_dir: the target project
  @libs_dir: the name of the static libraries parent dir (e.g. 'nta')
  @search_path: the list of include directories
  @extensions: the list of file extensions that are checked for dependencies
  """
  files = glob.glob(os.path.join(project_dir, '*.*'))
  all_dependencies = []
  for f in files:
    if not os.path.isfile(f):
      continue
    if not os.path.splitext(f)[1] in extensions:
      continue

    file_dependencies = []
    get_file_dependencies(f, libs_dir, file_dependencies, search_path)
    all_dependencies += file_dependencies

  temp = [os.path.dirname(f) for f in all_dependencies]

  dependencies = []
  for p in temp:
    if not p in dependencies:
      dependencies.append(p)

  dependencies.remove(project_dir)
  
  # Get all recursive dependencies 
  for d in dependencies:
    sub_dependencies = get_project_dependencies(d,
                                                libs_dir,
                                                search_path,
                                                extensions)
    
    for sd in sub_dependencies:
      if not sd in dependencies:
        dependencies.append(sd)
    
    assert not project_dir in dependencies 
  
  return dependencies
