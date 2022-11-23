#!/usr/bin/env python
import os
import sys
import glob
import string
import uuid
from pprint import pprint as pp

sys.path.insert(0, os.path.join(os.path.abspath(os.path.dirname(__file__)), '../'))
from build_system_generator import (BaseHelper,
                                    Template,
                                    title)

def make_guid():
  #title()
  return '{' + str(uuid.uuid4()) + '}'

def get_guid(filename):
  #title(additional=filename)
  if os.path.isfile(filename):
    lines = open(filename).readlines()
    guid_line = lines[5]
    assert 'ProjectGUID=' in guid_line
    g = guid_line.split('=')[1][1:-2]
  else:
    g = make_guid()

  return g.upper()

class Helper(BaseHelper):
  """VC++ 2008 helper
  """
  def __init__(self, templates_dir):
    BaseHelper.__init__(self, templates_dir)
    self.sep = '\\'

  def get_templates(self, template_type):
    """Get all the template files associated with a particular template type

    Often there will be just one template file, but some build systems
    require multiple build files per template type

    @param template_type - 'program', 'dynamic_lib' or 'static_lib'
    @return a Template object
    """
    result = []

    vcproj_file = os.path.join(self.templates_dir,
                               template_type,
                               '%s.vcproj' % template_type)

    assert os.path.isfile(vcproj_file)
    relative_path = '%s.vcproj'
    template = Template(vcproj_file, relative_path, template_type)
    return [template]

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
    def make_files_section(file_list):
      def icase_cmp(s1, s2):
        return cmp(s1.lower(), s2.lower())
      file_template = """\
\t\t\t<File
\t\t\t\tRelativePath=".\%s"
\t\t\t\t>
\t\t\t</File>"""

      if file_list == []:
        return ''
      file_list = sorted(file_list, icase_cmp)
      files = [file_template % os.path.basename(f) for f in file_list]
      return '\n'.join(files) + '\n'

    filename = os.path.join(project_dir, project_name + '.vcproj')
    return dict(GUID=get_guid(filename),
                SourceFiles=make_files_section(source_files),
                HeaderFiles=make_files_section(header_files))

  def generate_workspace_files(self, solution_name, root_path, projects):
    """Generate a VC++ 2008 solution file

    """
    folders = {}
    for d, subdirs, files in os.walk(root_path):
      if os.path.dirname(d) != root_path:
        continue
      folder_projects = []
      for s in subdirs:
        project_dir = os.path.join(d, s)
        if not project_dir in projects:
          continue
        vcproj_filename = os.path.join(project_dir,
                                       os.path.basename(s) + '.vcproj')
        assert os.path.isfile(vcproj_filename)

        guid = get_guid(vcproj_filename)

        # Get the directories of of all the dependency projects
        dependencies = projects[project_dir].dependencies

        # Get the GUIDs of all the dependency projects
        dependencies = [get_guid(os.path.join(dep, os.path.basename(dep) + '.vcproj'))
                                 for dep in dependencies]
        folder_projects.append(SolutionItem(item_type=project_type,
                                            name=s,
                                            path=vcproj_filename,
                                            guid=guid,
                                            dependencies=dependencies,
                                            projects=[]))

      guid = '?'
      if folder_projects != []:
        name = os.path.basename(d)
        print name
        folder = SolutionItem(name=name,
                              item_type=folder_type,
                              path=None,
                              guid=guid,
                              dependencies=[],
                              projects=folder_projects)
        folders[os.path.basename(d)] = folder

    gen_solution = make_solution(root_path, folders)
    #solution_filename = os.path.join(root_path, 'gen.' + solution_name + '.sln')
    solution_filename = os.path.join(root_path, solution_name + '.sln')
    open(solution_filename, 'w').write(gen_solution)

    #lines1 = open(os.path.join(root_path, solution_name + '.sln')).readlines()
    #lines2 = open(solution_filename).readlines()
    #
    #print len(lines1), len(lines2)
    #
    #lines1 = sorted(zip(range(len(lines1)), lines1))
    #lines2 = sorted(zip(range(len(lines2)), lines2))
    #
    #for i,line in enumerate(zip(lines1, lines2)):
    #  if line[0][1] != line[1][1]:
    #    print i
    #    print line[0]
    #    print line[1]
    #    print

    #print gen_solution
    #print


