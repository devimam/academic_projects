# websearch1.py



from webindexer1 import *


QUIT = "QUIT"
search = ""


print ("100 Web Search! (version 1)")
print

print ("Building the index... (this may take a while)")
build_index()
print ("Done!")

print
print ("At any time, you may search for", ('"' + QUIT + '"'), "to quit.")
print

while True:     # generally this is bad, but I'm using a break statement.
                # still not great, but the code seems cleaner with break.

    print ("- " * 39)
    print
    
    search = raw_input("What would you like to search for? ")
    print

    if search == QUIT:
        break
    
    sites = search_multiple_words(search.lower().split())
    n = len(sites)

    if n == 0:
        print ("No sites found with the terms", ('"' + search + '".'))
        print ("Try a broader search.")
        print
    else:
        print (n, "site(s) found with the terms", ('"' + search + '":'))
        print
        for site in sites:
            print (site)
        print

print ("Thanks for searching!")
