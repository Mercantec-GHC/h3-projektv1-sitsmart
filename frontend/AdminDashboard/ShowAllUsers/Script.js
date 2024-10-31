document.addEventListener("DOMContentLoaded", function() {
    const token = localStorage.getItem("authToken");
    console.log("Hentet token fra localStorage:", token);

    if (token) {
        fetchUserData(token);
    } else {
        console.error("Ingen token fundet i localStorage. Log ind først.");
        document.getElementById("userList").textContent = "Ingen token fundet. Log ind for at få adgang.";
    }
});

function fetchUserData(token) {
    const myHeaders = new Headers();
    myHeaders.append("Accept", "application/json");
    myHeaders.append("Authorization", `Bearer ${token}`);

    const requestOptions = {
        method: "GET",
        headers: myHeaders,
        redirect: "follow"
    };

    fetch("https://sit-api.mercantec.tech/api/Users", requestOptions)
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            return response.json();
        })
        .then(data => {
            console.log("User data received:", data);
            const userList = document.getElementById("userList");
            userList.innerHTML = ""; // Clear previous content

            data.forEach(user => {
                const userCard = document.createElement("div");
                userCard.classList.add("user-card");

                userCard.innerHTML = `
                    <h3>${user.name}</h3>
                    <p><strong>Email:</strong> ${user.email}</p>
                    <button class="edit-button" onclick="showEditForm('${user.id}', '${user.name}', '${user.email}', this)">Edit</button>
                    <button class="delete-button" onclick="deleteUser('${user.id}', this)">Delete</button>
                `;

                userList.appendChild(userCard);
            });
        })
        .catch(error => {
            console.error("Error:", error);
            document.getElementById("userList").textContent = `Could not fetch data: ${error.message}`;
        });
}

function deleteUser(userId, buttonElement) {
    const token = localStorage.getItem("authToken");

    const myHeaders = new Headers();
    myHeaders.append("Authorization", `Bearer ${token}`);

    const requestOptions = {
        method: "DELETE",
        headers: myHeaders,
        redirect: "follow"
    };

    fetch(`https://sit-api.mercantec.tech/api/Users/${userId}`, requestOptions)
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            const userCard = buttonElement.closest(".user-card");
            userCard.remove();
            console.log("Bruger slettet:", userId);
        })
        .catch(error => {
            console.error("Error:", error);
            alert(`Kunne ikke slette brugeren: ${error.message}`);
        });
}

// Funktion til at vise redigeringsformularen
function showEditForm(userId, currentName, currentEmail, buttonElement) {
    const userCard = buttonElement.closest(".user-card");
    
    // Opret et simpelt redigeringsskema og indsæt det i kortet
    userCard.innerHTML = `
        <input type="text" id="editName" value="${currentName}" placeholder="Name">
        <input type="email" id="editEmail" value="${currentEmail}" placeholder="Email">
        <button onclick="saveUserEdits('${userId}', this)">Save</button>
        <button onclick="cancelEdit('${userId}', '${currentName}', '${currentEmail}', this)">Cancel</button>
    `;
}

// Funktion til at gemme ændringer
function saveUserEdits(userId, buttonElement) {
    const token = localStorage.getItem("authToken");
    const userCard = buttonElement.closest(".user-card");

    const updatedName = userCard.querySelector("#editName").value;
    const updatedEmail = userCard.querySelector("#editEmail").value;

    const myHeaders = new Headers();
    myHeaders.append("Authorization", `Bearer ${token}`);
    myHeaders.append("Content-Type", "application/json");

    const updatedData = JSON.stringify({
        name: updatedName,
        email: updatedEmail
    });

    const requestOptions = {
        method: "PUT",
        headers: myHeaders,
        body: updatedData,
        redirect: "follow"
    };

    fetch(`https://sit-api.mercantec.tech/api/Users/${userId}`, requestOptions)
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            return response.json();
        })
        .then(data => {
            console.log("User updated:", data);

            // Opdater kortet med de nye oplysninger
            userCard.innerHTML = `
                <h3>${data.name}</h3>
                <p><strong>Email:</strong> ${data.email}</p>
                <button class="edit-button" onclick="showEditForm('${userId}', '${data.name}', '${data.email}', this)">Edit</button>
                <button class="delete-button" onclick="deleteUser('${userId}', this)">Delete</button>
            `;
        })
        .catch(error => {
            console.error("Error:", error);
            alert(`Kunne ikke opdatere brugeren: ${error.message}`);
        });
}

// Funktion til at annullere redigering
function cancelEdit(userId, name, email, buttonElement) {
    const userCard = buttonElement.closest(".user-card");

    userCard.innerHTML = `
        <h3>${name}</h3>
        <p><strong>Email:</strong> ${email}</p>
        <button class="edit-button" onclick="showEditForm('${userId}', '${name}', '${email}', this)">Edit</button>
        <button class="delete-button" onclick="deleteUser('${userId}', this)">Delete</button>
    `;
}




function saveUserEdits(userId, buttonElement) {
    const token = localStorage.getItem("authToken");
    const userCard = buttonElement.closest(".user-card");

    const updatedName = userCard.querySelector("#editName").value;
    const updatedEmail = userCard.querySelector("#editEmail").value;

    const myHeaders = new Headers();
    myHeaders.append("Authorization", `Bearer ${token}`);
    myHeaders.append("Content-Type", "application/json");

    const updatedData = JSON.stringify({
        name: updatedName,
        email: updatedEmail
    });

    const requestOptions = {
        method: "PUT",
        headers: myHeaders,
        body: updatedData,
        redirect: "follow"
    };

    // Log data til fejlfinding
    console.log("Updated Data:", updatedData);
    console.log("Request Options:", requestOptions);

    fetch(`https://sit-api.mercantec.tech/api/Users/${userId}`, requestOptions)
        .then(response => {
            if (!response.ok) {
                return response.text().then(text => {
                    console.error("Response Text:", text); // Log serverens respons
                    throw new Error(`HTTP error! status: ${response.status} - ${text}`);
                });
            }
            return response.json();
        })
        .then(data => {
            console.log("User updated:", data);

            // Opdater kortet med de nye oplysninger
            userCard.innerHTML = `
                <h3>${data.name}</h3>
                <p><strong>Email:</strong> ${data.email}</p>
                <button class="edit-button" onclick="showEditForm('${userId}', '${data.name}', '${data.email}', this)">Edit</button>
                <button class="delete-button" onclick="deleteUser('${userId}', this)">Delete</button>
            `;
        })
        .catch(error => {
            console.error("Error:", error);
            alert(`Kunne ikke opdatere brugeren: ${error.message}`);
        });
}
