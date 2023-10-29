let temperatureLevel = document.getElementById('temperatureLevel');
let temperatureSensor = document.getElementById('temperatureSensor');
let humidityLevel = document.getElementById('humidityLevel');
let humiditySensor = document.getElementById('humiditySensor');
let flameLevel = document.getElementById('flameLevel');
let flameSensor = document.getElementById('flameSensor');
let smokeLevel = document.getElementById('smokeLevel');
let smokeSensor = document.getElementById('smokeSensor');
let btnDoor = document.getElementById('btnDoor');
let btnCooler = document.getElementById('btnCooler');


window.addEventListener('load', () => { 
    window.localStorage.clear(); 
    setInterval(receiveData, 1000); 
});

btnDoor.addEventListener("click", () => {
    sendReq("/door")
});

btnCooler.addEventListener("click", () => {
    sendReq("/cooler")
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
            if (dados[0] == 'D') {

                temperatureSensor.innerHTML = dados[1]+"*C";
                if (dados[1] >= 35)
                    temperatureLevel.innerHTML = "ALTA";
                else if (dados[1] >= 10)
                    temperatureLevel.innerHTML = "NORMAL";
                else
                    temperatureLevel.innerHTML = "BAIXA";

                humiditySensor.innerHTML = dados[2]+"%";
                if (dados[1] >= 85)
                    humidityLevel.innerHTML = "ALTA";
                else if (dados[1] >= 45)
                    humidityLevel.innerHTML = "NORMAL";
                else
                    humidityLevel.innerHTML = "BAIXA";
                
                flameSensor.innerHTML = dados[3]+"%";
                if(dados[3] >= 65)
                {
                    flameLevel.innerHTML = "FOGO DETECTADO";
                    document.getElementById('imgFlameLevel').src="fire.png";
                    document.getElementById('imgFlameSensor').src="fire.png";
                }
                else{
                    flameLevel.innerHTML = "SEM FOGO";
                    document.getElementById('imgFlameLevel').src="nofire.png";
                    document.getElementById('imgFlameSensor').src="nofire.png";
                }

                smokeSensor.innerHTML = dados[4]+"%";
                if(dados[4] >= 60)
                {
                    smokeLevel.innerHTML = "FUMO DETECTADO";
                    document.getElementById('imgSmokeLevel').src =  "smoke.png";
                    document.getElementById('imgSmokeSensor').src = "smoke.png";
                }
                else{
                    smokeLevel.innerHTML = "SEM FUMO";
                    document.getElementById('imgSmokeLevel').src  = "nosmoke.png";
                    document.getElementById('imgSmokeSensor').src = "nosmoke.png";
                }
        
                btnCooler.innerHTML = (dados[5] == "1") ? "DESLIGAR" : "LIGAR";
                document.getElementById('imgVentilador').src = (dados[5] == "1") ? "ventilador.png" :"ventiladorOff.png";
                
                btnDoor.innerHTML = (dados[6] == "1") ? "FECHAR" : "ABRIR";
                document.getElementById('imgDoor').src = (dados[6] == "1") ? "doorOpen.png" :"doorClose.png";
            }
        })
        .catch(error => { console.error("## ERRO PEGANDO OS DADOS:" + error); })
}
