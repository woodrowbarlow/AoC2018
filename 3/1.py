from parse import *
from parse import compile

tiles = {}

def tag_coverage(left, top, width, height):
	for x in range(left, left + width):
		for y in range(top, top + height):
			if not (x,y) in tiles:
				tiles[(x,y)] = 1
			else:
				tiles[(x,y)] += 1

with open("input", "r") as file:
	p = compile("#{:d} @ {:d},{:d}: {:d}x{:d}")
	for line in file:
		(num, left, top, width, height) = p.parse(line)
		tag_coverage(left, top, width, height)

count = 0
for coverage in tiles.values():
	if coverage >= 2:
		count += 1

print(count)
