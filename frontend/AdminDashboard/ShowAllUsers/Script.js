// Hent token fra localStorage
const token = localStorage.getItem("authToken");

// Funktion til at hente og vise brugerdata
function fetchUserData() {
    const myHeaders = new Headers();
    myHeaders.append("Accept", "application/json");
    myHeaders.append("Authorization", `Bearer ${token}`);

    fetch("https://ergo.mercantec.tech/api/Users", { method: "GET", headers: myHeaders })
        .then(response => {
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            return response.json();
        })
        .then(data => {
            const userList = document.getElementById("userList");
            userList.innerHTML = ""; // Ryd tidligere indhold

            data.forEach(user => {
                const userCard = document.createElement("div");
                userCard.classList.add("user-card");
                
                // Startopbygning af HTML-strukturen for hvert brugerkort
                userCard.innerHTML = `
                    <h3><span id="name-${user.id}">${user.name}</span></h3>
                    <p><strong>Email:</strong> <span id="email-${user.id}">${user.email}</span></p>
                    <button id="editButton-${user.id}" onclick="toggleEditUser('${user.id}')">Edit</button>
                    <button class="delete-button" onclick="deleteUser('${user.id}')">Delete</button>
                `;

                userList.appendChild(userCard);
            });
        })
        .catch(error => {
            console.error("Error:", error);
            document.getElementById("userList").textContent = `Could not fetch data: ${error.message}`;
        });
}

// Funktion til at slette bruger
function deleteUser(userId) {
    if (confirm("Er du sikker på, at du vil slette denne bruger?")) {
        fetch(`https://ergo.mercantec.tech/api/Users/${userId}`, {
            method: "DELETE",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Accept": "application/json"
            }
        })
        .then(response => {
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            fetchUserData(); // Opdater brugerlisten efter sletning
        })
        .catch(error => {
            console.error("Error:", error);
            alert(`Kunne ikke slette brugeren: ${error.message}`);
        });
    }
}

// Funktion til at skifte mellem visningstilstand og redigeringstilstand
function toggleEditUser(userId) {
    const nameElement = document.getElementById(`name-${userId}`);
    const emailElement = document.getElementById(`email-${userId}`);
    const editButton = document.getElementById(`editButton-${userId}`);

    if (editButton.textContent === "Edit") {
        // Skift til redigeringstilstand
        const currentName = nameElement.textContent;
        const currentEmail = emailElement.textContent;

        nameElement.innerHTML = `<input type="text" id="editName-${userId}" value="${currentName}">`;
        emailElement.innerHTML = `<input type="text" id="editEmail-${userId}" value="${currentEmail}">`;
        editButton.textContent = "Save";
    } else {
        // Skift til visningstilstand og gem ændringer
        const updatedName = document.getElementById(`editName-${userId}`).value;
        const updatedEmail = document.getElementById(`editEmail-${userId}`).value;

        editUser(userId, updatedName, updatedEmail)
            .then(() => {
                // Opdater felter til de nye værdier og afslut redigering
                nameElement.textContent = updatedName;
                emailElement.textContent = updatedEmail;
                editButton.textContent = "Edit";
            })
            .catch(error => {
                console.error("Error:", error);
                alert(`Kunne ikke opdatere brugeren: ${error.message}`);
            });
    }
}

// Funktion til at redigere brugerdata
function editUser(userId, name, email) {
    const updatedData = JSON.stringify({
        id: userId,
        name: name,
        email: email
    });

    return fetch(`https://ergo.mercantec.tech/api/Users/${userId}`, {
        method: "PUT",
        headers: {
            "Authorization": `Bearer ${token}`,
            "Content-Type": "application/json"
        },
        body: updatedData
    })
    .then(response => {
        if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
    });
}

// Hent brugerdata ved indlæsning af siden
document.addEventListener("DOMContentLoaded", fetchUserData);
