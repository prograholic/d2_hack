import pprint

indent = 0

def printi(line):
    prefix = ' ' * (indent * 4)
    print(prefix + line)
    

    
def read_naive():
    global indent

    count_333 = 0
    with open('cerr.log') as f:
        for line in f:
            line = line.strip()
            if line.startswith('FILE:'):
                indent = 0
                printi(line)
            elif line.startswith('SEP: 111'):
                printi('{')
                indent += 1
            elif line.startswith('SEP: 333'):
                printi('{')
                indent += 1
            elif line.startswith('SEP: 222'):
                indent -= 1
                printi('}')
            elif line.startswith('SEP: 555'):
                indent -= 1
                printi('}')
            else:
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
    with open('cerr.log') as f:
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

