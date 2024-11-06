// Check if token is valid and refresh buttons accordingly
let token = localStorage.getItem("authToken");

function isTokenExpired(token) {
    const expirationTime = getTokenExpiration(token); // Define how to get expiration from token
    return Date.now() >= expirationTime;
}

// Function to update login/logout buttons based on token presence
function updateAuthButtons() {
    const authButton = document.getElementById("authButton");
    const signupButton = document.getElementById("signupButton");

    token = localStorage.getItem("authToken");

    if (token) {
        authButton.textContent = "Logout";
        signupButton.style.display = "none";

        authButton.onclick = () => {
            localStorage.removeItem("authToken");
            alert("You have logged out.");
            updateAuthButtons();
            window.location.href = "index.html";
        };
    } else {
        authButton.textContent = "Login";
        signupButton.style.display = "inline-block";

        authButton.onclick = () => alert("Open login form");
        signupButton.onclick = () => alert("Open signup form");
    }
}

// Function to fetch and display SitSmartDevice list
function fetchSitSmartDevices() {
    if (!token || isTokenExpired(token)) {
        localStorage.removeItem("authToken");
        alert("Session expired. Please log in again.");
        updateAuthButtons();
        window.location.href = "login.html";
        return;
    }

    const headers = new Headers();
    headers.append("Accept", "application/json");
    headers.append("Authorization", `Bearer ${token}`);

    fetch("https://ergo.mercantec.tech/api/SitSmartDevices", { method: "GET", headers })
        .then(response => {
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            return response.json();
        })
        .then(devices => {
            const deviceList = document.getElementById("deviceList");
            deviceList.innerHTML = "";

            devices.forEach(device => {
                const deviceItem = document.createElement("div");
                deviceItem.classList.add("device-item");

                deviceItem.innerHTML = `
                    <h3>Device ID: ${device.id}</h3>
                    <h3>Pew:</h3>
                    <button class="delete-device-button" onclick="deleteDevice('${device.id}')">Delete</button>
                `;

                deviceList.appendChild(deviceItem);
            });
        })
        .catch(error => {
            console.error("Error:", error);
            document.getElementById("deviceList").textContent = `Could not fetch devices: ${error.message}`;
        });
}

function getTokenExpiration(token) {
    const payloadBase64 = token.split(".")[1];
    const payload = JSON.parse(atob(payloadBase64));
    return payload.exp * 1000; // Returner som millisekunder
}

// Function to delete a device
function deleteDevice(deviceId) {
    if (confirm("Are you sure you want to delete this device?")) {
        if (!token || isTokenExpired(token)) {
            localStorage.removeItem("authToken");
            alert("Session expired. Please log in again.");
            updateAuthButtons();
            window.location.href = "login.html";
            return;
        }

        fetch(`https://ergo.mercantec.tech/api/SitSmartDevices/${deviceId}`, {
            method: "DELETE",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Accept": "application/json"
            }
        })
        .then(response => {
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            fetchSitSmartDevices();
        })
        .catch(error => {
            console.error("Error:", error);
            alert(`Could not delete device: ${error.message}`);
        });
    }
}

// Initialize buttons and fetch devices if token exists
document.addEventListener("DOMContentLoaded", function() {
    updateAuthButtons();
    if (token && !isTokenExpired(token)) {
        fetchSitSmartDevices();
    }
});
