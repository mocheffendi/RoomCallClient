// socket3 = new WebSocket("ws:/" + "/" + location.host + ":82");
socket3 = new WebSocket("ws://192.168.0.8:83");
socket3.onopen = function (e) {
    console.log("[socket] socket.onopen ");
};
socket3.onerror = function (e) {
    console.log("[socket] socket.onerror ");
};
socket3.onmessage = function (e) {
    console.log("[socket] " + e.data);
    let message = e.data;
    // message = message.replace(/\n/g, "<br>");

    if (message === "") {
        console.log("kosong");
    }
    else {
        // message = message.replace(/\n|\r\n|\r/g, "<br>");

        // var stringArray = message.split('<br>');

        // for (var i = 0; i < stringArray.length; i++) {
        //     var myString = stringArray[i];
        //     myString = myString.replace('<br>', "");

        //     response.write(myString);
        // }

        document.getElementById("serial").innerHTML = message;

        var br = document.createElement("br");
        var serial = document.getElementById("serial");
        serial.appendChild(br);
    }

};