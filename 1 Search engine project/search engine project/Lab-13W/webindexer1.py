# webindexer1.py


from urllib import urlopen
from htmltext import HtmlTextParser


FILENAME = "smallsites.txt"
dic = {}


def get_sites():
    """ Return all the sites that are in FILENAME. """
    sites_file = open(FILENAME)
    sites = []
    for site in sites_file:
        sites.append("http://" + site.strip())
    return sites

def read_site(site):
    """ Attempt to read the given site. Return the text of the site if
    successful, otherwise returns False. """
    try:
        connection =urllib.urlopen(site)
        html = connection.read()
        connection.close()
    except:
        return False
    
    parser = HtmlTextParser()
    parser.parse(html)
    return parser.get_text()

def index_site(site, text):
    wordlist=text.lower().split()
    for i in wordlist:
        listkeys=list(dic.keys())
        if(i not in listkeys):
            dic[i]=[[1,site]]
        else:
            p=list(dic[i])
            sitelist=[]
            for j in range(len(p)):
                sitelist.append(p[j][1])
            if(site not in sitelist):
                p.append([1,site])
            else:
                for m in range(len(p)):
                    if(p[m][1]==site):
                        p[m][0]+=1
                        break
            p.sort()
            p.reverse()
            dic[i]=p
            
    return dic

def search_single_word(word):
    enginelist=list(dic[word])
    resultlist=[]
    for i in range(len(enginelist)):
        resultlist.append(enginelist[i][1])
    return resultlist

def search_multiple_words(words):
    wordlist=words.split()
    siteslist=[]
    for word in wordlist:
        siteslist.extend(search_single_word(word))
    return siteslist

def build_index():
    """ Build the index by reading and indexing each site. """
    for site in get_sites():
        text = read_site(site)
        while text == False:
            text = read_site(site)  # keep attempting to read until successful
        index_site(site, text)
