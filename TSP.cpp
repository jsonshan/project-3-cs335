#include "TSP.hpp"

/**
 * Displays the edges and total distance of the tour.
 * Each edge is printed in the format: "EDGE start_id -> end_id | WEIGHT: weight".
 */
void TSP::Tour::display() const {
  for (size_t i = 1; i < path.size(); i++) {
    std::cout << "EDGE " << path[i-1].id << " -> " << path[i].id << " | WEIGHT : " << weights[i] << std::endl;
  }
  std::cout << "TOTAL DISTANCE: " << total_distance << std::endl;
}

/**
 * Reads a .tsp file and constructs a list of cities as nodes.
 * The file should have a section labeled "NODE_COORD_SECTION" followed by lines with the format: ID x-coordinate y-coordinate.
 * 
 * @param filename The path to the TSP file.
 * @return A list of `Node` objects representing cities and their coordinates.
 * @throws std::runtime_error If the file cannot be read or parsed.
 * 
 * @pre The file specified by `filename` exists and follows the TSP format.
 */
std::list<Node> TSP::constructCities(const std::string& filename) {
  // Read past metadata
  std::ifstream fin(filename);
  if (fin.fail()) {
    std::cerr << "ERROR: Could not read file: " << filename << std::endl;
    throw std::runtime_error("Failed to read file. Terminating.");
  }

  std::string line;
  do { std::getline(fin, line); }
  while (line.find("NODE_COORD_SECTION"));

  // Read data from file into Node list "cities"
  std::list<Node> cities;
  size_t ID;
  double x, y;
  while (!fin.eof()){
    if (!(fin >> ID >> x >> y)) break;
    cities.push_back(Node(ID, x, y));
  }
  return cities;
}
  /**
 * Constructs a tour using the nearest neighbor heuristic for the traveling salesperson problem (TSP).
 * Starting from the city of the given ID, it iteratively visits the nearest unvisited city and returns to the start.
 *
 * @param cities A list of `Node` objects representing the cities to be visited.
 * @param start_id The unique identifier of the starting city.
 * @return A `TSP::Tour` object representing the path, edge weights, and total distance of the computed tour.
 * 
 * @pre `start_id` must be a valid city ID within the range of IDs in `cities`.
 * @note The ith weight in `weights` is the distance traveled from the previous (i-1) city to the current i city (represented by the ith city in the returned Tour.path)
 *       As such, the first weight will ALWAYS equal 0, since there is no edge from the start city to itself
 *
 */

Node TSP::getStartCity(std::list<Node> cities, const size_t& start_id)
{
  // Find start city first by iterating through list
  for (auto itr = cities.begin(); itr != cities.end(); ++itr)
  {
    if (itr->id == start_id)
    {
      return *itr;
    }
  }
  return *cities.begin();
}

TSP::Tour TSP::nearestNeighbor(std::list<Node> cities, const size_t& start_id)
{
  // Use getStartCity to retrieve startingcity
  Node startCity = getStartCity(cities, start_id);
  // Create the tour and initialize the path to be on the start_city first
  Tour tour;
  tour.path.push_back(startCity);
  tour.weights.push_back(0);

  // Create visited hashmap which will keep track of which cities were visited
  std::unordered_map<size_t, bool> visited;
  visited[startCity.id] == true;


  // Continues to run until all cities are visited
  while (visited.size() < cities.size())
  {
    // Initialize a new node, nextCity and keep track of the minDist with this variable
    Node nextCity = startCity;
    size_t minDist = std::numeric_limits<size_t>::max();

    for (auto itr = cities.begin(); itr != cities.end(); ++itr)
    {
      if (visited.find(itr->id) == visited.end())
      {
        size_t temp = startCity.distance(*itr);
        if (temp < minDist)
        {
          minDist = temp;
          nextCity = *itr;
        }
      }
    }
    tour.path.push_back(nextCity);
    tour.weights.push_back(minDist);
    tour.total_distance += minDist;

    visited[nextCity.id] = true;
    startCity = nextCity;
  }

  size_t backToStartDist = startCity.distance(tour.path.front());
  tour.weights.push_back(backToStartDist);
  tour.path.push_back(tour.path.front());
  tour.total_distance += backToStartDist;

  return tour;

}