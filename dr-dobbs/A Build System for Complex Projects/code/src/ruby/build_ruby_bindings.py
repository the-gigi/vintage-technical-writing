import os
import sys
import subprocess

extconf_template = "require 'mkmf'\n create_makefile(\"%s\")"


def build_ruby_binding(project_path):
  """Build a Ruby binding
  
  - Generate an extconf.rb file (configuration file)
  - Run it through Ruby to generate a Makefile
  - Run the Makefile to build the actual binding
  """
  project_path = os.path.abspath(project_path)
  # Verify the project dir exists
  assert os.path.isdir(project_path)
  name = project_path.split('/')[-1]
  # make sure the binding file exists
  assert os.path.isfile(os.path.join(project_path, name + '.c'))

  save_dir = os.getcwd()
  try:
    os.chdir(project_path)

    # Generate the extconf.rb file
    extconf_rb = extconf_template % name
    open('extconf.rb', 'w').write(extconf_rb)

    # Remove exisitng Makefile
    if os.path.isfile('Makefile'):
      os.remove('Makefile')

    # Invoke the extconf.rb file to generate the Makefile
    subprocess.call(['ruby', 'extconf.rb'])
    assert os.path.isfile('Makefile')

    # Remove exisitng bundle and make a new one
    bundle = name + '.bundle'
    if os.path.isfile(bundle):
      os.remove(bundle)
    subprocess.call(['make'])
    assert os.path.isfile(bundle)


  finally:
    os.chdir(save_dir)

def build_all_ruby_bindings(ruby_dir):
  subdirs = os.walk(ruby_dir).next()[1]
  for s in subdirs:
    build_ruby_binding(s)


if __name__=='__main__':
  ruby_dir = '.'
  build_all_ruby_bindings(ruby_dir)
  