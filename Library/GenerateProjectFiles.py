#! /usr/bin/env python

if __name__ == '__main__':
    import GenerateProjectFiles

    raise SystemExit(GenerateProjectFiles.main())

from optparse import OptionParser
import os
import fileinput
import sys
import shutil
from xml.etree import ElementTree as eT


def gen_android_proj(cxx_root, out_folder):
    shutil.copyfile(os.getcwd() + "/Android.mk", out_folder + "/Android.mk")
    cocos_root_marker = "CCX_ROOT :="
    gaf_sources_marker = "GAF_LIB_SOURCES :="
    for line in fileinput.input(out_folder + "/Android.mk", inplace=True):
        if cocos_root_marker in line:
            line = "%s %s\n" % (cocos_root_marker, cxx_root)
        elif gaf_sources_marker in line:
            line = "%s %s/Sources\n" % (gaf_sources_marker, os.getcwd())
            line = line.replace("\\", "/")

        sys.stdout.write(line)


def gen_xcode_proj(cxx_root, out_folder):
    if os.path.exists(out_folder + "/GAFPlayer.xcodeproj/"):
        shutil.rmtree(out_folder + "/GAFPlayer.xcodeproj/")

    shutil.copytree(os.getcwd() + "/GAFPlayer.xcodeproj", out_folder + "/GAFPlayer.xcodeproj/")

    newcwd = out_folder + "/GAFPlayer.xcodeproj/"
    project = "project.pbxproj"

    cocos_root_marker = "CCX_ROOT ="
    gaf_sources_marker = "path = Sources;"
    pch_marker = 'GCC_PREFIX_HEADER = "Sources/GAFPlayer-Prefix.pch";'

    for line in fileinput.input(newcwd + project, inplace=True):
        if cocos_root_marker in line:
            print 'CCX_ROOT_OVERRIDE = "%s";' % cxx_root
        if gaf_sources_marker in line:
            line = line.replace(gaf_sources_marker, "path = " + os.getcwd() + "/Sources;")
        if pch_marker in line:
            line = line.replace(pch_marker,
                                'GCC_PREFIX_HEADER = "' + os.getcwd() + '/Sources/GAFPlayer-Prefix.pch";')
        sys.stdout.write(line)


def gen_vc_proj(ccx_root, out_folder):
    vs_xml_namespace = "http://schemas.microsoft.com/developer/msbuild/2003"
    eT.register_namespace('', vs_xml_namespace)

    shutil.copyfile(os.getcwd() + "/GAFPlayer.vcxproj.filters", out_folder + "/GAFPlayer.vcxproj.filters")

    # props
    props_filename = out_folder + "/Library.props"
    shutil.copyfile(os.getcwd() + "/Library.props", props_filename)
    props_tree = eT.parse(props_filename)
    props_root = props_tree.getroot()

    for ccx_root_param in props_root.iter(
                    "{%s}CCX_ROOT" % vs_xml_namespace):  # todo - change to find (only one entrance in this file)
        ccx_root_param.text = ccx_root
    for gaf_sources_param in props_root.iter("{%s}GAF_SOURCES_ROOT" % vs_xml_namespace):
        gaf_sources_param.text = os.getcwd() + "\Sources"

    props_tree.write(props_filename)

    # project
    vcxproj_filename = out_folder + "/GAFPlayer.vcxproj"
    shutil.copyfile(os.getcwd() + "/GAFPlayer.vcxproj", vcxproj_filename)

    vcx_tree = eT.parse(vcxproj_filename)
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

    if not opts.ccx_root or not opts.out_folder or len(args) < 1:
        print """
        Usage:
        -c {Cocos2d-x root} -o {Folder to store generated projects} TARGETS
         Where TARGETS can be: all, xcode, android, msvs_wp8, msvs_desktop

         F.E.:
         -c E:\Projects\Cocos2d-x\ -o E:\Projects\GAFLibraryProject\ xcode msvs_desktop
        """

    out_folder = opts.out_folder

    if not out_folder:
        os.mkdir(out_folder)

    if 'all' in args or 'xcode' in args:
        gen_xcode_proj(opts.ccx_root, out_folder)
    if 'all' in args or 'android' in args:
        gen_android_proj(opts.ccx_root, out_folder)
    if 'all' in args or 'msvs_desktop' in args:
        gen_vc_proj(opts.ccx_root, out_folder)
    if 'all' in args or 'msvs_wp8' in args:
        gen_wp8_proj(opts.ccx_root, out_folder)