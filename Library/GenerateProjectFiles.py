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


def gen_win32_bindings_proj(ccx_root, out_folder):
    print " * Generating JS Bindings project"

    vs_xml_namespace = "http://schemas.microsoft.com/developer/msbuild/2003"
    eT.register_namespace('', vs_xml_namespace)
    proj_filters_src = os.path.abspath("../js_bindings/proj.win32/libGAFJSBinding.vcxproj.filters")
    proj_filters_dst = os.path.join(out_folder, "libGAFJSBinding.vcxproj.filters")
    shutil.copyfile(proj_filters_src,  proj_filters_dst)

    # project
    proj_src = os.path.abspath("../js_bindings/proj.win32/libGAFJSBinding.vcxproj")
    proj_dst = os.path.join(out_folder, "libGAFJSBinding.vcxproj")
    shutil.copyfile(proj_src, proj_dst)

    vcx_tree = eT.parse(proj_dst)
    vcx_root = vcx_tree.getroot()
    info_occurrences = 0
    for sources_info in vcx_root.iter("{%s}ClCompile" % vs_xml_namespace):
        file_path = sources_info.get('Include')
        if file_path:
            file_path = file_path.replace('..\\bindings', os.path.abspath('../js_bindings/bindings'))
            sources_info.set("Include", file_path)
            info_occurrences += 1

    for includes_info in vcx_root.iter("{%s}ClInclude" % vs_xml_namespace):
        file_path = includes_info.get('Include')
        if file_path:
            file_path = file_path.replace('..\\bindings', os.path.abspath('../js_bindings/bindings'))
            includes_info.set("Include", file_path)
            info_occurrences += 1

    vcx_tree.write(proj_dst)

    print " ** Replaced occurrences [Source Files] - %d" % info_occurrences
    print " * JS Bindings project generated\n"


def gen_android_proj(ccx_root, out_folder):
    print " * Generating Android project"

    shutil.copyfile(os.path.join(os.getcwd(), "Android.mk"), os.path.join(out_folder, "Android.mk"))
    cocos_root_marker = "CCX_ROOT :="
    gaf_sources_marker = "GAF_LIB_SOURCES :="
    info_occurrences = [0] * 2
    for line in fileinput.input(os.path.join(out_folder, "Android.mk"), inplace=True):
        if cocos_root_marker in line:
            line = "%s %s\n" % (cocos_root_marker, ccx_root)
            info_occurrences[0] += 1
        elif gaf_sources_marker in line:
            line = "%s %s/Sources\n" % (gaf_sources_marker, os.getcwd())
            line = line.replace("\\", "/")
            info_occurrences[1] += 1
        sys.stdout.write(line)

    print " ** Replaced occurrences [Cocos2d-x root] - %d, [GAF Sources] - %d" % (info_occurrences[0], info_occurrences[1])
    print " * Android project generated\n"


def gen_xcode_proj(ccx_root, out_folder):
    print " * Generating XCode project"

    newcwd = os.path.join(out_folder, "GAFPlayer.xcodeproj/")

    if os.path.exists(newcwd):
        print " ** Remove existing folder"
        shutil.rmtree(newcwd)

    shutil.copytree(os.path.join(os.getcwd(), "GAFPlayer.xcodeproj"), newcwd)

    project = "project.pbxproj"

    cocos_root_marker = "CCX_ROOT ="
    gaf_sources_marker = "path = Sources;"
    pch_marker = 'GCC_PREFIX_HEADER = "Sources/GAFPlayer-Prefix.pch";'
    info_occurrences = [0] * 3
    for line in fileinput.input(newcwd + project, inplace=True):
        if cocos_root_marker in line:
            print 'CCX_ROOT_OVERRIDE = "%s";' % ccx_root
            info_occurrences[0] += 1
        if gaf_sources_marker in line:
            line = line.replace(gaf_sources_marker, "path = %s;" % os.path.join(os.getcwd(), "Sources"))
            info_occurrences[1] += 1
        if pch_marker in line:
            line = line.replace(pch_marker,
                                'GCC_PREFIX_HEADER = "%s";' % os.path.join(os.getcwd(), 'Sources/GAFPlayer-Prefix.pch'))
            info_occurrences[2] += 1
        sys.stdout.write(line)

    print " ** Replaced occurrences [Cocos2d-x root] - %d, [GAF Sources] - %d, [PCH] - %d" % (info_occurrences[0], info_occurrences[1], info_occurrences[2])
    print " * XCode project generated\n"


def gen_vc_proj(ccx_root, out_folder):
    print " * Generating VS project"

    vs_xml_namespace = "http://schemas.microsoft.com/developer/msbuild/2003"
    eT.register_namespace('', vs_xml_namespace)

    shutil.copyfile(os.path.join(os.getcwd(), "GAFPlayer.vcxproj.filters"), os.path.join(out_folder, "GAFPlayer.vcxproj.filters"))

    gen_vs_props_file(ccx_root, out_folder)

    # project
    vcxproj_filename = os.path.join(out_folder, "GAFPlayer.vcxproj")
    shutil.copyfile(os.path.join(os.getcwd(), "GAFPlayer.vcxproj"), vcxproj_filename)

    vcx_tree = eT.parse(vcxproj_filename)
    vcx_root = vcx_tree.getroot()
    info_occurrences = 0
    for sources_info in vcx_root.iter("{%s}ClCompile" % vs_xml_namespace):
        file_path = sources_info.get('Include')
        if file_path:
            file_path = file_path.replace('Sources', os.path.join(os.getcwd(), 'Sources'))
            sources_info.set("Include", file_path)
            info_occurrences += 1

    for includes_info in vcx_root.iter("{%s}ClInclude" % vs_xml_namespace):
        file_path = includes_info.get('Include')
        if file_path:
            file_path = file_path.replace('Sources', os.path.join(os.getcwd(), 'Sources'))
            includes_info.set("Include", file_path)
            info_occurrences += 1

    vcx_tree.write(vcxproj_filename)

    print " ** Replaced occurrences [GAF Source Files] - %d" % info_occurrences
    print " * VS project generated\n"


