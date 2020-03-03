import jieba
import sys
import json


txt = open(r'庆余年.txt','r',encoding='utf-8').read()
#txt = sys.argv[1]

jieba.set_dictionary("./dict.txt")  
jieba.initialize()                  
 
wordsls=jieba.lcut(txt)
wcdict={}

for word in wordsls:
    if len(word)==1:
        continue
    else:
        wcdict[word]=wcdict.get(word,0)+1

#word在wcdict中没有找到对应的词语，则返回0
str_json = json.dumps(wcdict, ensure_ascii=False)
print(str_json)
        
#wcls=list(wcdict.items())
#wcls.sort(key=lambda x:x[1],reverse=True)
 
#for i in range(len(wcls)):
#    print(wcls[i])
