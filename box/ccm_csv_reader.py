#!/usr/bin/env python

__author__ = "Firo Yang"
__license__ = "GPL"
__version__ = "1.0.1"
__maintainer__ = "Firo Yang"
__email__ = "yongming.yang@windriver.com"

import csv
import xlwt
import argparse

def param_init():
        parser = argparse.ArgumentParser(description='CCM csv reader')
        parser.add_argument('-c', nargs=1, metavar='cve.csv', help='Specific the CVE list csv.')
        parser.add_argument('-t', nargs=1, metavar='time', help='Specific the time of the CVE.')

        return parser.parse_args();
def main():

        args = param_init()
        if args.c:
		
                print "CVE file is %s" %(args.c)
		data = open(args.c[0], 'rb')
		cve_lists = csv.reader(data)
		
		workbook = xlwt.Workbook(encoding = 'ascii')
		worksheet = workbook.add_sheet('CVEs')
		cves_row = 0	
		for row in cve_lists:
			if args.t[0] == row[1]:
				for col in range(0, len(row)):
					worksheet.write(cves_row, col, row[col])
				cves_row += 1
				print len(row)
		workbook.save('huawei_cves.xls')
        return 1

if __name__ == "__main__":
        main()
