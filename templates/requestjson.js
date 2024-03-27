var serverurl = window.location.origin;
async function clicsearch() {

  var form2 = document.getElementById("myForm").elements[0].value;
  document.getElementById("response").innerHTML = "buscando";

  fetch(serverurl+"/balance", {
    method: 'POST',
    headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({ "resourse": form2, "b": 2 })
})
.then(response => response.text())
.then(data => document.getElementById("response").innerHTML = data)
.catch(error => console.log(error));

}

async function MakeWallet() {

  var form2 = document.getElementById("myForm").elements[0].value;
  document.getElementById("response").innerHTML = "buscando";

  fetch(serverurl+"/MakeWallet", {
    method: 'POST',
    headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({ "resourse": form2, "b": 2 })
})
.then(response => response.text())
.then(data => document.getElementById("response").innerHTML = data)
.catch(error => console.log(error));
}

async function balance() {


  var form2 = document.getElementById("myForm").elements[0].value;
  document.getElementById("response").innerHTML = "buscando";

  fetch(serverurl+"/balance", {
    method: 'POST',
    headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({ "resourse": form2, "b": 2 })
})
.then(response => response.text())
.then(data => document.getElementById("response").innerHTML = parseInt(Number("0x"+data), 10))
.catch(error => console.log(error));

}

async function transfer() {

  var w= document.getElementById("myForm").elements[0].value;
  var x = document.getElementById("myForm").elements[1].value;
  var y = document.getElementById("myForm").elements[2].value;
  document.getElementById("response").innerHTML = "buscando";

  fetch("https://0.0.0.0:18090/transac", {
    method: 'POST',
    headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({ "w": w, "x": x, "y": y })
})
.then(response => response.text())
.then(data => document.getElementById("response").innerHTML = data)
.catch(error => console.log(error));

}

async function blocksearch() {


  var form2 = document.getElementById("myForm").elements[0].value;
  document.getElementById("response").innerHTML = "buscando";

  fetch(serverurl+"/blocksearch", {
    method: 'POST',
    headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({ "resourse": form2, "b": 2 })
})
.then(response => response.text())
.then(data => {
  let porcion1 = data.substring(0, 64); // extraer los primeros 10 caracteres
  let porcion2 = data.substring(10, 20); // extraer los siguientes 10 caracteres
  let porcion3 = data.slice(20); // extraer el resto de la cadena

  // asignar cada porción a un contenedor HTML
  document.getElementById("response").innerHTML = data;
 
})
.catch(error => console.log(error));

}

setInterval(function() {
  fetch(serverurl + '/status', {
    method: 'GET'
  })
  .then(function(response) {
    if (response.ok) {
      return response.json(); // Analiza la respuesta como JSON
    } else {
      throw new Error('Error en la solicitud');
    }
  })
  .then(function(jsonArray) {
    jsonArray.forEach(function(elemento) {

      const elementIds = ["sync", "lastOpConfirmed", "blksize" ,"lastblblocal" , "lstblnetwork" , "ShaLBB"]; // Mapeo entre los índices del JSON array y los IDs de los elementos HTML

      // Iterar por cada elemento del JSON array
      jsonArray.forEach(function(elemento, indice) {
        const elementId = elementIds[indice]; // Obtener el ID correspondiente al índice
        
        const myElement = document.getElementById(elementId); // Seleccionar el elemento HTML por su ID
        
        if (myElement) {
          myElement.innerHTML = elemento; 
          if( elemento == "synced" ){
            const circleElement = document.querySelector('.circle'); // Seleccionar el elemento con la clase 'circle'

            circleElement.style.backgroundColor = "rgb(52, 137, 222)"; // Cambiar el color de fondo a rojo
          }

          if( elemento == "syncing..." ){
            const circleElement = document.querySelector('.circle'); // Seleccionar el elemento con la clase 'circle'

            circleElement.style.backgroundColor = "rgb(228, 102, 34)"; // Cambiar el color de fondo a rojo
          }
        }
      });


      // Realiza cualquier otra operación con cada elemento aquí
    });
  })
  .catch(function(error) {
    console.log('Error:', error);
  });
}, 15000);


async function blks(x) {

  var form2 = document.getElementById("myForm").elements[0].value;
  document.getElementById("response").innerHTML = "buscando";

  fetch(serverurl+"/blks", {
    method: 'POST',
    headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({ "x": x, "b": 2 })
})
.then(response => response.text())
.then(data => document.getElementById("response").innerHTML = data)
.catch(error => console.log(error));
}




// Función para cifrar un texto utilizando una clave AES
async function encryptText(text, key) {
  const encoder = new TextEncoder();
  const data = encoder.encode(text);

  const iv = window.crypto.getRandomValues(new Uint8Array(16));

  const encryptedData = await window.crypto.subtle.encrypt(
    {
      name: "AES-CBC",
      iv: iv
    },
    key,
    data
  );

  const encryptedBytes = new Uint8Array(encryptedData);
  const encryptedHex = Array.prototype.map
    .call(encryptedBytes, byte => ("00" + byte.toString(16)).slice(-2))
    .join("");

  const ivHex = Array.prototype.map
    .call(iv, byte => ("00" + byte.toString(16)).slice(-2))
    .join("");

  return ivHex + encryptedHex;
}

// Función para descifrar un texto cifrado utilizando una clave AES
async function decryptText(ciphertext, key) {
  const iv = ciphertext.slice(0, 32);
  const encryptedDataHex = ciphertext.slice(32);

  const encryptedBytes = new Uint8Array(
    encryptedDataHex.match(/.{1,2}/g).map(byte => parseInt(byte, 16))
  );

  const decryptedData = await window.crypto.subtle.decrypt(
    {
      name: "AES-CBC",
      iv: new Uint8Array(iv.match(/.{1,2}/g).map(byte => parseInt(byte, 16)))
    },
    key,
    encryptedBytes
  );

  const decoder = new TextDecoder();
  return decoder.decode(decryptedData);
}


async function calculateSHA3Hash(input) {
  const encoder = new TextEncoder();
  const data = encoder.encode(input);

  const hashBuffer = await crypto.subtle.digest("SHA-256", data);
  const hashArray = Array.from(new Uint8Array(hashBuffer));
  const hashHex = hashArray.map(byte => ("00" + byte.toString(16)).slice(-2)).join("");

  return hashHex;
}

function hexStringToUint8Array(hexString) {
  const hexArray = hexString.match(/.{1,2}/g);
  return new Uint8Array(hexArray.map(byte => parseInt(byte, 16)));
}




