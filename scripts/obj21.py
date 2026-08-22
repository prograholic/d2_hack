
damagewheels = set()
damages = set()
shadows = set()
lights = set()
commons = set()


def classify(item):
    if 'DamageWheel' in item:
        damagewheels.add(item)
    elif 'Damage' in item:
        damages.add(item)
    elif 'Shadow' in item:
        shadows.add(item)
    elif 'Light' in item:
        lights.add(item)
    else:
        commons.add(item)


with open('TRUCKS_onlynames.txt') as names:
    for line in names:
        if 'GroupObjects21' in line:
            item = line.replace('GroupObjects21:', '').replace('"', '').replace('refer_', '').strip()
            classify(item)


print('DAMAGEWHEEL:')
for item in damagewheels:
    print('  ' + item)

print('\nDAMAGE:')
for item in damages:
    print('  ' + item)


print('\nSHADOW:')
for item in shadows:
    print('  ' + item)

print('\nLIGHTS:')
for item in lights:
    print('  ' + item)
    
print('\nCOMMONS:')
for item in commons:
    print('  ' + item)