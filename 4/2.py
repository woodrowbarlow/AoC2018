from parse import *
from parse import compile
from dateutil.parser import parse as dateparse
from collections import OrderedDict

sleep_records = {}

def record_sleep(guard_id, start, stop):
	if not guard_id in sleep_records:
		sleep_records[guard_id] = [0] * 60
	for i in range(start, stop):
		sleep_records[guard_id][i] += 1

messages = OrderedDict()

with open("input", "r") as file:
	p = compile("[{}] {}")
	for line in file:
		(datestr, message) = p.parse(line)
		date = dateparse(datestr)
		messages[date] = message

messages = OrderedDict(sorted(messages.items(), key=lambda t: t[0]))

guard_id = 0
sleep_date = {}
for date, message in messages.items():
	if message == "falls asleep":
		sleep_date = date
	elif message == "wakes up":
		record_sleep(guard_id, sleep_date.minute, date.minute)
	else:
		guard_id = parse("Guard #{:d} begins shift", message)[0]

max_minute = 0
max_minute_idx = 0
max_id = 0
max_guard_id = 0
for guard_id, sleep_record in sleep_records.items():
	idx = max(range(len(sleep_record)), key=sleep_record.__getitem__)
	minute = max(sleep_record)
	if minute > max_minute:
		max_minute = minute
		max_minute_idx = idx
		max_guard_id = guard_id

print(max_guard_id * max_minute_idx)
