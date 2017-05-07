#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <fstream>
#include "SimpleGraph.h"
using namespace std;
const double kPi = 3.14159265358979323;
const double kRepel = 0.01;
const double kAttract = 0.01;

void Welcome();
void ReadFile(SimpleGraph &graph);
void updateGraph(SimpleGraph &graph);

// Main method
int main() {
    Welcome();

    while (true) {
        SimpleGraph graph;
        ReadFile(graph);
        InitGraphVisualizer(graph);

        int second;
        cout << "Enter the elapsed time:";
        cin >> second;

        time_t startTime = time(NULL);
        while (difftime(time(NULL), startTime) < second) {
            updateGraph(graph);
            DrawGraph(graph);
        }

        string endOrNot, dummy;
        cout << "Press Y/y if you want to draw another graph: ";
        getline(cin, dummy);
        getline(cin, endOrNot);
        if (endOrNot != "Y" && endOrNot != "y")
            break;
    }

    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

/* Asks the user for the name of a file, which contains
 * the node information, repromt the user if it is
 * not valid */
void ReadFile(SimpleGraph &graph) {
    while (true) {
        cout << "Please enter a filename: ";
        string filename;
        getline(cin, filename);
        ifstream input;
        input.open(filename.c_str());
        if(input.is_open()) {
            size_t num_node;
            Edge edge;
            Node node;
            input >> num_node;
            for (size_t i = 0; i < num_node; ++i) {
                node.x = cos(2*kPi*i/num_node);
                node.y = sin(2*kPi*i/num_node);
                graph.nodes.push_back(node);
            }
            while(input >> edge.start && input >> edge.end) {
                graph.edges.push_back(edge);
            }
            input.close();
            break;
        } else {
            cerr << "Can't find the file: " << filename << endl;
        }
    }
}

/* updates the location of each node by the Fruchterman-Reingold algorithm */
void updateGraph(SimpleGraph &graph) {
    size_t num_node = graph.nodes.size();
    vector<double> increx(num_node);
    vector<double> increy(num_node);

    for (size_t i = 0; i < num_node; ++i) {
        for (size_t j = i + 1; j < num_node; ++j) {
            Node node_1 = graph.nodes.at(i);
            Node node_2 = graph.nodes.at(j);
            double distance = (node_1.x - node_2.x) * (node_1.x - node_2.x)
                    + (node_1.y - node_2.y) *(node_1.y - node_2.y);
            double Frepel = kRepel / sqrt(distance);
            double theta = atan2(node_2.y - node_1.y, node_2.x - node_1.x);
            increx[i] -= Frepel * cos(theta);
            increy[i] -= Frepel * sin(theta);
            increx[j] += Frepel * cos(theta);
            increy[j] += Frepel * sin(theta);
        }
    }


    for (size_t i = 0; i < graph.edges.size(); ++i) {
        Edge tmp_edge = graph.edges.at(i);
        size_t first = tmp_edge.start;
        size_t second = tmp_edge.end;
        Node node_1 = graph.nodes.at(first);
        Node node_2 = graph.nodes.at(second);
        double Fattract = kAttract * ((node_1.x - node_2.x) * (node_1.x - node_2.x)
                + (node_1.y - node_2.y) * (node_1.y - node_2.y));
        double theta = atan2(node_2.y - node_1.y, node_2.x - node_1.x);
        increx[first] += Fattract * cos(theta);
        increy[first] += Fattract * sin(theta);
        increx[second] -= Fattract * cos(theta);
        increy[second] -= Fattract * sin(theta);
    }

    for (size_t i = 0; i < num_node; ++i) {
        graph.nodes.at(i).x += increx[i];
        graph.nodes.at(i).y += increy[i];
    }
}
