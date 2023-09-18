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
let btnReservation1 = document.getElementById("btnReservation1");
let btnReserve1 = document.getElementById("btnReserve1");
let btnOpenReservation1 = document.getElementById("btnOpenReservation1");

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
let btnModeDark = document.getElementById('btnModeDark');
let btnReservation2 = document.getElementById("btnReservation2");
let btnReserve2 = document.getElementById("btnReserve2");
let btnOpenReservation2 = document.getElementById("btnOpenReservation2");


window.addEventListener('load', ()=>{ setInterval(receiveData,1000);});

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

btnOpenReservation1.addEventListener("click", ()=>{sendReq("/openReserve1");});
btnOpenReservation2.addEventListener("click", ()=>{ sendReq("/openReserve2");});

btnReserve1.addEventListener("click", ()=>{
    let username = document.getElementById("username1").value;
    let password = document.getElementById("password1").value;
    sendReserve2("/reservation1?username="+username+"&password="+password);
})

btnReserve2.addEventListener("click", ()=>{
    let username = document.getElementById("username2").value;
    let password = document.getElementById("password2").value;
    sendReserve2("/reservation2?username="+username+"&password="+password);
})

function cleanFields(){
    document.getElementById("username1").value="";
    document.getElementById("password1").value="";
    document.getElementById("username2").value="";
    document.getElementById("password2").value="";
}

function sendReq(url) {
    fetch(url)
    .then(resp=>resp.json())
    .then((resp)=>{
        console.log(resp);
        if(resp["status"]=="success")
            console.log("Aberto!");
        else
            console.log("Erro!");
    })
    .catch(error =>{console.error("## ERRO PEGANDO OS DADOS:"+error);})
}

function sendReserve(url,username, password) 
{  
    fetch(url, {
        method:"post",
        headers:{
            "Content-Type":"application/json"
        },
        body: JSON.stringify({username, password})
    })
    .then(resp=>resp.json())
    .then((resp)=>{
        console.log(resp);
        if(resp["status"]=="success")
            alert("Espaço Agendado!");
        else
            alert("Espaço Já se Encontra Agendado por Outra Pessoa!")
    })
    .catch(error =>{console.error("## ERRO PEGANDO OS DADOS:"+error);})
}

function sendReserve2(url) 
{  
    fetch(url)
    .then(resp=>resp.json())
    .then((resp)=>{
        console.log(resp);
        alert(resp["message"]);
        cleanFields();
    })
    .catch(error =>{console.error("## ERRO PEGANDO OS DADOS:"+error);})
}

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
                    btnReservation1.classList.remove("disabled");
                else
                    btnReservation1.classList.add("disabled");
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
                    btnReservation2.classList.remove("disabled");
                else
                    btnReservation2.classList.add("disabled");
                break;
        }
    }
}

