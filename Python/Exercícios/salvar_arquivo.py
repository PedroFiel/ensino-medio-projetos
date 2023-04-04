try:
    with open('EXs/nomes.txt','a') as arquivo:
        arquivo.write('Pedro \n')
except Exception as error:
    print('Algum erro ocorreu')
    print(error)

'''
r - abre para ler
w - abre para escrever /  arquivo é sobrescrito
a - abre para escrever / novo conteudo é adicionado ao final do arquivo
+ -
b -
'''