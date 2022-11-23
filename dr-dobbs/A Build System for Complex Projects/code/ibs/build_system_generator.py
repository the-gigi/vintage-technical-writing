#!/usr/bin/env python

import os
import sys
import glob
import string
import shutil
import re
import inspect
import platform
import copy
import DependencyAnalyzer

from pprint import pprint as pp

# Provide access to python build modules
script_dir = os.path.abspath(os.path.dirname(os.path.abspath(__file__)))
source_dir = os.path.normpath(os.path.join(script_dir, os.pardir, 'src'))

# save original sys_path
sys_path = sys.path[:]
sys.path.insert(0, script_dir)

# Globals

def get_caller_info(depth=2):
  f = sys._getframe(depth)
  method_name = f.f_code.co_name
  filename = f.f_code.co_filename

  arg_class = None
  args = inspect.getargvalues(f)
  if len(args[0]) > 0:
    arg_name = args[0][0] # potentially the 'self' arg if its a method
    arg_class = type(args[3][arg_name])
  return (method_name, filename, arg_class)

def title(s=None, additional=''):
  if s is None:
    s = get_caller_info(2)[0]
  print '-' * len(s + additional)
  print s, additional
  print '-' * len(s + additional)

class Template(object):
  """The template class has a template file and a substitution dict

  Applying the substitution dict to template file results in a final build file
  """
  def __init__(self, template_file, relative_path, template_type):
    """
    @param template_file - absolute filename of the template file
    @param relative_path - relative path of this build file from the project dir
    @param template_type - static_lib, dynamic_lib or program
    """
    self.template_file = template_file
    self.relative_path = relative_path
    self.template_type = template_type
    self.template_text = open(template_file).read()
    self.substitution_dict = {}
    self.build_file = ''

  def apply(self):
    # Generate A build file from a template file
    # (using PEP-292 string.Template substitution)
    # convert the template_text to string in case its unicode
    # (VisualStudio .vcproj files)
    t = string.Template(str(self.template_text))
    self.build_file =  t.substitute(self.substitution_dict)

class BaseHelper(object):
  """This class represents a particular build system

  It provides to the Generator the templates and support code
  to populate the templates and create the actual build files.

  Specific build system helpers implement the same interface
  and are passed to the Generator in its __init__ method.
  """
  def __init__(self, templates_dir):
    self.templates_dir = templates_dir
    self.templates = {}
    self.skip_dir = None

  def get_templates(self, template_type):
    """Get all the template files associated with a particular template type

    Often there will be just one template file, but some build systems
    require multiple build files per template type
    """
    templates_dir = os.path.join(self.templates_dir, template_type)
    assert os.path.isdir(templates_dir)
    result = []
    for parent_dir, sub_dirs, files in os.walk(templates_dir):
      # Don't descend into sub-dirs
      if parent_dir != templates_dir:
        break
      for f in files:
        result.append(Template(os.path.join(parent_dir, f), template_type))
    return result

  def get_ignore_string(self):
    """Return a string that will mark files tha should be ignored

    Filenames that include the ignore string will not be collected during
    the source files or header files collection process.

    If None then all files are collected
    """
    return None

  @staticmethod
  def collect_files(project_dir, regexp=None):
    """Collect selectively source and header files under the project dir

    Used by most projects to dynamically collect their source and header
    files. Doesn't recurse into subdirs.
    """
    source_files = []
    header_files = []

    for ext_list, file_list in ((('.cpp', '.c'), source_files),
                                (('.hpp', '.h'), header_files)):
      for ext in ext_list:
        files = glob.glob(os.path.join(project_dir, '*' + ext))
        if regexp:
          files = [f for f in files if regexp.match(os.path.splitext(f)[0])]
        file_list += files

    return (header_files, source_files)

  def prepare_substitution_dict(self,
                                project_name,
                                project_type,
                                project_file_template,
                                project_dir,
                                libs_dir,
                                dependencies,
                                source_files,
                                header_files,
                                platform):
    """Prepare a dictionary of substitution for the input template

    Each placeholder in the template must have a correponding key in the
    dictionary. This method may be called multiple times for each project
    if there are multiple project file templates (e.g. NetBeans).
    """
    return {}

