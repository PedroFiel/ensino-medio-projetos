pessoas = {
    'tijolo' : 'objeto de construção',
    'cimento' : 'objeto de construção',
}

print(pessoas['cimento'])

for pessoa in pessoas.values():
    print(pessoa)

for pessoa in pessoas:
    print(pessoas[pessoa])