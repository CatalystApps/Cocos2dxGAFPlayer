#! /usr/bin/env python

if __name__ == '__main__':
	import GenerateProjectFiles
	raise SystemExit(GenerateProjectFiles.main())

from optparse import OptionParser
import os
import fileinput
import sys
import shutil

def gen_xcode_proj(cxx_root, out_folder):

	shutil.copytree(os.getcwd() + "/GAFPlayer.xcodeproj", out_folder)

	newcwd = out_folder + "/GAFPlayer.xcodeproj"
	project = "project.pbxproj"

	searchExp = "CCX_ROOT ="

	for line in fileinput.input(newcwd + project, inplace=True):
		if searchExp in line:
			print 'CCX_ROOT_OVERRIDE = "%s";' % cxx_root
		sys.stdout.write(line)
	

def main():
	parser = OptionParser()
	parser.add_option('-c', '--ccxroot', dest='ccx_root', help='directory of Cocos2d-x root, e.g E:/Projects/Cocos2d-x/')
	parser.add_option('-o', '--out', dest='out_folder', help='where to store generated project files, e.g E:/Projects/GAFLibraryProject')
	opts, args = parser.parse_args()

	out_folder = opts.out_folder

	if out_folder:
		if os.path.exists(out_folder):
			os.removedirs(out_folder)

	gen_xcode_proj(opts.ccx_root, out_folder)