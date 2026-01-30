#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip>

const double PI= 3.14159265358979323846;

double func(double x){
    return 1.0/(1.0+x*x);
}

double lagrange_interpolation(double x, const std::vector<double>& nodes_x, const std::vector<double>& nodes_y){
    double result= 0.0;
    size_t N= nodes_x.size();

    for(size_t i=0;i<N;i++){
        double term= nodes_y[i];
        for(size_t j=0;j<N;j++){
            if(i !=j){
                term *= (x-nodes_x[j])/ (nodes_x[i]-nodes_x[j]);
            }
        }
        result += term;
    }
    return result;
}

int main() {
    double start= -5.0;
    double end= 5.0;

    double min_error_cheb= 1e30;
    int best_N_cheb= 0;

    std::cout << "Analiza bledu interpolacji (max |f(x) - P(x)|):" << std::endl;
    std::cout << std::setw(5) << "N" << std::setw(20) << "Blad (Rownoodlegle)" << std::setw(20) << "Blad (Czebyszew)" << std::endl;

    for(int N=5;N<=45;N+=2){ 
        
        std::vector<double> nodes_eq_x(N), nodes_eq_y(N);
        std::vector<double> nodes_ch_x(N), nodes_ch_y(N);

        for(int n=0;n<N;n++){
                                      
            nodes_eq_x[n]= -5.0+10.0*n/(double)(N-1);
            nodes_eq_y[n]= func(nodes_eq_x[n]);

            nodes_ch_x[n]= 5.0*std::cos(n*PI/(double)(N-1));
            nodes_ch_y[n]= func(nodes_ch_x[n]);
        }

        double error_eq = 0.0;
        double error_ch = 0.0;
        int check_points = 500;

        for(int i=0;i<check_points;i++){
            double x= start+(end-start)*i/(double)(check_points-1);
            double exact= func(x);

            double approx_eq= lagrange_interpolation(x, nodes_eq_x, nodes_eq_y);
            double approx_ch= lagrange_interpolation(x, nodes_ch_x, nodes_ch_y);

            double curr_err_eq= std::abs(exact - approx_eq);
            double curr_err_ch= std::abs(exact - approx_ch);

            if(curr_err_eq>error_eq) error_eq= curr_err_eq;
            if(curr_err_ch>error_ch) error_ch= curr_err_ch;
        }

        std::cout << std::setw(5) << N << std::setw(20) << error_eq << std::setw(20) << error_ch << std::endl << std::endl;

        if(error_ch<min_error_cheb){
            min_error_cheb= error_ch;
            best_N_cheb= N;
        }
    }

    std::cout << "Najmniejszy blad dla wezlow Czebyszewa osiagnieto dla N = " << best_N_cheb << std::endl;

    std::ofstream file("wyniki_interpolacji.csv");
    file << "x,f(x),Lagrange_Equidistant,Lagrange_Chebyshev\n";

    int N_plot= best_N_cheb; 
    std::vector<double> plot_eq_x(N_plot), plot_eq_y(N_plot);
    std::vector<double> plot_ch_x(N_plot), plot_ch_y(N_plot);

    for(int n=0;n< N_plot;n++){
        plot_eq_x[n]= -5.0+10.0*n/(double)(N_plot-1);
        plot_eq_y[n]= func(plot_eq_x[n]);
        plot_ch_x[n]= 5.0*std::cos(n*PI/(double)(N_plot-1));
        plot_ch_y[n]= func(plot_ch_x[n]);
    }

    int plot_resolution = 1000;
    for(int i=0;i<plot_resolution;i++){
        double x= start+(end-start)*i/(double)(plot_resolution-1);
        double y_exact= func(x);
        double y_eq= lagrange_interpolation(x, plot_eq_x, plot_eq_y);
        double y_ch= lagrange_interpolation(x, plot_ch_x, plot_ch_y);
        
        file << x << "," << y_exact << "," << y_eq << "," << y_ch << std::endl;
    }
    file.close();

    std::cout << "Dane do wykresu zapisano w pliku 'wyniki_interpolacji.csv'." << std::endl;

    return 0;
}
