
obj21Names = set()

with open('TRUCKS_onlynames.txt') as names:
    for line in names:
        if 'GroupObjects21' in line:
            item = line.replace('GroupObjects21:', '').replace('"', '').strip()
            obj21Names.add(item)

for item in obj21Names:
    print(item)
