import chardet
import os

out_dir = "utf8"

if not os.path.exists(out_dir):
    os.mkdir(out_dir)

for i in os.listdir("."):
    if out_dir == i or "gbk2utf8.py" == i: continue
    f = open(i, 'rb')
    res = chardet.detect(f.read())
    f.close()
    with open(i, 'r', encoding=res["encoding"]) as readFile:
        with open(out_dir + "/" + i, 'w', encoding='utf-8') as writeFile:
            writeFile.write(readFile.read())