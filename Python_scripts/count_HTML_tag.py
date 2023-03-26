from bs4 import BeautifulSoup
from urllib.request import urlopen
import ssl

ctx=ssl.create_default_context()
ctx.check_hostname=False
ctx.verify_mode = ssl.CERT_NONE

url = input("Enter URL:")
html=urlopen(url, context=ctx).read()
soup = BeautifulSoup(html, "html.parser")

tags = soup('span')
total=list()
count=0
for tag in tags:
#    print('TAG:', tag)
#    print('URL:',tag.get('class', None))
#    print('Contents:', tag.contents[0])
#    print('Attr:', tag.attrs)
    total.append(int(tag.contents[0]))
    count=count+1
print(count)
print(sum(total))