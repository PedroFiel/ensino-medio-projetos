AGENDA = {
    "guilherme":{
        "tell": "999999-1010",
        "email": "eaaaaaa@gmail.com",
        "endereco": "av. aa"
    },
    "João":{
        "tell": "998888-1010",
        "email": "bbbbp@gmail.com",
        "endereco": "av. bb"},
    "Maria":{
        "tell": "999777-1010",
        "email": "ccccp@gmail.com",
        "endereco": "av. cc"},
    "Pedro":{
        "tell": "99666-1010",
        "email": "ddddp@gmail.com",
        "endereco": "av. dd"
        }
}

AGENDA['guilherme'] ['endereco'] = "Rua das Nações"

AGENDA['Lucas'] = {
        "tell": "99666-1010",
        "email": "ola@gmail.com",
        "endereco": "av. dd"
        }

AGENDA.pop("Lucas")

print(AGENDA["Maria"]['tell'])
print(AGENDA["guilherme"])

for contato in AGENDA:
    print(contato)

print(AGENDA["Lucas"])