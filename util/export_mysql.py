#!/usr/bin/env python

import MySQLdb
import csv
from pprint import pprint

user = 'root' # your username
passwd = 'jjfjj' # your password
host = 'localhost' # your host
db = 'test' # database where your table is stored
table = 'crew' # table you want to save
cursor = None
try:
    con = MySQLdb.connect(user=user, passwd=passwd, host=host, db=db)
    cursor = con.cursor()
except MySQLdb.Error as e:
    printf("Mysql Error %d: %s" % (e.args[0]), e.args[1])


query = "select * from test.crew;"
try:
    cursor.execute(query)
except MySQLdb.Error as e:
    print("mysql Error:%s \n SQL:%s" %(e, sql.encode("utf8")))


with open('yanqiu_daren.csv','w') as f:
    writer = csv.writer(f)
    for row in cursor.fetchall():
        pprint(row)
        writer.writerow(row)

