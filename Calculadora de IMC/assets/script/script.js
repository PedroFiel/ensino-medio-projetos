function calc(){
    var resultpeso = parseFloat(document.querySelector("#peso").value);
    var resultaltura = parseFloat(document.querySelector("#altura").value);

    var imc = resultpeso / ( resultaltura * resultaltura);

    document.querySelector(".resultado").innerHTML = `O seu IMC é ${imc.toFixed(2)}`;
}



