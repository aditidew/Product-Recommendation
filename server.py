@app.route("/recommend", methods=["GET"])
def recommend_products():
    try:
        budget = float(request.args.get("budget"))
    except ValueError:
        return jsonify({"error": "Invalid budget input"}), 400
    
    conn = sqlite3.connect("products.db")
    cursor = conn.cursor()

    # Fetch all in-stock products ordered by best satisfaction
    cursor.execute("SELECT Name, Price, Satisfaction FROM Products WHERE InStock = 1 ORDER BY Satisfaction DESC;")
    
    selected_products = []
    total_budget = 0
    
    for row in cursor.fetchall():
        if total_budget + row[1] <= budget:
            selected_products.append({"name": row[0], "price": row[1], "satisfaction": row[2]})
            total_budget += row[1]

    conn.close()
    
    # Debugging output
    print(f"Budget: {budget}, Selected Products: {selected_products}")

    return jsonify(selected_products)