# A project template has a header and a list of project sections
# such as ProjectDependencies. The ProjectDependencies
# duplicate the dependency information in .vcproj files in VS2005.
# For generating a solution that contains only C++ projects, no other
# project section is needed.
project_template_with_dependencies = """\
Project("${TypeGUID}") = "${Name}", "${Filename}", "${GUID}"
	ProjectSection(ProjectDependencies) = postProject
${ProjectDependencies}
	EndProjectSection
EndProject
"""

project_template_without_dependencies = """\
Project("${TypeGUID}") = "${Name}", "${Filename}", "${GUID}"
EndProject
"""


project_configuration_platform_template = """\
\t\t${GUID}.Debug|Win32.ActiveCfg = Debug|Win32
\t\t${GUID}.Debug|Win32.Build.0 = Debug|Win32
\t\t${GUID}.Release|Win32.ActiveCfg = Release|Win32
\t\t${GUID}.Release|Win32.Build.0 = Release|Win32
"""

# This is the solution template for VS 2008
# The template arguments are:
#
# Projects
# ProjectConfigurationPlatforms
# NestedProjects
#
solution_template = """
Microsoft Visual Studio Solution File, Format Version 10.00
# Visual Studio 2008
${Projects}
Global
\tGlobalSection(SolutionConfigurationPlatforms) = preSolution
\t\tDebug|Win32 = Debug|Win32
\t\tRelease|Win32 = Release|Win32
\tEndGlobalSection
\tGlobalSection(ProjectConfigurationPlatforms) = postSolution
${Configurations}
\tEndGlobalSection
\tGlobalSection(SolutionProperties) = preSolution
\t\tHideSolutionNode = FALSE
\tEndGlobalSection
\tGlobalSection(NestedProjects) = preSolution
${NestedProjects}
\tEndGlobalSection
EndGlobal
"""

# Guids for regular project and solution folder
project_type = '{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}'
folder_type = '{2150E333-8FDC-42A3-9474-1A3956D46DE8}'

class SolutionItem(object):
  """Represents a solution folder or project

  The set of solution projects contain all the information
  necessary to generate a solution file.

  name - the name of the project/folder
  type - folder_type or project_type
  path - the relative path from the root dir to the .vcproj file for projects,
         same as name for folders
  guid - the GUID of the project/folder
  dependencies - A list of project guids the project depends on.
    It is empty for folders and projects with no dependencies.

  projects - list of projects hosted by the folder. It is empty for projects.
  """
  def __init__(self, item_type, name, path, guid, dependencies, projects):
    #title()
    self.name = name
    self.type = item_type
    self.path = path
    self.guid = guid
    self.dependencies = dependencies
    self.projects = projects

