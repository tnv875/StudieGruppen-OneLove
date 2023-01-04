import re


txt = "GET / HTTP/1.1\r\nHost: sns.cs.princeton.edu\r\nAccept: */*\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: Keep-Alive\r\n\r\n"
x = re.split("\r\n", txt)

x = x[1:-2]

temp_dict = {}

for i in range(0, len(x)):
  name, value = x[i].split(sep=": ")
  temp_dict.update({name: value})

print(temp_dict)