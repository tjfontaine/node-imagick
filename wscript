import Options
from os import unlink, symlink, popen
from os.path import exists 

VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.tool_options("compiler_cc")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("compiler_cc")
  conf.check_tool("node_addon")
  conf.check_cfg(package="MagickWand", args='--cflags --libs', uselib_store='MAGICKWAND')

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.cxxflags = ["-g", "-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE"]
  obj.target = "imagick"
  obj.source = "src/imagick.cc"
  obj.uselib = ['MAGICKWAND']
