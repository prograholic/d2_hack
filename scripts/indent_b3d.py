import pprint

indent = 0

def printi(line):
    prefix = ' ' * (indent * 4)
    print(prefix + line)
    

    
def read_naive():
    global indent

    count_333 = 0
    with open('sep2.txt') as f:
        for line in f:
            line = line.strip()
            if line.startswith('FILE:'):
                count_333 = 0
                indent = 0
                printi(line)

            elif line.startswith('BH:'):
                printi(line)
            elif line == '111':
                printi('{')
                indent += 1
            elif line == '333':
                count_333 += 1
                printi('{')
                indent += 1
            elif line == '222':
                indent -= 1
                printi('}')
            elif line == '555':
                indent -= 1
                printi('}')
            elif line == '444':
                if count_333 == 1:
                    indent -= 1
                    printi('}')
                printi(line)
            
def update_counter(counter, field):
    counter[field] = counter.get(field, 0) + 1
    return counter


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

#read_naive2()
read_naive()

