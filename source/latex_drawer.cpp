//
// Created by acco on 11/9/18.
//
// Simple utility to print .tex images
//

#include <iostream>
#include <cfloat>
#include "latex_drawer.hpp"

std::string extract_filename(std::string path) {
    size_t slashPosition = path.find_last_of('/');
    if (slashPosition != std::string::npos) { slashPosition++; }
    std::string fileName = path.substr(slashPosition, std::string::npos);
    return fileName;
}

std::string replace(std::string word, std::string target, std::string replacement){
    int len, loop=0;
    std::string nword="", let;
    len=word.length();
    len--;
    while(loop<=len){
        let=word.substr(loop, 1);
        if(let==target){
            nword=nword+replacement;
        }else{
            nword=nword+let;
        }
        loop++;
    }
    return nword;

}

void latex_drawer::draw(xsttrp_instance &instance, xsttrp_solution &solution, int run) {

    float xmin = FLT_MAX;
    float xmax = -FLT_MAX;

    float ymin = FLT_MAX;
    float ymax = -FLT_MAX;

    for(auto i = 0; i < instance.matrix_size; i++) {
        if(instance.x[i] < xmin) {
            xmin = instance.x[i];
        }
        if(instance.x[i] > xmax) {
            xmax = instance.x[i];
        }
        if(instance.y[i] < ymin) {
            ymin = instance.y[i];
        }
        if(instance.y[i] > ymax) {
            ymax = instance.y[i];
        }
    }


    std::string filename = instance.path + "_run" + std::to_string(run) +".tex";


    std::ofstream stream(filename);
    if (!stream.is_open()) {
        std::cout << "Error...\n";
        return;
    }


    stream << "\\documentclass[preview]{standalone}\n";
    //stream << "\\usepackage[pdf]{pdftricks}\n";
    stream << "\\usepackage{pdftricks}\n";
    stream << "\\begin{psinputs}\n";
    stream << "\\usepackage{pstricks}\n";
    stream << "\\usepackage{multido}\n";
    stream << "\\end{psinputs}\n";

    stream << "\\begin{document}\n";
    stream << "\\begin{pdfpic}\n";

    stream << "\\begin{pspicture}(0,0)(10,10)\n";

    auto routes = solution.get_routes();

    if(instance.ptype == problem_type::xsttrp) {

        auto main_route = routes[instance.parking_locations_num].front();
        for (auto curr = main_route.get_first_vertex(), n = 0;
             n < main_route.size() + 1; curr = main_route.next(curr), n++) {
            const auto next = main_route.next(curr);
            line(stream, map(instance.x[curr], xmin, xmax), map(instance.y[curr], ymin, ymax),
                 map(instance.x[next], xmin, xmax), map(instance.y[next], ymin, ymax));
        }

    }

    for (int k = 0; k < instance.parking_locations_num; k++) {
        for (const auto &route : routes[k]) {
            for(auto curr = route.get_first_vertex(), n = 0; n < route.size() +1; curr = route.next(curr), n++) {
                const auto next = route.next(curr);
                dashed(stream, map(instance.x[curr], xmin, xmax), map(instance.y[curr], ymin, ymax), map(instance.x[next], xmin, xmax), map(instance.y[next], ymin, ymax));
            }
            dashed(stream, map(instance.x[route.get_last_vertex()], xmin, xmax), map(instance.y[route.get_last_vertex()], ymin, ymax), map(instance.x[k+instance.parking_locations_begin], xmin, xmax), map(instance.y[k+instance.parking_locations_begin], ymin, ymax));
        }
    }



    if(instance.ptype == xsttrp) {
        depot(stream, map(instance.x[instance.depot], xmin, xmax), map(instance.y[instance.depot], ymin, ymax));
    }

    for(auto i = instance.satellites_begin; i < instance.satellites_end; i++) {
        satellite(stream, map(instance.x[i], xmin, xmax), map(instance.y[i], ymin, ymax));
    }
    for(auto i = instance.truck_customers_begin; i < instance.truck_customers_end; i++) {
        truck_customer(stream, map(instance.x[i], xmin, xmax), map(instance.y[i], ymin, ymax));
    }
    for(auto i = instance.vehicle_customers_no_park_begin; i < instance.vehicle_customers_no_park_end; i++) {
        vehicle_customer_no_park(stream, map(instance.x[i], xmin, xmax), map(instance.y[i], ymin, ymax));
    }
    for(auto i = instance.vehicle_customers_yes_park_begin; i < instance.vehicle_customers_yes_park_end; i++) {
        vehicle_customer_yes_park(stream, map(instance.x[i], xmin, xmax), map(instance.y[i], ymin, ymax));
    }

    stream << "\\rput(5,-1){"<<replace(extract_filename(instance.path), "_", "\\_") << " run = "<< run << " z = "<< solution.get_cost() <<"}\n";

    stream << "\\end{pspicture}\n";

    stream << "\\end{pdfpic}\n";
    stream << "\\end{document}\n";

    stream.close();

}

void latex_drawer::vehicle_customer_yes_park(std::ofstream &stream, float x, float y) {
    stream << "\\psdots[linecolor=black, dotstyle=triangle*, dotsize=0.15](" << x << "," << y << ")\n";
}

void latex_drawer::vehicle_customer_no_park(std::ofstream &stream, float x, float y) {
    stream << "\\psdots[linecolor=black, dotsize=0.15](" << x << "," << y << ")\n";
}

void latex_drawer::truck_customer(std::ofstream &stream, float x, float y) {
    stream << "\\psdots[linecolor=black, dotstyle=o, dotsize=0.15, fillcolor=white](" << x << "," << y << ")\n";
}

void latex_drawer::satellite(std::ofstream& stream, float x, float y) {
    stream << "\\psdots[linecolor=black, dotstyle=triangle, dotsize=0.15, fillcolor=white](" << x << "," << y << ")\n";
}

void latex_drawer::depot(std::ofstream &stream, float x, float y) {
    stream << "\\psdots[linecolor=black, dotstyle=square, dotsize=0.15, fillcolor=white](" << x << "," << y << ")\n";
}

float latex_drawer::map(float value, float min, float max) {
    return 10.0f * (value - min) / (max - min);
}

void latex_drawer::line(std::ofstream &stream, float x1, float y1, float x2, float y2) {
    stream << "\\psline[linecolor=black, linewidth=0.02]("<<x1<<","<<y1<<")("<<x2<<","<<y2<<")\n";
}

void latex_drawer::dashed(std::ofstream &stream, float x1, float y1, float x2, float y2) {
    stream << "\\psline[linecolor=black, linewidth=0.02, linestyle=dashed, dash=0.17638889cm 0.10583334cm]("<<x1<<","<<y1<<")("<<x2<<","<<y2<<")\n";
}


