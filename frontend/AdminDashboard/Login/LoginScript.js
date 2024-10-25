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

});
