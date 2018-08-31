#!/usr/bin/env python

__author__ = "Firo Yang"
__license__ = "GPL"
__version__ = "1.0.1"
__maintainer__ = "Firo Yang"
__email__ = "yongming.yang@windriver.com"

from xlrd import open_workbook
import argparse
import re
import xlwt

def read_from_patch_xls(patch_xls_tuple):
	ccm_xls = patch_xls_tuple[0]
	wb = open_workbook(ccm_xls)
	for s in wb.sheets():
		print 'Sheet:',s.name
		for row in range(s.nrows):
			summary = s.cell(row,1).value.strip('\n').replace('\n',' ')
			url = s.cell(row,3).value.strip('\n').replace('\n',' ')
			commit_id = url[ url.rfind('=') + 1:]
			if commit_id == 'url':
				commit_id = "Commit ID"
				url = "URL"
			print	"| %s | %s | %s |" % (summary, commit_id, url)
		break

def read_from_sow_xls(sow_xls_tuple):
	sow_xls = sow_xls_tuple[0]
	wb = open_workbook(sow_xls)
	version = "NULL"
	workbook = xlwt.Workbook(encoding = 'ascii')
	worksheet = workbook.add_sheet('My Worksheet')
	for s in wb.sheets():
		print 'Sow:',s.name
		print 'Sow:',s.nrows
		for row in range(1, s.nrows):
			print "row", row
			summary = s.cell(row,0).value.strip('\n').replace('\n',' ')
			# url = s.cell(row,3).value.strip('\n').replace('\n',' ')
			# commit_id = url[ url.rfind('=') + 1:]
			# if commit_id == 'url':
			#	commit_id = "Commit ID"
			#	url = "URL"
			mg = re.match(r'(.*)-(.*)-(.*)rpm', summary, re.I)  
			print	"| %s | %s | %s|" % (mg.group(), mg.group(1), mg.group(2))
			#print	"| %s | %s " % (mg.group(), mg.group(1))
			worksheet.write(row, 0, label = mg.group(1))
			worksheet.write(row, 1, label = mg.group(2))
		break
	workbook.save('firo_sow.xls')


def read_from_cve_xls(cve_xls_tuple):
	cve_xls = cve_xls_tuple[0]
	print 'cve_xls', cve_xls
	wb = open_workbook(cve_xls)
	print 'open cve_xls'
	for s in wb.sheets():
		print 'Sheet:',s.name
		for row in range(s.nrows):
			summary = s.cell(row,0).value.strip('\n').replace('\n',' ')
			url = s.cell(row,4).value.strip('\n').replace('\n',' ')
			package = s.cell(row,1).value.strip('\n').replace('\n',' ')
			solution =""
			status = ""
			if row == 0:
				solution = 'commit/solution'
				status = 'Accepted status'
			print	"| %s | %s | %s | %s | %s |" % (summary, solution, url, package, status)
		break

def param_init():
	parser = argparse.ArgumentParser(description='CCM xls reader')
	parser.add_argument('-p', nargs=1, metavar='path.xls', help='Specific the patch list xls.')
	parser.add_argument('-c', nargs=1, metavar='cve.xls', help='Specific the CVE list xls.')
	parser.add_argument('-s', nargs=1, metavar='sow.xlsx', help='Specific the SOW list xls.')
	
	return parser.parse_args();
	
def main():
	args = param_init()
	if args.s:
		print "SOW file is %s" %(args.s)
		read_from_sow_xls(args.s)
	if args.c:
		print "CVE file is %s" %(args.c)
		read_from_cve_xls(args.c)

	if args.p:
		print "Patch file is %s" %(args.p)
		read_from_patch_xls(args.p)

	return 1

if __name__ == "__main__":
	main()
