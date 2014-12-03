#! /usr/bin/env python

if __name__ == '__main__':
    import GenerateProjectFiles

    raise SystemExit(GenerateProjectFiles.main())

from optparse import OptionParser
import os
import fileinput
import sys
import shutil
from xml.etree import ElementTree as ET
from xml.dom import minidom


def gen_android_proj(cxx_root, out_folder):
    shutil.copyfile(os.getcwd() + "/Android.mk", out_folder + "/Android.mk")
    searchExpCocosRoot = "CCX_ROOT :="
    searchExpGafSources = "GAF_LIB_SOURCES :="
    for line in fileinput.input(out_folder + "/Android.mk", inplace=True):
        if searchExpCocosRoot in line:
            line = "%s %s\n" % (searchExpCocosRoot, cxx_root)
        elif searchExpGafSources in line:
            line = "%s %s/Sources\n" % (searchExpGafSources, os.getcwd())
            line = line.replace("\\", "/")

        sys.stdout.write(line)


def gen_xcode_proj(cxx_root, out_folder):
    shutil.copytree(os.getcwd() + "/GAFPlayer.xcodeproj", out_folder)

    newcwd = out_folder
    project = "project.pbxproj"

    searchExpCCX_ROOT = "CCX_ROOT ="
    searchExpSourcesPath = "path = Sources;"
    searchExpPCH = 'GCC_PREFIX_HEADER = "Sources/GAFPlayer-Prefix.pch";'

    for line in fileinput.input(newcwd + project, inplace=True):
        if searchExpCCX_ROOT in line:
            print 'CCX_ROOT_OVERRIDE = "%s";' % cxx_root
        if searchExpSourcesPath in line:
            line = line.replace(searchExpSourcesPath, "path = " + os.getcwd() + "/Sources;")
        if searchExpPCH in line:
            line = line.replace(searchExpPCH, 'GCC_PREFIX_HEADER = ' + '"' + os.getcwd() + '/Sources/GAFPlayer-Prefix.pch";')
        sys.stdout.write(line)

def gen_vc_proj(ccx_root, out_folder):
    vs_xml_namespace = "http://schemas.microsoft.com/developer/msbuild/2003"
    ET.register_namespace('', vs_xml_namespace)

    shutil.copyfile(os.getcwd() + "/GAFPlayer.vcxproj.filters", out_folder + "/GAFPlayer.vcxproj.filters")

    # props
    props_filename = out_folder + "/Library.props"
    shutil.copyfile(os.getcwd() + "/Library.props", props_filename)
    props_tree = ET.parse(props_filename)
    props_root = props_tree.getroot()

    for ccx_root_param in props_root.iter("{%s}CCX_ROOT" % vs_xml_namespace):
        ccx_root_param.text = ccx_root

    props_tree.write(props_filename)

    # project
    vcxproj_filename = out_folder + "/GAFPlayer.vcxproj"
    shutil.copyfile(os.getcwd() + "/GAFPlayer.vcxproj", vcxproj_filename)

    vcx_tree = ET.parse(vcxproj_filename)
    vcx_root = vcx_tree.getroot()

    for sources_info in vcx_root.iter("{%s}ClCompile" % vs_xml_namespace):
        file_path = sources_info.get('Include')
        if file_path:
            file_path = file_path.replace('Sources', os.getcwd() + '\\Sources')
            sources_info.set("Include", file_path)

    for includes_info in vcx_root.iter("{%s}ClInclude" % vs_xml_namespace):
        file_path = includes_info.get('Include')
        if file_path:
            file_path = file_path.replace('Sources', os.getcwd() + '\\Sources')
            includes_info.set("Include", file_path)

    vcx_tree.write(vcxproj_filename)


def gen_wp8_proj(cxx_root, out_folder):
    print "gen_wp8_proj"


def main():
    parser = OptionParser()
    parser.add_option('-c', '--ccxroot', dest='ccx_root',
                      help='directory of Cocos2d-x root, e.g E:/Projects/Cocos2d-x/')
    parser.add_option('-o', '--out', dest='out_folder',
                      help='where to store generated project files, e.g E:/Projects/GAFLibraryProject')
    opts, args = parser.parse_args()

    out_folder = opts.out_folder

    if out_folder:
        if os.path.exists(out_folder):
            shutil.rmtree(out_folder)

    os.mkdir(out_folder)

    gen_xcode_proj(opts.ccx_root, out_folder + "/GAFPlayer.xcodeproj/")
    gen_android_proj(opts.ccx_root, out_folder)
    gen_vc_proj(opts.ccx_root, out_folder)

