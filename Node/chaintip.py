import sys
import json
from pprint import pprint

data = json.load(sys.stdin)

height = []
length = len(data)
for i in range(0, length):
	height.append(data[i]["height"])


#pprint(max(height))
pprint(len(height))