def make_solution(source_dir, folders):
  """Return a string representing the .sln file

  It uses a lot of nested functions to make the different parts
  of a solution file:
   - make_project_dependencies
   - make_projects
   - make_configurations
   - make nested_projects

  @param folders - a dictionary whose keys are VS folders and the values
    are the projects each folder contains. Each project must be an object that
    has a directory path (relative to the root dir), a guid and a
    list of dependencies (each dependency is another projects). This directory
    should contain a .vcproj file whose name matches the directory name.
  @param projects - a list of projects that don't have a folder and are contained
    directly by the solution node.
  """

  def get_existing_folders(sln_filename):
    """Return the names and guids of current folders

    This is useful for generating a .sln file with identical guids.
    """
    title()
    lines = open(sln_filename).readlines()
    results = {}
    for line in lines:
      if line.startswith('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") ='):
        tokens = line.split('"')
        #print tokens
        name = tokens[-4]
        guid = tokens[-2]
        results[name] = guid

    return results

  def make_project_dependencies(dependency_guids):
    """Return a string representing all the dependencies in
    a ProjectDependencies sub-section

    @param dependency_guids - A list of GUIDs of all the dependencies
    """
    #title()
    if dependency_guids == []:
      return ''

    result = []
    for g in dependency_guids:
      result.append('\t\t%s = %s' % (g, g))

    result = '\n'.join(result)
    return result

  def make_projects(source_dir, projects):
    """Return a string representing all the projects in the .sln file

    It works by iterating over the projects and populating
    the project_template. Note that some projects are actually just
    solution folders. It is the responsibility of the caller to pass
    proper project objects that has type, name, filename, guid and dependencies
    attributes.
    """
    title()
    result = ''
    t1 = string.Template(project_template_with_dependencies)
    t2 = string.Template(project_template_without_dependencies)
    for p in projects:
      if p.type == project_type:
        filename = p.path[len(source_dir) + 1:].replace('/', '\\')
      else:
        filename = p.name
      dependency_guids = [get_guid(p.path) for d in p.dependencies]
      guid = get_guid(filename) if p.guid is None else p.guid
      d = dict(TypeGUID=p.type,
               Name=p.name,
               Filename=filename,
               GUID=guid,
               ProjectDependencies=make_project_dependencies(p.dependencies))
      t = t1 if p.dependencies != [] else t2
      s = t.substitute(d)
      result += s

    return result[:-1]

  def make_configurations(projects):
    """Return a string representing all the project configuration platforms
       in the .sln file

    It works by iterating over the projects and populating
    the project_configuration_platform template.
    """
    title()
    result = ''
    t = string.Template(project_configuration_platform_template)
    for p in projects:
      d = dict(GUID=p.guid)
      s = t.substitute(d)
      result += s

    return result[:-1]

  def make_nested_projects(folders):
    """Return a string representing all the nested projects
       in the .sln file

    It works by iterating over the folders and populating
    the nested_project template with the guids of each nested project
    and its containing folder.

    Each folder is an object that has guid attribute and a projects
    attribute (which is a list of its contained projects)
    """
    title()
    for f in folders.values():
      assert hasattr(f, 'guid') and type(f.guid) == str
      assert hasattr(f, 'projects') and type(f.projects) in (list, tuple)

    result = ''
    nested_project = '\t\t${GUID} = ${FolderGUID}\n'
    t = string.Template(nested_project)
    for folder in folders.values():
      for p in folder.projects:
        d = dict(GUID=p.guid, FolderGUID=folder.guid)
        s = t.substitute(d)
        result += s

    return result[:-1]

  # Folders
  title()
  try:
    sln_filename = glob.glob(os.path.join(source_dir, '*.sln'))[0]
    existing_folders = get_existing_folders(sln_filename)
  except:
    existing_folders = []

  # Use folders GUIDs from existing .sln file (if there is any)
  for name, f in folders.items():
    if name in existing_folders:
      f.guid = existing_folders[name]
    else:
      f.guid = make_guid()

  # Prepare a flat list of all projects
  all_projects =[]
  for f in folders.values():
    all_projects.append(f)
    all_projects += f.projects

  pp([p.name for p in all_projects])

  # Prepare the substitution dict for the solution template
  projects = [p for p in all_projects if p.type == project_type]
  all_projects = make_projects(source_dir, all_projects)

  configurations = make_configurations(projects)
  nested_projects = make_nested_projects(folders)
  d = dict(Projects=all_projects,
           Configurations=configurations,
           NestedProjects=nested_projects)

  # Create the final solution text by substituting the dict into the template
  t = string.Template(solution_template)
  solution = t.substitute(d)

  return solution

if __name__=='__main__':
  pass
  # Test static libs
  #dependencies = []
  #test_prepare_substitution_dict('src/hw', ('hello', 'world', 'utils'), dependencies, 'static_lib')

  # Test dynamic libs
  #dependencies = []
  #test_prepare_substitution_dict('src/dlls', ('punctuator',), dependencies, 'dynamic_lib')

  #dependencies = ['utils', 'hello', 'world']
  #test_prepare_substitution_dict('src/apps', ('hello_world',), dependencies, 'program')
