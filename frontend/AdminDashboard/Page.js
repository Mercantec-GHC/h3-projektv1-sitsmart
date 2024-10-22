const myHeaders = new Headers();
myHeaders.append("Accept", "application/json");
myHeaders.append("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiI2ZDVjM2JlMTk0NTY0ZjNmODFhNmJlMDZhOTlhY2I4ZSIsImp0aSI6ImI3YTkxMDk2LTg5ZjgtNDcwNC1hOTQ4LWM2MDU2Y2U2Y2I3YSIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL25hbWUiOiJKYW4iLCJleHAiOjE3Mjk2MDEzMDAsImlzcyI6IkgzLUFQSS1TaXRTbWFydCIsImF1ZCI6IlVzZXJzIn0.EOCcrtO0fHB5DFfdAq8M6qdaKgejIovCe1AYxcMtj_Q");

const requestOptions = {
    method: "GET",
    headers: myHeaders,
    redirect: "follow"
};

fetch("https://ergo.mercantec.tech/api/Users", requestOptions)
    .then((response) => {
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response.json();
    })
    .then((data) => {
        const userList = document.getElementById("userList");
        userList.innerHTML = ""; // Clear any previous loading message

        // Loop through each user and create a card
        data.forEach(user => {
            const userCard = document.createElement("div");
            userCard.classList.add("user-card");

            // Set the inner HTML of the card
            userCard.innerHTML = `
                <h3>${user.name}</h3>
                <p><strong>Email:</strong> ${user.email}</p>
                <p><strong>Password:</strong> ${user.password}</p>
                <button class="delete-button" onclick="deleteUser('${user.id}', this)">Delete</button>
            `;

            // Append the card to the user list
            userList.appendChild(userCard);
        });
    })
    .catch((error) => {
        console.error("Error:", error);
        document.getElementById("userList").textContent = `Could not fetch data: ${error.message}`;
    });

// Function to delete a user
function deleteUser(userId, buttonElement) {
    const deleteHeaders = new Headers();
    deleteHeaders.append("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiI2ZDVjM2JlMTk0NTY0ZjNmODFhNmJlMDZhOTlhY2I4ZSIsImp0aSI6ImI3YTkxMDk2LTg5ZjgtNDcwNC1hOTQ4LWM2MDU2Y2U2Y2I3YSIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL25hbWUiOiJKYW4iLCJleHAiOjE3Mjk2MDEzMDAsImlzcyI6IkgzLUFQSS1TaXRTbWFydCIsImF1ZCI6IlVzZXJzIn0.EOCcrtO0fHB5DFfdAq8M6qdaKgejIovCe1AYxcMtj_Q");

    const requestOptions = {
        method: "DELETE",
        headers: deleteHeaders,
        redirect: "follow"
    };

    fetch(`https://ergo.mercantec.tech/api/Users/${userId}`, requestOptions)
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            // Remove the card from the DOM
            const userCard = buttonElement.parentElement;
            userCard.remove();
        })
        .catch(error => console.error("Error deleting user:", error));
}
