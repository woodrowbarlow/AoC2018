from parse import *
from parse import compile

claims = []

class Claim:
	def __init__(self, num, left, top, width, height):
		self.num = num
		self.left = left
		self.top = top
		self.width = width
		self.height = height
	def x1(self):
		return self.left
	def x2(self):
		return self.left + self.width
	def y1(self):
		return self.top
	def y2(self):
		return self.top + self.height
	def overlaps_with(self, other):
		if self.x2() <= other.x1() or self.x1() > other.x2():
			return False
		if self.y2() <= other.y1() or self.y1() > other.y2():
			return False
		return True
	def overlaps_with_any(self, others):
		for other in others:
			if self.num == other.num:
				continue
			if self.overlaps_with(other):
				return True
		return False

with open("input", "r") as file:
	p = compile("#{:d} @ {:d},{:d}: {:d}x{:d}")
	count = 0
	for line in file:
		(num, left, top, width, height) = p.parse(line)
		claims.append(Claim(num, left, top, width, height))

claims[:] = [claim for claim in claims if not claim.overlaps_with_any(claims)]

for claim in claims:
	print(claim.num)
