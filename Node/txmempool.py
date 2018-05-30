import sys

import json
from pprint import pprint

data = json.load(sys.stdin)

pprint(data["size"])

