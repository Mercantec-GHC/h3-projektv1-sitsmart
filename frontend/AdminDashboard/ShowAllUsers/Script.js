document.addEventListener("DOMContentLoaded", function() {
    const token = localStorage.getItem("authToken");

    // Funktion til at hente og vise brugerne på siden
    function fetchUserData() {
        const myHeaders = new Headers();
        myHeaders.append("Accept", "application/json");
        myHeaders.append("Authorization", `Bearer ${token}`);

        const requestOptions = {
            method: "GET",
            headers: myHeaders,
            redirect: "follow"
        };

        fetch("https://ergo.mercantec.tech/api/Users", requestOptions)
            .then(response => {
                if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
                return response.json();
            })
            .then(data => {
                const userList = document.getElementById("userList");
                userList.innerHTML = ""; // Ryd tidligere brugerliste

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

    // Funktion til at vise redigeringsformularen
    window.showEditForm = function(userId, currentName, currentEmail, buttonElement) {
        const userCard = buttonElement.closest(".user-card");

        userCard.innerHTML = `
            <input type="text" id="editName" value="${currentName}" placeholder="New name" />
            <input type="email" id="editEmail" value="${currentEmail}" placeholder="New email" />
            <button onclick="saveUserEdits('${userId}', this)">Save</button>
            <button onclick="fetchUserData()">Cancel</button>
        `;
    };

    // Opdateret funktion til at gemme brugerændringer med alle nødvendige felter og PUT
    window.saveUserEdits = function(userId, buttonElement) {
        const userCard = buttonElement.closest(".user-card");
        const updatedName = userCard.querySelector("#editName").value;
        const updatedEmail = userCard.querySelector("#editEmail").value;

        // Først henter vi brugerens nuværende data
        fetch(`https://ergo.mercantec.tech/api/Users/${userId}`, {
            method: "GET",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Accept": "application/json"
            }
        })
        .then(response => {
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            return response.json();
        })
        .then(existingData => {
            // Flet opdaterede felter ind i de eksisterende data
            const updatedData = JSON.stringify({
                id: existingData.id,
                name: updatedName,
                email: updatedEmail,
                password: existingData.password, // Krævede felter inkluderet
                salt: existingData.salt,
                sitSmarts: existingData.sitSmarts,
                realPassword: existingData.realPassword
            });

            return fetch(`https://ergo.mercantec.tech/api/Users/${userId}`, {
                method: "PUT",
                headers: {
                    "Authorization": `Bearer ${token}`,
                    "Content-Type": "application/json"
                },
                body: updatedData
            });
        })
        .then(response => {
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            return response.json();
        })
        .then(data => {
            console.log("User updated:", data);
            fetchUserData(); // Opdater listen af brugere
        })
        .catch(error => {
            console.error("Error:", error);
            alert(`Kunne ikke opdatere brugeren: ${error.message}`);
        });
    };

    // Funktion til at slette en bruger
    window.deleteUser = function(userId, buttonElement) {
        const myHeaders = new Headers();
        myHeaders.append("Authorization", `Bearer ${token}`);

        const requestOptions = {
            method: "DELETE",
            headers: myHeaders,
            redirect: "follow"
        };

        fetch(`https://ergo.mercantec.tech/api/Users/${userId}`, requestOptions)
            .then(response => {
                if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
                console.log("User deleted:", userId);
                fetchUserData(); // Opdater listen af brugere
            })
            .catch(error => {
                console.error("Error:", error);
                alert(`Kunne ikke slette brugeren: ${error.message}`);
            });
    };

    // Hent og vis alle brugere ved indlæsning af siden
    fetchUserData();
});
