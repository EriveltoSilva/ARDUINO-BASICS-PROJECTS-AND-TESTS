let p1Image1 = document.getElementById('p1Image1');
let p1Image2 = document.getElementById('p1Image2');
let p1Image3 = document.getElementById('p1Image3');
let p1Image4 = document.getElementById('p1Image4');
let p1Vaga1 = document.getElementById('p1Vaga1');
let p1Vaga2 = document.getElementById('p1Vaga2');
let p1Vaga3 = document.getElementById('p1Vaga3');
let p1Vaga4 = document.getElementById('p1Vaga4');
let p1Vaga1LastUpdate = document.getElementById('p1Vaga1LastUpdate');
let p1Vaga2LastUpdate = document.getElementById('p1Vaga2LastUpdate');
let p1Vaga3LastUpdate = document.getElementById('p1Vaga3LastUpdate');
let p1Vaga4LastUpdate = document.getElementById('p1Vaga4LastUpdate');
let bntReservation1 = document.getElementById('bntReservation1');

let p2Image1 = document.getElementById('p2Image1');
let p2Image2 = document.getElementById('p2Image2');
let p2Image3 = document.getElementById('p2Image3');
let p2Image4 = document.getElementById('p2Image4');
let p2Vaga1 = document.getElementById('p2Vaga1');
let p2Vaga2 = document.getElementById('p2Vaga2');
let p2Vaga3 = document.getElementById('p2Vaga3');
let p2Vaga4 = document.getElementById('p2Vaga4');
let p2Vaga1LastUpdate = document.getElementById('p2Vaga1LastUpdate');
let p2Vaga2LastUpdate = document.getElementById('p2Vaga2LastUpdate');
let p2Vaga3LastUpdate = document.getElementById('p2Vaga3LastUpdate');
let p2Vaga4LastUpdate = document.getElementById('p2Vaga4LastUpdate');
let bntReservation2 = document.getElementById('bntReservation2');
let btnModeDark = document.getElementById('btnModeDark');

function setPark(numberPark, numberVaga, status) {
    const images = {
        "livre": "vaga-livre.png",
        "ocupada": "vaga-ocupada.png",
        "ocupado": "vaga-ocupada.png",
    }
    if (numberPark==1) {
        switch (numberVaga) {
            case 1:
                p1Vaga1.innerHTML = status.toUpperCase();
                p1Image1.src = images[status.toLowerCase()];
                break;
            case 2:
                p1Vaga2.innerHTML = status.toUpperCase();
                p1Image2.src = images[status.toLowerCase()];
                break;
            case 3:
                p1Vaga3.innerHTML = status.toUpperCase();
                p1Image3.src = images[status.toLowerCase()];
                break;
            case 4:
                p1Vaga4.innerHTML = status.toUpperCase();
                p1Image4.src = images[status.toLowerCase()];
                if(status=="livre")
                    bntReservation1.classList.remove("disabled");
                else
                    bntReservation1.classList.add("disabled");
                break;
        }
    }
    else{
        switch (numberVaga) {
            case 1:
                p2Vaga1.innerHTML = status.toUpperCase();
                p2Image1.src = images[status.toLowerCase()];
                break;
            case 2:
                p2Vaga2.innerHTML = status.toUpperCase();
                p2Image2.src = images[status.toLowerCase()];
                break;
            case 3:
                p2Vaga3.innerHTML = status.toUpperCase();
                p2Image3.src = images[status.toLowerCase()];
                break;
            case 4:
                p2Vaga4.innerHTML = status.toUpperCase();
                p2Image4.src = images[status.toLowerCase()];
                if(status=="livre")
                    bntReservation2.classList.remove("disabled");
                else
                    bntReservation2.classList.add("disabled");
                break;
        }
    }
}

bntReservation1.addEventListener("click", ()=>{
    console.log("btn1")
    receiveData();
});

bntReservation2.addEventListener("click", ()=>{
    console.log("btn2");
    receiveData();
});

btnModeDark.addEventListener('click', ()=>{
    if(document.body.classList.contains("mode-dark"))
    {
        document.body.classList.remove("mode-dark");
        document.body.classList.add("mode-light");
    }
    else if(document.body.classList.contains("mode-light"))
    {
        document.body.classList.remove("mode-light");
        document.body.classList.add("mode-dark");    
    }
});

window.addEventListener('load', ()=>{
    setInterval(receiveData,1000);
});


function receiveData() 
{
    fetch('/dados')
    .then(resp=>resp.json())
    .then((resp)=>{
        console.log(resp);
        let dados = resp["data"].split('*');
        if(dados[0]=='D')
        {
            setPark(numberPark=1, numberVaga=1, status = (dados[1]=='1')?"ocupada":"livre");   
            setPark(numberPark=1, numberVaga=2, status = (dados[2]=='1')?"ocupada":"livre");   
            setPark(numberPark=1, numberVaga=3, status = (dados[3]=='1')?"ocupada":"livre");   
            setPark(numberPark=1, numberVaga=4, status = (dados[4]=='1')?"ocupada":"livre");   
            
            setPark(numberPark=2, numberVaga=1, status = (dados[5]=='1')?"ocupada":"livre");   
            setPark(numberPark=2, numberVaga=2, status = (dados[6]=='1')?"ocupada":"livre");   
            setPark(numberPark=2, numberVaga=3, status = (dados[7]=='1')?"ocupada":"livre");   
            setPark(numberPark=2, numberVaga=4, status = (dados[8]=='1')?"ocupada":"livre");   
        }
    })
    .catch(error =>{console.error("## ERRO PEGANDO OS DADOS:"+error);})
}
