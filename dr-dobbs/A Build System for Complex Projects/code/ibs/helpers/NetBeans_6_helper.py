#!/usr/bin/env python
import os, sys, string
from pprint import pprint as pp

sys.path.insert(0, os.path.join(os.path.abspath(os.path.dirname(__file__)), '../'))
from build_system_generator import (BaseHelper,
                                    Template,
                                    title)

class Helper(BaseHelper):
  """NetBeans6 helper
  """
  def __init__(self, templates_dir):
    BaseHelper.__init__(self, templates_dir)
    self.sep = '/'
    self.linesep = '\n'
    self.skip_dir = 'nbproject'

  def get_templates(self, template_type):
    """Get all the template files associated with a particular template type

    Often there will be just one template file, but some build systems
    require multiple build files per template type

    @param template_type - 'program', 'dynamic_lib' or 'static_lib'
    @return a Template object

    """
    result = [Template(os.path.join(self.templates_dir,
                                    template_type,
                                    'Makefile'),
                                    'Makefile',
                                    template_type)]

    nb_project = os.path.join(self.templates_dir, template_type, 'nbproject')
    assert os.path.isdir(nb_project)
    for f in os.listdir(nb_project):
      project_file_template = os.path.join(nb_project, f)
      if not os.path.isfile(project_file_template):
        continue

      filename = os.path.join(nb_project, f)
      relative_path = '/'.join(['nbproject', f])
      result.append(Template(filename, relative_path, template_type))
    return result

  def _prepare_ruby_substitution_dict(self,
                                project_name,
                                project_type,
                                project_file_template,
                                project_dir,
                                libs_dir,
                                dependencies,
                                source_files,
                                header_files,
                                platform):
    """
    """
    

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

    def prepare_makefile(conf, operating_system):
      compile_flag = '-g' if conf == 'Debug' else '-O2'

      d = dict(Name=project_name)
      object_file_template = '	${OBJECTDIR}/%s.o \\\n'
      object_files = ''
      for f in source_files:
        f = os.path.splitext(os.path.basename(f))[0]
        object_files += object_file_template % f

      # Flag for dynamic libraries
      fpic = '-fPIC  ' if project_type == 'dynamic_lib' else ''

      # Get rid of last forward slash
      if len(object_files) > 2:
        object_files = object_files[:-3]
      d['ObjectFiles'] = object_files

      compile_file_template = \
        '$${OBJECTDIR}/${File}.o: ${File}.cpp \n' + \
        '\t$${MKDIR} -p $${OBJECTDIR}\n' + \
        '\t$$(COMPILE.cc) ${CompileFlag} -I../.. ${FPIC}-o $${OBJECTDIR}/${File}.o ${File}.cpp\n\n'

      t = string.Template(compile_file_template)
      compile_files = ''
      for f in source_files:
        f = os.path.splitext(os.path.basename(f))[0]
        text = t.substitute(dict(File=f,
                                 CompileFlag=compile_flag,
                                 Platform=platform,
                                 FPIC=fpic))
        compile_files += text

      # Get rid of the last two \n\n.
      compile_files = compile_files[:-2]
      d['CompileFiles'] = compile_files

      link_command = ''
      if project_type == 'dynamic_lib':
        if platform.startswith('darwin'):
          link_command = '${LINK.cc} -dynamiclib -install_name lib%s.dylib' % project_name
        else:
          assert platform.startswith('linux')
          link_command = '${LINK.c} -shared'
        d['LinkCommand'] = link_command

      ldlibsoptions = ''
      if dependencies != []:
        ldliboption_template = '../../hw/%s/dist/%s/GNU-%s/lib%s.a'

        ldlibsoptions = ' '.join([ldliboption_template % \
                        (dep.name, conf, operating_system, dep.name)
                        for dep in dependencies])
        if operating_system == 'Linux':
          ldlibsoptions += ' -ldl'
      d['LDLIBSOPTIONS'] = ldlibsoptions

      build_subproject_template = '\tcd ../../hw/%s && ${MAKE}  -f Makefile CONF=%s'
      clean_subproject_template = build_subproject_template + ' clean'

      build_list = [build_subproject_template % (dep.name, conf) for dep in dependencies]
      clean_list = [clean_subproject_template % (dep.name, conf) for dep in dependencies]
      d['BuildSubprojects'] = '\n'.join(build_list)
      d['CleanSubprojects'] = '\n'.join(clean_list)
      d['OperatingSystem'] = operating_system
      d['DynamicLibExtension'] = ext

      return d

    def prepare_configurations_xml(operating_system):
      d = {}
      # Make sure the platform ID matches NetBeans expectations
      # To figure out the platform ID for new platforms just
      # create a project manually and check the configurations.xml file.
      d['PlatformID'] = 2 if operating_system.lower().startswith('linux') else 4
      file_template = '      <itemPath>%s</itemPath>\n'
      cpp_files = ''
      for f in source_files:
        f = os.path.basename(f)
        cpp_files += file_template % f
      #d['CppFiles'] = cpp_files[:-1]
      d['CppFiles'] = cpp_files

      hpp_files = ''
      for f in header_files:
        f = os.path.basename(f)
        hpp_files += file_template % f
      #d['HppFiles'] = hpp_files[:-1]
      d['HppFiles'] = hpp_files

      # Magic numbers used by NetBeans (probably enums)
      cpp_tool = 1
      hpp_tool = 3
      tool_item_template = \
        '      <item path="%s">\n' \
        '        <itemTool>%d</itemTool>\n' \
        '      </item>\n'

      tool_cpp_files = ''
      for f in source_files:
        f = os.path.basename(f)
        tool_cpp_files += tool_item_template % (f, cpp_tool)
      d['ToolCppFiles'] = tool_cpp_files

      tool_hpp_files = ''
      for f in header_files:
        f = os.path.basename(f)
        tool_hpp_files += tool_item_template % (f, hpp_tool)
      d['ToolHppFiles'] = tool_hpp_files

      # Inialize the linker tool section (with special case for dynamic libs)
      if project_type == 'dynamic_lib':
        linker_tool_debug =                                              \
          '        <linkerTool>\n'                                       \
          '          <linkerLibItems>\n'                                 \
          '          </linkerLibItems>\n'                                \
          '        </linkerTool>\n'
      else:
        linker_tool_debug = ''
      linker_tool_release = linker_tool_debug

      if dependencies != []:
        linker_lib_item_template = ' ' * 12 + \
          '<linkerLibLibItem>%s</linkerLibLibItem>\n'

        output = ''
        linker_tool_template =                                           \
          '        <linkerTool>\n' +                                     \
          output +                                                       \
          '          <linkerLibItems>\n'                                 \
          '${LinkerProjectItems}${LinkerLibItems}'                       \
          '          </linkerLibItems>\n'                                \
          '        </linkerTool>'

        linker_project_item_template = \
          '            <linkerLibProjectItem>\n'                                             \
          '              <makeArtifact PL="../../hw/${Lib}"\n'                               \
          '                            CT="3"\n'                                             \
          '                            CN="$${Conf}"\n'                                    \
          '                            AC="$${IsDebugConf}"\n'                             \
          '                            BL="true"\n'                                          \
          '                            WD="../../hw/${Lib}"\n'                               \
          '                            BC="$$$${MAKE}  -f Makefile CONF=$${Conf}"\n'       \
          '                            CC="$$$${MAKE}  -f Makefile CONF=$${Conf} clean"\n' \
          '                            OP="dist/$${Conf}/GNU-$${OperatingSystem}/lib${Lib}.a">\n'       \
          '              </makeArtifact>\n'                                                  \
          '            </linkerLibProjectItem>\n'

        linker_lib_items = ''
        linker_project_items = ''
        for dep in dependencies:
          if os.path.isdir(os.path.join(libs_dir, dep.name)):
            lib_dict = dict(Lib=dep.name)
            linker_project_items += \
              string.Template(linker_project_item_template).substitute(lib_dict)
          else:
            linker_lib_items += linker_lib_item_template % dep.name

        linker_dict = dict(LinkerLibItems=linker_lib_items)

        linker_dict['LinkerProjectItems'] = \
          string.Template(linker_project_items).substitute(dict(Conf='Debug',
                                                           IsDebugConf='true',
                                                           OperatingSystem=operating_system))
        linker_dict['Conf'] = 'Debug'
        linker_tool_debug = \
          string.Template(linker_tool_template).substitute(linker_dict)

        linker_dict['LinkerProjectItems'] = \
          string.Template(linker_project_items).substitute(dict(Conf='Release',
                                                           IsDebugConf='false',
                                                           OperatingSystem=operating_system))
        linker_dict['Conf'] = 'Release'
        linker_tool_release = \
          string.Template(linker_tool_template).substitute(linker_dict)

      d ['LinkerToolDebug'] = linker_tool_debug
      d ['LinkerToolRelease'] = linker_tool_release

      return d

    def prepare_project_properties():
      d = {}
      return d

    def prepare_project_xml(dependencies):
      if not dependencies:
        return dict(MakeDepProjects='            <make-dep-projects />\n')

      make_dep_project_template = \
        ' ' * 16 + '<make-dep-project>../../hw/%s</make-dep-project>'

      make_dep_projects_template = """\
            <make-dep-projects>
%s
            </make-dep-projects>
"""
      dependencies = sorted(d.name for d in dependencies)
      make_deps = [make_dep_project_template % d for d in dependencies]

      make_deps = make_dep_projects_template % '\n'.join(make_deps)
      return dict(MakeDepProjects=make_deps)


    if platform.startswith('darwin'):
      operating_system = 'MacOSX'
      ext = 'dylib'
    elif platform.startswith('linux'):
      operating_system = 'Linux'
      ext = 'so'

    temaplate_name = os.path.basename(project_file_template)
    if temaplate_name ==  'Makefile':
      return {}

    if temaplate_name ==  'Makefile-Debug.mk':
      return prepare_makefile('Debug', operating_system)

    if temaplate_name ==  'Makefile-Release.mk':
      return prepare_makefile('Release', operating_system)

    if temaplate_name == 'Makefile-impl.mk':
      return dict(Name=os.path.basename(project_dir))

    if temaplate_name == 'configurations.xml':
      return prepare_configurations_xml(operating_system)

    if temaplate_name == 'project.properties':
      return prepare_project_properties()

    if temaplate_name == 'project.xml':
      return prepare_project_xml(dependencies)

    assert False, 'Invalid project file template: ' + temaplate_name
    return {}

  def generate_workspace_files(self, name, root_path, projects):
    """Generate a NetBeans project group for all the generated projects

    """
    base_path = \
      '~/.netbeans/6.7/config/Preferences/org/netbeans/modules/projectui'
    base_path = os.path.expanduser(base_path)

    if not os.path.exists(base_path):
      os.makedirs(base_path)

    # Create a project group
    groups_path = os.path.join(base_path, 'groups')
    if not os.path.exists(groups_path):
      os.makedirs(groups_path)

    text = """\
name=%s
kind=directory
path=file\:%s"""
    group_filename = os.path.join(groups_path, name + '.properties')
    open(group_filename, 'w').write(text % (name, root_path))

    # Make it the active project
    text = 'active=' + name
    open(os.path.join(base_path, 'groups.properties'), 'w').write(text)

if __name__=='__main__':
  print 'Done'
  pass
  # Test static libs
  #dependencies = []
  #test_prepare_substitution_dict('src/hw', ('hello', 'world', 'utils'), dependencies, 'static_lib')

  # Test dynamic libs
  #dependencies = []
  #test_prepare_substitution_dict('src/dlls', ('punctuator',), dependencies, 'dynamic_lib')

  #dependencies = ['utils', 'hello', 'world']
  #test_prepare_substitution_dict('src/apps', ('hello_world',), dependencies, 'program')
