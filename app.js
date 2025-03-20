function getRecommendations() {
    const budget = document.getElementById("budget").value;
    if (!budget) {
        alert("Please enter a valid budget!");
        return;
    }

    fetch(`http://localhost:5000/recommend?budget=${budget}`)
        .then(response => response.json())
        .then(data => {
            console.log("API Response:", data); // Debugging output

            const list = document.getElementById("recommendation-list");
            list.innerHTML = ""; // Clear previous recommendations

            if (data.length === 0) {
                list.innerHTML = "<li>No recommendations available.</li>";
            } else {
                data.forEach(product => {
                    const listItem = `<li>${product.name} - $${product.price} | Satisfaction: ${product.satisfaction}</li>`;
                    list.innerHTML += listItem;
                });
            }
        })
        .catch(error => console.error("Error fetching recommendations:", error));
}
