#try:
#    arquivo = open('EXs/emails.txt')
#except FileNotFoundError:
#    print('Arquivo não encontrado')
#except:
#    print('Algum erro ocorreu')
#finally:
#    arquivo.close() 

#conteudo = arquivo.readlines()

#for linha in conteudo:
#    print(linha.strip())

try:
    with open('EXs/emails.txt') as arquivo:  # ELE ABRE E FECHA O ARQUIVO SEM PRECISAR DO .CLOSE
        print(arquivo.readlines())
except FileNotFoundError:
    print('Arquivo não existe')