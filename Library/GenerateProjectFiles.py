#! /usr/bin/env python

if __name__ == '__main__':
    import GenerateProjectFiles

    raise SystemExit(GenerateProjectFiles.main())

from optparse import OptionParser
import os
import fileinput
import sys
import shutil


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

    gen_xcode_proj(opts.ccx_root, out_folder + "/GAFPlayer.xcodeproj/")
    #gen_android_proj(opts.ccx_root, out_folder)

