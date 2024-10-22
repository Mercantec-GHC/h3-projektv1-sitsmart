const myHeaders = new Headers();
myHeaders.append("Accept", "application/json");
myHeaders.append("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiI2ZDVjM2JlMTk0NTY0ZjNmODFhNmJlMDZhOTlhY2I4ZSIsImp0aSI6IjU1NjQ2NTAyLWZjNmUtNGVmNC1hYmVjLTQ5ZDkwNmE1Y2U4OCIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL25hbWUiOiJKYW4iLCJleHAiOjE3Mjk1OTg4MTEsImlzcyI6IkgzLUFQSS1TaXRTbWFydCIsImF1ZCI6IlVzZXJzIn0.XVHsty3XrPfZf7nX80SczIwh97BNHqmwaJZGdZ9hJ2w");

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
            `;

            // Append the card to the user list
            userList.appendChild(userCard);
        });
    })
    .catch((error) => {
        console.error("Error:", error);
        document.getElementById("userList").textContent = `Could not fetch data: ${error.message}`;
    });
