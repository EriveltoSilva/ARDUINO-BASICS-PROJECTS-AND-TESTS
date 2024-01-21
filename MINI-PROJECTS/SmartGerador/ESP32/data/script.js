const btnModoGerador = document.getElementById("btnModo");
const btnGerador = document.getElementById("btnGerador");

const tensaoRede = document.getElementById("tensaoRede");
const potenciaRede = document.getElementById("potenciaRede");

const tensaoGerador = document.getElementById("tensaoGerador");
const potenciaGerador = document.getElementById("potenciaGerador");

const humidade = document.getElementById("humidade");
const temperatura = document.getElementById("temperatura");

const corrente = document.getElementById("corrente");
// const tensaoBateria = document.getElementById("tensaoBateria");


window.addEventListener('load', () => {
    setInterval(receiveData, 2000);
});


btnModoGerador.addEventListener("click", () => {
    sendReq("/modo");
});


btnGerador.addEventListener("click", () => {
    sendReq("/gerador");
});


function sendReq(url) {
    fetch(url)
        .then(resp => resp.json())
        .then((resp) => {
            console.log(resp);
            if (resp["status"] == "success")
                console.log("Pedido Realizado!");
            else
                alert(resp["message"]);
        })
        .catch(error => { console.error("## ERRO PEGANDO OS DADOS:" + error); })
}


function receiveData() {
    fetch('/dados')
        .then(resp => resp.json())
        .then((resp) => {
            console.log(resp);
            let dados = resp["data"].split('*');

            if(dados[0]==='D')
            {
                console.log("DAdos");
                temperatura.innerHTML = dados[1] + "*C";
                humidade.innerHTML = dados[2] + "%";
                corrente.innerHTML = dados[3] + "A";
                tensaoRede.innerHTML = dados[4] + "V";
                potenciaRede.innerHTML = dados[5] + "W";
                
                tensaoGerador.innerHTML = dados[6] + "V";
                potenciaGerador.innerHTML = dados[7] + "W"; 
                
                // tensaoBateria.innerHTML = dados[8] + "V";
                
                btnModoGerador.innerHTML = (dados[9]==="A")? "AUTOMÁTICO": "MANUAL";
                btnGerador.innerHTML = (dados[10]==="0")? "DESLIGADO": "LIGADO";
            }
        })
        .catch(error => { console.error("## ERRO PEGANDO OS DADOS:" + error); })
}