class Project(object):
  """The project class stores all build information of a single project

  A project has a name, a directory, a list of project templates
  and dependencies (other projects) that must be built before the project itself.
  """
  def __init__(self, path):
    self.path = path
    self.name = os.path.basename(path)
    self.templates = {}
    print self.name
    if self.name == 'testHello':
      print
    dependencies = DependencyAnalyzer.get_project_dependencies(path,
                                                              'hw',
                                                               [source_dir])
    self.dependencies = sorted(dependencies)

class Generator(object):
  """The generator is responsible for generating all the build files

  It relies on its knowledge of the directory structure of the entire tree
  under the root dir. Certain sub-dirs of the root dir group projects of the
  same type.
  """
  def __init__(self, root_dir, platform, helper):

    self.root_dir = root_dir
    self.libs_dir = os.path.join(root_dir, 'src/hw')
    self.platform = platform
    self.helper = helper
    self.ignore = helper.get_ignore_string()
    self.projects = {}
    self._populate_project_list()

  def generate_projects(self):
    title()
    for p in self.projects.values():
      self._generate_project(p)

  def _populate_project_list(self):
    """Populate the list of projects to be generated
    """
    title()

    # Map project parent directories to templates
    static_lib_template = self.helper.get_templates('static_lib')
    dynamic_lib_template = self.helper.get_templates('dynamic_lib')
    program_template = self.helper.get_templates('program')
    
    mapping = dict(hw=static_lib_template,
                   dlls=dynamic_lib_template,
                   apps=program_template,
                   test=program_template)


    skip_dir = self.helper.skip_dir
    for parent_dir, templates in mapping.items():
      title(parent_dir)
      for project_dir, s, files in os.walk(os.path.join(source_dir, parent_dir)):
        # Treat only sub-directories directly under the parent dir
        # as projects (except .svn or skip_dir)
        if '.svn' in project_dir or \
           (skip_dir and skip_dir in project_dir) or \
           os.path.basename(os.path.dirname(project_dir)) != parent_dir:
          continue

        assert not project_dir in self.projects
        #print project_dir

        project_type = ''

        project = Project(project_dir)
        project.generated = False
        project.templates = copy.deepcopy(templates)
        self.projects[project.path] = project

  def _fix_path_separators(self, file_list):
    """Convert slashes to the correct form

    Both forward and back slashes are replaced with the proper
    separator from the helper (self.helper.sep). This ensures
    that file paths use "/" on *nix (including Mac OS X) and "\" on windows
    """

    file_list = [l.replace("\\", self.helper.sep) for l in file_list]
    file_list = [l.replace("/", self.helper.sep) for l in file_list]

    return sorted(file_list)

  def _generate_project(self, project):
    #title(additional=os.path.basename(project.path))
    if project.generated:
      return

    project.generated = True

    # Collect source and header files
    project.header_files, project.source_files = self.helper.collect_files(project.path)

    # Bail out on projects with no source and/or header files
    if project.header_files == [] and project.source_files == []:
      return

    # Make sure the paths are correct
    project.source_files = self._fix_path_separators(project.source_files)
    project.header_files = self._fix_path_separators(project.header_files)

    # Make sure all the dependencies are in the project list
    for d in project.dependencies:
      assert d in self.projects

    dependencies = [self.projects[d] for d in project.dependencies]

    # Make sure that everything we depend on already has dependencies generated
    # This is recursive. It terminates as long as we have no circular dependencies
    # @todo detect circular dependency
    for dep in dependencies:
      if dep.generated:
        continue
      self._generate_project(dep)

    # Generate the project build files by creating a substitution dict
    # and applying it to each template file
    h = self.helper


    for t in project.templates:
      #print t.relative_path
      d = h.prepare_substitution_dict(project.name,
                                      t.template_type,
                                      t.template_file,
                                      project.path,
                                      self.libs_dir,
                                      dependencies,
                                      project.source_files,
                                      project.header_files,
                                      self.platform)

      d['Name'] = project.name
      d['name'] = project.name.lower()
      d['Platform'] = self.platform

      t.substitution_dict = d
      #if project.name == 'testWorld':
      #  if t.relative_path == 'nbproject/Makefile-Debug.mk':
      #    print t.template_text
      t.apply()

  def save_projects(self, save):
    title()
    for p in self.projects.values():
      # Save build file only if modified
      for t in p.templates:
        modified = False
        filename = os.path.join(p.path, t.relative_path)
        if '%s' in filename:
          filename = filename % p.name
        if os.path.exists(filename):
          text = open(filename).read()
        else:
          text = None
          modified = True

        if text is not None:
          if t.build_file != text:
            modified = True

        # Save build file file if necessary and different
        if modified and save:
          parent_dir = os.path.dirname(filename)
          if not os.path.exists(parent_dir):
            os.makedirs(parent_dir)
          open(filename, 'w').write(t.build_file)

        # Check generated file
        if modified and not save:
          dirname, basename = os.path.split(filename)
          gen_filename = os.path.join(dirname, 'gen.' + basename)
          open(gen_filename, 'w').write(t.build_file)
          real_lines = open(filename).readlines()
          gen_lines = open(gen_filename).readlines()
          if real_lines != gen_lines:
            assert len(real_lines) ==  len(gen_lines)
            for i in range(min(len(real_lines), len(gen_lines))):
              if real_lines[i] != gen_lines[i]:
                print i
                print "'%s'" % real_lines[i]
                print "'%s'" % gen_lines[i]
                print
          if sorted(real_lines) != sorted (gen_lines):
            print filename
            print
            assert False

  def generate_workspace_files(self):
    title()
    self.helper.generate_workspace_files('hello_world',
                                         source_dir,
                                         self.projects)

