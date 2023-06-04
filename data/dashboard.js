let hostIPAdress = document.getElementById("hostIPAdress");
let roomID = document.getElementById("roomID");

let createTasks = async () => {
    hostIPAddress.value = data.hostIPAddress.toString();
    roomID.value = data.roomID.toString();
}

(async () => {
    try {
        // console.log('try to get data');
        const response = await fetch('/get');
        data = await response.json();

    } catch (e) {

        console.log('Error: ' + e.message);
    }
    //data = JSON.parse(localStorage.getItem("data")) || [];
    console.log(data);
    createTasks();
})()
