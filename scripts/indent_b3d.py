import pprint

indent = 0

def printi(line):
    prefix = ' ' * (indent * 4)
    print(prefix + line)
    
def update_counter(counter, field):
    counter[field] = counter.get(field, 0) + 1
    
    return counter
    
def read_naive():
    global indent
    got_file = False
    prev = None
    counter = dict()
    with open('sep.log') as f:
        for line in f:
            line = line.strip()
            if line.startswith('FILE:'):
                print_counter(counter)
                counter = dict()

            elif line.startswith('BH:'):
                counter = update_counter(counter, 'Block')
            elif line == 'NESTED: begin':
                counter['nested begin'] = counter.get('BH', 0) + 1
                printi('{')
                indent += 1
            elif line == 'NESTED: end':
                indent -= 1
                printi('}')
                
            elif line == '111':
                prev = line
                printi('{')
                indent += 1
            elif line == '333':
                prev = line
                printi('{')
                indent += 1
            elif line == '222':
                prev = line
                indent -= 1
                printi('}')
            elif line == '555':
                prev = line
                indent -= 1
                printi('}')
            elif line == '444':
                indent -= 1
                #if count_444 % 2 == 0:
                #    printi('[')
                #    indent += 1
                #else:
                #    indent -= 1
                #    printi(']')
                    
                count_444 += 1

        print_counter(counter)
            
            
def print_counter(counter, line):
    print(line)
    pprint.pprint(counter)
    print
            
def read_naive2():
    counter = dict()
    with open('sep.log') as f:
        prev_file = None
        for line in f:
            line = line.strip()
            if line.startswith('FILE:'):
                if prev_file is not None:
                    print_counter(counter, prev_file)
                    counter = dict()
                prev_file = line

            elif line.startswith('BH:'):
                counter = update_counter(counter, 'Block')
            else:
                if line in ['NESTED: begin', 'NESTED: end', '111', '222', '333', '444', '555']:
                    counter = update_counter(counter, line)

        print_counter(counter, prev_file)

read_naive2()