def generate_build_files(root_dir, save_project_files):
  def get_platform():
    arch = platform.architecture()[0][:2]
    operating_system = platform.system().lower()
    processor = platform.processor()

    if operating_system == 'darwin' and arch == '32' and processor == 'i386':
      return 'darwin86'
    elif operating_system == 'linux':
      return 'linux' + arch
    elif operating_system == 'windows':
      return 'win' + arch
    else:
      raise Exception('Unsupported platform: ' + platform.platform())

  title()
  # Dynamically import the build system specific helper module
  p = get_platform()
  print 'platform:', p
  build_system = 'VC_2008' if p.startswith('win') else 'NetBeans_6'
  helper_name = build_system + '_helper'
  sys_path = sys.path[:]
  sys.path.insert(0, os.path.join(script_dir, 'helpers'))
  helper_module = __import__(helper_name)

  templates_dir = os.path.join(script_dir, 'project_templates', build_system)
  # Instatiate the Helper class
  helper = getattr(helper_module, 'Helper')(templates_dir)

  g = Generator(root_dir, p, helper)
  g.generate_projects()
  g.save_projects(save_project_files)
  g.generate_workspace_files()


def usage():
  message = \
"""
Purpose:
Generate build files for a particular build system

Usage:
python generate_build_systemy.py <root dir>
                           
<root dir> (mandatory): the root directory of the code tree

--save (optional): if '--save' is specified the generated project files
will replace the exisitng ones. The default is not to save and just to generate
the projects in memory and verify if they are different.
"""
  print message
  sys.exit(1)


def main(argv):
  """Parse command-line arguments and call generate_build_files()

  """
  import getopt
  optionSpec = ['root_dir=']

  save_project_files = True

  root_dir = os.path.join(script_dir, '..')
  
  try:
    opts, args = getopt.gnu_getopt(argv, '', optionSpec)
  except Exception, e:
    print "Error parsing command line: %s" % e
    usage()

  if len(args) > 0:
    usage()

  for (option, val) in opts:
    if option == '--root_dir':
      root_dir = val

  # Convenience. Allows use of "~" in pathnames and other things.
  root_dir = os.path.abspath(os.path.normpath(os.path.expanduser(root_dir)))

  generate_build_files(root_dir, save_project_files)

if __name__=='__main__':
  main(sys.argv[1:])
