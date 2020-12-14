imam="Hai this is Mohammad Imam Hossain. He is  a student of BUET in CSE department. He is very proud to be a student of this department. He is a very familiar guy in his class specially known as circuit Sopdrul's friend. Actually what a interesting university life is! he can't think before admitted into BUET. Please contact with me for any kind of information.!?"
shimul="Hai this is Mahbub Alam Shimul. He is a student of IUT in Mechanical Department. He feel very proud of being a student of this department. He is  a very familiar youngster in his class as he is the CR in his class. Once time he is very introvert but time changes. Now he is very update guy. He is fashionable & stylist boy."
def remove_punctuation(message):
    p="(/.,<>?!#$%^*()[]{}:;+_-=|\)"
    new_message=""
    k=0
    for i in range(len(message)):
        if(message[i] in p):
            new_message=new_message+message[k:i]
            k=i+1
    return (new_message)
            
imam1=remove_punctuation(imam)
imam2=imam1.lower()
p=imam2.split()


shimul1=remove_punctuation(shimul)
shimul2=shimul1.lower()
q=shimul2.split()

dic={}
url="Google"
for i in p:
    li=list(dic.keys())
    if(i not in li):
        dic[i]=[[url,1]]
    else:
        p=dic[i]
        p[0][1]+=1
        
        
print(dic)
