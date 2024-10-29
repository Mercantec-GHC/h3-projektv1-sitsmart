document.addEventListener("DOMContentLoaded", function() {
    document.getElementById("loginForm").addEventListener("submit", function(event) {
        event.preventDefault(); // Prevent the form from submitting normally

        const email = document.getElementById("email").value;
        const password = document.getElementById("password").value;

        // Define headers and body for the login request
        const loginHeaders = new Headers();
        loginHeaders.append("Content-Type", "application/json");

        const loginData = JSON.stringify({
            email: email,
            password: password
        });

        const requestOptions = {
            method: "POST",
            headers: loginHeaders,
            body: loginData,
            redirect: "follow"
        };

        // Send login request to the backend
        fetch("https://ergo.mercantec.tech/api/Users/login", requestOptions)
            .then(response => {
                if (!response.ok) {
                    // Handle cases where the server responds with an error status code (e.g., 400, 401, 500)
                    return response.text().then(text => {
                        throw new Error(text || "Login failed");
                    });
                }
                // Check if the response has content before trying to parse it
                return response.text().then(text => text ? JSON.parse(text) : {});
            })
            .then(data => {
                const token = data.token; // Assuming the token is returned as { token: "your_jwt_token" }
                if (!token) {
                    throw new Error("Token not received");
                }
                localStorage.setItem("authToken", token); // Store token locally for later use

                // Hide login form and show user list container
                document.getElementById("loginContainer").style.display = "none";
                document.getElementById("userList").style.display = "block";

                // Fetch user data with the token
                fetchUserData(token);
            })
            .catch(error => {
                console.error("Error:", error);
                document.getElementById("loginError").style.display = "block"; // Show error message
                document.getElementById("loginError").textContent = error.message;
            });
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

        fetch("https://ergo.mercantec.tech/api/Users", requestOptions)
            .then(response => {
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                return response.json();
            })
            .then(data => {
                const userList = document.getElementById("userList");
                userList.innerHTML = ""; // Clear any previous loading message

                data.forEach(user => {
                    const userCard = document.createElement("div");
                    userCard.classList.add("user-card");

                    userCard.innerHTML = `
                        <h3>${user.name}</h3>
                        <p><strong>Email:</strong> ${user.email}</p>
                        <button class="delete-button" onclick="deleteUser('${user.id}', this)">Delete</button>
                    `;

                    userList.appendChild(userCard);
                });
            })
            .catch((error) => {
                console.error("Error:", error);
                document.getElementById("userList").textContent = `Could not fetch data: ${error.message}`;
            });
    }

    function deleteUser(userId, buttonElement) {
        const token = localStorage.getItem("authToken");
        const deleteHeaders = new Headers();
        deleteHeaders.append("Authorization", `Bearer ${token}`);

        const requestOptions = {
            method: "DELETE",
            headers: deleteHeaders,
            redirect: "follow"
        };

        fetch(`https://ergo.mercantec.tech/api/Users`, requestOptions)
            .then(response => {
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                const userCard = buttonElement.parentElement;
                userCard.remove();
            })
            .catch(error => console.error("Error deleting user:", error));
    }

    document.getElementById("registerForm").addEventListener("submitCreate", function(event) {
        event.preventDefault(); // Forhindrer formularen i at sende normalt
    
        const name = document.getElementById("newName").value;
        const email = document.getElementById("newEmail").value;
        const password = document.getElementById("newPassword").value;
    
        // Headers og body til registrerings-anmodningen
        const registerHeaders = new Headers();
        registerHeaders.append("Content-Type", "application/json");
    
        const registerData = JSON.stringify({
            name: name,
            email: email,
            password: password
        });
    
        const requestOptions = {
            method: "POST",
            headers: registerHeaders,
            body: registerData,
            redirect: "follow"
        };
    
        // Send registrerings-anmodningen til backend
        fetch("https://ergo.mercantec.tech/api/Users", requestOptions)
            .then(response => {
                if (!response.ok) {
                    // Håndterer fejlstatuskoder (fx 400, 401, 500)
                    return response.text().then(text => {
                        throw new Error(text || "Kunne ikke oprette bruger.");
                    });
                }
                return response.json();
            })
            .then(data => {
                // Hvis registrering er succesfuld, vis succesmeddelelse
                document.getElementById("registerSuccess").style.display = "block";
                document.getElementById("registerError").style.display = "none";
    
                // Ryd input felter
                document.getElementById("newName").value = "";
                document.getElementById("newEmail").value = "";
                document.getElementById("newPassword").value = "";
            })
            .catch(error => {
                console.error("Error:", error);
                document.getElementById("registerError").style.display = "block";
                document.getElementById("registerError").textContent = error.message;
                document.getElementById("registerSuccess").style.display = "none";
            });
        });
    
});