def gen_wp8_proj(ccx_root, out_folder):
    print " * Generating VS WP8 project"

    vs_xml_namespace = "http://schemas.microsoft.com/developer/msbuild/2003"
    eT.register_namespace('', vs_xml_namespace)

    gen_vs_props_file(ccx_root, out_folder)

    win8_project_folder = os.path.join(out_folder, "proj.wp8/")
    if os.path.exists(win8_project_folder):
        print " ** Remove existing folder"
        shutil.rmtree(win8_project_folder)
    os.mkdir(win8_project_folder)
    required_files = ('GAFPlayer.vcxproj', 'GAFPlayer.vcxproj.filters', 'pch.cpp', 'pch.h', 'targetver.h')
    for file_name in required_files:
        shutil.copyfile(os.path.join(os.getcwd(), "proj.wp8", file_name), os.path.join(win8_project_folder, file_name))

    vcxproj_filename = os.path.join(win8_project_folder, "GAFPlayer.vcxproj")

    vcx_tree = eT.parse(vcxproj_filename)
    vcx_root = vcx_tree.getroot()
    info_occurrences = 0
    for sources_info in vcx_root.iter("{%s}ClCompile" % vs_xml_namespace):
        file_path = sources_info.get('Include')
        if file_path:
            file_path = file_path.replace('..\Sources', os.path.join(os.getcwd(), 'Sources'))
            sources_info.set("Include", file_path)
            info_occurrences += 1

    for includes_info in vcx_root.iter("{%s}ClInclude" % vs_xml_namespace):
        file_path = includes_info.get('Include')
        if file_path:
            file_path = file_path.replace('..\Sources', os.path.join(os.getcwd(), 'Sources'))
            includes_info.set("Include", file_path)
            info_occurrences += 1

    vcx_tree.write(vcxproj_filename)

    print " ** Replaced occurrences [GAF Source Files] - %d" % info_occurrences
    print " * VS WP8 project generated\n"


def gen_vs_props_file(ccx_root, out_folder):
    vs_xml_namespace = "http://schemas.microsoft.com/developer/msbuild/2003"
    eT.register_namespace('', vs_xml_namespace)

    # props
    props_filename = os.path.join(out_folder, "Library.props")
    shutil.copyfile(os.path.join(os.getcwd(), "Library.props"), props_filename)
    props_tree = eT.parse(props_filename)
    props_root = props_tree.getroot()

    info_occurrences = [0] * 2
    for ccx_root_param in props_root.iter(
                    "{%s}CCX_ROOT" % vs_xml_namespace):  # todo - change to find (only one entrance in this file)
        ccx_root_param.text = ccx_root
        info_occurrences[0] += 1
    for gaf_sources_param in props_root.iter("{%s}GAF_SOURCES_ROOT" % vs_xml_namespace):
        gaf_sources_param.text = os.path.join(os.getcwd(), "Sources")
        info_occurrences[1] += 1

    props_tree.write(props_filename)

    print " ** Props file changed. Replaced occurrences: [Cocos2d-x root] - %d, [GAF Sources] - %d" % (info_occurrences[0], info_occurrences[1])


def main():
    parser = OptionParser()
    parser.add_option('-c', '--ccxroot', dest='ccx_root',
                      help='directory of Cocos2d-x root, e.g E:/Projects/Cocos2d-x/')
    parser.add_option('-o', '--out', dest='out_folder',
                      help='where to store generated project files, e.g E:/Projects/GAFLibraryProject')
    parser.add_option('-j', '--jsbindings', action="store_true", dest='jsbindings', default=False,
                      help='whether to generate project for js bindings')
    opts, args = parser.parse_args()

    if len(args) < 1:
        args = ('all', )

    if not opts.ccx_root or not opts.out_folder:
        print """
        Usage:
        -c {Cocos2d-x root} -o {Folder to store generated projects} TARGETS
         Where TARGETS can be: all, xcode, android, msvs_wp8, msvs_desktop

         F.E.:
         -c E:\Projects\Cocos2d-x\ -o E:\Projects\GAFLibraryProject\ xcode msvs_desktop
        """
        return

    out_folder = opts.out_folder

    if os.getcwd() in os.path.abspath(out_folder):
        print "Please specify not child folder"
        return

    if not out_folder:
        os.mkdir(out_folder)

    if opts.jsbindings:
        gen_win32_bindings_proj(opts.ccx_root, out_folder)
    if 'all' in args or 'xcode' in args:
        gen_xcode_proj(opts.ccx_root, out_folder)
    if 'all' in args or 'android' in args:
        gen_android_proj(opts.ccx_root, out_folder)
    if 'all' in args or 'msvs_desktop' in args:
        gen_vc_proj(opts.ccx_root, out_folder)
    if 'all' in args or 'msvs_wp8' in args:
        gen_wp8_proj(opts.ccx_root, out_folder)

    print "Finished"