from pathlib import Path


all_samples = set()

for p in Path( 'res_output' ).rglob( '*.d2_material' ):
    with open(p, 'r') as f:
        data = f.read().strip()
        all_samples.add(data)
        print(f'done with {p}')


print('All done, printing:\n\n')

for sample in all_samples:
    print('    "' + sample.replace('"', '\\"') + '",')
