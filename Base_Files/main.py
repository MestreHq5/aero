def read_routes_and_remove_duplicates(input_file):
    # Read lines from the file and store them in a list
    with open(input_file, 'r') as file:
        routes_list = file.readlines()

    # Remove duplicates while preserving the order
    routes_set = set()
    unique_routes = []
    for route in routes_list:
        if route not in routes_set:
            routes_set.add(route)
            unique_routes.append(route)

    # Print initial length of the vector
    print("Initial vector length:", len(routes_list))

    # Print final length of the vector
    print("Final vector length:", len(unique_routes))

    return unique_routes

def print_to_file(routes, output_file):
    # Print each route on a new line in a new text file
    with open(output_file, 'w') as file:
        for route in routes:
            file.write(route)

def main():
    input_file = "routes.txt"  # Change this to your input file name
    output_file = "rotas.txt"  # Change this to your output file name

    # Read routes from the file and remove duplicates
    routes = read_routes_and_remove_duplicates(input_file)

    # Print each route to a new text file
    print_to_file(routes, output_file)

if __name__ == "__main__":
    main()

