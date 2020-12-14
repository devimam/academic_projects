Google="It is a global search engine. In this site you can search anything."
Facebook="it is not only a site but also a medium of making the world like a cottage where all the country are as the menmber of the cottage. it is a populer web site which makes relationsip among all the people,"
Twitter="this  is also a familiar site to all. This is mainly used as a site for downloading all the things according to the users quest."
Skype="this is also a medium which works like a  phone to all. By using this site you can chat with anyone."
Yahoo="this is a e-mail site. we can't find anyone who has no e-mail ID"
listofsites=['Google','Facebook','Yahoo','Twitter','Skype']
dic={}

def index_site(site,text):
    wordlist=text.lower().split()
    for i in wordlist:
        listkeys=list(dic.keys())
        if(i not in listkeys):
            dic[i]=[[1,site]]
        else:
            p=list(dic[i])
            sitelist=[]
            for j in range(len(p)):
                sitelist.append(site)
            if(site not in sitelist):
                p.append([1,site])
            else:
                for j in range(len(p)):
                    if(p[j][1]==site):
                        p[j][0]+=1
                        break
                p.sort()
                p.reverse()
                dic[i]=p
            
    return dic

print(index_site("google",Google))


def search_single_word(word):
    enginelist=dic[word]
    resultlist=[]
    for i in range(len(enginelist)):
        resultlist.append(enginelist[i][1])
    return resultlist

def search_multiple_words(words):
    wordslist=words
    siteslist=[]
    for word in wordslist:
        
        siteslist.extend(search_single_word(word))
    
    return siteslist
        
print(search_multiple_words(["anything.","site"]))    
    
    
    
