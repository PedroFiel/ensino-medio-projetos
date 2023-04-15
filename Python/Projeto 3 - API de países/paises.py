import json
import sys

import requests

URL_ALL = "https://restcountries.com/v2/all"
URL_NAME = "https://restcountries.com/v2/name"

def requisicao(url):
    try:
        resposta = requests.get(url)
        if resposta.status_code == 200:
            return resposta.text
    except:
        print("Erro ao fazer requisição em", url)

def parsing(texto_da_resposta):
    try:
        return json.loads(texto_da_resposta) #PARSING DE JSON PARA PYTHON
    except:
        print('Erro ao fazer parsing')

def contagem_de_paises():
    resposta = requisicao(URL_ALL)

    if resposta:
        lista_de_paises = parsing(resposta)
        if lista_de_paises:
            return len(lista_de_paises)

def listar_paises(lista_de_paises):
    for pais in lista_de_paises:
        print(pais['name'])

def mostrar_populacao(nome_do_pais):
    resposta = requisicao("{}/{}".format(URL_NAME,nome_do_pais))

    if resposta:
        lista_de_paises = parsing(resposta)
        if lista_de_paises:
            for pais in lista_de_paises:
                print('{}: {}'.format(pais['name'], pais['population']))
    else:
        print('Pais não encontrado')

def mostrar_moedas(nome_do_pais):
    resposta = requisicao("{}/{}".format(URL_NAME,nome_do_pais))

    if resposta:
        lista_de_paises = parsing(resposta)
        if lista_de_paises:
            for pais in lista_de_paises:
                print('Moedas do', pais['name'])
                moedas =pais['currencies']
                for moeda in moedas:
                    print('{} - {}'.format(moeda['name'], moeda['code']))
    else:
        print('Pais não encontrado')

def ler_nome_do_pais():
    try:
        nome_do_pais = sys.argv[2]
        return nome_do_pais
    except:
        print('É preciso passar o nome do país')


if __name__ == '__main__':
    if len(sys.argv) == 1:
        print("----------------------------------------")
        print("## Bem vindo ao sitema de países##")
        print("----------------------------------------")
        print("Uso: python paises.py <ação> <nome do país>")
        print("")
        print('Ações disponíveis:  contagem, moeda, populacao')
    else:
        argumento1= sys.argv[1]

        if argumento1 == "contagem":
            numero_de_paises = contagem_de_paises()
            print("Existem {} países no mundo  todo ".format(numero_de_paises))
            exit(0)
        elif argumento1 == "moeda":
            pais = ler_nome_do_pais()
            if pais:
                mostrar_moedas(pais)
        elif argumento1 == "populacao":
            pais =ler_nome_do_pais()
            if pais:
                mostrar_populacao(pais)
        else:
            print('Argumento inválido')
  

