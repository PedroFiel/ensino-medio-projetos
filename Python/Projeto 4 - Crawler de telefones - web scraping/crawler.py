import re
import threading

import requests

from bs4 import BeautifulSoup

DOMINIO = "https://django-anuncios.solyd.com.br"
URL_AUTOMOVEIS = "https://django-anuncios.solyd.com.br/automoveis/"

LINKS = []
TELEFONES = []

def requisicao(url):
    try:
        resposta = requests.get(url)
        if resposta.status_code == 200:
            return resposta.text
        else:
            print("Erro ao fazer requisição")
    except Exception as error:
        print("Erro ao fazer requisição")
        print(error)

def parsing(resposta_html):
    try:
        soup = BeautifulSoup(resposta_html, 'html.parser')
        return soup
    except Exception as error:
        print("Erro ao fazer o parsing HTML")
        print(error)

def encontrar_links(soup):
    try:
        cards_pai = soup.find("div", class_="ui three doubling link cards")
        cards = cards_pai.find_all("a")
    except:
        print("Erro ao encontrar links")
        return None

    links = []
    for card in cards:
        try:
            link = card['href']
            links.append(link)
        except:
            pass

    return links

def encontrar_telefone(soup):
    try:
        descricao = soup.find_all("div", class_="sixteen wide column")[2].p.get_text().strip()
    except:
        print("Erro ai encontrar descricao")
        return None

    regex = re.findall(r"\(?0?([1-9]{2})[ \-\.\)]{0,2}(9[ \-\.]?\d{4})[ \-\.]?(\d{4})", descricao)
    if regex:
        return regex
    
    print(regex)

def descobrir_telefones():
    while True:
        try:
            link_anuncio = LINKS.pop(0)
        except:
            return None
        
        requisicao(DOMINIO + link_anuncio)

        resposta_anuncio = requisicao(DOMINIO + link_anuncio)

        if resposta_anuncio:
            soup_anuncio = parsing(resposta_anuncio)
            if soup_anuncio:
                telefones = encontrar_telefone(soup_anuncio)
                if telefones:
                    for telefone in telefones:
                        print('Telefone encontrado:', telefone)
                        TELEFONES.append(telefone)
                        salvar_telefone(telefone)

def salvar_telefone(telefone):
    string_telefone = "({}) {}{}\n".format(telefone[0],telefone[1],telefone[2])
    try:
        with open("telefone.csv", 'a') as arquivo:
            arquivo.write(string_telefone)
    except:
        print('Erro ao salvar arquivos')




if __name__ == "__main__":
    resposta_busca = requisicao(URL_AUTOMOVEIS)
    if resposta_busca:
        soup_busca = parsing(resposta_busca)
        if soup_busca:
            LINKS = encontrar_links(soup_busca)

            THREADS = []
            for i in range(5):
                t = threading.Thread(target=descobrir_telefones)
                THREADS.append(t)

            for t in THREADS: 
                t.start()
            
            for t in THREADS: 
                t.join()

            print(TELEFONES)