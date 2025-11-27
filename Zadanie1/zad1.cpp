#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

//algorytm Thomasa
vector<double> solveThomas(int N){
    double h= 2.0/(N-1);
    double val= 1.0/(h*h);

    vector<double> a(N, val);        
    vector<double> b(N, -2.0*val);  
    vector<double> c(N, val);         
    vector<double> d(N, 0.0);         

    //warunki brzegowe
    b[0]= 1.0; 
    c[0]= 0.0; 
    d[0]= 1.0; 
    a[0]= 0.0;
    b[N-1]= 1.0; 
    a[N-1]= 0.0; 
    d[N-1]= 1.0; 
    c[N-1]= 0.0;

    //eliminacja w przod
    c[0]= c[0]/b[0];
    d[0]= d[0]/b[0];

    for(int i=1;i<N;i++){
        double temp= 1.0/(b[i]-a[i]*c[i-1]);
        c[i]=c[i]*temp;
        d[i]= (d[i]-a[i]*d[i-1])*temp;
    }

    //podstawianie wsteczne
    vector<double> x(N);
    x[N-1]= d[N-1];

    for(int i=N-2;i>=0;i--){
        x[i]=d[i]-c[i]*x[i+1];
    }

    return x;
}

//metoda Gaussa–Seidela
vector<double> solveGaussSeidel(int N, int maxIter= 50000, double tol= 1e-9){
    vector<double> u(N, 0.0);

    //warunki brzegowe
    u[0]= 1.0;
    u[N-1]= 1.0;

    for(int k=0;k<maxIter;k++){
        double maxDiff= 0.0;

        for(int i=1;i<N-1;i++){
            double oldVal= u[i];
            u[i]= 0.5*(u[i-1]+u[i + 1]);

            double diff= abs(u[i] - oldVal);
            if(diff > maxDiff) maxDiff = diff;
        }

        if (maxDiff < tol) break;
    }

    return u;
}

//eliminacja Gaussa
vector<double> solveFullMatrix(int N){
    vector<vector<double>> A(N, vector<double>(N+1, 0.0));

    double h= 2.0/(N-1);
    double val= 1.0/(h*h);
    double diag= -2.0*val;

    A[0][0]= 1.0;
    A[0][N]= 1.0;

    for(int i=1;i<N-1;i++){
        A[i][i-1]= val;
        A[i][i]= diag;
        A[i][i+1]= val;
        A[i][N]= 0.0;
    }

    A[N-1][N-1]= 1.0;
    A[N-1][N]= 1.0;

    for(int i=0;i<N;i++){
        int maxRow= i;

        for(int k=i+1;k<N;k++){
            if(abs(A[k][i])>abs(A[maxRow][i])){
                maxRow = k;
            }
        }
        swap(A[i], A[maxRow]);

        for(int k=i+1;k<N;k++){
            double c= -A[k][i]/A[i][i];
            for(int j=i;j<=N;j++){
                if(i==j){ 
                    A[k][j]= 0.0;
                }
                else{ 
                    A[k][j] +=c*A[i][j];
                }
            }
        }
    }

    //podstawianie wsteczne
    vector<double> x(N);

    for(int i=N-1;i>=0;i--){
        double sum= 0.0;

        for(int j=i+1;j<N;j++){
            sum += A[i][j]*x[j];
        }

        x[i]= (A[i][N]-sum)/A[i][i];
    }

    return x;
}

int main(){
    vector<int> testNs= {10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000};

    ofstream timeFile("czasy.csv");
    timeFile << "N,Thomas,GaussSeidel,FullMatrix\n";

    cout << "Start testów\n";
    cout << setw(10) << "N" << setw(15) << "Thomas" << setw(15) << "G-Seidel" << setw(15) << "Full\n";

    for(int index=0; index<testNs.size();index++){
        int N= testNs[index];

        double tThomas, tGauss, tFull= -1;

        auto start= chrono::high_resolution_clock::now();
        vector<double> solThomas= solveThomas(N);
        auto end= chrono::high_resolution_clock::now();
        tThomas= chrono::duration<double>(end - start).count();

        start= chrono::high_resolution_clock::now();
        vector<double> solGS= solveGaussSeidel(N);
        end= chrono::high_resolution_clock::now();
        tGauss= chrono::duration<double>(end - start).count();

        vector<double> solFull;
        if(N<=1000){
            start = chrono::high_resolution_clock::now();
            solFull = solveFullMatrix(N);
            end = chrono::high_resolution_clock::now();
            tFull = chrono::duration<double>(end - start).count();
        }

        cout << setw(10) << N << setw(15) << tThomas << setw(15) << tGauss << setw(15) << (tFull >= 0 ? to_string(tFull) : "-") << "\n";
        timeFile << N << "," << tThomas << "," << tGauss << ",";
        if (tFull >= 0) timeFile << tFull;
        timeFile << "\n";

        //zapis
        if(N==1000){
            double h= 2.0/(N-1);

            ofstream f1("thomas_1000.csv");
            f1 << "x,y\n";
            for(int i=0;i<N;i++){
                double x=i*h;
                f1 << x << "," << solThomas[i] << "\n";
            }
            f1.close();

            ofstream f2("gauss_seidel_1000.csv");
            f2 << "x,y\n";
            for(int i=0;i<N;i++){
                double x=i*h;
                f2 << x << "," << solGS[i] << "\n";
            }
            f2.close();

            if(N<=1000){
                ofstream f3("full_matrix_1000.csv");
                f3 << "x,y\n";
                for(int i=0;i<N;i++){
                    double x=i*h;
                    f3 << x << "," << solFull[i] << "\n";
                }
                f3.close();
            }

            cout << "zapisano 3 pliki csv dla N=1000\n";
        }
    }

    timeFile.close();
    cout << "Zakonczono.\n";
    return 0;
}

